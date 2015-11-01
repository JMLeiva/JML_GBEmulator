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

#include "../../stdafx.h"
#include "../../Core/Memory/MemoryElementTest.h"
#include "Tools\Debugger\Globals.h"
#include "Tools\Disassembler\Mnemonics.h"
#include "Tools\Disassembler\DisassemblerInstructionTable.h"
#include "Metadata\Configuration.h"
#include <string>


#ifdef UNIT_TEST_ON

BOOST_AUTO_TEST_SUITE(DISASSEMBLER_INSTRUCTION_TABLE_Suite)

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_R_R)
{
	MemoryController::Shared()->Clear();

	DisassemblerInstructionTable diTable;
	DisassemblerInstruction di;

	MemoryElementSmall memorySmall;

	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	memorySmall.Write(0x00, 0x7F); //LD A A
	memorySmall.Write(0x01, 0x78); //LD A B
	memorySmall.Write(0x02, 0x79); //LD A C
	memorySmall.Write(0x03, 0x7A); //LD A D
	memorySmall.Write(0x04, 0x7B); //LD A E
	memorySmall.Write(0x05, 0x7C); //LD A H
	memorySmall.Write(0x06, 0x7D); //LD A L

	memorySmall.Write(0x07, 0x47); //LD B A
	memorySmall.Write(0x08, 0x40); //LD B B
	memorySmall.Write(0x09, 0x41); //LD B C
	memorySmall.Write(0x0A, 0x42); //LD B D
	memorySmall.Write(0x0B, 0x43); //LD B E
	memorySmall.Write(0x0C, 0x44); //LD B H
	memorySmall.Write(0x0D, 0x45); //LD B L

	memorySmall.Write(0x0E, 0x4F); //LD C A
	memorySmall.Write(0x0F, 0x48); //LD C B
	memorySmall.Write(0x10, 0x49); //LD C C
	memorySmall.Write(0x11, 0x4A); //LD C D
	memorySmall.Write(0x12, 0x4B); //LD C E
	memorySmall.Write(0x13, 0x4C); //LD C H
	memorySmall.Write(0x14, 0x4D); //LD C L

	memorySmall.Write(0x15, 0x57); //LD D A
	memorySmall.Write(0x16, 0x50); //LD D B
	memorySmall.Write(0x17, 0x51); //LD D C
	memorySmall.Write(0x18, 0x52); //LD D D
	memorySmall.Write(0x19, 0x53); //LD D E
	memorySmall.Write(0x1A, 0x54); //LD D H
	memorySmall.Write(0x1B, 0x55); //LD D L

	memorySmall.Write(0x1C, 0x5F); //LD E A
	memorySmall.Write(0x1D, 0x58); //LD E B
	memorySmall.Write(0x1E, 0x59); //LD E C
	memorySmall.Write(0x1F, 0x5A); //LD E D
	memorySmall.Write(0x20, 0x5B); //LD E E
	memorySmall.Write(0x21, 0x5C); //LD E H
	memorySmall.Write(0x22, 0x5D); //LD E L

	memorySmall.Write(0x23, 0x67); //LD H A
	memorySmall.Write(0x24, 0x60); //LD H B
	memorySmall.Write(0x25, 0x61); //LD H C
	memorySmall.Write(0x26, 0x62); //LD H D
	memorySmall.Write(0x27, 0x63); //LD H E
	memorySmall.Write(0x28, 0x64); //LD H H
	memorySmall.Write(0x29, 0x65); //LD H L

	memorySmall.Write(0x2A, 0x6F); //LD L A
	memorySmall.Write(0x2B, 0x68); //LD L B
	memorySmall.Write(0x2C, 0x69); //LD L C
	memorySmall.Write(0x2D, 0x6A); //LD L D
	memorySmall.Write(0x2E, 0x6B); //LD L E
	memorySmall.Write(0x2F, 0x6C); //LD L H
	memorySmall.Write(0x30, 0x6D); //LD L L

	char disass[32];
	WORD address;
	////////////////////////////////////////////////////////////////
	//LD A A
	address = 0x0000;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x7F); 
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_A, REG_NAME_A);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD A B
	address = 0x0001;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x78);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_A, REG_NAME_B);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD A C
	address = 0x0002;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x79);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_A, REG_NAME_C);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD A D
	address = 0x0003;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x7A);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_A, REG_NAME_D);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD A E
	address = 0x0004;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x7B);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_A, REG_NAME_E);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD A H
	address = 0x0005;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == 0x0005);
	BOOST_CHECK(di.bytes[0] == 0x7C);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == 0x0006);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_A, REG_NAME_H);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD A L
	address = 0x0006;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x7D);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_A, REG_NAME_L);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	////////////////////////////////////////////////////////////////
	//LD B A
	address = 0x0007;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x47);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_B, REG_NAME_A);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD B B
	address = 0x0008;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x40);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_B, REG_NAME_B);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD B C
	address = 0x0009;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x41);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_B, REG_NAME_C);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD B D
	address = 0x000A;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x42);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_B, REG_NAME_D);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD B E
	address = 0x000B;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x43);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_B, REG_NAME_E);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD B H
	address = 0x000C;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x44);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_B, REG_NAME_H);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD B L
	address = 0x000D;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x45);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address+1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_B, REG_NAME_L);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	////////////////////////////////////////////////////////////////
	//LD C A
	address = 0x000E;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x4F);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_C, REG_NAME_A);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD C B
	address = 0x000F;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x48);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_C, REG_NAME_B);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD C C
	address = 0x0010;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x49);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_C, REG_NAME_C);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD C D
	address = 0x0011;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x4A);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_C, REG_NAME_D);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD C E
	address = 0x0012;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x4B);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_C, REG_NAME_E);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD C H
	address = 0x0013;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x4C);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_C, REG_NAME_H);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD C L
	address = 0x0014;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x4D);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_C, REG_NAME_L);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	////////////////////////////////////////////////////////////////
	//LD D A
	address = 0x0015;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x57);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_D, REG_NAME_A);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD D B
	address = 0x0016;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x50);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_D, REG_NAME_B);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD D C
	address = 0x0017;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x51);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_D, REG_NAME_C);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD D D
	address = 0x0018;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x52);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_D, REG_NAME_D);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD D E
	address = 0x0019;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x53);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_D, REG_NAME_E);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD D H
	address = 0x001A;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x54);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_D, REG_NAME_H);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD D L
	address = 0x001B;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x55);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_D, REG_NAME_L);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);
	
	////////////////////////////////////////////////////////////////
	//LD E A
	address = 0x001C;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x5F);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_E, REG_NAME_A);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD E B
	address = 0x001D;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x58);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_E, REG_NAME_B);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD E C
	address = 0x001E;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x59);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_E, REG_NAME_C);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD E D
	address = 0x001F;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x5A);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_E, REG_NAME_D);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD E E
	address = 0x0020;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x5B);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_E, REG_NAME_E);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD E H
	address = 0x0021;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x5C);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_E, REG_NAME_H);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD E L
	address = 0x0022;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x5D);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_E, REG_NAME_L);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	////////////////////////////////////////////////////////////////
	//LD H A
	address = 0x0023;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x67);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_H, REG_NAME_A);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD H B
	address = 0x0024;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x60);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_H, REG_NAME_B);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD H C
	address = 0x0025;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x61);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_H, REG_NAME_C);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD H D
	address = 0x0026;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x62);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_H, REG_NAME_D);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD H E
	address = 0x0027;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x63);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_H, REG_NAME_E);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD H H
	address = 0x0028;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x64);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_H, REG_NAME_H);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD H L
	address = 0x0029;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x65);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_H, REG_NAME_L);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	////////////////////////////////////////////////////////////////
	//LD L A
	address = 0x002A;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x6F);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_L, REG_NAME_A);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD L B
	address = 0x002B;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x68);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_L, REG_NAME_B);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD L C
	address = 0x002C;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x69);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_L, REG_NAME_C);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD L D
	address = 0x002D;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x6A);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_L, REG_NAME_D);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD L E
	address = 0x002E;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x6B);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_L, REG_NAME_E);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD L H
	address = 0x002F;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x6C);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_L, REG_NAME_H);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	//LD L L
	address = 0x0030;
	di = diTable.Disassemble(MemoryController::Shared(), address);
	BOOST_CHECK(di.address == address);
	BOOST_CHECK(di.bytes[0] == 0x6D);
	BOOST_CHECK(di.size == 1);
	BOOST_CHECK(di.referencesAddresses.size() == 1);
	BOOST_CHECK(di.referencesAddresses[0] == address + 1);
	sprintf_s(disass, "%s %s, %s", MN_LD, REG_NAME_L, REG_NAME_L);
	BOOST_CHECK(strcmp(di.disass, disass) == 0);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_SUITE_END()

#endif