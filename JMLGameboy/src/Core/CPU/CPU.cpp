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

#include "CPU.h"
#include "../../Tools/Console.h"
#include <stdexcept>
//#include <Windows.h>

#ifdef DEBUGGER_ON
#include <ctime>
#endif

#define IF_ADDRESS 0xFF0F
#define IE_ADDRESS 0xFFFF
#define STACK_RAM_START_ADDRESS 0xFF80

#ifdef DEBUGGER_ON
int cyclesCount = 0;

int ownershipId[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int ownerIndex = 0;

clock_t lastTime = clock();
clock_t freqTime = 0;
int frequency = 0;
#endif

int CPU::TARGET_CPU_SPEED = 4194304; //4Mhz

CPU::CPU()
{
	Reset();
	
	LoadInstructions();

#ifdef DEBUGGER_ON
	for (short i = 0; i < 256; i++)
	{
		intructionCounter[i] = 0;
		instructionCBCounter[i] = 0;
	} 

	emulationCycleCount = 0;
	realCycleCount = 0;
#endif
}

void CPU::Reset()
{
#ifndef UNIT_TEST_ON
#if BOOTSTRAP_ENABLED
	PC = 0x0000;
#else
	PC = 0x0100;
	AF.w = 0x01B0;
	BC.w = 0x0013;
	DE.w = 0x00D8;
	HL.w = 0x014D;
	IF = 0xE1;
#endif
#else
	PC == 0x0000;
#endif
	//IE = ???
	SP.w = 0xFFFE;
	IE = 0x00;

	IME = false;
	haltMode = false;
	stopMode = false;
}

CPU::~CPU()
{
	WriteLineE("CPU Destructor");
}


#ifdef DEBUGGER_ON
int CPU::GetOwnershipId()
{
	ownerIndex++;
	return ownershipId[ownerIndex];
}

void CPU::ReleaseOwnership()
{
	ownerIndex--;
}

BYTE CPU::RunCycle(int ownerId)
#else
BYTE CPU::RunCycle()
#endif
{



#ifdef DEBUGGER_ON	
	clock_t currentTime =  clock();
	freqTime += currentTime - lastTime;
	lastTime = currentTime;

	if(freqTime >= CLOCKS_PER_SEC)
	{
		WriteLineI("CPU Freq: %d Hz", frequency);
		frequency = 0;
		freqTime = 0;
	}

	if(ownershipId[ownerIndex] != ownerId)
	{
		return 0;
	}
#endif

	char opMCycles = 0;

	//FETCH
	if(!haltMode)
	{
		if(PC == 0x02A5)
		{
			int a = 0;
		}

		//WriteLineE("%#04x", PC);
		opcode = MemoryController::Shared()->ReadMemory(PC);

		//DECODE & EXCECUTE
		opMCycles = (this->*instructions[opcode])();

#ifdef DEBUGGER_ON
		realCycleCount += opMCycles;
		intructionCounter[opcode]++;
		emulationCycleCount++;
#endif
	}
	else
	{
		//Sleep(1);
		opMCycles = 4; //???
	}


#ifdef DEBUGGER_ON
	frequency += opMCycles;
#endif
	//Check Interrupts
	CheckInterrupt();

	

	return opMCycles;
}

//MEMORY ELEMENT STUFF (INTERRUPTS)
bool CPU::Write(const WORD &address, const BYTE &value)
{
	if(address == IF_ADDRESS)
	{
		IF = value;
		return true;
	}
	else if(address == IE_ADDRESS)
	{
		IE = value;
		return true;
	}
	else if(address >= STACK_RAM_START_ADDRESS)
	{
		stackRam[address - STACK_RAM_START_ADDRESS] = value;
		return true;
	}
	return false;
}

bool CPU::Read(const WORD &address, BYTE &out)
{
	if(address == IF_ADDRESS)
	{
		out = IF;
		return true;
	}
	else if(address == IE_ADDRESS)
	{
		out = IE;
		return true;
	}
	else if(address >= STACK_RAM_START_ADDRESS)
	{
		out = stackRam[address - STACK_RAM_START_ADDRESS];
		return true;
	}

	return false;
}

void CPU::SetInterrupt(INTERRUPT_FLAG flag)
{
	IF |= flag;
}

void CPU::CheckInterrupt()
{
	BYTE interrupts = IF & IE;

	if(interrupts & INTERRUPT_FLAG::IF_V_BLANK)
	{
		StartInterrupt(INTERRUPT_FLAG::IF_V_BLANK);
	}
	else if(interrupts & INTERRUPT_FLAG::IF_LCDC)
	{
		StartInterrupt(INTERRUPT_FLAG::IF_LCDC);
	}
	else if(interrupts & INTERRUPT_FLAG::IF_TIMER_OVERFLOW)
	{
		StartInterrupt(INTERRUPT_FLAG::IF_TIMER_OVERFLOW);
	}
	else if(interrupts & INTERRUPT_FLAG::IF_SERIAL_IO_TRANSFER)
	{
		StartInterrupt(INTERRUPT_FLAG::IF_SERIAL_IO_TRANSFER);
	}
	else if(interrupts & INTERRUPT_FLAG::IF_P10_P13_LOW)
	{
		StartInterrupt(INTERRUPT_FLAG::IF_P10_P13_LOW);
	}
}

void CPU::StartInterrupt(INTERRUPT_FLAG flag)
{
	//Remove Halt (Even if IME is FALSE)
	haltMode = false;

	if(!IME) return;

	//Reset IME
	IME = false;

	//Push PC
	PushPC();

	//Disable flag
	IF &= ~flag;

	//Jump to Interrupt
	switch(flag)
	{
	case INTERRUPT_FLAG::IF_V_BLANK:
		PC = INTERRUPT_ADDRESS::IA_V_BLANK;
		break;
	case INTERRUPT_FLAG::IF_LCDC:
		PC = INTERRUPT_ADDRESS::IA_LCDC;
		break;
	case INTERRUPT_FLAG::IF_TIMER_OVERFLOW:
		PC = INTERRUPT_ADDRESS::IA_TIMER_OVERFLOW;
		break;
	case INTERRUPT_FLAG::IF_SERIAL_IO_TRANSFER:
		PC = INTERRUPT_ADDRESS::IA_SERIAL_IO_TRANSFER;
		break;
	case INTERRUPT_FLAG::IF_P10_P13_LOW:
		PC = INTERRUPT_ADDRESS::IA_P10_P13_LOW;
		break;
	}
}

void CPU::PushPC()
{
	// First 1 is substracted from SP
	// The contents of the higher portion of PC are placed on the stack. 
	// The conetns of the lower portion of PC are then placed on the stack. 
	// The contents of SP are automatically decremented by 2
	SP.w -= 1;
	MemoryController::Shared()->WriteMemory(SP.w, (PC & 0xFF00) >> 8);
	SP.w -= 1;
	MemoryController::Shared()->WriteMemory(SP.w, (PC & 0x00FF) );
}

//FLAGS //Set inline in the future?
void CPU::SetZ()
{
	AF.l |= 0x80;	//10000000
}

void CPU::ResetZ()
{
	AF.l &= 0x7F;	//01111111
}

void CPU::SetN()
{
	AF.l |= 0x40;	//01000000
}

void CPU::ResetN()
{
	AF.l &= 0xBF;	//10111111
}

void CPU::SetH()
{
	AF.l |= 0x20;	//00100000
}

void CPU::ResetH()
{
	AF.l &= 0xDF;	//11011111
}

void CPU::SetCY()
{
	AF.l |= 0x10;	//00010000
}

void CPU::ResetCY()
{
	AF.l &= 0xEF;	//11101111
}

char CPU::GetZ()
{
	return (AF.l & 0x80) != 0 ? 1 : 0;
}

char CPU::GetN()
{
	return (AF.l & 0x40) != 0 ? 1 : 0;
}

char CPU::GetH()
{
	return (AF.l & 0x20) != 0 ? 1 : 0;
}

char CPU::GetCY()
{
	return (AF.l & 0x10) != 0 ? 1 : 0;
}

//Operations With That Affects Flags
void CPU::Add8AndChangeFlags(BYTE& dest, const BYTE& s)
{
	//Set CY
	if(dest + s > 0xFF)
	{
		SetCY();
	}
	else
	{
		ResetCY();
	}

	//Set H
	if((dest & 0x0F) + (s & 0x0F) > 0x0F)
	{
		SetH();
	}
	else
	{
		ResetH();
	}

	dest += s;

	// Set Z
	if(dest == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	//Set N
	ResetN();
}

void CPU::Add16AndChangeFlags(WORD& dest, const WORD& s)
{
	//Set CY
	if(0xFFFF - dest < s)
	{
		SetCY();
	}
	else
	{
		ResetCY();
	}

	//Set H
	if((dest & 0x0FFF) + (s & 0x0FFF) > 0x0FFF)
	{
		SetH();
	}
	else
	{
		ResetH();
	}

	dest += s;

	//Set N
	ResetN();
}

void CPU::Sub8AndChangeFlags(BYTE& dest, const BYTE& s)
{
	//Set CY
	if(dest < s)
	{
		SetCY();
	}
	else
	{
		ResetCY();
	}

	//Set H
	if((dest & 0x0F) < (s & 0x0F) )
	{
		SetH();
	}
	else
	{
		ResetH();
	}


	dest -= s;

	// Set Z
	if(dest == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}
	//Set N
	SetN();
}

//REGISTER FROM CODES
BYTE& CPU::Get_r(BYTE code)
{
	switch (code)
	{
	case 0x07:
		return AF.h; //A
	case 0x00:
		return BC.h; //B
	case 0x01:
		return BC.l; //C
	case 0x02:
		return DE.h; //D
	case 0x03:
		return DE.l; //E
	case 0x04:
		return HL.h; //H
	case 0x05:
		return HL.l; //L
	}


	WriteLineC("Trying to access to Simple Register with invalid code %d", code);
	throw std::runtime_error("WRONG SINGLE REGISTER CODE");
}

DREG& CPU::Get_qq(BYTE code)
{
	switch (code)
	{
	case 0x00:
		return BC;
	case 0x01:
		return DE;
	case 0x02:
		return HL;
	case 0x03:
		return AF;
	}

	WriteLineC("Trying to access to Double Register (qq) with invalid code %d", code);
	throw std::runtime_error("WRONG DOUBLE (qq) REGISTER CODE");
}

DREG& CPU::Get_dd(BYTE code)
{
	switch (code)
	{
	case 0x00:
		return BC;
	case 0x01:
		return DE;
	case 0x02:
		return HL;
	case 0x03:
		return SP;
	}

	WriteLineC("Trying to access to Double Register (dd) with invalid code %d", code);
	throw std::runtime_error("WRONG DOUBLE (dd) REGISTER CODE");
}

bool  CPU::GetFlag_cc(BYTE code)
{
	switch (code)
	{
	case 0x00:
		return !GetZ(); //NZ
	case 0x01:
		return GetZ(); //Z
	case 0x02:
		return !GetCY(); //NCY
	case 0x03:
		return GetCY(); //CY
	}

	WriteLineC("Trying to access to Flag (cc) with invalid code %d", code);
	throw std::runtime_error("WRONG FLAT (cc) CODE");
}

//INSTRUCTIONS
unsigned char CPU::LD_R_R()
{
	//10 [ r ] [ r']
	//Loads the content of register r2 in r1
	BYTE& r1 = Get_r((opcode >> 3) & 0x07);
	BYTE& r2 = Get_r(opcode & 0x07);

	r1 = r2;

	PC += 1;

	return 1;
}

unsigned char CPU::LD_R_n()
{
	//00 [ r ] 110
	//Loads 8-Bit immediate data into register r

	BYTE& r = Get_r((opcode >> 3) & 0x07);
	r = MemoryController::Shared()->ReadMemory(PC + 1);

	PC += 2;

	return 2;
}

unsigned char CPU::LD_R_$HL()
{
	// 01 [ r ] 110
	//Loads the contents of memory (8-Bits) specified by register pair HL into register r

	BYTE& r = Get_r((opcode >> 3) & 0x07);
	r = MemoryController::Shared()->ReadMemory(HL.w);

	PC += 1;


	return 2;

}

unsigned char CPU::LD_$HL_R()
{
	//01 110 [ r ]
	// Stores the contents of register R in memory specified by register pair HL

	BYTE& r = Get_r(opcode & 0x07);
	MemoryController::Shared()->WriteMemory(HL.w, r);

	PC += 1;

	return 2;

}

unsigned char CPU::LD_$HL_n()
{
	//01 110 110
	// Loads 8-Bits immediate data n info memory specified by register pair HL

	BYTE n = MemoryController::Shared()->ReadMemory(PC + 1);
	MemoryController::Shared()->WriteMemory(HL.w, n);

	PC += 2;

	return 3;

}

unsigned char CPU::LD_A_$BC()
{
	//00 001 010
	// Loads the contents specified by the contents of register pair BC into register A
	AF.h = MemoryController::Shared()->ReadMemory(BC.w);
	
	PC += 1;

	return 2;

}

unsigned char CPU::LD_A_$DE()
{
	//00 011 010
	// Loads the contents specified by the contents of register pair DE into register A
	AF.h = MemoryController::Shared()->ReadMemory(DE.w);

	PC += 1;

	return 2;

}

unsigned char CPU::LD_A_$C()
{
	//11 110 010
	// Loads into register A the contents of the internal RAM, port register, or mode register at the address in the range
	// 0xFF00-0xFFFF specified by register C
	AF.h = MemoryController::Shared()->ReadMemory(0xFF00 | BC.l); //0xFF00 + C

	PC += 1;

	return 2;

}

unsigned char CPU::LD_$C_A()
{
	//11 100 010
	// Loads the contents of register A in the internal RAM, port register, or mode register at the address in the range
	// 0xFF00-0xFFFF specified by register C
	MemoryController::Shared()->WriteMemory(0xFF00 | BC.l, AF.h); //0xFF00 + C

	PC += 1;

	return 2;
}

unsigned char CPU::LD_A_$n()
{
	//11 110 000
	// Loads into register A the contents of the internal RAM, port register, or mode register at the address in the range
	// 0xFF00-0xFFFF specified by the 8-Bits immediate operand n
	BYTE n = MemoryController::Shared()->ReadMemory(PC + 1);
	AF.h = MemoryController::Shared()->ReadMemory(0xFF00 | n); //0xFF00 + C

	PC += 2;

	return 3;

}

unsigned char CPU::LD_$n_A()
{
	//11 100 000
	// Loads the contents of register A in the internal RAM, port register, or mode register at the address in the range
	// 0xFF00-0xFFFF specified by the 8-Bits immediate operand n
	BYTE n = MemoryController::Shared()->ReadMemory(PC + 1);
	MemoryController::Shared()->WriteMemory(0xFF00 | n, AF.h); //0xFF00 + n

	PC += 2;

	return 3;

}

unsigned char CPU::LD_A_$nn()
{
	//11 111 010
	// Loads into register A the contents of the internal RAM or register specified by 16-Bit immediate openrand nn
	WORD n1 = MemoryController::Shared()->ReadMemory(PC + 2);
	WORD n2 = MemoryController::Shared()->ReadMemory(PC + 1);

	WORD address = (n1 << 8) | n2;

	AF.h = MemoryController::Shared()->ReadMemory(address);

	PC += 3;

	return 4;

}

unsigned char CPU::LD_$nn_A()
{
	//11 101 010
	// Loads the contents of register A in the internal RAM, port register, or mode register specified by 16-Bits
	// immediate operand nn
	WORD n1 = MemoryController::Shared()->ReadMemory(PC + 2);
	WORD n2 = MemoryController::Shared()->ReadMemory(PC + 1);

	WORD address = (n1 << 8) | n2;

	MemoryController::Shared()->WriteMemory(address, AF.h);

	PC += 3;

	return 4;

}

unsigned char CPU::LD_A_$HLI()
{
	//00 101 010
	// Loads in register A the contents of memory specified by the contents of register pair HL and simultaneously
	// increments the conents of HL
	AF.h = MemoryController::Shared()->ReadMemory(HL.w);
	HL.w += 1;

	PC += 1;

	return 2;

}

unsigned char CPU::LD_A_$HLD()
{
	//00 111 010
	// Loads in register A the contents of memory specified by the contents of register pair HL and simultaneously
	// decrements the conents of HL
	AF.h = MemoryController::Shared()->ReadMemory(HL.w);
	HL.w -= 1;

	PC += 1;

	return 2;

}

unsigned char CPU::LD_$BC_A()
{
	// 00 000 010
	// Stores the contents of register A in the memory specified by register pair BC

	MemoryController::Shared()->WriteMemory(BC.w, AF.h);
		
	PC += 1;

	return 2;

}

unsigned char CPU::LD_$DE_A()
{
	// 00 010 010
	// Stores the contents of register A in the memory specified by register pair BC

	MemoryController::Shared()->WriteMemory(DE.w, AF.h);

	PC += 1;

	return 2;

}

unsigned char CPU::LD_$HLI_A()
{
	// 00 100 010
	// Stores the contents of register A in the memory specified by register pair HL and simultaneously
	// increments the contents of HL

	MemoryController::Shared()->WriteMemory(HL.w, AF.h);
	HL.w += 1;

	PC += 1;

	return 2;

}

unsigned char CPU::LD_$HLD_A()
{
	// 00 110 010
	// Stores the contents of register A in the memory specified by register pair HL and simultaneously
	// decrements the contents of HL

	MemoryController::Shared()->WriteMemory(HL.w, AF.h);
	HL.w -= 1;

	PC += 1;

	return 2;

}

unsigned char CPU::LD_dd_nn()
{
	//00 dd0 001
	// Loads 2 bytes of immediate data to register pair dd
	BYTE dd = (opcode >> 4) & 0x03;
	DREG& dd_reg = Get_dd(dd);

	BYTE nl = MemoryController::Shared()->ReadMemory(PC + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(PC + 2);

	dd_reg.h = nh;
	dd_reg.l = nl;

	PC += 3;

	return 3;

}

unsigned char CPU::LD_SP_HL()
{
	// 11 111 001
	// Loads the contents of register pair HL in stack pointer SP

	SP.w = HL.w;

	PC += 1;

	return 2;

}

unsigned char CPU::PUSH_qq()
{
	// 11 qq0 101
	// Pushes the contents of register pair qq onto the memory stack.
	// First 1 is substracted from SP and the contents of the higher portion 
	// of qq are placed on the stack. The conetns of the lower portion of qq are then placed
	// on the stack. The contents of SP are automatically decremented by 2

	DREG& reg_val = Get_qq((opcode >> 4) & 0x03);

	SP.w -= 1;
	MemoryController::Shared()->WriteMemory(SP.w, reg_val.h);
	SP.w -= 1;
	MemoryController::Shared()->WriteMemory(SP.w, reg_val.l);

	PC += 1;

	return 4;

}

unsigned char CPU::POP_qq()
{
	// 11 qq0 001
	// Pops contents from the memory stack into reguster pair qq
	// First the conetns of memory specified by the conents of SP are loeaded in the lower portion of qq. Next,
	// the contnets of SP are incremented by 1 and the contents of the memory they specify are loaded in the
	// upper portion of qq. The contents of SP are automatically incremented by 2.

	DREG& reg_val = Get_qq((opcode >> 4) & 0x03);
	
	BYTE l = MemoryController::Shared()->ReadMemory(SP.w);
	BYTE h = MemoryController::Shared()->ReadMemory(SP.w + 1);

	reg_val.h = h;
	reg_val.l = l;
	
	SP.w += 2;
	PC += 1;

	return 4;

}

unsigned char CPU::LDHL_PS_e()
{
	// 11 111 000
	// The 8-Bits operand e is added to SP and the result is stored in HL
	BYTE e = MemoryController::Shared()->ReadMemory(PC + 1);
	
	ResetZ();
	ResetN();

	if((SP.w & 0x000F) + (e & 0x0F) > 0x0F)
	{
		SetH();
	}
	else
	{
		ResetH();
	}

	if((SP.w & 0x00FF) + e > 0xFF)
	{
		SetCY();
	}
	else
	{
		ResetCY();
	}

	HL.w = SP.w + e;

	PC += 2;

	return 3;

}

unsigned char CPU::LD_$nn_SP()
{
	// 00 001 000
	// Stores the lower byte SP at address nn specified by the 16-Bits immediate operand nn and the upper
	// byte of sp at address nn + 1

	BYTE nl = MemoryController::Shared()->ReadMemory(PC + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(PC + 2);

	WORD address = nh;
	address <<= 8;
	address |= nl;

	MemoryController::Shared()->WriteMemory(address, SP.l);
	MemoryController::Shared()->WriteMemory(address+1, SP.h);

	PC += 3;

	return 5;


}

unsigned char CPU::ADD_A_r()
{
	// 10 000 [ r ]
	// Adds the contents of register r to those of register A and stores the result in register A
	BYTE rCode = opcode & 0x07;
	BYTE& r = Get_r(rCode);

	Add8AndChangeFlags(AF.h, r);

	PC += 1;

	return 1;

}

unsigned char CPU::ADD_A_n()
{
	// 11 000 110
	// Adds 8-Bits immediate operand n to the contetns of register A and stores the results in register A
	BYTE n = MemoryController::Shared()->ReadMemory(PC + 1);

	Add8AndChangeFlags(AF.h, n);

	PC += 2;

	return 2;

}

unsigned char CPU::ADD_A_$HL()
{
	// 10 000 110
	// Adds the contents of memory specified by the contents of register pair HL to the contents of register A
	// and stores the results in register A

	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);
	Add8AndChangeFlags(AF.h, n);

	PC += 1;

	return 2;

}

unsigned char CPU::ADC_A_r()
{
	// 10 001 [ r ]
	// Adds the contents of operand r and CY to the contents of register A and stores the results in register A.
	BYTE rCode = opcode & 0x07;
	BYTE& r = Get_r(rCode);

	Add8AndChangeFlags(AF.h, r + GetCY());
	PC += 1;

	return 1;

}

unsigned char CPU::ADC_A_n()
{
	// 11 001 110
	// Adds the contents of operand n and CY to the contents of register A and stores the results in register A.
	BYTE n = MemoryController::Shared()->ReadMemory(PC + 1);
	Add8AndChangeFlags(AF.h, n + GetCY());
	PC += 2;

	return 2;

}

unsigned char CPU::ADC_A_$HL()
{
	// 10 001 110
	// Adds the contents of operand (HL) and CY to the contents of register A and stores the results in register A.
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);
	Add8AndChangeFlags(AF.h, n + GetCY());
	PC += 1;

	return 2;

}

unsigned char CPU::SUB_A_r()
{
	// 10 010 [ r ]
	// Substracts the contents of register r from those of register A and stores the result in register A
	BYTE rCode = opcode & 0x07;
	BYTE& r = Get_r(rCode);

	Sub8AndChangeFlags(AF.h, r);

	PC += 1;

	return 1;

}


unsigned char CPU::SUB_A_n()
{
	// 11 010 110
	// Substracts the contents of n from those of register A and stores the result in register A
	BYTE n = MemoryController::Shared()->ReadMemory(PC + 1);
	Sub8AndChangeFlags(AF.h, n);

	PC += 2;

	return 2;

}


unsigned char CPU::SUB_A_$HL()
{
	// 10 010 110
	// Substracts the contents of HL from those of register A and stores the result in register A
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);
	Sub8AndChangeFlags(AF.h, n);

	PC += 1;

	return 2;

}

unsigned char CPU::SBC_A_r()
{
	// 10 011 [ r ]
	// Substracts the contents of register r and CY from those of register A and stores the result in register A
	BYTE rCode = opcode & 0x07;
	BYTE& r = Get_r(rCode);

	Sub8AndChangeFlags(AF.h, r + GetCY());

	PC += 1;

	return 1;

}


unsigned char CPU::SBC_A_n()
{
	// 11 011 110
	// Substracts the contents of n and CY from those of register A and stores the result in register A
	BYTE n = MemoryController::Shared()->ReadMemory(PC + 1);
	Sub8AndChangeFlags(AF.h, n + GetCY());

	PC += 2;

	return 2;

}


unsigned char CPU::SBC_A_$HL()
{
	// 10 011 110
	// Substracts the contents of (HL) and CY from those of register A and stores the result in register A
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);
	Sub8AndChangeFlags(AF.h, n + GetCY());

	PC += 1;

	return 2;

}

