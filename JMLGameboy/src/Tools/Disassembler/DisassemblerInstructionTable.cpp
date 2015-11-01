#include "DisassemblerInstructionTable.h"
#include "../Debugger/Globals.h"
#include "Mnemonics.h"
#include <string>

const char* INVALID_INSTRUCTION	= "<INVALID_INSTRUCTION>";
const char* INVALID_REG		= "<INVALID_REG>";
const char* INVALID_FLAG	= "<INVALID_FLAG>";

DisassemblerInstructionTable::DisassemblerInstructionTable()
{
	LoadInstructions();
}

DisassemblerInstruction DisassemblerInstructionTable::Disassemble(MemoryController* memoryController, WORD address)
{
	BYTE opcode = memoryController->ReadMemory(address);
	return (this->*instructions[opcode])(memoryController, address, opcode);
}

const char* DisassemblerInstructionTable::Get_r(BYTE code)
{
	switch(code)
	{
	case 0x07:
		return REG_NAME_A;
	case 0x00:
		return REG_NAME_B;
	case 0x01:
		return REG_NAME_C;
	case 0x02:
		return REG_NAME_D;
	case 0x03:
		return REG_NAME_E;
	case 0x04:
		return REG_NAME_H;
	case 0x05:
		return REG_NAME_L;
	}


	return INVALID_REG;
}

const char* DisassemblerInstructionTable::Get_qq(BYTE code)
{
	switch(code)
	{
	case 0x00:
		return REG_NAME_BC;
	case 0x01:
		return REG_NAME_DE;
	case 0x02:
		return REG_NAME_HL;
	case 0x03:
		return REG_NAME_AF;
	}

	return INVALID_REG;
}

const char* DisassemblerInstructionTable::Get_dd(BYTE code)
{
	switch(code)
	{
	case 0x00:
		return REG_NAME_BC;
	case 0x01:
		return REG_NAME_DE;
	case 0x02:
		return REG_NAME_HL;
	case 0x03:
		return REG_NAME_SP;
	}

	return INVALID_REG;
}

const char*  DisassemblerInstructionTable::GetFlag_cc(BYTE code)
{
	switch(code)
	{
	case 0x00:
		return MN_FLAG_NZ; //NZ
	case 0x01:
		return MN_FLAG_Z; //Z
	case 0x02:
		return MN_FLAG_NCY; //NCY
	case 0x03:
		return MN_FLAG_CY; //CY
	}

	return INVALID_FLAG;
}

//INSTRUCTIONS
DisassemblerInstruction DisassemblerInstructionTable::LD_R_R(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//10 [ r ] [ r']
	//Loads the content of register r2 in r1
	const char* r1 = Get_r((opcode >> 3) & 0x07);
	const char* r2 = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, %s", MN_LD, r1, r2);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::LD_R_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//00 [ r ] 110
	//Loads 8-Bit immediate data into register r

	const char* r = Get_r((opcode >> 3) & 0x07);
	BYTE n = MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = n;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, %d", MN_LD, r, n);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::LD_R_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 01 [ r ] 110
	//Loads the contents of memory (8-Bits) specified by register pair HL into register r

	const char* r = Get_r((opcode >> 3) & 0x07);
	
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %d, [%s]", MN_LD, r, REG_NAME_HL);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_$HL_R(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//01 110 [ r ]
	// Stores the contents of register R in memory specified by register pair HL

	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s [%s], %d", MN_LD, REG_NAME_HL, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::LD_$HL_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//01 110 110
	// Loads 8-Bits immediate data n info memory specified by register pair HL

	BYTE n = MemoryController::Shared()->ReadMemory(address + 1);
	
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = n;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s [%s], %d", MN_LD, REG_NAME_HL, n);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::LD_A_$BC(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//00 001 010
	// Loads the contents specified by the contents of register pair BC into register A
	BYTE n = MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, [%s]", MN_LD, REG_NAME_A, REG_NAME_BC);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_A_$DE(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//00 011 010
	// Loads the contents specified by the contents of register pair DE into register A
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, [%s]", MN_LD, REG_NAME_A, REG_NAME_DE);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::LD_A_$C(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 110 010
	// Loads into register A the contents of the internal RAM, port register, or mode register at the address in the range
	// 0xFF00-0xFFFF specified by register C
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, [%s]", MN_LD, REG_NAME_A, REG_NAME_C);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_$C_A(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 100 010
	// Loads the contents of register A in the internal RAM, port register, or mode register at the address in the range
	// 0xFF00-0xFFFF specified by register C
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s [%s], %s", MN_LD, REG_NAME_C, REG_NAME_A);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::LD_A_$n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 110 000
	// Loads into register A the contents of the internal RAM, port register, or mode register at the address in the range
	// 0xFF00-0xFFFF specified by the 8-Bits immediate operand n
	BYTE n = MemoryController::Shared()->ReadMemory(address + 1);
	
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = n;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, [%d]", MN_LD, REG_NAME_A, n);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_$n_A(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 100 000
	// Loads the contents of register A in the internal RAM, port register, or mode register at the address in the range
	// 0xFF00-0xFFFF specified by the 8-Bits immediate operand n
	BYTE n = MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = n;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s [%d], %s", MN_LD, n, REG_NAME_A);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_A_$nn(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 111 010
	// Loads into register A the contents of the internal RAM or register specified by 16-Bit immediate openrand nn
	WORD n1 = MemoryController::Shared()->ReadMemory(address + 2);
	WORD n2 = MemoryController::Shared()->ReadMemory(address + 1);

	WORD memAddress = (n1 << 8) | n2;

	
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 3;
	dI.bytes[0] = opcode;
	dI.bytes[1] = (BYTE)n2;
	dI.bytes[2] = (BYTE)n1;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, [%d]", MN_LD, REG_NAME_A, memAddress);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_$nn_A(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 101 010
	// Loads the contents of register A in the internal RAM, port register, or mode register specified by 16-Bits
	// immediate operand nn
	WORD n1 = MemoryController::Shared()->ReadMemory(address + 2);
	WORD n2 = MemoryController::Shared()->ReadMemory(address + 1);

	WORD memAddress = (n1 << 8) | n2;


	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 3;
	dI.bytes[0] = opcode;
	dI.bytes[1] = (BYTE)n2;
	dI.bytes[2] = (BYTE)n1;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s [%d], %s", MN_LD, memAddress, REG_NAME_A);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::LD_A_$HLI(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//00 101 010
	// Loads in register A the contents of memory specified by the contents of register pair HL and simultaneously
	// increments the conents of HL
	
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, [%s+]", MN_LD, REG_NAME_A, REG_NAME_HL);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_A_$HLD(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//00 111 010
	// Loads in register A the contents of memory specified by the contents of register pair HL and simultaneously
	// decrements the conents of HL
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, [%s-]", MN_LD, REG_NAME_A, REG_NAME_HL);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_$BC_A(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 000 010
	// Stores the contents of register A in the memory specified by register pair BC

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s [%s], %s", MN_LD, REG_NAME_BC, REG_NAME_A);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_$DE_A(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 010 010
	// Stores the contents of register A in the memory specified by register pair BC

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s [%s], %s", MN_LD, REG_NAME_DE, REG_NAME_A);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_$HLI_A(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 100 010
	// Stores the contents of register A in the memory specified by register pair HL and simultaneously
	// increments the contents of HL

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s [%s+], %s", MN_LD, REG_NAME_HL, REG_NAME_A);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_$HLD_A(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 110 010
	// Stores the contents of register A in the memory specified by register pair HL and simultaneously
	// decrements the contents of HL

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s [%s-], %s", MN_LD, REG_NAME_HL, REG_NAME_A);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_dd_nn(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//00 dd0 001
	// Loads 2 bytes of immediate data to register pair dd
	BYTE dd = (opcode >> 4) & 0x03;
	const char* dd_reg = Get_dd(dd);

	BYTE nl = MemoryController::Shared()->ReadMemory(address + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(address + 2);

	WORD word = nh;
	word <<= 8;
	word |= nh;

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 3;
	dI.bytes[0] = opcode;
	dI.bytes[1] = nl;
	dI.bytes[2] = nh;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, %d", MN_LD, dd_reg, word);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::LD_SP_HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 111 001
	// Loads the contents of register pair HL in stack pointer SP

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s, %s", MN_LD, REG_NAME_SP, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::PUSH_qq(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 qq0 101
	// Pushes the contents of register pair qq onto the memory stack.
	// First 1 is substracted from SP and the contents of the higher portion 
	// of qq are placed on the stack. The conetns of the lower portion of qq are then placed
	// on the stack. The contents of SP are automatically decremented by 2

	const char* reg_val = Get_qq((opcode >> 4) & 0x03);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s", MN_PUSH, reg_val);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::POP_qq(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 qq0 001
	// Pops contents from the memory stack into reguster pair qq
	// First the conetns of memory specified by the conents of SP are loeaded in the lower portion of qq. Next,
	// the contnets of SP are incremented by 1 and the contents of the memory they specify are loaded in the
	// upper portion of qq. The contents of SP are automatically incremented by 2.

	const char* reg_val = Get_qq((opcode >> 4) & 0x03);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);
	sprintf(dI.disass, "%s %s", MN_POP, reg_val);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::LDHL_PS_e(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 111 000
	// The 8-Bits operand e is added to SP and the result is stored in HL

	// char beacuse it must be signed
	char e = MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = e;
	dI.referencesAddresses.push_back(address + dI.size);
	
	if(e >= 0)
	{
		sprintf(dI.disass, "%s %s, %s +%d", MN_LD, e);
	}
	else
	{
		sprintf(dI.disass, "%s %s, %s -%d", MN_LD, -e);
	}


	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::LD_$nn_SP(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 001 000
	// Stores the lower byte SP at address nn specified by the 16-Bits immediate operand nn and the upper
	// byte of sp at address nn + 1

	BYTE nl = MemoryController::Shared()->ReadMemory(address + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(address + 2);

	WORD word = nh;
	word <<= 8;
	word |= nl;

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 3;
	dI.bytes[0] = opcode;
	dI.bytes[1] = nl;
	dI.bytes[2] = nh;
	dI.referencesAddresses.push_back(address + dI.size);

	
	sprintf(dI.disass, "%s [%d], %s", MN_LD, word, REG_NAME_SP);
	

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::ADD_A_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 000 [ r ]
	// Adds the contents of register r to those of register A and stores the result in register A
	BYTE rCode = opcode & 0x07;
	const char* r = Get_r(rCode);
	
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);


	sprintf(dI.disass, "%s %s, %s", MN_ADD, REG_NAME_A, r);


	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::ADD_A_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 000 110
	// Adds 8-Bits immediate operand n to the contetns of register A and stores the results in register A
	BYTE n = MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = n;
	dI.referencesAddresses.push_back(address + dI.size);


	sprintf(dI.disass, "%s %s, %d", MN_ADD, REG_NAME_A, n);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::ADD_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 000 110
	// Adds the contents of memory specified by the contents of register pair HL to the contents of register A
	// and stores the results in register A

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, [%s]", MN_ADD, REG_NAME_A, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::ADC_A_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 001 [ r ]
	// Adds the contents of operand r and CY to the contents of register A and stores the results in register A.
	BYTE rCode = opcode & 0x07;
	const char* r = Get_r(rCode);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_ADC, REG_NAME_A, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::ADC_A_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 110
	// Adds the contents of operand n and CY to the contents of register A and stores the results in register A.
	BYTE n = MemoryController::Shared()->ReadMemory(address + 1);
	
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_ADC, REG_NAME_A, n);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::ADC_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 001 110
	// Adds the contents of operand (HL) and CY to the contents of register A and stores the results in register A.
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, [%s]", MN_ADC, REG_NAME_A, REG_NAME_HL);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::SUB_A_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 010 [ r ]
	// Substracts the contents of register r from those of register A and stores the result in register A
	BYTE rCode = opcode & 0x07;
	const char* r = Get_r(rCode);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = 0x8E;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_SUB, REG_NAME_A, r);

	return dI;

}


