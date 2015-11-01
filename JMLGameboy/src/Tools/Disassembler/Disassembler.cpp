/*
JML_GBEmulator
Copyright (C) 2015 Leiva Juan Martin

This file is part of JML_GBEmulator.

JML_GBEmulator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

JML_GBEmulator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JML_GBEmulator.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Disassembler.h"
#include "../../Core/Memory/MemoryController.h"

Disassembler::Disassembler(MemoryController* memoryController)
{
	this->memoryController = memoryController;
	LoadInstructions();
	InitialParse();
}

void Disassembler::InitialParse()
{
	WORD i = 0;

	while(i < 0xffff)
	{
		BYTE byte = memoryController->ReadMemory(i);
		DisassemberInstruction instruction = DisassembleAtReal(i);

		if(jumpIntructions[byte])
		{
			EntryPoint* entryPoint = new EntryPoint;
			entryPoint->relativePosition = codeSize;
			entryPoint->realAddress = i;

			entryPoints.push_back(entryPoint);
		}

		i += instruction.size;
		codeSize++;
	}

	int a = 0;
}

int Disassembler::GetCodeSize()
{
	return codeSize;
}

WORD Disassembler::RelativeToRealPosition(WORD relativePosition)
{
	EntryPoint previousEntryPoint;
	previousEntryPoint.realAddress = 0;
	previousEntryPoint.relativePosition = 0;

	// IF less, nearer entry point is ZERO
	if(entryPoints.size() > 0)
	{
		if(entryPoints[0]->relativePosition <= relativePosition)
		{
			for(int i = 0; entryPoints.size(); i++)
			{
				EntryPoint* entryPoint = entryPoints[i];
				if(entryPoint->relativePosition == relativePosition)
				{
					return entryPoint->realAddress;
				}
				else if(entryPoint->relativePosition < relativePosition)
				{
					break;
				}

				previousEntryPoint.realAddress = entryPoint->realAddress;
				previousEntryPoint.relativePosition = i;
			}
		}
	}

	// Iterate instructions until reach the desired position
	while(previousEntryPoint.relativePosition < relativePosition)
	{
		DisassemberInstruction disassemberInstruction = DisassembleAtReal(previousEntryPoint.realAddress);
		previousEntryPoint.realAddress += disassemberInstruction.size;
		previousEntryPoint.relativePosition += 1;
	}

	return previousEntryPoint.realAddress;
}

DisassemberInstruction Disassembler::DisassembleAtRelative(WORD relativePosition)
{
	WORD realPosition = RelativeToRealPosition(relativePosition);
	return DisassembleAtReal(realPosition);
}

DisassemberInstruction Disassembler::DisassembleAtReal(WORD position)
{
	DisassemberInstruction disassemberInstruction;
	BYTE byte = memoryController->ReadMemory(position);
	
	// BC
	if(byte == 0xCB)
	{
		position +=1;
		byte = memoryController->ReadMemory(position);
		disassemberInstruction.instructionBase = cbInstructions[byte];
		disassemberInstruction.hasParameter = false;
		disassemberInstruction.size = 2;
	}
	else
	{
		disassemberInstruction.instructionBase = instructions[byte];

		Disassembler::InstructionParameter instructionParameterFunction = instructionsParameter[byte];

		if(instructionParameterFunction != 0)
		{
			disassemberInstruction.hasParameter = true;
			(this->*instructionParameterFunction)(position, disassemberInstruction);
		}
		else
		{
			disassemberInstruction.hasParameter = false;
			disassemberInstruction.size = 1;
		}
	}

	

	return disassemberInstruction;
}


void Disassembler::Get16(WORD position, DisassemberInstruction &instruction)
{
	WORD n1 = memoryController->ReadMemory(position + 2);
	WORD n2 = memoryController->ReadMemory(position + 1);

	WORD nn = (n1 << 8) | n2;

	instruction.size = 3;
	instruction.hasParameter = true;
	instruction.parameter = nn;
}

void Disassembler::GetU8(WORD position, DisassemberInstruction &instruction)
{
	WORD n = memoryController->ReadMemory(position + 1);
	
	instruction.size = 2;
	instruction.hasParameter = true;
	instruction.parameter = n;
}

void Disassembler::GetS8(WORD position, DisassemberInstruction &instruction)
{
	signed char n = memoryController->ReadMemory(position + 1);

	instruction.size = 2;
	instruction.hasParameter = true;
	instruction.parameter = n;
}

void Disassembler::GetS8_FF00(WORD position, DisassemberInstruction &instruction)
{
	signed char n = memoryController->ReadMemory(position + 1);
	
	instruction.size = 2;
	instruction.hasParameter = true;
	instruction.parameter = 0xFF00 + n;
}

void Disassembler::GetDisplacementS8(WORD position, DisassemberInstruction &instruction)
{
	signed char n = memoryController->ReadMemory(position + 1);

	instruction.size = 2;
	instruction.hasParameter = true;
	instruction.parameter = position + n;
}

void Disassembler::LoadInstructions()
{
	instructions[0x00]	=	"NOP";							instructionsParameter[0x00] = 0;
	instructions[0x01]	=	"LD BC, %1";					instructionsParameter[0x01] = &Disassembler::Get16;
	instructions[0x02]	=	"LD (BC), A";					instructionsParameter[0x02] = 0;
	instructions[0x03]	=	"INC BC";						instructionsParameter[0x03] = 0;
	instructions[0x04]	=	"INC B";						instructionsParameter[0x04] = 0;
	instructions[0x05]	=	"DEC B";						instructionsParameter[0x05] = 0;
	instructions[0x06]	=	"LD B, %1";						instructionsParameter[0x06] = &Disassembler::GetU8;
	instructions[0x07]	=	"RLCA";							instructionsParameter[0x07] = 0;
	instructions[0x08]	=	"LD (%1), SP";					instructionsParameter[0x08] = &Disassembler::Get16;
	instructions[0x09]	=	"ADD HL, BC";					instructionsParameter[0x09] = 0;
	instructions[0x0A]	=	"LD A, (BC)";					instructionsParameter[0x0A] = 0;
	instructions[0x0B]	=	"DEC BC";						instructionsParameter[0x0B] = 0;
	instructions[0x0C]	=	"INC C";						instructionsParameter[0x0C] = 0;
	instructions[0x0D]	=	"DEC C";						instructionsParameter[0x0D] = 0;
	instructions[0x0E]	=	"LD C, %1";						instructionsParameter[0x0E] = &Disassembler::GetU8;
	instructions[0x0F]	=	"RRCA";							instructionsParameter[0x0F] = 0;

	instructions[0x10]	=	"STOP";							instructionsParameter[0x10] = 0;
	instructions[0x11]	=	"LD DE, %1";					instructionsParameter[0x11] = &Disassembler::Get16;
	instructions[0x12]	=	"LD (DE), A";					instructionsParameter[0x12] = 0;
	instructions[0x13]	=	"INC DE";						instructionsParameter[0x13] = 0;
	instructions[0x14]	=	"INC D";						instructionsParameter[0x14] = 0;
	instructions[0x15]	=	"DEC D";						instructionsParameter[0x15] = 0;
	instructions[0x16]	=	"LD D, %1";						instructionsParameter[0x16] = &Disassembler::GetU8;
	instructions[0x17]	=	"RLA";							instructionsParameter[0x17] = 0;
	instructions[0x18]	=	"JR %1";						instructionsParameter[0x18] = &Disassembler::GetDisplacementS8;
	instructions[0x19]	=	"ADD HL, DE";					instructionsParameter[0x19] = 0;
	instructions[0x1A]	=	"LD A, (DE)";					instructionsParameter[0x1A] = 0;
	instructions[0x1B]	=	"DEC BC";						instructionsParameter[0x1B] = 0;
	instructions[0x1C]	=	"INC C";						instructionsParameter[0x1C] = 0;
	instructions[0x1D]	=	"DEC C";						instructionsParameter[0x1D] = 0;
	instructions[0x1E]	=	"LD E, %1";						instructionsParameter[0x1E] = &Disassembler::GetU8;
	instructions[0x1F]	=	"RRA";							instructionsParameter[0x1F] = 0;

	instructions[0x20]	=	"JR NZ, %1";					instructionsParameter[0x20] = &Disassembler::GetDisplacementS8;
	instructions[0x21]	=	"LD HL, %1";					instructionsParameter[0x21] = &Disassembler::Get16;
	instructions[0x22]	=	"LD (HL+), A";					instructionsParameter[0x22] = 0;
	instructions[0x23]	=	"INC HL";						instructionsParameter[0x23] = 0;
	instructions[0x24]	=	"INC H";						instructionsParameter[0x24] = 0;
	instructions[0x25]	=	"DEC H";						instructionsParameter[0x25] = 0;
	instructions[0x26]	=	"LD H, %1";						instructionsParameter[0x26] = &Disassembler::GetU8;
	instructions[0x27]	=	"DAA";							instructionsParameter[0x27] = 0;
	instructions[0x28]	=	"JR Z, %1";						instructionsParameter[0x28] = &Disassembler::GetDisplacementS8;
	instructions[0x29]	=	"ADD HL, HL";					instructionsParameter[0x29] = 0;
	instructions[0x2A]	=	"LD A (HL+)";					instructionsParameter[0x2A] = 0;
	instructions[0x2B]	=	"DEC HL";						instructionsParameter[0x2B] = 0;
	instructions[0x2C]	=	"INC L";						instructionsParameter[0x2C] = 0;
	instructions[0x2D]	=	"DEC L";						instructionsParameter[0x2D] = 0;
	instructions[0x2E]	=	"LD L, %1";						instructionsParameter[0x2E] = &Disassembler::GetU8;
	instructions[0x2F]	=	"CPL";							instructionsParameter[0x2F] = 0;

	instructions[0x30]	=	"JR NC, %1";					instructionsParameter[0x30] = &Disassembler::GetDisplacementS8;
	instructions[0x31]	=	"LD SP, %1";					instructionsParameter[0x31] = &Disassembler::Get16;
	instructions[0x32]	=	"LD (HL-), A";					instructionsParameter[0x32] = 0;
	instructions[0x33]	=	"INC SP";						instructionsParameter[0x33] = 0;
	instructions[0x34]	=	"INC (HL)";						instructionsParameter[0x34] = 0;
	instructions[0x35]	=	"DEC_(HL)";						instructionsParameter[0x35] = 0;
	instructions[0x36]	=	"LD (HL), %1";					instructionsParameter[0x36] = &Disassembler::GetU8;
	instructions[0x37]	=	"SCF";							instructionsParameter[0x37] = 0;
	instructions[0x38]	=	"JR C, %1";						instructionsParameter[0x38] = &Disassembler::GetDisplacementS8;
	instructions[0x39]	=	"ADD HL, SP";					instructionsParameter[0x39] = 0;
	instructions[0x3A]	=	"LD A, (HL-)";					instructionsParameter[0x3A] = 0;
	instructions[0x3B]	=	"DEC SP";						instructionsParameter[0x3B] = 0;
	instructions[0x3C]	=	"INC A";						instructionsParameter[0x3C] = 0;
	instructions[0x3D]	=	"DEC A";						instructionsParameter[0x3D] = 0;
	instructions[0x3E]	=	"LD A, %1";						instructionsParameter[0x3E] = &Disassembler::GetU8;
	instructions[0x3F]	=	"CCF";							instructionsParameter[0x3F] = 0;

	instructions[0x40]	=	"LD B, B";						instructionsParameter[0x40] = 0;
	instructions[0x41]	=	"LD B, C";						instructionsParameter[0x41] = 0;
	instructions[0x42]	=	"LD B, D";						instructionsParameter[0x42] = 0;
	instructions[0x43]	=	"LD B, E";						instructionsParameter[0x43] = 0;
	instructions[0x44]	=	"LD B, H";						instructionsParameter[0x44] = 0;
	instructions[0x45]	=	"LD B, L";						instructionsParameter[0x45] = 0;
	instructions[0x46]	=	"LD B, (HL)";					instructionsParameter[0x46] = 0;
	instructions[0x47]	=	"LD B, A";						instructionsParameter[0x47] = 0;
	instructions[0x48]	=	"LD C, B";						instructionsParameter[0x48] = 0;
	instructions[0x49]	=	"LD C, C";						instructionsParameter[0x49] = 0;
	instructions[0x4A]	=	"LD C, D";						instructionsParameter[0x4A] = 0;
	instructions[0x4B]	=	"LD C, E";						instructionsParameter[0x4B] = 0;
	instructions[0x4C]	=	"LD C, H";						instructionsParameter[0x4C] = 0;
	instructions[0x4D]	=	"LD C, L";						instructionsParameter[0x4D] = 0;
	instructions[0x4E]	=	"LD C, (HL)";					instructionsParameter[0x4E] = 0;
	instructions[0x4F]	=	"LD C, A";						instructionsParameter[0x4F] = 0;

	instructions[0x50]	=	"LD D, B";						instructionsParameter[0x50] = 0;
	instructions[0x51]	=	"LD D, C";						instructionsParameter[0x51] = 0;
	instructions[0x52]	=	"LD D, D";						instructionsParameter[0x52] = 0;
	instructions[0x53]	=	"LD D, E";						instructionsParameter[0x53] = 0;
	instructions[0x54]	=	"LD D, H";						instructionsParameter[0x54] = 0;
	instructions[0x55]	=	"LD D, L";						instructionsParameter[0x55] = 0;
	instructions[0x56]	=	"LD D, (HL)";					instructionsParameter[0x56] = 0;
	instructions[0x57]	=	"LD D, A";						instructionsParameter[0x57] = 0;
	instructions[0x58]	=	"LD E, B";						instructionsParameter[0x58] = 0;
	instructions[0x59]	=	"LD E, C";						instructionsParameter[0x59] = 0;
	instructions[0x5A]	=	"LD E, D";						instructionsParameter[0x5A] = 0;
	instructions[0x5B]	=	"LD E, E";						instructionsParameter[0x5B] = 0;
	instructions[0x5C]	=	"LD E, H";						instructionsParameter[0x5C] = 0;
	instructions[0x5D]	=	"LD E, L";						instructionsParameter[0x5D] = 0;
	instructions[0x5E]	=	"LD E, (HL)";					instructionsParameter[0x5E] = 0;
	instructions[0x5F]	=	"LD E, A";						instructionsParameter[0x5F] = 0;

	instructions[0x60]	=	"LD H, B";						instructionsParameter[0x60] = 0;
	instructions[0x61]	=	"LD H, C";						instructionsParameter[0x61] = 0;
	instructions[0x62]	=	"LD H, D";						instructionsParameter[0x62] = 0;
	instructions[0x63]	=	"LD H, E";						instructionsParameter[0x63] = 0;
	instructions[0x64]	=	"LD H, H";						instructionsParameter[0x64] = 0;
	instructions[0x65]	=	"LD H, L";						instructionsParameter[0x65] = 0;
	instructions[0x66]	=	"LD H, (HL)";					instructionsParameter[0x66] = 0;
	instructions[0x67]	=	"LD H, A";						instructionsParameter[0x67] = 0;
	instructions[0x68]	=	"LD L, B";						instructionsParameter[0x68] = 0;
	instructions[0x69]	=	"LD L, C";						instructionsParameter[0x69] = 0;
	instructions[0x6A]	=	"LD L, D";						instructionsParameter[0x6A] = 0;
	instructions[0x6B]	=	"LD L, E";						instructionsParameter[0x6B] = 0;
	instructions[0x6C]	=	"LD L, H";						instructionsParameter[0x6C] = 0;
	instructions[0x6D]	=	"LD L, L";						instructionsParameter[0x6D] = 0;
	instructions[0x6E]	=	"LD L, (HL)";					instructionsParameter[0x6E] = 0;
	instructions[0x6F]	=	"LD L, A";						instructionsParameter[0x6F] = 0;

	instructions[0x70]	=	"LD (HL), B";					instructionsParameter[0x70] = 0;
	instructions[0x71]	=	"LD (HL), C";					instructionsParameter[0x71] = 0;
	instructions[0x72]	=	"LD (HL), D";					instructionsParameter[0x72] = 0;
	instructions[0x73]	=	"LD (HL), E";					instructionsParameter[0x73] = 0;
	instructions[0x74]	=	"LD (HL), H";					instructionsParameter[0x74] = 0;
	instructions[0x75]	=	"LD (HL), L";					instructionsParameter[0x75] = 0;
	instructions[0x76]	=	"HALT";							instructionsParameter[0x76] = 0;
	instructions[0x77]	=	"LD (HL), A";					instructionsParameter[0x77] = 0;
	instructions[0x78]	=	"LD A, B";						instructionsParameter[0x78] = 0;
	instructions[0x79]	=	"LD A, C";						instructionsParameter[0x79] = 0;
	instructions[0x7A]	=	"LD A, D";						instructionsParameter[0x7A] = 0;
	instructions[0x7B]	=	"LD A, E";						instructionsParameter[0x7B] = 0;
	instructions[0x7C]	=	"LD A, H";						instructionsParameter[0x7C] = 0;
	instructions[0x7D]	=	"LD A, L";						instructionsParameter[0x7D] = 0;
	instructions[0x7E]	=	"LD A, (HL)";					instructionsParameter[0x7E] = 0;
	instructions[0x7F]	=	"LD A, A";						instructionsParameter[0x7F] = 0;

	instructions[0x80]	=	"ADD A, B";						instructionsParameter[0x80] = 0;
	instructions[0x81]	=	"ADD A, C";						instructionsParameter[0x81] = 0;
	instructions[0x82]	=	"ADD A, D";						instructionsParameter[0x82] = 0;
	instructions[0x83]	=	"ADD A, E";						instructionsParameter[0x83] = 0;
	instructions[0x84]	=	"ADD A, H";						instructionsParameter[0x84] = 0;
	instructions[0x85]	=	"ADD A, L";						instructionsParameter[0x85] = 0;
	instructions[0x86]	=	"ADD A, (HL)";					instructionsParameter[0x86] = 0;
	instructions[0x87]	=	"ADD A, A";						instructionsParameter[0x87] = 0;
	instructions[0x88]	=	"ADC A, B";						instructionsParameter[0x88] = 0;
	instructions[0x89]	=	"ADC A, C";						instructionsParameter[0x89] = 0;
	instructions[0x8A]	=	"ADC A, D";						instructionsParameter[0x8A] = 0;
	instructions[0x8B]	=	"ADC A, E";						instructionsParameter[0x8B] = 0;
	instructions[0x8C]	=	"ADC A, H";						instructionsParameter[0x8C] = 0;
	instructions[0x8D]	=	"ADC A, L";						instructionsParameter[0x8D] = 0;
	instructions[0x8E]	=	"ADC A, (HL)";					instructionsParameter[0x8E] = 0;
	instructions[0x8F]	=	"ADC A, A";						instructionsParameter[0x8F] = 0;

	instructions[0x90]	=	"SUB {A}, B";					instructionsParameter[0x90] = 0;
	instructions[0x91]	=	"SUB {A}, C";					instructionsParameter[0x91] = 0;
	instructions[0x92]	=	"SUB {A}, D";					instructionsParameter[0x92] = 0;
	instructions[0x93]	=	"SUB {A}, E";					instructionsParameter[0x93] = 0;
	instructions[0x94]	=	"SUB {A}, H";					instructionsParameter[0x94] = 0;
	instructions[0x95]	=	"SUB {A}, L";					instructionsParameter[0x95] = 0;
	instructions[0x96]	=	"SUB {A}, (HL)";				instructionsParameter[0x96] = 0;
	instructions[0x97]	=	"SUB {A}, A";					instructionsParameter[0x97] = 0;
	instructions[0x98]	=	"SBC A, B";						instructionsParameter[0x98] = 0;
	instructions[0x99]	=	"SBC A, C";						instructionsParameter[0x99] = 0;
	instructions[0x9A]	=	"SBC A, D";						instructionsParameter[0x9A] = 0;
	instructions[0x9B]	=	"SBC A, E";						instructionsParameter[0x9B] = 0;
	instructions[0x9C]	=	"SBC A, H";						instructionsParameter[0x9C] = 0;
	instructions[0x9D]	=	"SBC A, L";						instructionsParameter[0x9D] = 0;
	instructions[0x9E]	=   "SBC A, (HL)";					instructionsParameter[0x9E] = 0;
	instructions[0x9F]	=	"SBC A, A";						instructionsParameter[0x9F] = 0;

	instructions[0xA0]	=	"AND {A}, B";					instructionsParameter[0xA0] = 0;
	instructions[0xA1]	=	"AND {A}, C";					instructionsParameter[0xA1] = 0;
	instructions[0xA2]	=	"AND {A}, D";					instructionsParameter[0xA2] = 0;
	instructions[0xA3]	=	"AND {A}, E";					instructionsParameter[0xA3] = 0;
	instructions[0xA4]	=	"AND {A}, H";					instructionsParameter[0xA4] = 0;
	instructions[0xA5]	=	"AND {A}, L";					instructionsParameter[0xA5] = 0;
	instructions[0xA6]	=	"AND {A}, (HL)";				instructionsParameter[0xA6] = 0;
	instructions[0xA7]	=	"AND {A}, A";					instructionsParameter[0xA7] = 0;
	instructions[0xA8]	=	"XOR {A}, B";					instructionsParameter[0xA8] = 0;
	instructions[0xA9]	=	"XOR {A}, C";					instructionsParameter[0xA9] = 0;
	instructions[0xAA]	=	"XOR {A}, D";					instructionsParameter[0xAA] = 0;
	instructions[0xAB]	=	"XOR {A}, E";					instructionsParameter[0xAB] = 0;
	instructions[0xAC]	=	"XOR {A}, H";					instructionsParameter[0xAC] = 0;
	instructions[0xAD]	=	"XOR {A}, L";					instructionsParameter[0xAD] = 0;
	instructions[0xAE]	=	"XOR {A}, (HL)";				instructionsParameter[0xAE] = 0;
	instructions[0xAF]	=	"XOR {A}, A";					instructionsParameter[0xAF] = 0;

	instructions[0xB0]	=	"OR {A}, B";					instructionsParameter[0xB0] = 0;
	instructions[0xB1]	=	"OR {A}, C";					instructionsParameter[0xB1] = 0;
	instructions[0xB2]	=	"OR {A}, D";					instructionsParameter[0xB2] = 0;
	instructions[0xB3]	=	"OR {A}, E";					instructionsParameter[0xB3] = 0;
	instructions[0xB4]	=	"OR {A}, H";					instructionsParameter[0xB4] = 0;
	instructions[0xB5]	=	"OR {A}, L";					instructionsParameter[0xB5] = 0;
	instructions[0xB6]	=	"OR {A}, (HL)";					instructionsParameter[0xB6] = 0;
	instructions[0xB7]	=	"OR {A}, A";					instructionsParameter[0xB7] = 0;
	instructions[0xB8]	=	"CP {A}, B";					instructionsParameter[0xB8] = 0;
	instructions[0xB9]	=	"CP {A}, C";					instructionsParameter[0xB9] = 0;
	instructions[0xBA]	=	"CP {A}, D";					instructionsParameter[0xBA] = 0;
	instructions[0xBB]	=	"CP {A}, E";					instructionsParameter[0xBB] = 0;
	instructions[0xBC]	=	"CP {A}, H";					instructionsParameter[0xBC] = 0;
	instructions[0xBD]	=	"CP {A}, L";					instructionsParameter[0xBD] = 0;
	instructions[0xBE]	=	"CP {A}, (HL)";					instructionsParameter[0xBE] = 0;
	instructions[0xBF]	=	"CP {A}, A";					instructionsParameter[0xBF] = 0;

	instructions[0xC0]	=	"RET NZ";						instructionsParameter[0xC0] = 0;
	instructions[0xC1]	=	"POP BC";						instructionsParameter[0xC1] = 0;
	instructions[0xC2]	=	"JP NZ, %1";					instructionsParameter[0xC2] = &Disassembler::Get16;
	instructions[0xC3]	=	"JP %1";						instructionsParameter[0xC3] = &Disassembler::Get16;
	instructions[0xC4]	=	"CALL NZ, %1";					instructionsParameter[0xC4] = &Disassembler::Get16;
	instructions[0xC5]	=	"PUSH BC";						instructionsParameter[0xC5] = 0;
	instructions[0xC6]	=	"ADD A, %1";					instructionsParameter[0xC6] = &Disassembler::GetU8;
	instructions[0xC7]	=	"RST 0 (0X000)";				instructionsParameter[0xC7] = 0;
	instructions[0xC8]	=	"RET Z";						instructionsParameter[0xC8] = 0;
	instructions[0xC9]	=	"RET";							instructionsParameter[0xC9] = 0;
	instructions[0xCA]	=	"JP Z, %1";						instructionsParameter[0xCA] = &Disassembler::Get16;
	instructions[0xCB]	=	"UNUSED";						instructionsParameter[0xCB] = 0;
	instructions[0xCC]	=	"CALL Z, %1";					instructionsParameter[0xCC] = &Disassembler::Get16;
	instructions[0xCD]	=	"CALL %1";						instructionsParameter[0xCD] = &Disassembler::Get16;
	instructions[0xCE]	=	"ADC A, %1";					instructionsParameter[0xCE] = &Disassembler::GetU8;
	instructions[0xCF]	=	"RST 8 (0x0008)";				instructionsParameter[0xCF] = 0;

	instructions[0xD0]	=	"RET NC";						instructionsParameter[0xD0] = 0;
	instructions[0xD1]	=	"POP DE";						instructionsParameter[0xD1] = 0;
	instructions[0xD2]	=	"JP NC, %1";					instructionsParameter[0xD2] = &Disassembler::Get16;
	instructions[0xD3]	=	"UNSUPPORTED (D3)";				instructionsParameter[0xD3] = 0;
	instructions[0xD4]	=	"CALL NC, %1";					instructionsParameter[0xD4] = &Disassembler::Get16;
	instructions[0xD5]	=	"PUSH DE";						instructionsParameter[0xD5] = 0;
	instructions[0xD6]	=	"SUB {A}, %1";					instructionsParameter[0xD6] = &Disassembler::Get16;
	instructions[0xD7]	=	"RST 2 (0x0010)";				instructionsParameter[0xD7] = 0;
	instructions[0xD8]	=	"RET C";						instructionsParameter[0xD8] = 0;
	instructions[0xD9]	=	"RETI";							instructionsParameter[0xD9] = 0;
	instructions[0xDA]	=	"JP C, %1";						instructionsParameter[0xDA] = &Disassembler::Get16;
	instructions[0xDB]	=	"UNSUPPORTED (DB)";				instructionsParameter[0xDB] = 0;
	instructions[0xDC]	=	"CALL C, %1";					instructionsParameter[0xDC] = &Disassembler::Get16;
	instructions[0xDD]	=	"UNSUPPORTED (DD)";				instructionsParameter[0xDD] = 0;
	instructions[0xDE]	=	"SBC A, %1";					instructionsParameter[0xDE] = &Disassembler::GetU8;
	instructions[0xDF]	=	"RST 3 (0x0018)";				instructionsParameter[0xDF] = 0;

	instructions[0xE0]	=	"LD (%1), A";					instructionsParameter[0xE0] = &Disassembler::GetS8_FF00;
	instructions[0xE1]	=	"POP HL";						instructionsParameter[0xE1] = 0;
	instructions[0xE2]	=	"LD (C),_A";					instructionsParameter[0xE2] = 0;
	instructions[0xE3]	=	"UNSUPPORTED (E3)";				instructionsParameter[0xE3] = 0;
	instructions[0xE4]	=	"UNSUPPORTED (E4)";				instructionsParameter[0xE4] = 0;
	instructions[0xE5]	=	"PUSH HL";						instructionsParameter[0xE5] = 0;
	instructions[0xE6]	=	"AND {A}, %1";					instructionsParameter[0xE6] = &Disassembler::GetU8;
	instructions[0xE7]	=	"RST 4 (0x0020)";				instructionsParameter[0xE7] = 0;
	instructions[0xE8]	=	"ADD SP, %1";					instructionsParameter[0xE8] = &Disassembler::GetS8;
	instructions[0xE9]	=	"JP (HL)";						instructionsParameter[0xE9] = 0;
	instructions[0xEA]	=	"LD (%1), A";					instructionsParameter[0xEA] = &Disassembler::Get16;
	instructions[0xEB]	=	"UNSUPPORTED (EB)";				instructionsParameter[0xEB] = 0;
	instructions[0xEC]	=	"UNSUPPORTED (EC)";				instructionsParameter[0xEC] = 0;
	instructions[0xED]	=	"UNSUPPORTED (ED)";				instructionsParameter[0xED] = 0;
	instructions[0xEE]	=	"XOR {A}, %1";					instructionsParameter[0xEE] = &Disassembler::GetU8;
	instructions[0xEF]	=	"RST 5 (0x0028)";				instructionsParameter[0xEF] = 0;

	instructions[0xF0]	=	"LD A, (%1)";					instructionsParameter[0xF0] = &Disassembler::GetS8_FF00;
	instructions[0xF1]	=	"POP AF";						instructionsParameter[0xF1] = 0;
	instructions[0xF2]	=	"LD A ,(C)";					instructionsParameter[0xF2] = 0;
	instructions[0xF3]	=	"DI";							instructionsParameter[0xF3] = 0;
	instructions[0xF4]	=	"UNSUPPORTED (F4)";				instructionsParameter[0xF4] = 0;
	instructions[0xF5]	=	"PUSH AF";						instructionsParameter[0xF5] = 0;
	instructions[0xF6]	=	"OR {A}, %1";					instructionsParameter[0xF6] = &Disassembler::GetU8;
	instructions[0xF7]	=	"RST 6 (0x0030)";				instructionsParameter[0xF7] = 0;
	instructions[0xF8]	=	"LD	HL SP+%1";					instructionsParameter[0xF8] = &Disassembler::GetS8;
	instructions[0xF9]	=	"LD SP, HL";					instructionsParameter[0xF9] = 0;
	instructions[0xFA]	=	"LD A, (%1)";					instructionsParameter[0xFA] = &Disassembler::Get16;
	instructions[0xFB]	=	"EI";							instructionsParameter[0xFB] = 0;
	instructions[0xFC]	=	"UNSUPPORTED (FC)";				instructionsParameter[0xFC] = 0;
	instructions[0xFD]	=	"UNSUPPORTED (FD)";				instructionsParameter[0xFD] = 0;
	instructions[0xFE]	=	"CP A, %1";						instructionsParameter[0xFE] = &Disassembler::GetU8;
	instructions[0xFF]	=	"RST 7 (0x0038)";				instructionsParameter[0xFF] = 0;

	//---------------------------------------
	//---------------------------------------
	cbInstructions[0x00]	=	"RLC B";
	cbInstructions[0x01]	=	"RLC C";
	cbInstructions[0x02]	=	"RLC D";
	cbInstructions[0x03]	=	"RLC E";
	cbInstructions[0x04]	=	"RLC H";
	cbInstructions[0x05]	=	"RLC L";
	cbInstructions[0x06]	=	"RLC (HL)";
	cbInstructions[0x07]	=	"RLC A";
	cbInstructions[0x08]	=	"RRC B";
	cbInstructions[0x09]	=	"RRC C";
	cbInstructions[0x0A]	=	"RRC D";
	cbInstructions[0x0B]	=	"RRC E";
	cbInstructions[0x0C]	=	"RRC H";
	cbInstructions[0x0D]	=	"RRC L";
	cbInstructions[0x0E]	=	"RRC (HL)";
	cbInstructions[0x0F]	=	"RRC A";

	cbInstructions[0x10]	=	"RL B";
	cbInstructions[0x11]	=	"RL C";
	cbInstructions[0x12]	=	"RL D";
	cbInstructions[0x13]	=	"RL E";
	cbInstructions[0x14]	=	"RL H";
	cbInstructions[0x15]	=	"RL L";
	cbInstructions[0x16]	=	"RL (HL)";
	cbInstructions[0x17]	=	"RL A";
	cbInstructions[0x18]	=	"RR B";
	cbInstructions[0x19]	=	"RR C";
	cbInstructions[0x1A]	=	"RR D";
	cbInstructions[0x1B]	=	"RR E";
	cbInstructions[0x1C]	=	"RR H";
	cbInstructions[0x1D]	=	"RR L";
	cbInstructions[0x1E]	=	"RR (HL)";
	cbInstructions[0x1F]	=	"RR A";

	cbInstructions[0x20]	=	"SLA B";
	cbInstructions[0x21]	=	"SLA C";
	cbInstructions[0x22]	=	"SLA D";
	cbInstructions[0x23]	=	"SLA E";
	cbInstructions[0x24]	=	"SLA H";
	cbInstructions[0x25]	=	"SLA L";
	cbInstructions[0x26]	=	"SLA (HL)";
	cbInstructions[0x27]	=	"SLA A";
	cbInstructions[0x28]	=	"SRA B";
	cbInstructions[0x29]	=	"SRA C";
	cbInstructions[0x2A]	=	"SRA D";
	cbInstructions[0x2B]	=	"SRA E";
	cbInstructions[0x2C]	=	"SRA H";
	cbInstructions[0x2D]	=	"SRA L";
	cbInstructions[0x2E]	=	"SRA (HL)";
	cbInstructions[0x2F]	=	"SRA A";

	cbInstructions[0x30]	=	"SWAP B";
	cbInstructions[0x31]	=	"SWAP C";
	cbInstructions[0x32]	=	"SWAP D";
	cbInstructions[0x33]	=	"SWAP E";
	cbInstructions[0x34]	=	"SWAP H";
	cbInstructions[0x35]	=	"SWAP L";
	cbInstructions[0x36]	=	"SWAP (HL)";
	cbInstructions[0x37]	=	"SWAP A";
	cbInstructions[0x38]	=	"SRL B";
	cbInstructions[0x39]	=	"SRL C";
	cbInstructions[0x3A]	=	"SRL D";
	cbInstructions[0x3B]	=	"SRL E";
	cbInstructions[0x3C]	=	"SRL H";
	cbInstructions[0x3D]	=	"SRL L";
	cbInstructions[0x3E]	=	"SRL (HL)";
	cbInstructions[0x3F]	=	"SRL A";

	cbInstructions[0x40]	=	"BIT 0, B";
	cbInstructions[0x41]	=	"BIT 0, C";
	cbInstructions[0x42]	=	"BIT 0, D";
	cbInstructions[0x43]	=	"BIT 0, E";
	cbInstructions[0x44]	=	"BIT 0, H";
	cbInstructions[0x45]	=	"BIT 0, L";
	cbInstructions[0x46]	=	"BIT 0, (HL)";
	cbInstructions[0x47]	=	"BIT 0, A";
	cbInstructions[0x48]	=	"BIT 1, B";
	cbInstructions[0x49]	=	"BIT 1, C";
	cbInstructions[0x4A]	=	"BIT 1, D";
	cbInstructions[0x4B]	=	"BIT 1, E";
	cbInstructions[0x4C]	=	"BIT 1, H";
	cbInstructions[0x4D]	=	"BIT 1, L";
	cbInstructions[0x4E]	=	"BIT 1, (HL)";
	cbInstructions[0x4F]	=	"BIT 1, A";

	cbInstructions[0x50]	=	"BIT 2, B";
	cbInstructions[0x51]	=	"BIT 2, C";
	cbInstructions[0x52]	=	"BIT 2, D";
	cbInstructions[0x53]	=	"BIT 2, E";
	cbInstructions[0x54]	=	"BIT 2, H";
	cbInstructions[0x55]	=	"BIT 2, L";
	cbInstructions[0x56]	=	"BIT 2, (HL)";
	cbInstructions[0x57]	=	"BIT 2, A";
	cbInstructions[0x58]	=	"BIT 3, B";
	cbInstructions[0x59]	=	"BIT 3, C";
	cbInstructions[0x5A]	=	"BIT 3, D";
	cbInstructions[0x5B]	=	"BIT 3, E";
	cbInstructions[0x5C]	=	"BIT 3, H";
	cbInstructions[0x5D]	=	"BIT 3, L";
	cbInstructions[0x5E]	=	"BIT 3, (HL)";
	cbInstructions[0x5F]	=	"BIT 3, A";

	cbInstructions[0x60]	=	"BIT 4, B";
	cbInstructions[0x61]	=	"BIT 4, C";
	cbInstructions[0x62]	=	"BIT 4, D";
	cbInstructions[0x63]	=	"BIT 4, E";
	cbInstructions[0x64]	=	"BIT 4, H";
	cbInstructions[0x65]	=	"BIT 4, L";
	cbInstructions[0x66]	=	"BIT 4, (HL)";
	cbInstructions[0x67]	=	"BIT 4, A";
	cbInstructions[0x68]	=	"BIT 5, B";
	cbInstructions[0x69]	=	"BIT 5, C";
	cbInstructions[0x6A]	=	"BIT 5, D";
	cbInstructions[0x6B]	=	"BIT 5, E";
	cbInstructions[0x6C]	=	"BIT 5, H";
	cbInstructions[0x6D]	=	"BIT 5, L";
	cbInstructions[0x6E]	=	"BIT 5, (HL)";
	cbInstructions[0x6F]	=	"BIT 5, A";

	cbInstructions[0x70]	=	"BIT 6, B";
	cbInstructions[0x71]	=	"BIT 6, C";
	cbInstructions[0x72]	=	"BIT 6, D";
	cbInstructions[0x73]	=	"BIT 6, E";
	cbInstructions[0x74]	=	"BIT 6, H";
	cbInstructions[0x75]	=	"BIT 6, L";
	cbInstructions[0x76]	=	"BIT 6, (HL)";
	cbInstructions[0x77]	=	"BIT 6, A";
	cbInstructions[0x78]	=	"BIT 7, B";
	cbInstructions[0x79]	=	"BIT 7, C";
	cbInstructions[0x7A]	=	"BIT 7, D";
	cbInstructions[0x7B]	=	"BIT 7, E";
	cbInstructions[0x7C]	=	"BIT 7, H";
	cbInstructions[0x7D]	=	"BIT 7, L";
	cbInstructions[0x7E]	=	"BIT 7, (HL)";
	cbInstructions[0x7F]	=	"BIT 7, A";

	cbInstructions[0x80]	=	"RES 0, B";
	cbInstructions[0x81]	=	"RES 0, C";
	cbInstructions[0x82]	=	"RES 0, D";
	cbInstructions[0x83]	=	"RES 0, E";
	cbInstructions[0x84]	=	"RES 0, H";
	cbInstructions[0x85]	=	"RES 0, L";
	cbInstructions[0x86]	=	"RES 0, (HL)";
	cbInstructions[0x87]	=	"RES 0, A";
	cbInstructions[0x88]	=	"RES 1, B";
	cbInstructions[0x89]	=	"RES 1, C";
	cbInstructions[0x8A]	=	"RES 1, D";
	cbInstructions[0x8B]	=	"RES 1, E";
	cbInstructions[0x8C]	=	"RES 1, H";
	cbInstructions[0x8D]	=	"RES 1, L";
	cbInstructions[0x8E]	=	"RES 1, (HL)";
	cbInstructions[0x8F]	=	"RES 1, A";

	cbInstructions[0x90]	=	"RES 2, B";
	cbInstructions[0x91]	=	"RES 2, C";
	cbInstructions[0x92]	=	"RES 2, D";
	cbInstructions[0x93]	=	"RES 2, E";
	cbInstructions[0x94]	=	"RES 2, H";
	cbInstructions[0x95]	=	"RES 2, L";
	cbInstructions[0x96]	=	"RES 2, (HL)";
	cbInstructions[0x97]	=	"RES 2, A";
	cbInstructions[0x98]	=	"RES 3, B";
	cbInstructions[0x99]	=	"RES 3, C";
	cbInstructions[0x9A]	=	"RES 3, D";
	cbInstructions[0x9B]	=	"RES 3, E";
	cbInstructions[0x9C]	=	"RES 3, H";
	cbInstructions[0x9D]	=	"RES 3, L";
	cbInstructions[0x9E]	=	"RES 3, (HL)";
	cbInstructions[0x9F]	=	"RES 3, A";

	cbInstructions[0xA0]	=	"RES 4, B";
	cbInstructions[0xA1]	=	"RES 4, C";
	cbInstructions[0xA2]	=	"RES 4, D";
	cbInstructions[0xA3]	=	"RES 4, E";
	cbInstructions[0xA4]	=	"RES 4, H";
	cbInstructions[0xA5]	=	"RES 4, L";
	cbInstructions[0xA6]	=	"RES 4, (HL)";
	cbInstructions[0xA7]	=	"RES 4, A";
	cbInstructions[0xA8]	=	"RES 5, B";
	cbInstructions[0xA9]	=	"RES 5, C";
	cbInstructions[0xAA]	=	"RES 5, D";
	cbInstructions[0xAB]	=	"RES 5, E";
	cbInstructions[0xAC]	=	"RES 5, H";
	cbInstructions[0xAD]	=	"RES 5, L";
	cbInstructions[0xAE]	=	"RES 5, (HL)";
	cbInstructions[0xAF]	=	"RES 5, A";

	cbInstructions[0xB0]	=	"RES 6, B";
	cbInstructions[0xB1]	=	"RES 6, C";
	cbInstructions[0xB2]	=	"RES 6, D";
	cbInstructions[0xB3]	=	"RES 6, E";
	cbInstructions[0xB4]	=	"RES 6, H";
	cbInstructions[0xB5]	=	"RES 6, L";
	cbInstructions[0xB6]	=	"RES 6, (HL)";
	cbInstructions[0xB7]	=	"RES 6, A";
	cbInstructions[0xB8]	=	"RES 7, B";
	cbInstructions[0xB9]	=	"RES 7, C";
	cbInstructions[0xBA]	=	"RES 7, D";
	cbInstructions[0xBB]	=	"RES 7, E";
	cbInstructions[0xBC]	=	"RES 7, H";
	cbInstructions[0xBD]	=	"RES 7, L";
	cbInstructions[0xBE]	=	"RES 7, (HL)";
	cbInstructions[0xBF]	=	"RES 7, A";

	cbInstructions[0xC0]	=	"SET 0, B";
	cbInstructions[0xC1]	=	"SET 0, C";
	cbInstructions[0xC2]	=	"SET 0, D";
	cbInstructions[0xC3]	=	"SET 0, E";
	cbInstructions[0xC4]	=	"SET 0, H";
	cbInstructions[0xC5]	=	"SET 0, L";
	cbInstructions[0xC6]	=	"SET 0, (HL)";
	cbInstructions[0xC7]	=	"SET 0, A";
	cbInstructions[0xC8]	=	"SET 1, B";
	cbInstructions[0xC9]	=	"SET 1, C";
	cbInstructions[0xCA]	=	"SET 1, D";
	cbInstructions[0xCB]	=	"SET 1, E";
	cbInstructions[0xCC]	=	"SET 1, H";
	cbInstructions[0xCD]	=	"SET 1, L";
	cbInstructions[0xCE]	=	"SET 1, (HL)";
	cbInstructions[0xCF]	=	"SET 1, A";

	cbInstructions[0xD0]	=	"SET 2, B";
	cbInstructions[0xD1]	=	"SET 2, C";
	cbInstructions[0xD2]	=	"SET 2, D";
	cbInstructions[0xD3]	=	"SET 2, E";
	cbInstructions[0xD4]	=	"SET 2, H";
	cbInstructions[0xD5]	=	"SET 2, L";
	cbInstructions[0xD6]	=	"SET 2, (HL)";
	cbInstructions[0xD7]	=	"SET 2, A";
	cbInstructions[0xD8]	=	"SET 3, B";
	cbInstructions[0xD9]	=	"SET 3, C";
	cbInstructions[0xDA]	=	"SET 3, D";
	cbInstructions[0xDB]	=	"SET 3, E";
	cbInstructions[0xDC]	=	"SET 3, H";
	cbInstructions[0xDD]	=	"SET 3, L";
	cbInstructions[0xDE]	=	"SET 3, (HL)";
	cbInstructions[0xDF]	=	"SET 3, A";

	cbInstructions[0xE0]	=	"SET 4, B";
	cbInstructions[0xE1]	=	"SET 4, C";
	cbInstructions[0xE2]	=	"SET 4, D";
	cbInstructions[0xE3]	=	"SET 4, E";
	cbInstructions[0xE4]	=	"SET 4, H";
	cbInstructions[0xE5]	=	"SET 4, L";
	cbInstructions[0xE6]	=	"SET 4, (HL)";
	cbInstructions[0xE7]	=	"SET 4, A";
	cbInstructions[0xE8]	=	"SET 5, B";
	cbInstructions[0xE9]	=	"SET 5, C";
	cbInstructions[0xEA]	=	"SET 5, D";
	cbInstructions[0xEB]	=	"SET 5, E";
	cbInstructions[0xEC]	=	"SET 5, H";
	cbInstructions[0xED]	=	"SET 5, L";
	cbInstructions[0xEE]	=	"SET 5, (HL)";
	cbInstructions[0xEF]	=	"SET 5, A";

	cbInstructions[0xF0]	=	"SET 6, B";
	cbInstructions[0xF1]	=	"SET 6, C";
	cbInstructions[0xF2]	=	"SET 6, D";
	cbInstructions[0xF3]	=	"SET 6, E";
	cbInstructions[0xF4]	=	"SET 6, H";
	cbInstructions[0xF5]	=	"SET 6, L";
	cbInstructions[0xF6]	=	"SET 6, (HL)";
	cbInstructions[0xF7]	=	"SET 6, A";
	cbInstructions[0xF8]	=	"SET 7, B";
	cbInstructions[0xF9]	=	"SET 7, C";
	cbInstructions[0xFA]	=	"SET 7, D";
	cbInstructions[0xFB]	=	"SET 7, E";
	cbInstructions[0xFC]	=	"SET 7, H";
	cbInstructions[0xFD]	=	"SET 7, L";
	cbInstructions[0xFE]	=	"SET 7, (HL)";
	cbInstructions[0xFF]	=	"SET 7, A";


	//---------------------------------------
	//---------------------------------------
	jumpIntructions[0x00]	=	false;
	jumpIntructions[0x01]	=	false;
	jumpIntructions[0x02]	=	false;
	jumpIntructions[0x03]	=	false;
	jumpIntructions[0x04]	=	false;
	jumpIntructions[0x05]	=	false;
	jumpIntructions[0x06]	=	false;
	jumpIntructions[0x07]	=	false;
	jumpIntructions[0x08]	=	false;
	jumpIntructions[0x09]	=	false;
	jumpIntructions[0x0A]	=	false;
	jumpIntructions[0x0B]	=	false;
	jumpIntructions[0x0C]	=	false;
	jumpIntructions[0x0D]	=	false;
	jumpIntructions[0x0E]	=	false;
	jumpIntructions[0x0F]	=	false;

	jumpIntructions[0x10]	=	false;
	jumpIntructions[0x11]	=	false;
	jumpIntructions[0x12]	=	false;
	jumpIntructions[0x13]	=	false;
	jumpIntructions[0x14]	=	false;
	jumpIntructions[0x15]	=	false;
	jumpIntructions[0x16]	=	false;
	jumpIntructions[0x17]	=	false;
	jumpIntructions[0x18]	=	true;//"JR %1";						
	jumpIntructions[0x19]	=	false;
	jumpIntructions[0x1A]	=	false;
	jumpIntructions[0x1B]	=	false;
	jumpIntructions[0x1C]	=	false;
	jumpIntructions[0x1D]	=	false;
	jumpIntructions[0x1E]	=	false;
	jumpIntructions[0x1F]	=	false;

	jumpIntructions[0x20]	=	true;//"JR NZ, %1";					
	jumpIntructions[0x21]	=	false;
	jumpIntructions[0x22]	=	false;
	jumpIntructions[0x23]	=	false;
	jumpIntructions[0x24]	=	false;
	jumpIntructions[0x25]	=	false;
	jumpIntructions[0x26]	=	false;
	jumpIntructions[0x27]	=	false;
	jumpIntructions[0x28]	=	true;//"JR Z, %1";						
	jumpIntructions[0x29]	=	false;
	jumpIntructions[0x2A]	=	false;
	jumpIntructions[0x2B]	=	false;
	jumpIntructions[0x2C]	=	false;
	jumpIntructions[0x2D]	=	false;
	jumpIntructions[0x2E]	=	false;;
	jumpIntructions[0x2F]	=	false;

	jumpIntructions[0x30]	=	true;//"JR NC, %1";		
	jumpIntructions[0x31]	=	false;
	jumpIntructions[0x32]	=	false;
	jumpIntructions[0x33]	=	false;
	jumpIntructions[0x34]	=	false;
	jumpIntructions[0x35]	=	false;
	jumpIntructions[0x36]	=	false;
	jumpIntructions[0x37]	=	false;
	jumpIntructions[0x38]	=	true;//"JR C, %1";		
	jumpIntructions[0x39]	=	false;
	jumpIntructions[0x3A]	=	false;
	jumpIntructions[0x3B]	=	false;
	jumpIntructions[0x3C]	=	false;
	jumpIntructions[0x3D]	=	false;
	jumpIntructions[0x3E]	=	false;
	jumpIntructions[0x3F]	=	false;

	jumpIntructions[0x40]	=	false;
	jumpIntructions[0x41]	=	false;
	jumpIntructions[0x42]	=	false;
	jumpIntructions[0x43]	=	false;
	jumpIntructions[0x44]	=	false;
	jumpIntructions[0x45]	=	false;
	jumpIntructions[0x46]	=	false;
	jumpIntructions[0x47]	=	false;
	jumpIntructions[0x48]	=	false;
	jumpIntructions[0x49]	=	false;
	jumpIntructions[0x4A]	=	false;
	jumpIntructions[0x4B]	=	false;
	jumpIntructions[0x4C]	=	false;
	jumpIntructions[0x4D]	=	false;
	jumpIntructions[0x4E]	=	false;
	jumpIntructions[0x4F]	=	false;

	jumpIntructions[0x50]	=	false;
	jumpIntructions[0x51]	=	false;
	jumpIntructions[0x52]	=	false;
	jumpIntructions[0x53]	=	false;
	jumpIntructions[0x54]	=	false;
	jumpIntructions[0x55]	=	false;
	jumpIntructions[0x56]	=	false;
	jumpIntructions[0x57]	=	false;
	jumpIntructions[0x58]	=	false;
	jumpIntructions[0x59]	=	false;
	jumpIntructions[0x5A]	=	false;
	jumpIntructions[0x5B]	=	false;
	jumpIntructions[0x5C]	=	false;
	jumpIntructions[0x5D]	=	false;
	jumpIntructions[0x5E]	=	false;
	jumpIntructions[0x5F]	=	false;

	jumpIntructions[0x60]	=	false;
	jumpIntructions[0x61]	=	false;
	jumpIntructions[0x62]	=	false;
	jumpIntructions[0x63]	=	false;
	jumpIntructions[0x64]	=	false;
	jumpIntructions[0x65]	=	false;
	jumpIntructions[0x66]	=	false;
	jumpIntructions[0x67]	=	false;
	jumpIntructions[0x68]	=	false;
	jumpIntructions[0x69]	=	false;
	jumpIntructions[0x6A]	=	false;
	jumpIntructions[0x6B]	=	false;
	jumpIntructions[0x6C]	=	false;
	jumpIntructions[0x6D]	=	false;
	jumpIntructions[0x6E]	=	false;
	jumpIntructions[0x6F]	=	false;

	jumpIntructions[0x70]	=	false;
	jumpIntructions[0x71]	=	false;
	jumpIntructions[0x72]	=	false;
	jumpIntructions[0x73]	=	false;
	jumpIntructions[0x74]	=	false;
	jumpIntructions[0x75]	=	false;
	jumpIntructions[0x76]	=	false;
	jumpIntructions[0x77]	=	false;
	jumpIntructions[0x78]	=	false;
	jumpIntructions[0x79]	=	false;
	jumpIntructions[0x7A]	=	false;
	jumpIntructions[0x7B]	=	false;
	jumpIntructions[0x7C]	=	false;
	jumpIntructions[0x7D]	=	false;
	jumpIntructions[0x7E]	=	false;
	jumpIntructions[0x7F]	=	false;

	jumpIntructions[0x80]	=	false;
	jumpIntructions[0x81]	=	false;
	jumpIntructions[0x82]	=	false;
	jumpIntructions[0x83]	=	false;
	jumpIntructions[0x84]	=	false;
	jumpIntructions[0x85]	=	false;
	jumpIntructions[0x86]	=	false;
	jumpIntructions[0x87]	=	false;
	jumpIntructions[0x88]	=	false;
	jumpIntructions[0x89]	=	false;
	jumpIntructions[0x8A]	=	false;
	jumpIntructions[0x8B]	=	false;
	jumpIntructions[0x8C]	=	false;
	jumpIntructions[0x8D]	=	false;
	jumpIntructions[0x8E]	=	false;
	jumpIntructions[0x8F]	=	false;

	jumpIntructions[0x90]	=	false;
	jumpIntructions[0x91]	=	false;
	jumpIntructions[0x92]	=	false;
	jumpIntructions[0x93]	=	false; 
	jumpIntructions[0x94]	=	false;
	jumpIntructions[0x95]	=	false;
	jumpIntructions[0x96]	=	false;
	jumpIntructions[0x97]	=	false;
	jumpIntructions[0x98]	=	false;
	jumpIntructions[0x99]	=	false;
	jumpIntructions[0x9A]	=	false;				
	jumpIntructions[0x9B]	=	false;
	jumpIntructions[0x9C]	=	false;
	jumpIntructions[0x9D]	=	false;
	jumpIntructions[0x9E]	=   false;
	jumpIntructions[0x9F]	=	false;

	jumpIntructions[0xA0]	=	false;
	jumpIntructions[0xA1]	=	false;
	jumpIntructions[0xA2]	=	false;
	jumpIntructions[0xA3]	=	false;
	jumpIntructions[0xA4]	=	false;
	jumpIntructions[0xA5]	=	false;
	jumpIntructions[0xA6]	=	false;
	jumpIntructions[0xA7]	=	false;
	jumpIntructions[0xA8]	=	false;
	jumpIntructions[0xA9]	=	false;
	jumpIntructions[0xAA]	=	false;
	jumpIntructions[0xAB]	=	false;
	jumpIntructions[0xAC]	=	false;
	jumpIntructions[0xAD]	=	false;
	jumpIntructions[0xAE]	=	false;
	jumpIntructions[0xAF]	=	false;

	jumpIntructions[0xB0]	=	false;
	jumpIntructions[0xB1]	=	false;
	jumpIntructions[0xB2]	=	false;
	jumpIntructions[0xB3]	=	false;
	jumpIntructions[0xB4]	=	false;
	jumpIntructions[0xB5]	=	false;
	jumpIntructions[0xB6]	=	false;
	jumpIntructions[0xB7]	=	false;
	jumpIntructions[0xB8]	=	false;
	jumpIntructions[0xB9]	=	false;
	jumpIntructions[0xBA]	=	false;
	jumpIntructions[0xBB]	=	false;
	jumpIntructions[0xBC]	=	false;
	jumpIntructions[0xBD]	=	false;
	jumpIntructions[0xBE]	=	false;
	jumpIntructions[0xBF]	=	false;

	jumpIntructions[0xC0]	=	false;
	jumpIntructions[0xC1]	=	false;
	jumpIntructions[0xC2]	=	true;//"JP NZ, %1";					
	jumpIntructions[0xC3]	=	true;//"JP %1";						
	jumpIntructions[0xC4]	=	true;//"CALL NZ, %1";					
	jumpIntructions[0xC5]	=	false;
	jumpIntructions[0xC6]	=	false;
	jumpIntructions[0xC7]	=	false;
	jumpIntructions[0xC8]	=	false;
	jumpIntructions[0xC9]	=	false;
	jumpIntructions[0xCA]	=	true;//"JP Z, %1";						
	jumpIntructions[0xCB]	=	false;
	jumpIntructions[0xCC]	=	false;
	jumpIntructions[0xCD]	=	false;
	jumpIntructions[0xCE]	=	false;
	jumpIntructions[0xCF]	=	false;

	jumpIntructions[0xD0]	=	false;
	jumpIntructions[0xD1]	=	false;
	jumpIntructions[0xD2]	=	true;//"JP NC, %1";
	jumpIntructions[0xD3]	=	false;
	jumpIntructions[0xD4]	=	true;//"CALL NC, %1";
	jumpIntructions[0xD5]	=	false;
	jumpIntructions[0xD6]	=	false;
	jumpIntructions[0xD7]	=	false;
	jumpIntructions[0xD8]	=	false;
	jumpIntructions[0xD9]	=	false;
	jumpIntructions[0xDA]	=	true;//"JP C, %1";						
	jumpIntructions[0xDB]	=	false;
	jumpIntructions[0xDC]	=	true;//"CALL C, %1";					
	jumpIntructions[0xDD]	=	false;
	jumpIntructions[0xDE]	=	false;
	jumpIntructions[0xDF]	=	false;

	jumpIntructions[0xE0]	=	false;
	jumpIntructions[0xE1]	=	false;
	jumpIntructions[0xE2]	=	false;
	jumpIntructions[0xE3]	=	false;
	jumpIntructions[0xE4]	=	false;
	jumpIntructions[0xE5]	=	false;
	jumpIntructions[0xE6]	=	false;
	jumpIntructions[0xE7]	=	false;
	jumpIntructions[0xE8]	=	false;
	jumpIntructions[0xE9]	=	true;// "JP (HL)";
	jumpIntructions[0xEA]	=	false;
	jumpIntructions[0xEB]	=	false;
	jumpIntructions[0xEC]	=	false;
	jumpIntructions[0xED]	=	false;
	jumpIntructions[0xEE]	=	false;
	jumpIntructions[0xEF]	=	false;

	jumpIntructions[0xF0]	=	false;
	jumpIntructions[0xF1]	=	false;
	jumpIntructions[0xF2]	=	false;
	jumpIntructions[0xF3]	=	false;
	jumpIntructions[0xF4]	=	false;
	jumpIntructions[0xF5]	=	false;
	jumpIntructions[0xF6]	=	false;
	jumpIntructions[0xF7]	=	false;
	jumpIntructions[0xF8]	=	false;
	jumpIntructions[0xF9]	=	false;
	jumpIntructions[0xFA]	=	false;
	jumpIntructions[0xFB]	=	false;
	jumpIntructions[0xFC]	=	false;
	jumpIntructions[0xFD]	=	false;
	jumpIntructions[0xFE]	=	false;
	jumpIntructions[0xFF]	=	false;
}