unsigned char CPU::AND_A_r()
{
	// 10 100 [ r ]
	// Takes the logical-AND for each bit of the contents of register r and register A and stores the results
	// in regisger A
	BYTE rCode = opcode & 0x07;
	BYTE& r = Get_r(rCode);

	AF.h &= r;

	ResetCY();
	SetH();
	ResetN();

	if(AF.h == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 1;

}

unsigned char CPU::AND_A_n()
{
	// 11 100 110
	// Takes the logical-AND for each bit of the contents of n and register A and stores the results
	// in regisger A
	BYTE n= MemoryController::Shared()->ReadMemory(PC + 1);

	AF.h &= n;

	ResetCY();
	SetH();
	ResetN();

	if(AF.h == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 2;

	return 2;

}

unsigned char CPU::AND_A_$HL()
{
	// 10 100 110
	// Takes the logical-AND for each bit of the contents of (HL) and register A and stores the results
	// in regisger A
	BYTE n= MemoryController::Shared()->ReadMemory(HL.w);

	AF.h &= n;

	ResetCY();
	SetH();
	ResetN();

	if(AF.h == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 2;

}

unsigned char CPU::OR_A_r()
{
	// 10 110 [ r ]
	// Takes the logical-OR for each bit of the contents of register r and register A and stores the results
	// in regisger A
	BYTE rCode = opcode & 0x07;
	BYTE& r = Get_r(rCode);

	AF.h |= r;

	ResetCY();
	ResetH();
	ResetN();

	if(AF.h == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 1;

}

unsigned char CPU::OR_A_n()
{
	// 11 110 110
	// Takes the logical-OR for each bit of the contents of n and register A and stores the results
	// in regisger A
	BYTE n= MemoryController::Shared()->ReadMemory(PC + 1);

	AF.h |= n;

	ResetCY();
	ResetH();
	ResetN();

	if(AF.h == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 2;

	return 2;

}

unsigned char CPU::OR_A_$HL()
{
	// 10 110 110
	// Takes the logical-OR for each bit of the contents of (HL) and register A and stores the results
	// in regisger A
	BYTE n= MemoryController::Shared()->ReadMemory(HL.w);

	AF.h |= n;

	ResetCY();
	ResetH();
	ResetN();

	if(AF.h == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 2;

}

unsigned char CPU::XOR_A_r()
{
	// 10 101 [ r ]
	// Takes the logical-XOR for each bit of the contents of register r and register A and stores the results
	// in regisger A
	BYTE rCode = opcode & 0x07;
	BYTE& r = Get_r(rCode);

	AF.h ^= r;

	ResetCY();
	ResetH();
	ResetN();

	if(AF.h == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 1;

}

unsigned char CPU::XOR_A_n()
{
	// 11 101 110
	// Takes the logical-XOR for each bit of the contents of n and register A and stores the results
	// in regisger A
	BYTE n= MemoryController::Shared()->ReadMemory(PC + 1);

	AF.h ^= n;

	ResetCY();
	ResetH();
	ResetN();

	if(AF.h == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 2;

	return 2;

}

unsigned char CPU::XOR_A_$HL()
{
	// 10 101 110
	// Takes the logical-XOR for each bit of the contents of (HL) and register A and stores the results
	// in regisger A
	BYTE n= MemoryController::Shared()->ReadMemory(HL.w);

	AF.h ^= n;

	ResetCY();
	ResetH();
	ResetN();

	if(AF.h == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 2;

}

unsigned char CPU::CP_A_r()
{
	// 10 111 [ r ]
	// Compares the contents of register r and register A and sets the flags as a substraction
	BYTE rCode = opcode & 0x07;
	BYTE& r = Get_r(rCode);

	BYTE aCopy = AF.h;

	Sub8AndChangeFlags(aCopy, r);

	PC += 1;

	return 1;

}


unsigned char CPU::CP_A_n()
{
	// 11 111 110
	// Compares the contents of n and register A and sets the flags as a substraction
	BYTE n = MemoryController::Shared()->ReadMemory(PC + 1);
	BYTE aCopy = AF.h;
	Sub8AndChangeFlags(aCopy, n);

	PC += 2;

	return 2;

}


unsigned char CPU::CP_A_$HL()
{
	// 10 11 110
	// Compares the contents of (HL) and register A and sets the flags as a substraction
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);
	BYTE aCopy = AF.h;
	Sub8AndChangeFlags(aCopy, n);

	PC += 1;

	return 2;

}

unsigned char CPU::INC_r()
{
	// 00 [ r ] 100
	// Increments the contents of register r by 1
	BYTE& r = Get_r((opcode >> 3) & 0x07);


	//Set CY
	ResetCY();
	//Set N
	ResetN();

	//Set H
	if((r & 0x0F) == 0x0F)
	{
		SetH();
	}
	else
	{
		ResetH();
	}

	r += 1;

	// Set Z
	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 1;

}

unsigned char CPU::INC_$HL()
{
	// 00 110 100
	// Increments the contents of (HL) by 1
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);


	//Set CY
	ResetCY();
	//Set N
	ResetN();

	//Set H
	if((n & 0x0F) == 0x0F)
	{
		SetH();
	}
	else
	{
		ResetH();
	}

	n += 1;
	MemoryController::Shared()->WriteMemory(HL.w, n);
	

	// Set Z
	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 3;

}

unsigned char CPU::DEC_r()
{
	// 00 [ r ] 101
	// Decrements the contents of register r by 1
	BYTE& r = Get_r((opcode >> 3) & 0x07);


	//Set CY
	ResetCY();
	//Set N
	SetN();

	//Set H
	if((r & 0x0F) == 0x00)
	{
		SetH();
	}
	else
	{
		ResetH();
	}

	if(r == 1)
	{
		int b = 0;
	}
	r -= 1;

	// Set Z
	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 1;

}

unsigned char CPU::DEC_$HL()
{
	// 00 110 101
	// Decrements the contents of (HL) by 1
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);


	//Set CY
	ResetCY();
	//Set N
	SetN();

	//Set H
	if((n & 0x0F) == 0x00)
	{
		SetH();
	}
	else
	{
		ResetH();
	}

	n -= 1;
	MemoryController::Shared()->WriteMemory(HL.w, n);


	// Set Z
	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	PC += 1;

	return 3;

}

unsigned char CPU::ADD_HL_ss()
{
	// 00 ss1 001
	// Adds the contents of register pair ss to the contents of register pair HL and stores the results in HL

	BYTE code = (opcode >> 4) & 0x03;
	DREG& reg = Get_dd(code);

	Add16AndChangeFlags(HL.w, reg.w);

	PC += 1;

	return 2;

}

unsigned char CPU::ADD_SP_e()
{
	//11 101 000
	// Adds the contents of the 8-Bits immediate operand e and SP and stores the results in SP
	BYTE e = MemoryController::Shared()->ReadMemory(PC + 1);
	Add16AndChangeFlags(SP.w, e);

	ResetZ();

	PC += 2;

	return 4;

}

unsigned char CPU::INC_ss()
{
	// 00 ss0 011
	// Increments the contents of register pair ss by 1
	BYTE code = (opcode >> 4) & 0x03;
	DREG& reg = Get_dd(code);

	reg.w += 1;

	PC += 1;

	return 2;

}

unsigned char CPU::DEC_ss()
{
	// 00 ss1 011
	// Increments the contents of register pair ss by 1
	BYTE code = (opcode >> 4) & 0x03;
	DREG& reg = Get_dd(code);

	reg.w -= 1;

	PC += 1;

	return 2;

}

unsigned char CPU::RLCA()
{
	// 00 000 111
	// Rotates the contents of register A to the left.
	// That is, the contents of bit 0 are copied to bit 1, and the previous contents of bit 1
	// is copied to bit 2, and so on.
	// The contents of bit 7 is placed in both CY and bit 0 of register A

	BYTE bit7 = AF.h >> 7;
	AF.h <<= 1;
	AF.h |= bit7;

	if(bit7 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	ResetH();
	ResetN();
	ResetZ();

	PC += 1;

	return 1;

}

unsigned char CPU::RLA()
{
	// 00 010 111
	// Rotates the contents of register A + CY to the left.
	
	BYTE bit7 = AF.h >> 7;
	AF.h <<= 1;
	AF.h |= GetCY();

	if(bit7 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	ResetH();
	ResetN();
	ResetZ();

	PC += 1;

	return 1;

}

unsigned char CPU::RRCA()
{
	// 00 001 111
	// Rotates the contents of register A to the right.
	

	BYTE bit0 = AF.h & 0x01;
	AF.h >>= 1;
	AF.h |= bit0 << 7;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	ResetH();
	ResetN();
	ResetZ();

	PC += 1;

	return 1;

}

unsigned char CPU::RRA()
{
	// 00 011 111
	// Rotates the contents of register A + CY to the right.

	BYTE bit0 = AF.h & 0x01;
	AF.h >>= 1;
	AF.h |= GetCY() << 7;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	ResetH();
	ResetN();
	ResetZ();

	PC += 1;

	return 1;

}


//CB PREFIX
unsigned char CPU::PREFIX_0xBC()
{
	// 11 001 011 ->

	PC += 1;
	opcode = MemoryController::Shared()->ReadMemory(PC);
	instructionCBCounter[opcode]++;

	return (this->*cbInstructions[opcode])();
}

unsigned char CPU::RLC_r()
{
	//11 001 011 | 00 000 [ r ]
	//Rotates the contents of register r to the left
	BYTE& r = Get_r(opcode & 0x07);

	BYTE bit7 = r >> 7;
	r <<= 1;
	r |= bit7;

	if(bit7 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();
	
	PC += 1;

	return 2;

}

unsigned char CPU::RLC_$HL()
{
	//11 001 011 | 00 000 110
	//Rotates the contents of register r to the left
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	BYTE bit7 = n >> 7;
	n <<= 1;
	n |= bit7;

	if(bit7 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	MemoryController::Shared()->WriteMemory(HL.w, n);

	PC += 1;

	return 4;

}

unsigned char CPU::RL_r()
{
	//11 001 011 | 00 010 [ r ]
	//Rotates the contents of register r + CY to the left
	BYTE& r = Get_r(opcode & 0x07);

	BYTE bit7 = r >> 7;
	r <<= 1;
	r |=  GetCY();

	if(bit7 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	PC += 1;

	return 2;

}

unsigned char CPU::RL_$HL()
{
	//11 001 011 | 00 010 110
	//Rotates the contents of (HL) + CY to the left
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	BYTE bit7 = n >> 7;
	n <<= 1;
	n |=  GetCY();

	if(bit7 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	MemoryController::Shared()->WriteMemory(HL.w, n);

	PC += 1;

	return 4;

}

unsigned char CPU::RRC_r()
{
	//11 001 011 | 00 001 [ r ]
	//Rotates the contents of register r to the right
	BYTE& r = Get_r(opcode & 0x07);

	BYTE bit0 = r & 0x01;
	r >>= 1;
	r |= bit0 << 7;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	PC += 1;

	return 2;

}

unsigned char CPU::RRC_$HL()
{
	//11 001 011 | 00 001 110
	//Rotates the contents of register r to the right
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	BYTE bit0 = n & 0x01;
	n >>= 1;
	n |= bit0 << 7;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	MemoryController::Shared()->WriteMemory(HL.w, n);

	PC += 1;

	return 4;

}

unsigned char CPU::RR_r()
{
	//11 001 011 | 00 011 [ r ]
	//Rotates the contents of register r + CY to the right
	BYTE& r = Get_r(opcode & 0x07);

	BYTE bit0 = r & 0x01;
	r >>= 1;
	r |= GetCY() << 7;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	PC += 1;

	return 2;

}

unsigned char CPU::RR_$HL()
{
	//11 001 011 | 00 011 110
	//Rotates the contents of (HL) + CY to the right
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	BYTE bit0 = n & 0x01;
	n >>= 1;
	n |= GetCY() << 7;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	MemoryController::Shared()->WriteMemory(HL.w, n);

	PC += 1;

	return 4;

}

unsigned char CPU::SLA_r()
{
	// 11 001 011 | 00 100 [ r ]
	// Shift the contents of register r to the left, to CY. BIT 0 of r is reset

	BYTE& r = Get_r(opcode & 0x07);

	BYTE bit7 = r >> 7;
	r <<= 1;
	
	if(bit7 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	PC += 1;

	return 2;

}

unsigned char CPU::SLA_$HL()
{
	// 11 001 011 | 00 100 110
	// Shift the contents of (HL) to the left, to CY. BIT 0 of r is reset

	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	BYTE bit7 = n >> 7;
	n <<= 1;

	if(bit7 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	MemoryController::Shared()->WriteMemory(HL.w, n);

	ResetH();
	ResetN();

	PC += 1;

	return 4;

}

unsigned char CPU::SRA_r()
{
	// 11 001 011 | 00 101 [ r ]
	// Shift the contents of register r to the right, to CY. BIT 7 is untouched

	BYTE& r = Get_r(opcode & 0x07);

	BYTE bit7 = r >> 7;
	BYTE bit0 = r & 0x01;
	r >>= 1;
	r |= bit7 << 7;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	PC += 1;

	return 2;

}

unsigned char CPU::SRA_$HL()
{
	// 11 001 011 | 00 101 110
	// Shift the contents of (HL) to the left, to CY. BIT 7 is untouched

	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	BYTE bit7 = n >> 7;
	BYTE bit0 = n & 0x01;
	n >>= 1;
	n |= bit7 << 7;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	MemoryController::Shared()->WriteMemory(HL.w, n);

	ResetH();
	ResetN();

	PC += 1;

	return 4;

}

unsigned char CPU::SRL_r()
{
	// 11 001 011 | 00 111 [ r ]
	// Shifts the contetns of register r to the right, to CY. BIT 7 is reset

	BYTE& r = Get_r(opcode & 0x07);

	BYTE bit0 = r & 0x01;
	r >>= 1;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	PC += 1;

	return 2;

}

unsigned char CPU::SRL_$HL()
{
	// 11 001 011 | 00 111 110
	// Shift the contents of (HL) to the left, to CY. BIT 7 of r is reset

	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	BYTE bit0 = n & 0x01;
	n >>= 1;

	if(bit0 == 0x00)
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	MemoryController::Shared()->WriteMemory(HL.w, n);

	ResetH();
	ResetN();

	PC += 1;

	return 4;

}

unsigned char CPU::SWAP_r()
{
	// 11 001 011 | 00 110 [ r ]
	// Shift the contents of the lower order 4 bits (0-3) of register r unmodified to the hight-order 4 bits (4-7)
	// of that operand and shifts the contents of hte higher order 4 bits to the lower-order 4 bits

	BYTE& r = Get_r(opcode & 0x07);

	BYTE lowNibble = r & 0x0F;
	r >>= 4;
	r |= lowNibble << 4;

	if(r == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();
	ResetCY();

	PC += 1;

	return 2;

}

unsigned char CPU::SWAP_$HL()
{
	// 11 001 011 | 00 110 110
	// Shift the contents of the lower order 4 bits (0-3) of register r unmodified to the hight-order 4 bits (4-7)
	// of that operand and shifts the contents of hte higher order 4 bits to the lower-order 4 bits

	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	BYTE lowNibble = n & 0x0F;
	n >>= 4;
	n |= lowNibble << 4;

	MemoryController::Shared()->WriteMemory(HL.w, n);

	if(n == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();
	ResetCY();

	PC += 1;

	return 4;

}

unsigned char CPU::BIT_b_r()
{
	// 11 001 011 | 01 [ b ] [ r ]
	// Copies the complement of the contents of the specified bit in register r to the Z flag of the program status word (PSW)

	BYTE b = (opcode >> 3) & 0x07;
	BYTE& r = Get_r(opcode & 0x07);

	BYTE bit = (r << b) & 0x80;

	if(bit == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	SetH();
	ResetN();

	PC += 1;

	return 2;

}

unsigned char CPU::BIT_b_$HL()
{
	// 11 001 011 | 01 [ b ] 110
	// Copies the complement of the contents of the specified bit in (HL) to the Z flag of the program status word (PSW)

	BYTE b = (opcode >> 3) & 0x07;
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	BYTE bit = (n << b) & 0x80;

	if(bit == 0x00)
	{
		SetZ();
	}
	else
	{
		ResetZ();
	}

	ResetH();
	ResetN();

	MemoryController::Shared()->WriteMemory(HL.w, n);

	PC += 1;

	return 3;

}

unsigned char CPU::SET_b_r()
{
	// 11 001 011 | 11 [ b ] [ r ]
	// Sets to 1 the specified bit in register r

	BYTE b = (opcode >> 3) & 0x07;
	BYTE& r = Get_r(opcode & 0x07);

	switch(b)
	{
	case 0x00:
		r |= 0x01;
		break;
	case 0x01:
		r |= 0x02;
		break;
	case 0x02:
		r |= 0x04;
		break;
	case 0x03:
		r |= 0x08;
		break;
	case 0x04:
		r |= 0x10;
		break;
	case 0x05:
		r |= 0x20;
		break;
	case 0x06:
		r |= 0x40;
		break;
	case 0x07:
		r |= 0x80;
		break;
	default:
		WriteLineC("Trying to access to Bit (bb) with invalid code %d", b);
		throw std::runtime_error("WRONG FLAT (bb) CODE");
	}

	PC += 1;

	return 2;

}

unsigned char CPU::SET_b_$HL()
{
	// 11 001 011 | 11 [ b ] 110
	// Sets to 1 the specified bit in (HL)

	BYTE b = (opcode >> 3) & 0x07;
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	switch(b)
	{
	case 0x00:
		n |= 0x01;
		break;
	case 0x01:
		n |= 0x02;
		break;
	case 0x02:
		n |= 0x04;
		break;
	case 0x03:
		n |= 0x08;
		break;
	case 0x04:
		n |= 0x10;
		break;
	case 0x05:
		n|= 0x20;
		break;
	case 0x06:
		n |= 0x40;
		break;
	case 0x07:
		n |= 0x80;
		break;
	default:
		WriteLineC("Trying to access to Bit (bb) with invalid code %d", b);
		throw std::runtime_error("WRONG FLAT (bb) CODE");
	}

	MemoryController::Shared()->WriteMemory(HL.w, n);

	PC += 1;

	return 2;

}

unsigned char CPU::RES_b_r()
{
	// 11 001 011 | 10 [ b ] [ r ]
	// Sets to 0 the specified bit in register r

	BYTE b = (opcode >> 3) & 0x07;
	BYTE& r = Get_r(opcode & 0x07);

	switch(b)
	{
	case 0x00:
		r &= 0xFE;
		break;
	case 0x01:
		r &= 0xFD;
		break;
	case 0x02:
		r &= 0xFB;
		break;
	case 0x03:
		r &= 0xF7;
		break;
	case 0x04:
		r &= 0xEF;
		break;
	case 0x05:
		r &= 0xDF;
		break;
	case 0x06:
		r &= 0xBF;
		break;
	case 0x07:
		r &= 0x7F;
		break;
	default:
		WriteLineC("Trying to access to Bit (bb) with invalid code %d", b);
		throw std::runtime_error("WRONG FLAT (bb) CODE");
	}

	PC += 1;

	return 2;

}

unsigned char CPU::RES_b_$HL()
{
	// 11 001 011 | 11 [ b ] 110
	// Sets to 1 the specified bit in (HL)

	BYTE b = (opcode >> 3) & 0x07;
	BYTE n = MemoryController::Shared()->ReadMemory(HL.w);

	switch(b)
	{
	case 0x00:
		n &= 0xFE;
		break;
	case 0x01:
		n &= 0xFD;
		break;
	case 0x02:
		n &= 0xFB;
		break;
	case 0x03:
		n &= 0xF7;
		break;
	case 0x04:
		n &= 0xEF;
		break;
	case 0x05:
		n &= 0xDF;
		break;
	case 0x06:
		n &= 0xBF;
		break;
	case 0x07:
		n &= 0x7F;
		break;
	default:
		WriteLineC("Trying to access to Bit (bb) with invalid code %d", b);
		throw std::runtime_error("WRONG FLAT (bb) CODE");
	}

	MemoryController::Shared()->WriteMemory(HL.w, n);

	PC += 1;

	return 2;

}

unsigned char CPU::JP_nn()
{
	// 11 000 010
	// Loads operand nn in the PC.
	BYTE nl = MemoryController::Shared()->ReadMemory(PC + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(PC + 2);


	PC = nh;
	PC <<= 8;
	PC |= nl;


	return 3;

}

unsigned char CPU::JP_cc_nn()
{
	// 11 0cc 010
	// Loads operand nn in the PC if condition cc and the flag status match.
	// If condition cc and the flag status do not match, the contetns of are incremented in 3

	BYTE ccCode = (opcode >> 3) & 0x03;
	
	if(GetFlag_cc(ccCode))
	{
		BYTE nl = MemoryController::Shared()->ReadMemory(PC + 1);
		BYTE nh = MemoryController::Shared()->ReadMemory(PC + 2);


		PC = nh;
		PC <<= 8;
		PC |= nl;


		return 4;

	}
	else
	{
		PC += 3;

		return 3;

	}
}

unsigned char CPU::JR_e()
{
	// 00 011 000
	// Jumps -127 +129 steps from current address
	// (add 2 bacause of the normal PC increment)

	signed char e = MemoryController::Shared()->ReadMemory(PC + 1);
	PC += e + 2;


	return 3;

}

unsigned char CPU::JR_cc_e()
{
	// 00 1cc 000
	// Jumps -127 +129 steps from current address (add 2 bacause of the normal PC increment)
	// if condition cc and flags match
	BYTE ccCode = (opcode >> 3) & 0x03;

	if(GetFlag_cc(ccCode))
	{

		signed char e = MemoryController::Shared()->ReadMemory(PC + 1);
		PC += e + 2;

		return 3;

	}
	else
	{
		PC += 2;

		return 2;

	}
}

unsigned char CPU::JP_HL()
{
	// 11 101 001
	// Loads the contents of register pair HL into PC

	PC = HL.w;


	return 1;

}

unsigned char CPU::CALL_nn()
{
	// 11 001 101
	// (SP - 1) <- PC[H]
	// (SP - 2) <- PC[L]
	// PC <- nn
	// SP <- SP-2
	
	MemoryController::Shared()->WriteMemory(SP.w - 1, ( (PC + 3) >> 8));
	MemoryController::Shared()->WriteMemory(SP.w - 2, ( (PC + 3) & 0xFF));

	BYTE nl = MemoryController::Shared()->ReadMemory(PC + 1);
	BYTE nh = MemoryController::Shared()->ReadMemory(PC + 2);


	PC = nh;
	PC <<= 8;
	PC |= nl;

	SP.w -= 2;


	return 6;
}

unsigned char CPU::CALL_cc_nn()
{
	// 11 0cc 100
	// if cc true,
	// (SP - 1) <- PC[H]
	// (SP - 2) <- PC[L]
	// PC <- nn
	// SP <- SP-2

	BYTE ccCode = (opcode >> 3) & 0x03;

	if(GetFlag_cc(ccCode))
	{

		MemoryController::Shared()->WriteMemory(SP.w - 1, ((PC + 3) >> 8));
		MemoryController::Shared()->WriteMemory(SP.w - 2, ((PC + 3) & 0xFF));

		BYTE nl = MemoryController::Shared()->ReadMemory(PC + 1);
		BYTE nh = MemoryController::Shared()->ReadMemory(PC + 2);


		PC = nh;
		PC <<= 8;
		PC |= nl;

		SP.w -= 2;


		return 6;

	}
	else
	{
		PC += 3;


		return 3;

	}


}

unsigned char CPU::RET()
{
	// 11 001 001
	// PC[L] <- (SP)
	// PC[H] <- (SP + 1)
	// SP <- SP + 2

	BYTE pcL = MemoryController::Shared()->ReadMemory(SP.w);
	BYTE pcH = MemoryController::Shared()->ReadMemory(SP.w + 1);

	PC = pcH;
	PC <<= 8;
	PC |= pcL;

	SP.w += 2;


	return 4;

}

unsigned char CPU::RETI()
{
	// 11 011 001
	// Same as RET, but used when an interrupt service routine finishes
	// The addess of the return from the interrupt is loaded in program counter PC
	// The master interrupt enable FLAG is returned to its pre-interrupt status

	IME = true;

	BYTE pcL = MemoryController::Shared()->ReadMemory(SP.w);
	BYTE pcH = MemoryController::Shared()->ReadMemory(SP.w + 1);

	PC = pcH;
	PC <<= 8;
	PC |= pcL;

	SP.w += 2;


	return 4;

}

unsigned char CPU::RET_cc()
{
	// 11 0cc 000
	// if cc true
	// PC[L] <- (SP)
	// PC[H] <- (SP + 1)
	// SP <- SP + 2

	BYTE ccCode = (opcode >> 3) & 0x03;

	if(GetFlag_cc(ccCode))
	{
		BYTE pcL = MemoryController::Shared()->ReadMemory(SP.w);
		BYTE pcH = MemoryController::Shared()->ReadMemory(SP.w + 1);

		PC = pcH;
		PC <<= 8;
		PC |= pcL;

		SP.w += 2;


		return 5;

	}
	else
	{
		PC += 1;

		return 2;

	}
}

unsigned char CPU::RST_t()
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

	MemoryController::Shared()->WriteMemory(SP.w - 1, ((PC + 1) >> 8));
	MemoryController::Shared()->WriteMemory(SP.w - 2, ((PC + 1) & 0xFF));

	BYTE t = (opcode >> 3) & 0x07;
	
	switch(t)
	{
	case 0x00:
		PC = 0x00;
		break;
	case 0x01:
		PC = 0x08;
		break;
	case 0x02:
		PC = 0x10;
		break;
	case 0x03:
		PC = 0x18;
		break;
	case 0x04:
		PC = 0x20;
		break;
	case 0x05:
		PC = 0x28;
		break;
	case 0x06:
		PC = 0x30;
		break;
	case 0x07:
		PC = 0x38;
		break;
	}

	SP.w -= 2;


	return 4;

}

unsigned char CPU::DAA()
{
	// 00 100 111
	// When performing addition adn substraction, binary coded decimal representation is used to set the
	// contents of register A to a binary coded decimal number (BCD)

	BYTE add = 0x00;

	if(!GetN())
	{
		//Check if A's High-Nibble > 0x9 or CY Flag, and add 0x60 and Set CY if any of both conditions is matched
		if(GetCY() || AF.h > 0x99)
		{
			add += 0x60;
			SetCY();
		}

		//Check if A's Low-Nibble > 0x9 or H Flag, and add 0x06 if any of both conditions is matched
		if(GetH() || ((AF.h & 0x0F) > 0x9))
		{
			add += 0x06;
		}
	}
	else
	{
		//Check if A's High-Nibble > 0x9 or CY Flag, and sub 0x60 (add A0) and Set CY if any of both conditions is matched
		if(GetCY() || AF.h > 0x99)
		{
			add += 0xA0;
			SetCY();
		}
		
		//Check if A's Low-Nibble > 0x9 or H Flag, and sub 0x06 (add FA)  if any of both conditions is matched
		if(GetH() || ((AF.h & 0x0F) > 0x9))
		{
			add += 0xFA;
		}
	}

	AF.h += add;

	PC += 1;


	return 4;

}

unsigned char CPU::CPL()
{
	// 00 101 111
	// Takes the one's complement of register A

	AF.h = ~AF.h;

	SetN();
	SetH();

	PC += 1;

	return 1;

}

unsigned char CPU::NOP()
{
	// 00 000 000
	// Only advances the PC by 1. Performs no other operations to have effect.

	PC += 1;

	return 1;

}

unsigned char CPU::CCF()
{
	// 00 111 111
	// CY	<-	!CY
	// H	<-	0
	// N	<-	0
	// Z --

	if(GetCY())
	{
		ResetCY();
	}
	else
	{
		SetCY();
	}

	ResetH();
	ResetN();

	PC += 1;

	return 1;

}

unsigned char CPU::SCF()
{
	// 00 110 111
	// CY	<-	1
	// H	<-	0
	// N	<-	0
	// Z --

	
	SetCY();
	ResetH();
	ResetN();

	PC += 1;

	return 1;

}

unsigned char CPU::DI()
{
	// 11	110	011  
	// IME  <- 0 

	IME = false;

	PC += 1;

	return 1;

}

unsigned char CPU::EI()
{
	// 11	111	011  
	// IME  <- 1 

	IME = true;

	PC += 1;

	return 1;

}

unsigned char CPU::HALT()
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

	haltMode = true;

	PC += 1;

	return 1;
}


unsigned char CPU::STOP()
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

	if(MemoryController::Shared()->ReadMemory(PC + 1) != 0x00)
	{
		UNSUPPORTED();
		return 1;
	}

	stopMode = true;

	PC += 1;

	return 1;

}

unsigned char CPU::UNSUPPORTED()
{
	WriteLineE("CALLED UNSSUPPORTED OPCODE");
	return 0;
}

void CPU::LoadInstructions()
{
	instructions[0x00]	=	&CPU::NOP;
	instructions[0x01]	=	&CPU::LD_dd_nn;
	instructions[0x02]	=	&CPU::LD_$BC_A;
	instructions[0x03]	=	&CPU::INC_ss;
	instructions[0x04]	=	&CPU::INC_r;
	instructions[0x05]	=	&CPU::DEC_r;
	instructions[0x06]	=	&CPU::LD_R_n;
	instructions[0x07]	=	&CPU::RLCA;
	instructions[0x08]	=	&CPU::LD_$nn_SP;
	instructions[0x09]	=	&CPU::ADD_HL_ss;
	instructions[0x0A]	=	&CPU::LD_A_$BC;
	instructions[0x0B]	=	&CPU::DEC_ss;
	instructions[0x0C]	=	&CPU::INC_r;
	instructions[0x0D]	=	&CPU::DEC_r;
	instructions[0x0E]	=	&CPU::LD_R_n;
	instructions[0x0F]	=	&CPU::RRCA;

	instructions[0x10]	=	&CPU::STOP;
	instructions[0x11]	=	&CPU::LD_dd_nn;
	instructions[0x12]	=	&CPU::LD_$DE_A;
	instructions[0x13]	=	&CPU::INC_ss;
	instructions[0x14]	=	&CPU::INC_r;
	instructions[0x15]	=	&CPU::DEC_r;
	instructions[0x16]	=	&CPU::LD_R_n;
	instructions[0x17]	=	&CPU::RLA;
	instructions[0x18]	=	&CPU::JR_e;
	instructions[0x19]	=	&CPU::ADD_HL_ss;
	instructions[0x1A]	=	&CPU::LD_A_$DE;
	instructions[0x1B]	=	&CPU::DEC_ss;
	instructions[0x1C]	=	&CPU::INC_r;
	instructions[0x1D]	=	&CPU::DEC_r;
	instructions[0x1E]	=	&CPU::LD_R_n;
	instructions[0x1F]	=	&CPU::RRA;

	instructions[0x20]	=	&CPU::JR_cc_e;
	instructions[0x21]	=	&CPU::LD_dd_nn;
	instructions[0x22]	=	&CPU::LD_$HLI_A;
	instructions[0x23]	=	&CPU::INC_ss;
	instructions[0x24]	=	&CPU::INC_r;
	instructions[0x25]	=	&CPU::DEC_r;
	instructions[0x26]	=	&CPU::LD_R_n;
	instructions[0x27]	=	&CPU::DAA;
	instructions[0x28]	=	&CPU::JR_cc_e;
	instructions[0x29]	=	&CPU::ADD_HL_ss;
	instructions[0x2A]	=	&CPU::LD_A_$HLI;
	instructions[0x2B]	=	&CPU::DEC_ss;
	instructions[0x2C]	=	&CPU::INC_r;
	instructions[0x2D]	=	&CPU::DEC_r;
	instructions[0x2E]	=	&CPU::LD_R_n;
	instructions[0x2F]	=	&CPU::CPL;

	instructions[0x30]	=	&CPU::JR_cc_e;
	instructions[0x31]	=	&CPU::LD_dd_nn;
	instructions[0x32]	=	&CPU::LD_$HLD_A;
	instructions[0x33]	=	&CPU::INC_ss;
	instructions[0x34]	=	&CPU::INC_$HL;
	instructions[0x35]	=	&CPU::DEC_$HL;
	instructions[0x36]	=	&CPU::LD_$HL_n;
	instructions[0x37]	=	&CPU::SCF;
	instructions[0x38]	=	&CPU::JR_cc_e;
	instructions[0x39]	=	&CPU::ADD_HL_ss;
	instructions[0x3A]	=	&CPU::LD_A_$HLD;
	instructions[0x3B]	=	&CPU::DEC_ss;
	instructions[0x3C]	=	&CPU::INC_r;
	instructions[0x3D]	=	&CPU::DEC_r;
	instructions[0x3E]	=	&CPU::LD_R_n;
	instructions[0x3F]	=	&CPU::CCF;
	
	instructions[0x40]	=	&CPU::LD_R_R;
	instructions[0x41]	=	&CPU::LD_R_R;
	instructions[0x42]	=	&CPU::LD_R_R;
	instructions[0x43]	=	&CPU::LD_R_R;
	instructions[0x44]	=	&CPU::LD_R_R;
	instructions[0x45]	=	&CPU::LD_R_R;
	instructions[0x46]	=	&CPU::LD_R_$HL;
	instructions[0x47]	=	&CPU::LD_R_R;
	instructions[0x48]	=	&CPU::LD_R_R;
	instructions[0x49]	=	&CPU::LD_R_R;
	instructions[0x4A]	=	&CPU::LD_R_R;
	instructions[0x4B]	=	&CPU::LD_R_R;
	instructions[0x4C]	=	&CPU::LD_R_R;
	instructions[0x4D]	=	&CPU::LD_R_R;
	instructions[0x4E]	=	&CPU::LD_R_$HL;
	instructions[0x4F]	=	&CPU::LD_R_R;
	
	instructions[0x50]	=	&CPU::LD_R_R;
	instructions[0x51]	=	&CPU::LD_R_R;
	instructions[0x52]	=	&CPU::LD_R_R;
	instructions[0x53]	=	&CPU::LD_R_R;
	instructions[0x54]	=	&CPU::LD_R_R;
	instructions[0x55]	=	&CPU::LD_R_R;
	instructions[0x56]	=	&CPU::LD_R_$HL;
	instructions[0x57]	=	&CPU::LD_R_R;
	instructions[0x58]	=	&CPU::LD_R_R;
	instructions[0x59]	=	&CPU::LD_R_R;
	instructions[0x5A]	=	&CPU::LD_R_R;
	instructions[0x5B]	=	&CPU::LD_R_R;
	instructions[0x5C]	=	&CPU::LD_R_R;
	instructions[0x5D]	=	&CPU::LD_R_R;
	instructions[0x5E]	=	&CPU::LD_R_$HL;
	instructions[0x5F]	=	&CPU::LD_R_R;

	instructions[0x60]	=	&CPU::LD_R_R;
	instructions[0x61]	=	&CPU::LD_R_R;
	instructions[0x62]	=	&CPU::LD_R_R;
	instructions[0x63]	=	&CPU::LD_R_R;
	instructions[0x64]	=	&CPU::LD_R_R;
	instructions[0x65]	=	&CPU::LD_R_R;
	instructions[0x66]	=	&CPU::LD_R_$HL;
	instructions[0x67]	=	&CPU::LD_R_R;
	instructions[0x68]	=	&CPU::LD_R_R;
	instructions[0x69]	=	&CPU::LD_R_R;
	instructions[0x6A]	=	&CPU::LD_R_R;
	instructions[0x6B]	=	&CPU::LD_R_R;
	instructions[0x6C]	=	&CPU::LD_R_R;
	instructions[0x6D]	=	&CPU::LD_R_R;
	instructions[0x6E]	=	&CPU::LD_R_$HL;
	instructions[0x6F]	=	&CPU::LD_R_R;

	instructions[0x70]	=	&CPU::LD_$HL_R;
	instructions[0x71]	=	&CPU::LD_$HL_R;
	instructions[0x72]	=	&CPU::LD_$HL_R;
	instructions[0x73]	=	&CPU::LD_$HL_R;
	instructions[0x74]	=	&CPU::LD_$HL_R;
	instructions[0x75]	=	&CPU::LD_$HL_R;
	instructions[0x76]	=	&CPU::HALT;
	instructions[0x77]	=	&CPU::LD_$HL_R;
	instructions[0x78]	=	&CPU::LD_R_R;
	instructions[0x79]	=	&CPU::LD_R_R;
	instructions[0x7A]	=	&CPU::LD_R_R;
	instructions[0x7B]	=	&CPU::LD_R_R;
	instructions[0x7C]	=	&CPU::LD_R_R;
	instructions[0x7D]	=	&CPU::LD_R_R;
	instructions[0x7E]	=	&CPU::LD_R_$HL;
	instructions[0x7F]	=	&CPU::LD_R_R;

	instructions[0x80]	=	&CPU::ADD_A_r;
	instructions[0x81]	=	&CPU::ADD_A_r;
	instructions[0x82]	=	&CPU::ADD_A_r;
	instructions[0x83]	=	&CPU::ADD_A_r;
	instructions[0x84]	=	&CPU::ADD_A_r;
	instructions[0x85]	=	&CPU::ADD_A_r;
	instructions[0x86]	=	&CPU::ADD_A_$HL;
	instructions[0x87]	=	&CPU::ADD_A_r;
	instructions[0x88]	=	&CPU::ADC_A_r;
	instructions[0x89]	=	&CPU::ADC_A_r;
	instructions[0x8A]	=	&CPU::ADC_A_r;
	instructions[0x8B]	=	&CPU::ADC_A_r;
	instructions[0x8C]	=	&CPU::ADC_A_r;
	instructions[0x8D]	=	&CPU::ADC_A_r;
	instructions[0x8E]	=	&CPU::ADC_A_$HL;
	instructions[0x8F]	=	&CPU::ADC_A_r;

	instructions[0x90]	=	&CPU::SUB_A_r;
	instructions[0x91]	=	&CPU::SUB_A_r;
	instructions[0x92]	=	&CPU::SUB_A_r;
	instructions[0x93]	=	&CPU::SUB_A_r;
	instructions[0x94]	=	&CPU::SUB_A_r;
	instructions[0x95]	=	&CPU::SUB_A_r;
	instructions[0x96]	=	&CPU::SUB_A_$HL;
	instructions[0x97]	=	&CPU::SUB_A_r;
	instructions[0x98]	=	&CPU::SBC_A_r;
	instructions[0x99]	=	&CPU::SBC_A_r;
	instructions[0x9A]	=	&CPU::SBC_A_r;
	instructions[0x9B]	=	&CPU::SBC_A_r;
	instructions[0x9C]	=	&CPU::SBC_A_r;
	instructions[0x9D]	=	&CPU::SBC_A_r;
	instructions[0x9E]	=	&CPU::SBC_A_$HL;
	instructions[0x9F]	=	&CPU::SBC_A_r;

	instructions[0xA0]	=	&CPU::AND_A_r;
	instructions[0xA1]	=	&CPU::AND_A_r;
	instructions[0xA2]	=	&CPU::AND_A_r;
	instructions[0xA3]	=	&CPU::AND_A_r;
	instructions[0xA4]	=	&CPU::AND_A_r;
	instructions[0xA5]	=	&CPU::AND_A_r;
	instructions[0xA6]	=	&CPU::AND_A_$HL;
	instructions[0xA7]	=	&CPU::AND_A_r;
	instructions[0xA8]	=	&CPU::XOR_A_r;
	instructions[0xA9]	=	&CPU::XOR_A_r;
	instructions[0xAA]	=	&CPU::XOR_A_r;
	instructions[0xAB]	=	&CPU::XOR_A_r;
	instructions[0xAC]	=	&CPU::XOR_A_r;
	instructions[0xAD]	=	&CPU::XOR_A_r;
	instructions[0xAE]	=	&CPU::XOR_A_$HL;
	instructions[0xAF]	=	&CPU::XOR_A_r;

	instructions[0xB0]	=	&CPU::OR_A_r;
	instructions[0xB1]	=	&CPU::OR_A_r;
	instructions[0xB2]	=	&CPU::OR_A_r;
	instructions[0xB3]	=	&CPU::OR_A_r;
	instructions[0xB4]	=	&CPU::OR_A_r;
	instructions[0xB5]	=	&CPU::OR_A_r;
	instructions[0xB6]	=	&CPU::OR_A_$HL;
	instructions[0xB7]	=	&CPU::OR_A_r;
	instructions[0xB8]	=	&CPU::CP_A_r;
	instructions[0xB9]	=	&CPU::CP_A_r;
	instructions[0xBA]	=	&CPU::CP_A_r;
	instructions[0xBB]	=	&CPU::CP_A_r;
	instructions[0xBC]	=	&CPU::CP_A_r;
	instructions[0xBD]	=	&CPU::CP_A_r;
	instructions[0xBE]	=	&CPU::CP_A_$HL;
	instructions[0xBF]	=	&CPU::CP_A_r;

	instructions[0xC0]	=	&CPU::RET_cc;
	instructions[0xC1]	=	&CPU::POP_qq;
	instructions[0xC2]	=	&CPU::JP_cc_nn;
	instructions[0xC3]	=	&CPU::JP_nn;
	instructions[0xC4]	=	&CPU::CALL_cc_nn;
	instructions[0xC5]	=	&CPU::PUSH_qq;
	instructions[0xC6]	=	&CPU::ADD_A_n;
	instructions[0xC7]	=	&CPU::RST_t;
	instructions[0xC8]	=	&CPU::RET_cc;
	instructions[0xC9]	=	&CPU::RET;
	instructions[0xCA]	=	&CPU::JP_cc_nn;
	instructions[0xCB]	=	&CPU::PREFIX_0xBC;
	instructions[0xCC]	=	&CPU::CALL_cc_nn;
	instructions[0xCD]	=	&CPU::CALL_nn;
	instructions[0xCE]	=	&CPU::ADC_A_n;
	instructions[0xCF]	=	&CPU::RST_t;
	
	instructions[0xD0]	=	&CPU::RET_cc;
	instructions[0xD1]	=	&CPU::POP_qq;
	instructions[0xD2]	=	&CPU::JP_cc_nn;
	instructions[0xD3]	=	&CPU::UNSUPPORTED;
	instructions[0xD4]	=	&CPU::CALL_cc_nn;
	instructions[0xD5]	=	&CPU::PUSH_qq;
	instructions[0xD6]	=	&CPU::SUB_A_n;
	instructions[0xD7]	=	&CPU::RST_t;
	instructions[0xD8]	=	&CPU::RET_cc;
	instructions[0xD9]	=	&CPU::RETI;
	instructions[0xDA]	=	&CPU::JP_cc_nn;
	instructions[0xDB]	=	&CPU::UNSUPPORTED;
	instructions[0xDC]	=	&CPU::CALL_cc_nn;
	instructions[0xDD]	=	&CPU::UNSUPPORTED;
	instructions[0xDE]	=	&CPU::SBC_A_n;
	instructions[0xDF]	=	&CPU::RST_t;

	instructions[0xE0]	=	&CPU::LD_$n_A;
	instructions[0xE1]	=	&CPU::POP_qq;
	instructions[0xE2]	=	&CPU::LD_$C_A;
	instructions[0xE3]	=	&CPU::UNSUPPORTED;
	instructions[0xE4]	=	&CPU::UNSUPPORTED;
	instructions[0xE5]	=	&CPU::PUSH_qq;
	instructions[0xE6]	=	&CPU::AND_A_n;
	instructions[0xE7]	=	&CPU::RST_t;
	instructions[0xE8]	=	&CPU::ADD_SP_e;
	instructions[0xE9]	=	&CPU::JP_HL;
	instructions[0xEA]	=	&CPU::LD_$nn_A;
	instructions[0xEB]	=	&CPU::UNSUPPORTED;
	instructions[0xEC]	=	&CPU::UNSUPPORTED;
	instructions[0xED]	=	&CPU::UNSUPPORTED;
	instructions[0xEE]	=	&CPU::XOR_A_n;
	instructions[0xEF]	=	&CPU::RST_t;
		
	instructions[0xF0]	=	&CPU::LD_A_$n;
	instructions[0xF1]	=	&CPU::POP_qq;
	instructions[0xF2]	=	&CPU::LD_A_$C;
	instructions[0xF3]	=	&CPU::DI;
	instructions[0xF4]	=	&CPU::UNSUPPORTED;
	instructions[0xF5]	=	&CPU::PUSH_qq;
	instructions[0xF6]	=	&CPU::OR_A_n;
	instructions[0xF7]	=	&CPU::RST_t;
	instructions[0xF8]	=	&CPU::LDHL_PS_e;
	instructions[0xF9]	=	&CPU::LD_SP_HL;
	instructions[0xFA]	=	&CPU::LD_A_$nn;
	instructions[0xFB]	=	&CPU::EI;
	instructions[0xFC]	=	&CPU::UNSUPPORTED;
	instructions[0xFD]	=	&CPU::UNSUPPORTED;
	instructions[0xFE]	=	&CPU::CP_A_n;
	instructions[0xFF]	=	&CPU::RST_t;

	//---------------------------------------
	//---------------------------------------
	cbInstructions[0x00]	=	&CPU::RLC_r;
	cbInstructions[0x01]	=	&CPU::RLC_r;
	cbInstructions[0x02]	=	&CPU::RLC_r;
	cbInstructions[0x03]	=	&CPU::RLC_r;
	cbInstructions[0x04]	=	&CPU::RLC_r;
	cbInstructions[0x05]	=	&CPU::RLC_r;
	cbInstructions[0x06]	=	&CPU::RLC_$HL;
	cbInstructions[0x07]	=	&CPU::RLC_r;
	cbInstructions[0x08]	=	&CPU::RRC_r;
	cbInstructions[0x09]	=	&CPU::RRC_r;
	cbInstructions[0x0A]	=	&CPU::RRC_r;
	cbInstructions[0x0B]	=	&CPU::RRC_r;
	cbInstructions[0x0C]	=	&CPU::RRC_r;
	cbInstructions[0x0D]	=	&CPU::RRC_r;
	cbInstructions[0x0E]	=	&CPU::RRC_$HL;
	cbInstructions[0x0F]	=	&CPU::RRC_r;

	cbInstructions[0x10]	=	&CPU::RL_r;
	cbInstructions[0x11]	=	&CPU::RL_r;
	cbInstructions[0x12]	=	&CPU::RL_r;
	cbInstructions[0x13]	=	&CPU::RL_r;
	cbInstructions[0x14]	=	&CPU::RL_r;
	cbInstructions[0x15]	=	&CPU::RL_r;
	cbInstructions[0x16]	=	&CPU::RL_$HL;
	cbInstructions[0x17]	=	&CPU::RL_r;
	cbInstructions[0x18]	=	&CPU::RR_r;
	cbInstructions[0x19]	=	&CPU::RR_r;
	cbInstructions[0x1A]	=	&CPU::RR_r;
	cbInstructions[0x1B]	=	&CPU::RR_r;
	cbInstructions[0x1C]	=	&CPU::RR_r;
	cbInstructions[0x1D]	=	&CPU::RR_r;
	cbInstructions[0x1E]	=	&CPU::RR_$HL;
	cbInstructions[0x1F]	=	&CPU::RR_r;

	cbInstructions[0x20]	=	&CPU::SLA_r;
	cbInstructions[0x21]	=	&CPU::SLA_r;
	cbInstructions[0x22]	=	&CPU::SLA_r;
	cbInstructions[0x23]	=	&CPU::SLA_r;
	cbInstructions[0x24]	=	&CPU::SLA_r;
	cbInstructions[0x25]	=	&CPU::SLA_r;
	cbInstructions[0x26]	=	&CPU::SLA_$HL;
	cbInstructions[0x27]	=	&CPU::SLA_r;
	cbInstructions[0x28]	=	&CPU::SRA_r;
	cbInstructions[0x29]	=	&CPU::SRA_r;
	cbInstructions[0x2A]	=	&CPU::SRA_r;
	cbInstructions[0x2B]	=	&CPU::SRA_r;
	cbInstructions[0x2C]	=	&CPU::SRA_r;
	cbInstructions[0x2D]	=	&CPU::SRA_r;
	cbInstructions[0x2E]	=	&CPU::SRA_$HL;
	cbInstructions[0x2F]	=	&CPU::SRA_r;

	cbInstructions[0x30]	=	&CPU::SWAP_r;
	cbInstructions[0x31]	=	&CPU::SWAP_r;
	cbInstructions[0x32]	=	&CPU::SWAP_r;
	cbInstructions[0x33]	=	&CPU::SWAP_r;
	cbInstructions[0x34]	=	&CPU::SWAP_r;
	cbInstructions[0x35]	=	&CPU::SWAP_r;
	cbInstructions[0x36]	=	&CPU::SWAP_$HL;
	cbInstructions[0x37]	=	&CPU::SWAP_r;
	cbInstructions[0x38]	=	&CPU::SRL_r;
	cbInstructions[0x39]	=	&CPU::SRL_r;
	cbInstructions[0x3A]	=	&CPU::SRL_r;
	cbInstructions[0x3B]	=	&CPU::SRL_r;
	cbInstructions[0x3C]	=	&CPU::SRL_r;
	cbInstructions[0x3D]	=	&CPU::SRL_r;
	cbInstructions[0x3E]	=	&CPU::SRL_$HL;
	cbInstructions[0x3F]	=	&CPU::SRL_r;

	cbInstructions[0x40]	=	&CPU::BIT_b_r;
	cbInstructions[0x41]	=	&CPU::BIT_b_r;
	cbInstructions[0x42]	=	&CPU::BIT_b_r;
	cbInstructions[0x43]	=	&CPU::BIT_b_r;
	cbInstructions[0x44]	=	&CPU::BIT_b_r;
	cbInstructions[0x45]	=	&CPU::BIT_b_r;
	cbInstructions[0x46]	=	&CPU::BIT_b_$HL;
	cbInstructions[0x47]	=	&CPU::BIT_b_r;
	cbInstructions[0x48]	=	&CPU::BIT_b_r;
	cbInstructions[0x49]	=	&CPU::BIT_b_r;
	cbInstructions[0x4A]	=	&CPU::BIT_b_r;
	cbInstructions[0x4B]	=	&CPU::BIT_b_r;
	cbInstructions[0x4C]	=	&CPU::BIT_b_r;
	cbInstructions[0x4D]	=	&CPU::BIT_b_r;
	cbInstructions[0x4E]	=	&CPU::BIT_b_$HL;
	cbInstructions[0x4F]	=	&CPU::BIT_b_r;

	cbInstructions[0x50]	=	&CPU::BIT_b_r;
	cbInstructions[0x51]	=	&CPU::BIT_b_r;
	cbInstructions[0x52]	=	&CPU::BIT_b_r;
	cbInstructions[0x53]	=	&CPU::BIT_b_r;
	cbInstructions[0x54]	=	&CPU::BIT_b_r;
	cbInstructions[0x55]	=	&CPU::BIT_b_r;
	cbInstructions[0x56]	=	&CPU::BIT_b_$HL;
	cbInstructions[0x57]	=	&CPU::BIT_b_r;
	cbInstructions[0x58]	=	&CPU::BIT_b_r;
	cbInstructions[0x59]	=	&CPU::BIT_b_r;
	cbInstructions[0x5A]	=	&CPU::BIT_b_r;
	cbInstructions[0x5B]	=	&CPU::BIT_b_r;
	cbInstructions[0x5C]	=	&CPU::BIT_b_r;
	cbInstructions[0x5D]	=	&CPU::BIT_b_r;
	cbInstructions[0x5E]	=	&CPU::BIT_b_$HL;
	cbInstructions[0x5F]	=	&CPU::BIT_b_r;

	cbInstructions[0x60]	=	&CPU::BIT_b_r;
	cbInstructions[0x61]	=	&CPU::BIT_b_r;
	cbInstructions[0x62]	=	&CPU::BIT_b_r;
	cbInstructions[0x63]	=	&CPU::BIT_b_r;
	cbInstructions[0x64]	=	&CPU::BIT_b_r;
	cbInstructions[0x65]	=	&CPU::BIT_b_r;
	cbInstructions[0x66]	=	&CPU::BIT_b_$HL;
	cbInstructions[0x67]	=	&CPU::BIT_b_r;
	cbInstructions[0x68]	=	&CPU::BIT_b_r;
	cbInstructions[0x69]	=	&CPU::BIT_b_r;
	cbInstructions[0x6A]	=	&CPU::BIT_b_r;
	cbInstructions[0x6B]	=	&CPU::BIT_b_r;
	cbInstructions[0x6C]	=	&CPU::BIT_b_r;
	cbInstructions[0x6D]	=	&CPU::BIT_b_r;
	cbInstructions[0x6E]	=	&CPU::BIT_b_$HL;
	cbInstructions[0x6F]	=	&CPU::BIT_b_r;

	cbInstructions[0x70]	=	&CPU::BIT_b_r;
	cbInstructions[0x71]	=	&CPU::BIT_b_r;
	cbInstructions[0x72]	=	&CPU::BIT_b_r;
	cbInstructions[0x73]	=	&CPU::BIT_b_r;
	cbInstructions[0x74]	=	&CPU::BIT_b_r;
	cbInstructions[0x75]	=	&CPU::BIT_b_r;
	cbInstructions[0x76]	=	&CPU::BIT_b_$HL;
	cbInstructions[0x77]	=	&CPU::BIT_b_r;
	cbInstructions[0x78]	=	&CPU::BIT_b_r;
	cbInstructions[0x79]	=	&CPU::BIT_b_r;
	cbInstructions[0x7A]	=	&CPU::BIT_b_r;
	cbInstructions[0x7B]	=	&CPU::BIT_b_r;
	cbInstructions[0x7C]	=	&CPU::BIT_b_r;
	cbInstructions[0x7D]	=	&CPU::BIT_b_r;
	cbInstructions[0x7E]	=	&CPU::BIT_b_$HL;
	cbInstructions[0x7F]	=	&CPU::BIT_b_r;

	cbInstructions[0x80]	=	&CPU::RES_b_r;
	cbInstructions[0x81]	=	&CPU::RES_b_r;
	cbInstructions[0x82]	=	&CPU::RES_b_r;
	cbInstructions[0x83]	=	&CPU::RES_b_r;
	cbInstructions[0x84]	=	&CPU::RES_b_r;
	cbInstructions[0x85]	=	&CPU::RES_b_r;
	cbInstructions[0x86]	=	&CPU::RES_b_$HL;
	cbInstructions[0x87]	=	&CPU::RES_b_r;
	cbInstructions[0x88]	=	&CPU::RES_b_r;
	cbInstructions[0x89]	=	&CPU::RES_b_r;
	cbInstructions[0x8A]	=	&CPU::RES_b_r;
	cbInstructions[0x8B]	=	&CPU::RES_b_r;
	cbInstructions[0x8C]	=	&CPU::RES_b_r;
	cbInstructions[0x8D]	=	&CPU::RES_b_r;
	cbInstructions[0x8E]	=	&CPU::RES_b_$HL;
	cbInstructions[0x8F]	=	&CPU::RES_b_r;

	cbInstructions[0x90]	=	&CPU::RES_b_r;
	cbInstructions[0x91]	=	&CPU::RES_b_r;
	cbInstructions[0x92]	=	&CPU::RES_b_r;
	cbInstructions[0x93]	=	&CPU::RES_b_r;
	cbInstructions[0x94]	=	&CPU::RES_b_r;
	cbInstructions[0x95]	=	&CPU::RES_b_r;
	cbInstructions[0x96]	=	&CPU::RES_b_$HL;
	cbInstructions[0x97]	=	&CPU::RES_b_r;
	cbInstructions[0x98]	=	&CPU::RES_b_r;
	cbInstructions[0x99]	=	&CPU::RES_b_r;
	cbInstructions[0x9A]	=	&CPU::RES_b_r;
	cbInstructions[0x9B]	=	&CPU::RES_b_r;
	cbInstructions[0x9C]	=	&CPU::RES_b_r;
	cbInstructions[0x9D]	=	&CPU::RES_b_r;
	cbInstructions[0x9E]	=	&CPU::RES_b_$HL;
	cbInstructions[0x9F]	=	&CPU::RES_b_r;

	cbInstructions[0xA0]	=	&CPU::RES_b_r;
	cbInstructions[0xA1]	=	&CPU::RES_b_r;
	cbInstructions[0xA2]	=	&CPU::RES_b_r;
	cbInstructions[0xA3]	=	&CPU::RES_b_r;
	cbInstructions[0xA4]	=	&CPU::RES_b_r;
	cbInstructions[0xA5]	=	&CPU::RES_b_r;
	cbInstructions[0xA6]	=	&CPU::RES_b_$HL;
	cbInstructions[0xA7]	=	&CPU::RES_b_r;
	cbInstructions[0xA8]	=	&CPU::RES_b_r;
	cbInstructions[0xA9]	=	&CPU::RES_b_r;
	cbInstructions[0xAA]	=	&CPU::RES_b_r;
	cbInstructions[0xAB]	=	&CPU::RES_b_r;
	cbInstructions[0xAC]	=	&CPU::RES_b_r;
	cbInstructions[0xAD]	=	&CPU::RES_b_r;
	cbInstructions[0xAE]	=	&CPU::RES_b_$HL;
	cbInstructions[0xAF]	=	&CPU::RES_b_r;

	cbInstructions[0xB0]	=	&CPU::RES_b_r;
	cbInstructions[0xB1]	=	&CPU::RES_b_r;
	cbInstructions[0xB2]	=	&CPU::RES_b_r;
	cbInstructions[0xB3]	=	&CPU::RES_b_r;
	cbInstructions[0xB4]	=	&CPU::RES_b_r;
	cbInstructions[0xB5]	=	&CPU::RES_b_r;
	cbInstructions[0xB6]	=	&CPU::RES_b_$HL;
	cbInstructions[0xB7]	=	&CPU::RES_b_r;
	cbInstructions[0xB8]	=	&CPU::RES_b_r;
	cbInstructions[0xB9]	=	&CPU::RES_b_r;
	cbInstructions[0xBA]	=	&CPU::RES_b_r;
	cbInstructions[0xBB]	=	&CPU::RES_b_r;
	cbInstructions[0xBC]	=	&CPU::RES_b_r;
	cbInstructions[0xBD]	=	&CPU::RES_b_r;
	cbInstructions[0xBE]	=	&CPU::RES_b_$HL;
	cbInstructions[0xBF]	=	&CPU::RES_b_r;

	cbInstructions[0xC0]	=	&CPU::SET_b_r;
	cbInstructions[0xC1]	=	&CPU::SET_b_r;
	cbInstructions[0xC2]	=	&CPU::SET_b_r;
	cbInstructions[0xC3]	=	&CPU::SET_b_r;
	cbInstructions[0xC4]	=	&CPU::SET_b_r;
	cbInstructions[0xC5]	=	&CPU::SET_b_r;
	cbInstructions[0xC6]	=	&CPU::SET_b_$HL;
	cbInstructions[0xC7]	=	&CPU::SET_b_r;
	cbInstructions[0xC8]	=	&CPU::SET_b_r;
	cbInstructions[0xC9]	=	&CPU::SET_b_r;
	cbInstructions[0xCA]	=	&CPU::SET_b_r;
	cbInstructions[0xCB]	=	&CPU::SET_b_r;
	cbInstructions[0xCC]	=	&CPU::SET_b_r;
	cbInstructions[0xCD]	=	&CPU::SET_b_r;
	cbInstructions[0xCE]	=	&CPU::SET_b_$HL;
	cbInstructions[0xCF]	=	&CPU::SET_b_r;

	cbInstructions[0xD0]	=	&CPU::SET_b_r;
	cbInstructions[0xD1]	=	&CPU::SET_b_r;
	cbInstructions[0xD2]	=	&CPU::SET_b_r;
	cbInstructions[0xD3]	=	&CPU::SET_b_r;
	cbInstructions[0xD4]	=	&CPU::SET_b_r;
	cbInstructions[0xD5]	=	&CPU::SET_b_r;
	cbInstructions[0xD6]	=	&CPU::SET_b_$HL;
	cbInstructions[0xD7]	=	&CPU::SET_b_r;
	cbInstructions[0xD8]	=	&CPU::SET_b_r;
	cbInstructions[0xD9]	=	&CPU::SET_b_r;
	cbInstructions[0xDA]	=	&CPU::SET_b_r;
	cbInstructions[0xDB]	=	&CPU::SET_b_r;
	cbInstructions[0xDC]	=	&CPU::SET_b_r;
	cbInstructions[0xDD]	=	&CPU::SET_b_r;
	cbInstructions[0xDE]	=	&CPU::SET_b_$HL;
	cbInstructions[0xDF]	=	&CPU::SET_b_r;

	cbInstructions[0xE0]	=	&CPU::SET_b_r;
	cbInstructions[0xE1]	=	&CPU::SET_b_r;
	cbInstructions[0xE2]	=	&CPU::SET_b_r;
	cbInstructions[0xE3]	=	&CPU::SET_b_r;
	cbInstructions[0xE4]	=	&CPU::SET_b_r;
	cbInstructions[0xE5]	=	&CPU::SET_b_r;
	cbInstructions[0xE6]	=	&CPU::SET_b_$HL;
	cbInstructions[0xE7]	=	&CPU::SET_b_r;
	cbInstructions[0xE8]	=	&CPU::SET_b_r;
	cbInstructions[0xE9]	=	&CPU::SET_b_r;
	cbInstructions[0xEA]	=	&CPU::SET_b_r;
	cbInstructions[0xEB]	=	&CPU::SET_b_r;
	cbInstructions[0xEC]	=	&CPU::SET_b_r;
	cbInstructions[0xED]	=	&CPU::SET_b_r;
	cbInstructions[0xEE]	=	&CPU::SET_b_$HL;
	cbInstructions[0xEF]	=	&CPU::SET_b_r;

	cbInstructions[0xF0]	=	&CPU::SET_b_r;
	cbInstructions[0xF1]	=	&CPU::SET_b_r;
	cbInstructions[0xF2]	=	&CPU::SET_b_r;
	cbInstructions[0xF3]	=	&CPU::SET_b_r;
	cbInstructions[0xF4]	=	&CPU::SET_b_r;
	cbInstructions[0xF5]	=	&CPU::SET_b_r;
	cbInstructions[0xF6]	=	&CPU::SET_b_$HL;
	cbInstructions[0xF7]	=	&CPU::SET_b_r;
	cbInstructions[0xF8]	=	&CPU::SET_b_r;
	cbInstructions[0xF9]	=	&CPU::SET_b_r;
	cbInstructions[0xFA]	=	&CPU::SET_b_r;
	cbInstructions[0xFB]	=	&CPU::SET_b_r;
	cbInstructions[0xFC]	=	&CPU::SET_b_r;
	cbInstructions[0xFD]	=	&CPU::SET_b_r;
	cbInstructions[0xFE]	=	&CPU::SET_b_$HL;
	cbInstructions[0xFF]	=	&CPU::SET_b_r;
}