DisassemblerInstruction DisassemblerInstructionTable::SUB_A_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 010 110
	// Substracts the contents of n from those of register A and stores the result in register A
	BYTE n = MemoryController::Shared()->ReadMemory(address + 1);
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = n;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_SUB, REG_NAME_A, n);

	return dI;

}


DisassemblerInstruction DisassemblerInstructionTable::SUB_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 010 110
	// Substracts the contents of HL from those of register A and stores the result in register A
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, [%s]", MN_SUB, REG_NAME_A, REG_NAME_HL);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::SBC_A_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 011 [ r ]
	// Substracts the contents of register r and CY from those of register A and stores the result in register A
	BYTE rCode = opcode & 0x07;
	const char* r = Get_r(rCode);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, [%s]", MN_SBC, REG_NAME_A, r);

	return dI;
}


DisassemblerInstruction DisassemblerInstructionTable::SBC_A_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 011 110
	// Substracts the contents of n and CY from those of register A and stores the result in register A
	BYTE n = MemoryController::Shared()->ReadMemory(address + 1);
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = n;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_SBC, REG_NAME_A, n);

	return dI;

}


DisassemblerInstruction DisassemblerInstructionTable::SBC_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 011 110
	// Substracts the contents of (HL) and CY from those of register A and stores the result in register A
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, [%s]", MN_SBC, REG_NAME_A, REG_NAME_HL);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::AND_A_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 100 [ r ]
	// Takes the logical-AND for each bit of the contents of register r and register A and stores the results
	// in regisger A
	BYTE rCode = opcode & 0x07;
	const char* r = Get_r(rCode);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_AND, REG_NAME_A, r);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::AND_A_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 100 110
	// Takes the logical-AND for each bit of the contents of n and register A and stores the results
	// in regisger A
	BYTE n= MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.bytes[0] = n;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_AND, REG_NAME_A, n);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::AND_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 100 110
	// Takes the logical-AND for each bit of the contents of (HL) and register A and stores the results
	// in regisger A
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, [%s]", MN_AND, REG_NAME_A, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::OR_A_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 110 [ r ]
	// Takes the logical-OR for each bit of the contents of register r and register A and stores the results
	// in regisger A
	
	BYTE rCode = opcode & 0x07;
	const char* r = Get_r(rCode);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_OR, REG_NAME_A, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::OR_A_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 110 110
	// Takes the logical-OR for each bit of the contents of n and register A and stores the results
	// in regisger A
	BYTE n= MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.bytes[0] = n;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_OR, REG_NAME_A, n);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::OR_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 110 110
	// Takes the logical-OR for each bit of the contents of (HL) and register A and stores the results
	// in regisger A
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, [%s]", MN_OR, REG_NAME_A, REG_NAME_HL);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::XOR_A_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 101 [ r ]
	// Takes the logical-XOR for each bit of the contents of register r and register A and stores the results
	// in regisger A
	
	
	BYTE rCode = opcode & 0x07;
	const char* r = Get_r(rCode);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_XOR, REG_NAME_A, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::XOR_A_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 101 110
	// Takes the logical-XOR for each bit of the contents of n and register A and stores the results
	// in regisger A
	BYTE n= MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.bytes[0] = n;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_XOR, REG_NAME_A, n);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::XOR_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 101 110
	// Takes the logical-XOR for each bit of the contents of (HL) and register A and stores the results
	// in regisger A
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, [%s]", MN_XOR, REG_NAME_A, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::CP_A_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 111 [ r ]
	// Compares the contents of register r and register A and sets the flags as a substraction
	
	BYTE rCode = opcode & 0x07;
	const char* r = Get_r(rCode);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_CP, REG_NAME_A, r);

	return dI;
}


DisassemblerInstruction DisassemblerInstructionTable::CP_A_n(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 111 110
	// Compares the contents of n and register A and sets the flags as a substraction
	BYTE n= MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.bytes[0] = n;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_CP, REG_NAME_A, n);

	return dI;
}


DisassemblerInstruction DisassemblerInstructionTable::CP_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 10 11 110
	// Compares the contents of (HL) and register A and sets the flags as a substraction
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, [%s]", MN_CP, REG_NAME_A, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::INC_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 [ r ] 100
	// Increments the contents of register r by 1
	const char* r = Get_r((opcode >> 3) & 0x07);


	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_INC, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::INC_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 110 100
	// Increments the contents of (HL) by 1
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_INC, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::DEC_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 [ r ] 101
	// Decrements the contents of register r by 1
	
	const char* r = Get_r((opcode >> 3) & 0x07);


	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_DEC, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::DEC_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 110 101
	// Decrements the contents of (HL) by 1
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_DEC, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::ADD_HL_ss(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 ss1 001
	// Adds the contents of register pair ss to the contents of register pair HL and stores the results in HL

	BYTE code = (opcode >> 4) & 0x03;
	const char* reg = Get_dd(code);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_ADD, REG_NAME_HL, reg);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::ADD_SP_e(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 101 000
	// Adds the contents of the 8-Bits immediate operand e and SP and stores the results in SP
	BYTE e = MemoryController::Shared()->ReadMemory(address + 1);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = e;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s, %s", MN_ADD, REG_NAME_SP, e);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::INC_ss(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 ss0 011
	// Increments the contents of register pair ss by 1
	BYTE code = (opcode >> 4) & 0x03;
	const char* reg = Get_dd(code);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_INC, reg);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::DEC_ss(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 ss1 011
	// Increments the contents of register pair ss by 1
	BYTE code = (opcode >> 4) & 0x03;
	const char* reg = Get_dd(code);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_DEC, reg);

	return dI;

}

DisassemblerInstruction DisassemblerInstructionTable::RLCA(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 000 111
	// Rotates the contents of register A to the left.
	// That is, the contents of bit 0 are copied to bit 1, and the previous contents of bit 1
	// is copied to bit 2, and so on.
	// The contents of bit 7 is placed in both CY and bit 0 of register A

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_RLCA);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RLA(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 010 111
	// Rotates the contents of register A + CY to the left.

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_RLA);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RRCA(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 001 111
	// Rotates the contents of register A to the right.

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_RRCA);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RRA(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 011 111
	// Rotates the contents of register A + CY to the right.

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_RRA);

	return dI;
}


//CB PREFIX
DisassemblerInstruction DisassemblerInstructionTable::PREFIX_0xBC(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 ->
	opcode = MemoryController::Shared()->ReadMemory(address+1);
	return (this->*cbInstructions[opcode])(memoryController, address, opcode);
}

DisassemblerInstruction DisassemblerInstructionTable::RLC_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 001 011 | 00 000 [ r ]
	//Rotates the contents of register r to the left
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_RLC, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RLC_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 001 011 | 00 000 110
	//Rotates the contents of register r to the left
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_RLC, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RL_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 001 011 | 00 010 [ r ]
	//Rotates the contents of register r + CY to the left
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_RL, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RL_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 001 011 | 00 010 110
	//Rotates the contents of (HL) + CY to the left
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_RL, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RRC_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 001 011 | 00 001 [ r ]
	//Rotates the contents of register r to the right
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_RRC, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RRC_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 001 011 | 00 001 110
	//Rotates the contents of register r to the right
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_RRC, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RR_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 001 011 | 00 011 [ r ]
	//Rotates the contents of register r + CY to the right
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_RR, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RR_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	//11 001 011 | 00 011 110
	//Rotates the contents of (HL) + CY to the right
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_RR, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SLA_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 00 100 [ r ]
	// Shift the contents of register r to the left, to CY. BIT 0 of r is reset
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_SLA, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SLA_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 00 100 110
	// Shift the contents of (HL) to the left, to CY. BIT 0 of r is reset
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_SLA, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SRA_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 00 101 [ r ]
	// Shift the contents of register r to the right, to CY. BIT 7 is untouched
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_SRA, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SRA_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 00 101 110
	// Shift the contents of (HL) to the left, to CY. BIT 7 is untouched
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_SRA, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SRL_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 00 111 [ r ]
	// Shifts the contetns of register r to the right, to CY. BIT 7 is reset
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_SRL, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SRL_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 00 111 110
	// Shift the contents of (HL) to the left, to CY. BIT 7 of r is reset
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_SRL, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SWAP_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 00 110 [ r ]
	// Shift the contents of the lower order 4 bits (0-3) of register r unmodified to the hight-order 4 bits (4-7)
	// of that operand and shifts the contents of hte higher order 4 bits to the lower-order 4 bits
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_SWAP, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SWAP_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 00 110 110
	// Shift the contents of the lower order 4 bits (0-3) of register r unmodified to the hight-order 4 bits (4-7)
	// of that operand and shifts the contents of hte higher order 4 bits to the lower-order 4 bits
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s [%s]", MN_SWAP, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::BIT_b_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 01 [ b ] [ r ]
	// Copies the complement of the contents of the specified bit in register r to the Z flag of the program status word (PSW)

	BYTE b = (opcode >> 3) & 0x07;
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %d, %s", MN_BIT, b, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::BIT_b_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 01 [ b ] 110
	// Copies the complement of the contents of the specified bit in (HL) to the Z flag of the program status word (PSW)

	BYTE b = (opcode >> 3) & 0x07;
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %d, [%s]", MN_BIT, b, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SET_b_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 11 [ b ] [ r ]
	// Sets to 1 the specified bit in register r

	BYTE b = (opcode >> 3) & 0x07;
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %d, %s", MN_SET, b, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SET_b_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 11 [ b ] 110
	// Sets to 1 the specified bit in (HL)

	BYTE b = (opcode >> 3) & 0x07;
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %d, [%s]", MN_SET, b, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RES_b_r(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 10 [ b ] [ r ]
	// Sets to 0 the specified bit in register r

	BYTE b = (opcode >> 3) & 0x07;
	const char* r = Get_r(opcode & 0x07);

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %d, %s", MN_RES, b, r);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RES_b_$HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 011 | 11 [ b ] 110
	// Sets to 1 the specified bit in (HL)

	BYTE b = (opcode >> 3) & 0x07;
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = 0xBC;
	dI.bytes[1] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %d, [%s]", MN_RES, b, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::JP_nn(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 000 010
	// Loads operand nn in the PC.
	BYTE nl = MemoryController::Shared()->ReadMemory(address + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(address + 2);

	WORD dst = nh;
	dst <<= 8;
	dst |= nl;


	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 3;
	dI.bytes[0] = opcode;
	dI.bytes[1] = nl;
	dI.bytes[2] = nh;
	dI.referencesAddresses.push_back(dst);

	sprintf(dI.disass, "%s %d", MN_JP, dst);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::JP_cc_nn(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 0cc 010
	// Loads operand nn in the PC if condition cc and the flag status match.
	// If condition cc and the flag status do not match, the contents of PC are incremented in 3

	BYTE ccCode = (opcode >> 3) & 0x03;

	const char* flag = GetFlag_cc(ccCode);
	
	BYTE nl = MemoryController::Shared()->ReadMemory(address + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(address + 2);


	BYTE dst = nh;
	dst <<= 8;
	dst |= nl;


	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 3;
	dI.bytes[0] = opcode;
	dI.bytes[1] = nl;
	dI.bytes[2] = nh;
	dI.referencesAddresses.push_back(address + dI.size);
	dI.referencesAddresses.push_back(dst);

	sprintf(dI.disass, "%s %s, %d", MN_JP, flag, dst);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::JR_e(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 011 000
	// Jumps -127 +129 steps from current address
	// (add 2 bacause of the normal PC increment)

	signed char e = MemoryController::Shared()->ReadMemory(address + 1) + 2;
	WORD dst = address + e;

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(dst);
	
	sprintf(dI.disass, "%s %d", MN_JR, dst);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::JR_cc_e(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 1cc 000
	// Jumps -127 +129 steps from current address (add 2 bacause of the normal PC increment)
	// if condition cc and flags match
	
	BYTE ccCode = (opcode >> 3) & 0x03;
	const char* flag = GetFlag_cc(ccCode);

	signed char e = MemoryController::Shared()->ReadMemory(address + 1) + 2;
	WORD dst = address + e;

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = e;
	dI.referencesAddresses.push_back(address + dI.size);
	dI.referencesAddresses.push_back(dst);

	sprintf(dI.disass, "%s %s, %d", MN_JR, flag, dst);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::JP_HL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 101 001
	// Loads the contents of register pair HL into PC
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;

	sprintf(dI.disass, "%s %s, %d", MN_JP, REG_NAME_HL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::CALL_nn(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 101
	// (SP - 1) <- PC[H]
	// (SP - 2) <- PC[L]
	// PC <- nn
	// SP <- SP-2

	BYTE nl = MemoryController::Shared()->ReadMemory(address + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(address + 2);

	BYTE dst = nh;
	dst <<= 8;
	dst |= nl;

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 3;
	dI.bytes[0] = opcode;
	dI.bytes[1] = nl;
	dI.bytes[2] = nh;
	dI.referencesAddresses.push_back(address + dI.size);
	dI.referencesAddresses.push_back(dst);

	sprintf(dI.disass, "%s %d", MN_CALL, dst);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::CALL_cc_nn(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 0cc 100
	// if cc true,
	// (SP - 1) <- PC[H]
	// (SP - 2) <- PC[L]
	// PC <- nn
	// SP <- SP-2

	BYTE ccCode = (opcode >> 3) & 0x03;
	const char* flag = GetFlag_cc(ccCode);

	BYTE nl = MemoryController::Shared()->ReadMemory(address + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(address + 2);

	BYTE dst = nh;
	dst <<= 8;
	dst |= nl;

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 3;
	dI.bytes[0] = opcode;
	dI.bytes[1] = nl;
	dI.bytes[2] = nh;
	dI.referencesAddresses.push_back(address + dI.size);
	dI.referencesAddresses.push_back(dst);

	sprintf(dI.disass, "%s %s, %d", MN_CALL, flag, dst);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RET(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 001 001
	// PC[L] <- (SP)
	// PC[H] <- (SP + 1)
	// SP <- SP + 2

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;

	sprintf(dI.disass, "%s", MN_RET);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RETI(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 011 001
	// Same as RET, but used when an interrupt service routine finishes
	// The addess of the return from the interrupt is loaded in program counter PC
	// The master interrupt enable FLAG is returned to its pre-interrupt status

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;

	sprintf(dI.disass, "%s", MN_RETI);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RET_cc(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 0cc 000
	// if cc true
	// PC[L] <- (SP)
	// PC[H] <- (SP + 1)
	// SP <- SP + 2

	BYTE ccCode = (opcode >> 3) & 0x03;
	const char* flag = GetFlag_cc(ccCode);

	
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s %s", MN_RET, flag);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::RST_t(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11 [ t ] 111
	// (SP - 1) <- PC[H]
	// (SP - 2) <- PC[L]
	// SP <- SP - 2
	// PC[H] <- 0x00
	// PC[L] <- P

	// Pushes the current value of the PC to the memory stack and loads to the PC the page 0 memory
	// Addressess provided by opernad t
	// The RST intruction can be used to jump 1 of 8 addressess
	// Bacause all addressess are held in the page 0 memory, 0x00 is loaded in the higher-oreder byte of
	// the PC, and the value of P is loaded in the lower-order byte.

	BYTE t = (opcode >> 3) & 0x07;
	WORD dst;

	switch(t)
	{
	case 0x00:
		dst = 0x00;
		break;
	case 0x01:
		dst = 0x08;
		break;
	case 0x02:
		dst = 0x10;
		break;
	case 0x03:
		dst = 0x18;
		break;
	case 0x04:
		dst = 0x20;
		break;
	case 0x05:
		dst = 0x28;
		break;
	case 0x06:
		dst = 0x30;
		break;
	case 0x07:
		dst = 0x38;
		break;
	}

	
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(dst);

	sprintf(dI.disass, "%s %d", MN_RST, t);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::DAA(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 100 111
	// When performing addition adn substraction, binary coded decimal representation is used to set the
	// contents of register A to a binary coded decimal number (BCD)

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_DAA);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::CPL(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 101 111
	// Takes the one's complement of register A

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_CPL);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::NOP(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 000 000
	// Only advances the PC by 1. Performs no other operations to have effect.

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_NOP);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::CCF(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 111 111
	// CY	<-	!CY
	// H	<-	0
	// N	<-	0
	// Z --

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_CCF);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::SCF(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 110 111
	// CY	<-	1
	// H	<-	0
	// N	<-	0
	// Z --

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_SCF);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::DI(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11	110	011  
	// IME  <- 0 

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_DI);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::EI(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 11	111	011  
	// IME  <- 1 

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_EI);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::HALT(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 01 110 110
	// After a HALT instruction is executed, the system clock is stopped and HALT mode is entered. Although
	// the system clock is stopped in this status, the oscillator circuit and LCD controller continue to opperate.
	// The status of the internal RAM register ports remains unchaged.

	// HALT mode is canceled by an interrupt or reset signal.

	// The PC is halted at the step after the HALT instruction. If both the interrupt request flag 
	// and the corresponding interrupt enable flag are set, HALT mode is exited, event if the IME is not set.
	// Once HALT mode is canceled, the program starts from the address indicated by the PC.

	// If the IME is set, the interrupt process (save PC in stack and jump) is made.

	// If the RESET terminal goes LOW in HALT mode, the mode becomes that of a normal reset.

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_HALT);

	return dI;
}


DisassemblerInstruction DisassemblerInstructionTable::STOP(MemoryController* memoryController, WORD address, BYTE opcode)
{
	// 00 010 000 | 00 000 000
	// Execution of a STOP instruction stops both the system clock and oscillator circuit. STOP mode is
	// entered, and the LCD controller also stops.

	// However, the status of the internal RAM register ports remains unchanged.

	// STOP mode can be cancelled by a reset signal.

	// If the RESET terminal goes LOW in STOP mode, it becomes that of a normal reset status.

	// The following conditions should be met before a STOP instruction is executed and STOP mode is
	// entered:
	//	* All interrupt-enable (IE) flags are reset
	//	* Input P10 - P13 is LOW for all

	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 2;
	dI.bytes[0] = opcode;
	dI.bytes[1] = 0x00;
	dI.referencesAddresses.push_back(address + dI.size);

	sprintf(dI.disass, "%s", MN_STOP);

	return dI;
}

DisassemblerInstruction DisassemblerInstructionTable::UNSUPPORTED(MemoryController* memoryController, WORD address, BYTE opcode)
{
	DisassemblerInstruction dI;

	dI.address = address;
	dI.size = 1;
	dI.bytes[0] = opcode;

	sprintf(dI.disass, "%s", INVALID_INSTRUCTION);

	return dI;
}

void DisassemblerInstructionTable::LoadInstructions()
{
	instructions[0x00]	=	&DisassemblerInstructionTable::NOP;
	instructions[0x01]	=	&DisassemblerInstructionTable::LD_dd_nn;
	instructions[0x02]	=	&DisassemblerInstructionTable::LD_$BC_A;
	instructions[0x03]	=	&DisassemblerInstructionTable::INC_ss;
	instructions[0x04]	=	&DisassemblerInstructionTable::INC_r;
	instructions[0x05]	=	&DisassemblerInstructionTable::DEC_r;
	instructions[0x06]	=	&DisassemblerInstructionTable::LD_R_n;
	instructions[0x07]	=	&DisassemblerInstructionTable::RLCA;
	instructions[0x08]	=	&DisassemblerInstructionTable::LD_$nn_SP;
	instructions[0x09]	=	&DisassemblerInstructionTable::ADD_HL_ss;
	instructions[0x0A]	=	&DisassemblerInstructionTable::LD_A_$BC;
	instructions[0x0B]	=	&DisassemblerInstructionTable::DEC_ss;
	instructions[0x0C]	=	&DisassemblerInstructionTable::INC_r;
	instructions[0x0D]	=	&DisassemblerInstructionTable::DEC_r;
	instructions[0x0E]	=	&DisassemblerInstructionTable::LD_R_n;
	instructions[0x0F]	=	&DisassemblerInstructionTable::RRCA;

	instructions[0x10]	=	&DisassemblerInstructionTable::STOP;
	instructions[0x11]	=	&DisassemblerInstructionTable::LD_dd_nn;
	instructions[0x12]	=	&DisassemblerInstructionTable::LD_$DE_A;
	instructions[0x13]	=	&DisassemblerInstructionTable::INC_ss;
	instructions[0x14]	=	&DisassemblerInstructionTable::INC_r;
	instructions[0x15]	=	&DisassemblerInstructionTable::DEC_r;
	instructions[0x16]	=	&DisassemblerInstructionTable::LD_R_n;
	instructions[0x17]	=	&DisassemblerInstructionTable::RLA;
	instructions[0x18]	=	&DisassemblerInstructionTable::JR_e;
	instructions[0x19]	=	&DisassemblerInstructionTable::ADD_HL_ss;
	instructions[0x1A]	=	&DisassemblerInstructionTable::LD_A_$DE;
	instructions[0x1B]	=	&DisassemblerInstructionTable::DEC_ss;
	instructions[0x1C]	=	&DisassemblerInstructionTable::INC_r;
	instructions[0x1D]	=	&DisassemblerInstructionTable::DEC_r;
	instructions[0x1E]	=	&DisassemblerInstructionTable::LD_R_n;
	instructions[0x1F]	=	&DisassemblerInstructionTable::RRA;

	instructions[0x20]	=	&DisassemblerInstructionTable::JR_cc_e;
	instructions[0x21]	=	&DisassemblerInstructionTable::LD_dd_nn;
	instructions[0x22]	=	&DisassemblerInstructionTable::LD_$HLI_A;
	instructions[0x23]	=	&DisassemblerInstructionTable::INC_ss;
	instructions[0x24]	=	&DisassemblerInstructionTable::INC_r;
	instructions[0x25]	=	&DisassemblerInstructionTable::DEC_r;
	instructions[0x26]	=	&DisassemblerInstructionTable::LD_R_n;
	instructions[0x27]	=	&DisassemblerInstructionTable::DAA;
	instructions[0x28]	=	&DisassemblerInstructionTable::JR_cc_e;
	instructions[0x29]	=	&DisassemblerInstructionTable::ADD_HL_ss;
	instructions[0x2A]	=	&DisassemblerInstructionTable::LD_A_$HLI;
	instructions[0x2B]	=	&DisassemblerInstructionTable::DEC_ss;
	instructions[0x2C]	=	&DisassemblerInstructionTable::INC_r;
	instructions[0x2D]	=	&DisassemblerInstructionTable::DEC_r;
	instructions[0x2E]	=	&DisassemblerInstructionTable::LD_R_n;
	instructions[0x2F]	=	&DisassemblerInstructionTable::CPL;

	instructions[0x30]	=	&DisassemblerInstructionTable::JR_cc_e;
	instructions[0x31]	=	&DisassemblerInstructionTable::LD_dd_nn;
	instructions[0x32]	=	&DisassemblerInstructionTable::LD_$HLD_A;
	instructions[0x33]	=	&DisassemblerInstructionTable::INC_ss;
	instructions[0x34]	=	&DisassemblerInstructionTable::INC_$HL;
	instructions[0x35]	=	&DisassemblerInstructionTable::DEC_$HL;
	instructions[0x36]	=	&DisassemblerInstructionTable::LD_$HL_n;
	instructions[0x37]	=	&DisassemblerInstructionTable::SCF;
	instructions[0x38]	=	&DisassemblerInstructionTable::JR_cc_e;
	instructions[0x39]	=	&DisassemblerInstructionTable::ADD_HL_ss;
	instructions[0x3A]	=	&DisassemblerInstructionTable::LD_A_$HLD;
	instructions[0x3B]	=	&DisassemblerInstructionTable::DEC_ss;
	instructions[0x3C]	=	&DisassemblerInstructionTable::INC_r;
	instructions[0x3D]	=	&DisassemblerInstructionTable::DEC_r;
	instructions[0x3E]	=	&DisassemblerInstructionTable::LD_R_n;
	instructions[0x3F]	=	&DisassemblerInstructionTable::CCF;

	instructions[0x40]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x41]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x42]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x43]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x44]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x45]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x46]	=	&DisassemblerInstructionTable::LD_R_$HL;
	instructions[0x47]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x48]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x49]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x4A]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x4B]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x4C]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x4D]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x4E]	=	&DisassemblerInstructionTable::LD_R_$HL;
	instructions[0x4F]	=	&DisassemblerInstructionTable::LD_R_R;

	instructions[0x50]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x51]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x52]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x53]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x54]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x55]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x56]	=	&DisassemblerInstructionTable::LD_R_$HL;
	instructions[0x57]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x58]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x59]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x5A]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x5B]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x5C]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x5D]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x5E]	=	&DisassemblerInstructionTable::LD_R_$HL;
	instructions[0x5F]	=	&DisassemblerInstructionTable::LD_R_R;

	instructions[0x60]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x61]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x62]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x63]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x64]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x65]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x66]	=	&DisassemblerInstructionTable::LD_R_$HL;
	instructions[0x67]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x68]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x69]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x6A]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x6B]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x6C]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x6D]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x6E]	=	&DisassemblerInstructionTable::LD_R_$HL;
	instructions[0x6F]	=	&DisassemblerInstructionTable::LD_R_R;

	instructions[0x70]	=	&DisassemblerInstructionTable::LD_$HL_R;
	instructions[0x71]	=	&DisassemblerInstructionTable::LD_$HL_R;
	instructions[0x72]	=	&DisassemblerInstructionTable::LD_$HL_R;
	instructions[0x73]	=	&DisassemblerInstructionTable::LD_$HL_R;
	instructions[0x74]	=	&DisassemblerInstructionTable::LD_$HL_R;
	instructions[0x75]	=	&DisassemblerInstructionTable::LD_$HL_R;
	instructions[0x76]	=	&DisassemblerInstructionTable::HALT;
	instructions[0x77]	=	&DisassemblerInstructionTable::LD_$HL_R;
	instructions[0x78]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x79]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x7A]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x7B]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x7C]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x7D]	=	&DisassemblerInstructionTable::LD_R_R;
	instructions[0x7E]	=	&DisassemblerInstructionTable::LD_R_$HL;
	instructions[0x7F]	=	&DisassemblerInstructionTable::LD_R_R;

	instructions[0x80]	=	&DisassemblerInstructionTable::ADD_A_r;
	instructions[0x81]	=	&DisassemblerInstructionTable::ADD_A_r;
	instructions[0x82]	=	&DisassemblerInstructionTable::ADD_A_r;
	instructions[0x83]	=	&DisassemblerInstructionTable::ADD_A_r;
	instructions[0x84]	=	&DisassemblerInstructionTable::ADD_A_r;
	instructions[0x85]	=	&DisassemblerInstructionTable::ADD_A_r;
	instructions[0x86]	=	&DisassemblerInstructionTable::ADD_A_$HL;
	instructions[0x87]	=	&DisassemblerInstructionTable::ADD_A_r;
	instructions[0x88]	=	&DisassemblerInstructionTable::ADC_A_r;
	instructions[0x89]	=	&DisassemblerInstructionTable::ADC_A_r;
	instructions[0x8A]	=	&DisassemblerInstructionTable::ADC_A_r;
	instructions[0x8B]	=	&DisassemblerInstructionTable::ADC_A_r;
	instructions[0x8C]	=	&DisassemblerInstructionTable::ADC_A_r;
	instructions[0x8D]	=	&DisassemblerInstructionTable::ADC_A_r;
	instructions[0x8E]	=	&DisassemblerInstructionTable::ADC_A_$HL;
	instructions[0x8F]	=	&DisassemblerInstructionTable::ADC_A_r;

	instructions[0x90]	=	&DisassemblerInstructionTable::SUB_A_r;
	instructions[0x91]	=	&DisassemblerInstructionTable::SUB_A_r;
	instructions[0x92]	=	&DisassemblerInstructionTable::SUB_A_r;
	instructions[0x93]	=	&DisassemblerInstructionTable::SUB_A_r;
	instructions[0x94]	=	&DisassemblerInstructionTable::SUB_A_r;
	instructions[0x95]	=	&DisassemblerInstructionTable::SUB_A_r;
	instructions[0x96]	=	&DisassemblerInstructionTable::SUB_A_$HL;
	instructions[0x97]	=	&DisassemblerInstructionTable::SUB_A_r;
	instructions[0x98]	=	&DisassemblerInstructionTable::SBC_A_r;
	instructions[0x99]	=	&DisassemblerInstructionTable::SBC_A_r;
	instructions[0x9A]	=	&DisassemblerInstructionTable::SBC_A_r;
	instructions[0x9B]	=	&DisassemblerInstructionTable::SBC_A_r;
	instructions[0x9C]	=	&DisassemblerInstructionTable::SBC_A_r;
	instructions[0x9D]	=	&DisassemblerInstructionTable::SBC_A_r;
	instructions[0x9E]	=	&DisassemblerInstructionTable::SBC_A_$HL;
	instructions[0x9F]	=	&DisassemblerInstructionTable::SBC_A_r;

	instructions[0xA0]	=	&DisassemblerInstructionTable::AND_A_r;
	instructions[0xA1]	=	&DisassemblerInstructionTable::AND_A_r;
	instructions[0xA2]	=	&DisassemblerInstructionTable::AND_A_r;
	instructions[0xA3]	=	&DisassemblerInstructionTable::AND_A_r;
	instructions[0xA4]	=	&DisassemblerInstructionTable::AND_A_r;
	instructions[0xA5]	=	&DisassemblerInstructionTable::AND_A_r;
	instructions[0xA6]	=	&DisassemblerInstructionTable::AND_A_$HL;
	instructions[0xA7]	=	&DisassemblerInstructionTable::AND_A_r;
	instructions[0xA8]	=	&DisassemblerInstructionTable::XOR_A_r;
	instructions[0xA9]	=	&DisassemblerInstructionTable::XOR_A_r;
	instructions[0xAA]	=	&DisassemblerInstructionTable::XOR_A_r;
	instructions[0xAB]	=	&DisassemblerInstructionTable::XOR_A_r;
	instructions[0xAC]	=	&DisassemblerInstructionTable::XOR_A_r;
	instructions[0xAD]	=	&DisassemblerInstructionTable::XOR_A_r;
	instructions[0xAE]	=	&DisassemblerInstructionTable::XOR_A_$HL;
	instructions[0xAF]	=	&DisassemblerInstructionTable::XOR_A_r;

	instructions[0xB0]	=	&DisassemblerInstructionTable::OR_A_r;
	instructions[0xB1]	=	&DisassemblerInstructionTable::OR_A_r;
	instructions[0xB2]	=	&DisassemblerInstructionTable::OR_A_r;
	instructions[0xB3]	=	&DisassemblerInstructionTable::OR_A_r;
	instructions[0xB4]	=	&DisassemblerInstructionTable::OR_A_r;
	instructions[0xB5]	=	&DisassemblerInstructionTable::OR_A_r;
	instructions[0xB6]	=	&DisassemblerInstructionTable::OR_A_$HL;
	instructions[0xB7]	=	&DisassemblerInstructionTable::OR_A_r;
	instructions[0xB8]	=	&DisassemblerInstructionTable::CP_A_r;
	instructions[0xB9]	=	&DisassemblerInstructionTable::CP_A_r;
	instructions[0xBA]	=	&DisassemblerInstructionTable::CP_A_r;
	instructions[0xBB]	=	&DisassemblerInstructionTable::CP_A_r;
	instructions[0xBC]	=	&DisassemblerInstructionTable::CP_A_r;
	instructions[0xBD]	=	&DisassemblerInstructionTable::CP_A_r;
	instructions[0xBE]	=	&DisassemblerInstructionTable::CP_A_$HL;
	instructions[0xBF]	=	&DisassemblerInstructionTable::CP_A_r;

	instructions[0xC0]	=	&DisassemblerInstructionTable::RET_cc;
	instructions[0xC1]	=	&DisassemblerInstructionTable::POP_qq;
	instructions[0xC2]	=	&DisassemblerInstructionTable::JP_cc_nn;
	instructions[0xC3]	=	&DisassemblerInstructionTable::JP_nn;
	instructions[0xC4]	=	&DisassemblerInstructionTable::CALL_cc_nn;
	instructions[0xC5]	=	&DisassemblerInstructionTable::PUSH_qq;
	instructions[0xC6]	=	&DisassemblerInstructionTable::ADD_A_n;
	instructions[0xC7]	=	&DisassemblerInstructionTable::RST_t;
	instructions[0xC8]	=	&DisassemblerInstructionTable::RET_cc;
	instructions[0xC9]	=	&DisassemblerInstructionTable::RET;
	instructions[0xCA]	=	&DisassemblerInstructionTable::JP_cc_nn;
	instructions[0xCB]	=	&DisassemblerInstructionTable::PREFIX_0xBC;
	instructions[0xCC]	=	&DisassemblerInstructionTable::CALL_cc_nn;
	instructions[0xCD]	=	&DisassemblerInstructionTable::CALL_nn;
	instructions[0xCE]	=	&DisassemblerInstructionTable::ADC_A_n;
	instructions[0xCF]	=	&DisassemblerInstructionTable::RST_t;

	instructions[0xD0]	=	&DisassemblerInstructionTable::RET_cc;
	instructions[0xD1]	=	&DisassemblerInstructionTable::POP_qq;
	instructions[0xD2]	=	&DisassemblerInstructionTable::JP_cc_nn;
	instructions[0xD3]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xD4]	=	&DisassemblerInstructionTable::CALL_cc_nn;
	instructions[0xD5]	=	&DisassemblerInstructionTable::PUSH_qq;
	instructions[0xD6]	=	&DisassemblerInstructionTable::SUB_A_n;
	instructions[0xD7]	=	&DisassemblerInstructionTable::RST_t;
	instructions[0xD8]	=	&DisassemblerInstructionTable::RET_cc;
	instructions[0xD9]	=	&DisassemblerInstructionTable::RETI;
	instructions[0xDA]	=	&DisassemblerInstructionTable::JP_cc_nn;
	instructions[0xDB]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xDC]	=	&DisassemblerInstructionTable::CALL_cc_nn;
	instructions[0xDD]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xDE]	=	&DisassemblerInstructionTable::SBC_A_n;
	instructions[0xDF]	=	&DisassemblerInstructionTable::RST_t;

	instructions[0xE0]	=	&DisassemblerInstructionTable::LD_$n_A;
	instructions[0xE1]	=	&DisassemblerInstructionTable::POP_qq;
	instructions[0xE2]	=	&DisassemblerInstructionTable::LD_$C_A;
	instructions[0xE3]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xE4]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xE5]	=	&DisassemblerInstructionTable::PUSH_qq;
	instructions[0xE6]	=	&DisassemblerInstructionTable::AND_A_n;
	instructions[0xE7]	=	&DisassemblerInstructionTable::RST_t;
	instructions[0xE8]	=	&DisassemblerInstructionTable::ADD_SP_e;
	instructions[0xE9]	=	&DisassemblerInstructionTable::JP_HL;
	instructions[0xEA]	=	&DisassemblerInstructionTable::LD_$nn_A;
	instructions[0xEB]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xEC]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xED]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xEE]	=	&DisassemblerInstructionTable::XOR_A_n;
	instructions[0xEF]	=	&DisassemblerInstructionTable::RST_t;

	instructions[0xF0]	=	&DisassemblerInstructionTable::LD_A_$n;
	instructions[0xF1]	=	&DisassemblerInstructionTable::POP_qq;
	instructions[0xF2]	=	&DisassemblerInstructionTable::LD_A_$C;
	instructions[0xF3]	=	&DisassemblerInstructionTable::DI;
	instructions[0xF4]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xF5]	=	&DisassemblerInstructionTable::PUSH_qq;
	instructions[0xF6]	=	&DisassemblerInstructionTable::OR_A_n;
	instructions[0xF7]	=	&DisassemblerInstructionTable::RST_t;
	instructions[0xF8]	=	&DisassemblerInstructionTable::LDHL_PS_e;
	instructions[0xF9]	=	&DisassemblerInstructionTable::LD_SP_HL;
	instructions[0xFA]	=	&DisassemblerInstructionTable::LD_A_$nn;
	instructions[0xFB]	=	&DisassemblerInstructionTable::EI;
	instructions[0xFC]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xFD]	=	&DisassemblerInstructionTable::UNSUPPORTED;
	instructions[0xFE]	=	&DisassemblerInstructionTable::CP_A_n;
	instructions[0xFF]	=	&DisassemblerInstructionTable::RST_t;

	//---------------------------------------
	//---------------------------------------
	cbInstructions[0x00]	=	&DisassemblerInstructionTable::RLC_r;
	cbInstructions[0x01]	=	&DisassemblerInstructionTable::RLC_r;
	cbInstructions[0x02]	=	&DisassemblerInstructionTable::RLC_r;
	cbInstructions[0x03]	=	&DisassemblerInstructionTable::RLC_r;
	cbInstructions[0x04]	=	&DisassemblerInstructionTable::RLC_r;
	cbInstructions[0x05]	=	&DisassemblerInstructionTable::RLC_r;
	cbInstructions[0x06]	=	&DisassemblerInstructionTable::RLC_$HL;
	cbInstructions[0x07]	=	&DisassemblerInstructionTable::RLC_r;
	cbInstructions[0x08]	=	&DisassemblerInstructionTable::RRC_r;
	cbInstructions[0x09]	=	&DisassemblerInstructionTable::RRC_r;
	cbInstructions[0x0A]	=	&DisassemblerInstructionTable::RRC_r;
	cbInstructions[0x0B]	=	&DisassemblerInstructionTable::RRC_r;
	cbInstructions[0x0C]	=	&DisassemblerInstructionTable::RRC_r;
	cbInstructions[0x0D]	=	&DisassemblerInstructionTable::RRC_r;
	cbInstructions[0x0E]	=	&DisassemblerInstructionTable::RRC_$HL;
	cbInstructions[0x0F]	=	&DisassemblerInstructionTable::RRC_r;

	cbInstructions[0x10]	=	&DisassemblerInstructionTable::RL_r;
	cbInstructions[0x11]	=	&DisassemblerInstructionTable::RL_r;
	cbInstructions[0x12]	=	&DisassemblerInstructionTable::RL_r;
	cbInstructions[0x13]	=	&DisassemblerInstructionTable::RL_r;
	cbInstructions[0x14]	=	&DisassemblerInstructionTable::RL_r;
	cbInstructions[0x15]	=	&DisassemblerInstructionTable::RL_r;
	cbInstructions[0x16]	=	&DisassemblerInstructionTable::RL_$HL;
	cbInstructions[0x17]	=	&DisassemblerInstructionTable::RL_r;
	cbInstructions[0x18]	=	&DisassemblerInstructionTable::RR_r;
	cbInstructions[0x19]	=	&DisassemblerInstructionTable::RR_r;
	cbInstructions[0x1A]	=	&DisassemblerInstructionTable::RR_r;
	cbInstructions[0x1B]	=	&DisassemblerInstructionTable::RR_r;
	cbInstructions[0x1C]	=	&DisassemblerInstructionTable::RR_r;
	cbInstructions[0x1D]	=	&DisassemblerInstructionTable::RR_r;
	cbInstructions[0x1E]	=	&DisassemblerInstructionTable::RR_$HL;
	cbInstructions[0x1F]	=	&DisassemblerInstructionTable::RR_r;

	cbInstructions[0x20]	=	&DisassemblerInstructionTable::SLA_r;
	cbInstructions[0x21]	=	&DisassemblerInstructionTable::SLA_r;
	cbInstructions[0x22]	=	&DisassemblerInstructionTable::SLA_r;
	cbInstructions[0x23]	=	&DisassemblerInstructionTable::SLA_r;
	cbInstructions[0x24]	=	&DisassemblerInstructionTable::SLA_r;
	cbInstructions[0x25]	=	&DisassemblerInstructionTable::SLA_r;
	cbInstructions[0x26]	=	&DisassemblerInstructionTable::SLA_$HL;
	cbInstructions[0x27]	=	&DisassemblerInstructionTable::SLA_r;
	cbInstructions[0x28]	=	&DisassemblerInstructionTable::SRA_r;
	cbInstructions[0x29]	=	&DisassemblerInstructionTable::SRA_r;
	cbInstructions[0x2A]	=	&DisassemblerInstructionTable::SRA_r;
	cbInstructions[0x2B]	=	&DisassemblerInstructionTable::SRA_r;
	cbInstructions[0x2C]	=	&DisassemblerInstructionTable::SRA_r;
	cbInstructions[0x2D]	=	&DisassemblerInstructionTable::SRA_r;
	cbInstructions[0x2E]	=	&DisassemblerInstructionTable::SRA_$HL;
	cbInstructions[0x2F]	=	&DisassemblerInstructionTable::SRA_r;

	cbInstructions[0x30]	=	&DisassemblerInstructionTable::SWAP_r;
	cbInstructions[0x31]	=	&DisassemblerInstructionTable::SWAP_r;
	cbInstructions[0x32]	=	&DisassemblerInstructionTable::SWAP_r;
	cbInstructions[0x33]	=	&DisassemblerInstructionTable::SWAP_r;
	cbInstructions[0x34]	=	&DisassemblerInstructionTable::SWAP_r;
	cbInstructions[0x35]	=	&DisassemblerInstructionTable::SWAP_r;
	cbInstructions[0x36]	=	&DisassemblerInstructionTable::SWAP_$HL;
	cbInstructions[0x37]	=	&DisassemblerInstructionTable::SWAP_r;
	cbInstructions[0x38]	=	&DisassemblerInstructionTable::SRL_r;
	cbInstructions[0x39]	=	&DisassemblerInstructionTable::SRL_r;
	cbInstructions[0x3A]	=	&DisassemblerInstructionTable::SRL_r;
	cbInstructions[0x3B]	=	&DisassemblerInstructionTable::SRL_r;
	cbInstructions[0x3C]	=	&DisassemblerInstructionTable::SRL_r;
	cbInstructions[0x3D]	=	&DisassemblerInstructionTable::SRL_r;
	cbInstructions[0x3E]	=	&DisassemblerInstructionTable::SRL_$HL;
	cbInstructions[0x3F]	=	&DisassemblerInstructionTable::SRL_r;

	cbInstructions[0x40]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x41]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x42]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x43]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x44]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x45]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x46]	=	&DisassemblerInstructionTable::BIT_b_$HL;
	cbInstructions[0x47]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x48]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x49]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x4A]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x4B]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x4C]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x4D]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x4E]	=	&DisassemblerInstructionTable::BIT_b_$HL;
	cbInstructions[0x4F]	=	&DisassemblerInstructionTable::BIT_b_r;

	cbInstructions[0x50]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x51]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x52]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x53]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x54]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x55]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x56]	=	&DisassemblerInstructionTable::BIT_b_$HL;
	cbInstructions[0x57]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x58]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x59]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x5A]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x5B]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x5C]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x5D]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x5E]	=	&DisassemblerInstructionTable::BIT_b_$HL;
	cbInstructions[0x5F]	=	&DisassemblerInstructionTable::BIT_b_r;

	cbInstructions[0x60]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x61]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x62]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x63]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x64]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x65]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x66]	=	&DisassemblerInstructionTable::BIT_b_$HL;
	cbInstructions[0x67]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x68]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x69]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x6A]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x6B]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x6C]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x6D]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x6E]	=	&DisassemblerInstructionTable::BIT_b_$HL;
	cbInstructions[0x6F]	=	&DisassemblerInstructionTable::BIT_b_r;

	cbInstructions[0x70]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x71]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x72]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x73]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x74]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x75]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x76]	=	&DisassemblerInstructionTable::BIT_b_$HL;
	cbInstructions[0x77]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x78]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x79]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x7A]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x7B]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x7C]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x7D]	=	&DisassemblerInstructionTable::BIT_b_r;
	cbInstructions[0x7E]	=	&DisassemblerInstructionTable::BIT_b_$HL;
	cbInstructions[0x7F]	=	&DisassemblerInstructionTable::BIT_b_r;

	cbInstructions[0x80]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x81]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x82]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x83]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x84]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x85]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x86]	=	&DisassemblerInstructionTable::RES_b_$HL;
	cbInstructions[0x87]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x88]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x89]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x8A]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x8B]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x8C]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x8D]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x8E]	=	&DisassemblerInstructionTable::RES_b_$HL;
	cbInstructions[0x8F]	=	&DisassemblerInstructionTable::RES_b_r;

	cbInstructions[0x90]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x91]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x92]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x93]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x94]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x95]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x96]	=	&DisassemblerInstructionTable::RES_b_$HL;
	cbInstructions[0x97]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x98]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x99]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x9A]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x9B]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x9C]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x9D]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0x9E]	=	&DisassemblerInstructionTable::RES_b_$HL;
	cbInstructions[0x9F]	=	&DisassemblerInstructionTable::RES_b_r;

	cbInstructions[0xA0]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xA1]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xA2]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xA3]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xA4]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xA5]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xA6]	=	&DisassemblerInstructionTable::RES_b_$HL;
	cbInstructions[0xA7]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xA8]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xA9]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xAA]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xAB]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xAC]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xAD]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xAE]	=	&DisassemblerInstructionTable::RES_b_$HL;
	cbInstructions[0xAF]	=	&DisassemblerInstructionTable::RES_b_r;

	cbInstructions[0xB0]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xB1]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xB2]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xB3]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xB4]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xB5]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xB6]	=	&DisassemblerInstructionTable::RES_b_$HL;
	cbInstructions[0xB7]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xB8]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xB9]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xBA]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xBB]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xBC]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xBD]	=	&DisassemblerInstructionTable::RES_b_r;
	cbInstructions[0xBE]	=	&DisassemblerInstructionTable::RES_b_$HL;
	cbInstructions[0xBF]	=	&DisassemblerInstructionTable::RES_b_r;

	cbInstructions[0xC0]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xC1]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xC2]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xC3]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xC4]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xC5]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xC6]	=	&DisassemblerInstructionTable::SET_b_$HL;
	cbInstructions[0xC7]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xC8]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xC9]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xCA]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xCB]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xCC]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xCD]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xCE]	=	&DisassemblerInstructionTable::SET_b_$HL;
	cbInstructions[0xCF]	=	&DisassemblerInstructionTable::SET_b_r;

	cbInstructions[0xD0]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xD1]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xD2]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xD3]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xD4]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xD5]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xD6]	=	&DisassemblerInstructionTable::SET_b_$HL;
	cbInstructions[0xD7]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xD8]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xD9]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xDA]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xDB]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xDC]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xDD]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xDE]	=	&DisassemblerInstructionTable::SET_b_$HL;
	cbInstructions[0xDF]	=	&DisassemblerInstructionTable::SET_b_r;

	cbInstructions[0xE0]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xE1]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xE2]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xE3]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xE4]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xE5]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xE6]	=	&DisassemblerInstructionTable::SET_b_$HL;
	cbInstructions[0xE7]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xE8]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xE9]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xEA]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xEB]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xEC]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xED]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xEE]	=	&DisassemblerInstructionTable::SET_b_$HL;
	cbInstructions[0xEF]	=	&DisassemblerInstructionTable::SET_b_r;

	cbInstructions[0xF0]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xF1]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xF2]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xF3]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xF4]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xF5]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xF6]	=	&DisassemblerInstructionTable::SET_b_$HL;
	cbInstructions[0xF7]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xF8]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xF9]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xFA]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xFB]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xFC]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xFD]	=	&DisassemblerInstructionTable::SET_b_r;
	cbInstructions[0xFE]	=	&DisassemblerInstructionTable::SET_b_$HL;
	cbInstructions[0xFF]	=	&DisassemblerInstructionTable::SET_b_r;
}