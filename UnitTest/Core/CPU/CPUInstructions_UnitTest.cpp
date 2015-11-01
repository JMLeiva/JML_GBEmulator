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
#include "..\Memory\MemoryElementTest.h"
#include "Core\CPU\CPU.h"
#include "Metadata\Configuration.h"

#ifdef UNIT_TEST_ON


BOOST_AUTO_TEST_SUITE(CPU_INTRUCTIONS_Suite)


BOOST_AUTO_TEST_CASE(CPUInstruction_LD_R_R)
{
	MemoryElementSmall memorySmall;

	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;

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

	int cpuOwnershipId = cpu.GetOwnershipId();

	////////////////////////
	//A
	//AA
	cpu.AF.h = 0xAA;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xAA);

	//AB
	cpu.AF.h = 0x00;
	cpu.BC.h = 0xAB;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xAB);

	//AC
	cpu.AF.h = 0x87;
	cpu.BC.l = 0xAC;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xAC);

	//AD
	cpu.AF.h = 0x87;
	cpu.DE.h = 0xAD;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xAD);

	//AE
	cpu.AF.h = 0x87;
	cpu.DE.l = 0xAE;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xAE);

	//AH
	cpu.AF.h = 0x87;
	cpu.HL.h = 0xA1;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xA1);

	//AL
	cpu.AF.h = 0x87;
	cpu.HL.l = 0xA2;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xA2);

	////////////////////////
	//B
	//BA
	cpu.BC.h = 0x00;
	cpu.AF.h = 0xAB;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0xAB);

	//BB
	cpu.BC.h = 0xBB;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0xBB);

	//BC
	cpu.BC.h = 0x00;
	cpu.BC.l = 0xBC;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0xBC);

	//BD
	cpu.BC.h = 0x00;
	cpu.DE.h = 0xBD;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0xBD);

	//BE
	cpu.BC.h = 0x00;
	cpu.DE.l = 0xBE;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0xBE);

	//BH
	cpu.BC.h = 0x00;
	cpu.HL.h = 0xB1;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0xB1);

	//BL
	cpu.BC.h = 0x00;
	cpu.HL.l = 0xB2;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0xB2);

	////////////////////////
	//C
	//CA
	cpu.BC.l = 0x00;
	cpu.AF.h = 0xCA;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xCA);

	//CB
	cpu.BC.l = 0x00;
	cpu.BC.h = 0xCB;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xCB);

	//CC
	cpu.BC.l = 0xCC;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xCC);

	//CD
	cpu.BC.l = 0x00;
	cpu.DE.h = 0xCD;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xCD);

	//CE
	cpu.BC.l = 0x00;
	cpu.DE.l = 0xCE;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xCE);

	//CH
	cpu.BC.l = 0x00;
	cpu.HL.h = 0xC1;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xC1);

	//CL
	cpu.BC.l = 0x00;
	cpu.HL.l = 0xC2;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xC2);

	////////////////////////
	//D
	//DA
	cpu.DE.h = 0x00;
	cpu.AF.h = 0xDA;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0xDA);

	//DB
	cpu.DE.h = 0x00;
	cpu.BC.h = 0xDB;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0xDB);

	//DC
	cpu.DE.h = 0x00;
	cpu.BC.l = 0xBC;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0xBC);

	//DD
	cpu.DE.h = 0xDD;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0xDD);

	//DE
	cpu.DE.h = 0x00;;
	cpu.DE.l = 0xDE;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0xDE);

	//DH
	cpu.DE.h = 0x00;
	cpu.HL.h = 0xD1;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0xD1);

	//DL
	cpu.DE.h = 0x00;
	cpu.HL.l = 0xD2;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0xD2);

	////////////////////////
	//E
	//EA
	cpu.DE.l = 0x00;
	cpu.AF.h = 0xEA;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0xEA);

	//EB
	cpu.DE.l = 0x00;
	cpu.BC.h = 0xEB;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0xEB);

	//EC
	cpu.DE.l = 0x00;
	cpu.BC.l = 0xEC;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0xEC);

	//ED
	cpu.DE.l = 0x00;
	cpu.DE.h = 0xED;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0xED);

	//EE
	cpu.DE.l = 0xEE;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0xEE);

	//EH
	cpu.DE.l = 0x00;
	cpu.HL.h = 0xE1;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0xE1);

	//EL
	cpu.DE.l = 0x00;
	cpu.HL.l = 0xE2;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0xE2);

	////////////////////////
	//H
	//HA
	cpu.HL.h = 0x00;
	cpu.AF.h = 0x1A;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x1A);

	//DB
	cpu.HL.h = 0x00;
	cpu.BC.h = 0x1B;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x1B);

	//DC
	cpu.HL.h = 0x00;
	cpu.BC.l = 0x1C;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x1C);

	//DD
	cpu.HL.h = 0x00;
	cpu.DE.h = 0x1D;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x1D);

	//DE
	cpu.HL.h = 0x00;
	cpu.DE.l = 0x1E;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x1E);

	//DH
	cpu.HL.h = 0x11;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x11);

	//DL
	cpu.HL.h = 0x00;
	cpu.HL.l = 0x12;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x12);

	////////////////////////
	//L
	//LA
	cpu.HL.l = 0x00;
	cpu.AF.h = 0x2A;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x2A);

	//EB
	cpu.HL.l = 0x00;
	cpu.BC.h = 0x2B;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x2B);

	//EC
	cpu.HL.l = 0x00;
	cpu.BC.l = 0x2C;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x2C);

	//ED
	cpu.HL.l = 0x00;
	cpu.DE.h = 0x2D;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x2D);

	//LE
	cpu.HL.l = 0x00;
	cpu.DE.l = 0x2E;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x2E);

	//CH
	cpu.HL.l = 0x00;
	cpu.HL.h = 0x21;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x21);

	//CL
	cpu.HL.l = 0x22;
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x22);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 49);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_R_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;

	memorySmall.Write(0x00, 0x3E); //LD A n
	memorySmall.Write(0x01, 0xAA);

	memorySmall.Write(0x02, 0x06); //LD B n
	memorySmall.Write(0x03, 0xBB);

	memorySmall.Write(0x04, 0x0E); //LD C n
	memorySmall.Write(0x05, 0xCC);

	memorySmall.Write(0x06, 0x16); //LD D n
	memorySmall.Write(0x07, 0xDD);

	memorySmall.Write(0x08, 0x1E); //LD E n
	memorySmall.Write(0x09, 0xEE);

	memorySmall.Write(0x0A, 0x26); //LD H n
	memorySmall.Write(0x0B, 0x11);

	memorySmall.Write(0x0C, 0x2E); //LD L n
	memorySmall.Write(0x0D, 0x22);

	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xAA);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0xBB);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xCC);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0xDD);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0xEE);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x11);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x22);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_R_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x00, 0x7E); //LD A (HL)
	memorySmall.Write(0x01, 0x46); //LD B (HL)
	memorySmall.Write(0x02, 0x4E); //LD C (HL)
	memorySmall.Write(0x03, 0x56); //LD D (HL)
	memorySmall.Write(0x04, 0x5E); //LD E (HL)
	memorySmall.Write(0x05, 0x66); //LD H (HL)
	memorySmall.Write(0x06, 0x6E); //LD L (HL)

	cpu.HL.w = 0x0031;
	memorySmall.Write(cpu.HL.w, 0xAA);
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xAA);

	
	cpu.HL.w = 0x0032;
	memorySmall.Write(cpu.HL.w, 0xBB);
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0xBB);

	
	cpu.HL.w = 0x0033;
	memorySmall.Write(cpu.HL.w, 0xCC);
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xCC);

	
	cpu.HL.w = 0x0034;
	memorySmall.Write(cpu.HL.w, 0xDD);
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0xDD);

	
	cpu.HL.w = 0x0035;
	memorySmall.Write(cpu.HL.w, 0xEE);
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0xEE);

	
	cpu.HL.w = 0x0036;
	memorySmall.Write(cpu.HL.w, 0x11);
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x11);

	
	cpu.HL.w = 0x0037;
	memorySmall.Write(cpu.HL.w, 0x22);
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x22);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$HL_R)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x00, 0x77); //LD (HL) A
	memorySmall.Write(0x01, 0x70); //LD (HL) B
	memorySmall.Write(0x02, 0x71); //LD (HL) C
	memorySmall.Write(0x03, 0x72); //LD (HL) D
	memorySmall.Write(0x04, 0x73); //LD (HL) E
	memorySmall.Write(0x05, 0x74); //LD (HL) H
	memorySmall.Write(0x06, 0x75); //LD (HL) L

	BYTE val = 0xFF;

	cpu.HL.w = 0x0021;
	cpu.AF.h = 0xAA;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xAA);

	cpu.HL.w = 0x0022;
	cpu.BC.h = 0xBB;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xBB);

	cpu.HL.w = 0x0023;
	cpu.BC.l = 0xCC;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xCC);

	cpu.HL.w = 0x0024;
	cpu.DE.h = 0xDD;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xDD);

	cpu.HL.w = 0x0025;
	cpu.DE.l = 0xEE;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xEE);

	cpu.HL.w = 0x0026; //H = 0x00
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x00);

	cpu.HL.w = 0x0027; //L = 0x27
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x27);

	
	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$HL_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x00, 0x36); //LD (HL) n
	memorySmall.Write(0x01, 0xD2);

	cpu.HL.w = 0x0021;
	cpu.RunCycle(cpuOwnershipId);
	BYTE val = 0xFF;
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xD2);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 2);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_LD_A_$BC)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.BC.w = 0x00C8;

	memorySmall.Write(0x00, 0x0A); //LD A (BC)
	memorySmall.Write(cpu.BC.w, 0x0F);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x0F);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_A_$DE)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.DE.w = 0x0035;

	memorySmall.Write(0x00, 0x1A); //LD A (DE)
	memorySmall.Write(cpu.DE.w, 0xDE);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xDE);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_LD_A_$C)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.BC.l = 0xC4;

	memoryFull.Write(0x00, 0xF2); //LD A [0xFF00 +] (C)
	memoryFull.Write(0xFF00 + cpu.BC.l, 0x71);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x71);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$C_A)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.BC.l = 0xD1;
	cpu.AF.h = 0xEE;
	memoryFull.Write(0x00, 0xE2); //LD [0xFF00 +] (C) A
	
	cpu.RunCycle(cpuOwnershipId);
	
	BYTE val = 0x00;
	memoryFull.Read(0xFF00 + cpu.BC.l, val);
	
	BOOST_CHECK(val == 0xEE);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_A_$n)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	BYTE n = 0xC4;

	memoryFull.Write(0x00, 0xF0); //LD A [0xFF00 +] (n)
	memoryFull.Write(0x01, n);
	memoryFull.Write(0xFF00 + n, 0x71);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x71);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 2);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$n_A)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	BYTE n = 0xD1;
	cpu.AF.h = 0xE1;
	memoryFull.Write(0x00, 0xE0); //LD [0xFF00 +] (n) A
	memoryFull.Write(0x01, n);

	cpu.RunCycle(cpuOwnershipId);

	BYTE val = 0x00;
	memoryFull.Read(0xFF00 + n, val);

	BOOST_CHECK(val == 0xE1);

	

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 2);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_LD_A_$nn)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	BYTE n1 = 0x12;
	BYTE n2 = 0x23;

	WORD address = n2;
	address <<= 8;
	address |= n1;

	memoryFull.Write(0x00, 0xFA); //LD A (nn)
	memoryFull.Write(0x01, n1);
	memoryFull.Write(0x02, n2);
	memoryFull.Write(address, 0xEF);

	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xEF);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 3);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$nn_A)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	BYTE n1 = 0x4E;
	BYTE n2 = 0xFF;

	WORD address = n2;
	address <<= 8;
	address |= n1;

	cpu.AF.h = 0xAA;

	memoryFull.Write(0x00, 0xEA); //LD (nn) A
	memoryFull.Write(0x01, n1);
	memoryFull.Write(0x02, n2);

	cpu.RunCycle(cpuOwnershipId);

	BYTE val = 0x00;
	memoryFull.Read(address, val);

	BOOST_CHECK(val == 0xAA);



	//CHECK PC state
	BOOST_CHECK(cpu.PC == 3);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_A_$HLI)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.HL.w = 0x0055;

	memorySmall.Write(0x00, 0x2A); //LD A (HLI)
	memorySmall.Write(cpu.HL.w, 0x12);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x12);
	BOOST_CHECK(cpu.HL.w == 0x0056);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_A_$HLD)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.HL.w = 0x00A5;

	memorySmall.Write(0x00, 0x2A); //LD A (HLI)
	memorySmall.Write(cpu.HL.w, 0x9F);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x9F);
	BOOST_CHECK(cpu.HL.w == 0x00A6);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$BC_A)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	cpu.BC.w = 0xBC;
	cpu.AF.h = 0xAA;

	memoryFull.Write(0x00, 0x02); //LD (BC) A
	
	cpu.RunCycle(cpuOwnershipId);

	BYTE val = 0x00;
	memoryFull.Read(cpu.BC.w, val);

	BOOST_CHECK(val == 0xAA);



	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$DE_A)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	cpu.DE.w = 0xDE;
	cpu.AF.h = 0xAD;

	memoryFull.Write(0x00, 0x12); //LD (DE) A

	cpu.RunCycle(cpuOwnershipId);

	BYTE val = 0x00;
	memoryFull.Read(cpu.DE.w, val);

	BOOST_CHECK(val == 0xAD);



	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$HLI_A)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	cpu.HL.w = 0xFFFF;
	cpu.AF.h = 0x23;

	memoryFull.Write(0x00, 0x22); //LD (DE) A

	cpu.RunCycle(cpuOwnershipId);

	BYTE val = 0x00;
	memoryFull.Read(cpu.HL.w - 0x0001, val);

	BOOST_CHECK(val == 0x23);
	BOOST_CHECK(cpu.HL.w == 0x0000);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$HLD_A)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	cpu.HL.w = 0x2220;
	cpu.AF.h = 0x43;

	memoryFull.Write(0x00, 0x32); //LD (DE) A

	cpu.RunCycle(cpuOwnershipId);

	BYTE val = 0x00;
	memoryFull.Read(cpu.HL.w + 0x0001, val);

	BOOST_CHECK(val == 0x43);
	BOOST_CHECK(cpu.HL.w == 0x221F);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_dd_nn)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x00, 0x01); //LD BC nn
	memorySmall.Write(0x01, 0xC0);
	memorySmall.Write(0x02, 0x0B);

	memorySmall.Write(0x03, 0x11); //LD DE nn
	memorySmall.Write(0x04, 0xE0);
	memorySmall.Write(0x05, 0x0D);

	memorySmall.Write(0x06, 0x21); //LD HL nn
	memorySmall.Write(0x07, 0x20);
	memorySmall.Write(0x08, 0x01);

	memorySmall.Write(0x09, 0x31); //LD SP nn
	memorySmall.Write(0x0A, 0x40);
	memorySmall.Write(0x0B, 0x03);

	cpu.BC.w	= 0x0000;
	cpu.DE.w	= 0x0000;
	cpu.HL.w	= 0x0000;
	cpu.SP.w	= 0x0000;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.w == 0x0BC0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.w == 0x0DE0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x0120);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.SP.w == 0x0340);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 12);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_SP_HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x00, 0xF9);

	cpu.HL.w = 0x1234;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.SP.w == 0x1234);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_PUSH_qq)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.SP.w = 0x00FF;
	cpu.BC.w = 0xBCBC;
	cpu.DE.w = 0xDEDE;
	cpu.HL.w = 0x1212;
	cpu.AF.w = 0xAFAF;

	BYTE h = 0x00;
	BYTE l = 0x00;

	memorySmall.Write(0x00, 0xC5); //PUSH BC
	memorySmall.Write(0x01, 0xD5); //PUSH DE
	memorySmall.Write(0x02, 0xE5); //PUSH HL
	memorySmall.Write(0x03, 0xF5); //PUSH AF
	
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0x00FE, h);
	memorySmall.Read(0x00FD, l);
	BOOST_CHECK(l == cpu.BC.l);
	BOOST_CHECK(h == cpu.BC.h);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0x00FC, h);
	memorySmall.Read(0x00FB, l);
	BOOST_CHECK(l == cpu.DE.l);
	BOOST_CHECK(h == cpu.DE.h);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0x00FA, h);
	memorySmall.Read(0x00F9, l);
	BOOST_CHECK(l == cpu.HL.l);
	BOOST_CHECK(h == cpu.HL.h);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0x00F8, h);
	memorySmall.Read(0x00F7, l);
	BOOST_CHECK(l == cpu.AF.l);
	BOOST_CHECK(h == cpu.AF.h);

	//CHECK SP STATE
	BOOST_CHECK(cpu.SP.w == 0x00F7);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_POP_qq)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.SP.w = 0x00FF;
	
	memorySmall.Write(0x00, 0xC1); //POP BC
	memorySmall.Write(0x01, 0xD1); //POP DE
	memorySmall.Write(0x02, 0xE1); //POP HL
	memorySmall.Write(0x03, 0xF1); //POP AF

	cpu.SP.w -= 1;
	memorySmall.Write(cpu.SP.w, 0x0A);
	cpu.SP.w -= 1;
	memorySmall.Write(cpu.SP.w, 0xF0);
	
	
	cpu.SP.w -= 1;
	memorySmall.Write(cpu.SP.w, 0x01);
	cpu.SP.w -= 1;
	memorySmall.Write(cpu.SP.w, 0x20);

	cpu.SP.w -= 1;
	memorySmall.Write(cpu.SP.w, 0x0D);
	cpu.SP.w -= 1;
	memorySmall.Write(cpu.SP.w, 0xE0);

	cpu.SP.w -= 1;
	memorySmall.Write(cpu.SP.w, 0x0B);
	cpu.SP.w -= 1;
	memorySmall.Write(cpu.SP.w, 0xC0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.w == 0x0BC0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.w == 0x0DE0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x0120);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.w == 0x0AF0);

	//CHECK SP STATE
	BOOST_CHECK(cpu.SP.w == 0x00FF);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_LDHL_SP_e)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.SP.w = 0x0FFF;
	memorySmall.Write(0x00, 0xF8); //DLHL SP e
	memorySmall.Write(0x01, 0x54); // 0x54

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x1053);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.SP.w = 0x0008;
	memorySmall.Write(0x02, 0xF8); //DLHL SP e
	memorySmall.Write(0x03, 0x0F); // 0x0F

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x0017);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);
	
	cpu.SP.w = 0x0081;
	memorySmall.Write(0x04, 0xF8); //DLHL SP e
	memorySmall.Write(0x05, 0xF0); // - 0x10

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x071);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.SP.w = 0x0000;
	memorySmall.Write(0x06, 0xF8); //DLHL SP e
	memorySmall.Write(0x07, 0x9C); // - 0x64

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0xFF9C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 8);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_LD_$nn_SP)
{
	MemoryElementFull memoryFull;
	
	MemoryController::Shared()->AppendMemoryElement(&memoryFull);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();
	
	cpu.SP.w = 0x147A;
	memoryFull.Write(0x0000, 0x08); //LD nn SP
	memoryFull.Write(0x0001, 0x55);
	memoryFull.Write(0x0002, 0x44);

	cpu.RunCycle(cpuOwnershipId);
	BYTE vall = 0x00;
	BYTE valh = 0x00;
	memoryFull.Read(0x4455, vall);
	memoryFull.Read(0x4456, valh);

	BOOST_CHECK(cpu.SP.l == vall);
	BOOST_CHECK(cpu.SP.h == valh);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 3);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_ADD_A_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;
	cpu.BC.h = 0x08;
	cpu.BC.l = 0x0F;
	cpu.DE.h = 0x20;
	cpu.DE.l = 0x40;
	cpu.HL.h = 0x80;
	cpu.HL.l = 0xF0;
	
	memorySmall.Write(0x0000, 0x80); //ADD A B
	memorySmall.Write(0x0001, 0x81); //ADD A C
	memorySmall.Write(0x0002, 0x82); //ADD A D
	memorySmall.Write(0x0003, 0x83); //ADD A E
	memorySmall.Write(0x0004, 0x84); //ADD A H
	memorySmall.Write(0x0005, 0x85); //ADD A L
	memorySmall.Write(0x0006, 0x87); //ADD A A

	cpu.RunCycle(cpuOwnershipId); // 0x04 + 0x08 = 0x0C
	BOOST_CHECK(cpu.AF.h == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	
	cpu.RunCycle(cpuOwnershipId); // 0x0C + 0x0F = 0x1B
	BOOST_CHECK(cpu.AF.h == 0x1B);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x1B + 0x20 = 0x3B
	BOOST_CHECK(cpu.AF.h == 0x3B);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x3B + 0x40 = 0x7B
	BOOST_CHECK(cpu.AF.h == 0x7B);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x7B + 0x80 = 0xFB
	BOOST_CHECK(cpu.AF.h == 0xFB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xFB + 0xF0 = 0xEB (+ Carry)
	BOOST_CHECK(cpu.AF.h == 0xEB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0x80;

	cpu.RunCycle(cpuOwnershipId); // 0x80 + 0x80 = 0x00 (+ Carry)
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);
	
	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_ADD_A_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;
	
	memorySmall.Write(0x0000, 0xC6); //ADD A 0x10
	memorySmall.Write(0x0001, 0x10); 
	memorySmall.Write(0x0002, 0xC6); //ADD A 0x0F
	memorySmall.Write(0x0003, 0x0F);
	memorySmall.Write(0x0004, 0xC6); //ADD A 0x88
	memorySmall.Write(0x0005, 0x88);
	memorySmall.Write(0x0006, 0xC6); //ADD A 0x77
	memorySmall.Write(0x0007, 0x77);
	memorySmall.Write(0x0008, 0xC6); //ADD A 0xDE
	memorySmall.Write(0x0009, 0xDE);

	cpu.RunCycle(cpuOwnershipId); // 0x04 + 0x10 = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x14 + 0x0F = 0x23
	BOOST_CHECK(cpu.AF.h == 0x23);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x23 + 0x88 = 0xAB
	BOOST_CHECK(cpu.AF.h == 0xAB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xAB + 0x77 = 0x22
	BOOST_CHECK(cpu.AF.h == 0x22);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId); // 0x22 + 0xDE = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 10);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_ADD_A_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;
	cpu.HL.w = 0xFF;

	memorySmall.Write(0x0000, 0x86); //ADD A (HL)
	memorySmall.Write(0x0001, 0x86); //ADD A (HL)
	memorySmall.Write(0x0002, 0x86); //ADD A (HL)
	memorySmall.Write(0x0003, 0x86); //ADD A (HL)
	memorySmall.Write(0x0004, 0x86); //ADD A (HL)

	memorySmall.Write(0x00FF, 0x10);
	cpu.RunCycle(cpuOwnershipId); // 0x04 + 0x10 = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(0x00FF, 0x0F);
	cpu.RunCycle(cpuOwnershipId); // 0x14 + 0x0F = 0x23
	BOOST_CHECK(cpu.AF.h == 0x23);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(0x00FF, 0x88);
	cpu.RunCycle(cpuOwnershipId); // 0x23 + 0x88 = 0xAB
	BOOST_CHECK(cpu.AF.h == 0xAB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(0x00FF, 0x77);
	cpu.RunCycle(cpuOwnershipId); // 0xAB + 0x77 = 0x22
	BOOST_CHECK(cpu.AF.h == 0x22);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	memorySmall.Write(0x00FF, 0xDE);
	cpu.RunCycle(cpuOwnershipId); // 0x22 + 0xDE = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 5);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_ADC_A_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;
	cpu.BC.h = 0x08;
	cpu.BC.l = 0x0F;
	cpu.DE.h = 0x20;
	cpu.DE.l = 0x40;
	cpu.HL.h = 0x80;
	cpu.HL.l = 0xF0;

	memorySmall.Write(0x0000, 0x88); //ADD A B
	memorySmall.Write(0x0001, 0x89); //ADD A C
	memorySmall.Write(0x0002, 0x8A); //ADD A D
	memorySmall.Write(0x0003, 0x8B); //ADD A E
	memorySmall.Write(0x0004, 0x8C); //ADD A H
	memorySmall.Write(0x0005, 0x8D); //ADD A L
	memorySmall.Write(0x0006, 0x8D); //ADD A L

	cpu.RunCycle(cpuOwnershipId); // 0x04 + 0x08 = 0x0C
	BOOST_CHECK(cpu.AF.h == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.RunCycle(cpuOwnershipId); // 0x0C + 0x0F = 0x1B
	BOOST_CHECK(cpu.AF.h == 0x1B);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x1B + 0x20 = 0x3B
	BOOST_CHECK(cpu.AF.h == 0x3B);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x3B + 0x40 = 0x7B
	BOOST_CHECK(cpu.AF.h == 0x7B);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x7B + 0x80 = 0xFB
	BOOST_CHECK(cpu.AF.h == 0xFB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xFB + 0xF0 = 0xEB (+ Carry)
	BOOST_CHECK(cpu.AF.h == 0xEB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);


	cpu.HL.l = 0x14;
	cpu.RunCycle(cpuOwnershipId); // 0xEB + 0x14 + 1 = 0x00 (+ Carry)
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_ADC_A_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;

	memorySmall.Write(0x0000, 0xCE); //ADD A 0x10
	memorySmall.Write(0x0001, 0x10);
	memorySmall.Write(0x0002, 0xCE); //ADD A 0x0F
	memorySmall.Write(0x0003, 0x0F);
	memorySmall.Write(0x0004, 0xCE); //ADD A 0x88
	memorySmall.Write(0x0005, 0x88);
	memorySmall.Write(0x0006, 0xCE); //ADD A 0x77
	memorySmall.Write(0x0007, 0x77);
	memorySmall.Write(0x0008, 0xCE); //ADD A 0xDE
	memorySmall.Write(0x0009, 0xDE);

	cpu.RunCycle(cpuOwnershipId); // 0x04 + 0x10 = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x14 + 0x0F = 0x23
	BOOST_CHECK(cpu.AF.h == 0x23);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x23 + 0x88 = 0xAB
	BOOST_CHECK(cpu.AF.h == 0xAB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xAB + 0x77 = 0x22
	BOOST_CHECK(cpu.AF.h == 0x22);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId); // 0x22 + 0xDE = 0x00
	BOOST_CHECK(cpu.AF.h == 0x01);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 10);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_ADC_A_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;
	cpu.HL.w = 0xFF;

	memorySmall.Write(0x0000, 0x8E); //ADD A (HL)
	memorySmall.Write(0x0001, 0x8E); //ADD A (HL)
	memorySmall.Write(0x0002, 0x8E); //ADD A (HL)
	memorySmall.Write(0x0003, 0x8E); //ADD A (HL)
	memorySmall.Write(0x0004, 0x8E); //ADD A (HL)

	memorySmall.Write(0x00FF, 0x10);
	cpu.RunCycle(cpuOwnershipId); // 0x04 + 0x10 = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(0x00FF, 0x0F);
	cpu.RunCycle(cpuOwnershipId); // 0x14 + 0x0F = 0x23
	BOOST_CHECK(cpu.AF.h == 0x23);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(0x00FF, 0x88);
	cpu.RunCycle(cpuOwnershipId); // 0x23 + 0x88 = 0xAB
	BOOST_CHECK(cpu.AF.h == 0xAB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(0x00FF, 0x77);
	cpu.RunCycle(cpuOwnershipId); // 0xAB + 0x77 = 0x22
	BOOST_CHECK(cpu.AF.h == 0x22);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	memorySmall.Write(0x00FF, 0xDE);
	cpu.RunCycle(cpuOwnershipId); // 0x22 + 0xDE + 1 = 0x01
	BOOST_CHECK(cpu.AF.h == 0x01);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 5);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SUB_A_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;
	cpu.BC.h = 0x08;
	cpu.BC.l = 0x0F;
	cpu.DE.h = 0x20;
	cpu.DE.l = 0x40;
	cpu.HL.h = 0x80;
	cpu.HL.l = 0xF0;

	memorySmall.Write(0x0000, 0x90); //SUB A B
	memorySmall.Write(0x0001, 0x91); //SUB A C
	memorySmall.Write(0x0002, 0x92); //SUB A D
	memorySmall.Write(0x0003, 0x93); //SUB A E
	memorySmall.Write(0x0004, 0x94); //SUB A H
	memorySmall.Write(0x0005, 0x95); //SUB A L
	memorySmall.Write(0x0006, 0x97); //SUB A A

	cpu.RunCycle(cpuOwnershipId); // 0x04 - 0x08 = 0xFC
	BOOST_CHECK(cpu.AF.h == 0xFC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);


	cpu.RunCycle(cpuOwnershipId); // 0xFC - 0x0F = 0xED
	BOOST_CHECK(cpu.AF.h == 0xED);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xED - 0x20 = 0xDC
	BOOST_CHECK(cpu.AF.h == 0xCD);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xCD - 0x40 = 0x8D
	BOOST_CHECK(cpu.AF.h == 0x8D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x8D + 0x80 = 0x0D
	BOOST_CHECK(cpu.AF.h == 0x0D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x0D - 0xF0 = 0x1D
	BOOST_CHECK(cpu.AF.h == 0x1D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0x80;

	cpu.RunCycle(cpuOwnershipId); // 0x80 - 0x80 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SUB_A_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;

	memorySmall.Write(0x00, 0xD6); //SUB A 0x08
	memorySmall.Write(0x01, 0x08);
	memorySmall.Write(0x02, 0xD6); //SUB A 0x0F
	memorySmall.Write(0x03, 0x0F);
	memorySmall.Write(0x04, 0xD6); //SUB A 0x20
	memorySmall.Write(0x05, 0x20);
	memorySmall.Write(0x06, 0xD6); //SUB A 0x40
	memorySmall.Write(0x07, 0x40);
	memorySmall.Write(0x08, 0xD6); //SUB A 0x80
	memorySmall.Write(0x09, 0x80);
	memorySmall.Write(0x0A, 0xD6); //SUB A 0xF0
	memorySmall.Write(0x0B, 0xF0);


	cpu.RunCycle(cpuOwnershipId); // 0x04 - 0x08 = 0xFC
	BOOST_CHECK(cpu.AF.h == 0xFC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);


	cpu.RunCycle(cpuOwnershipId); // 0xFC - 0x0F = 0xED
	BOOST_CHECK(cpu.AF.h == 0xED);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xED - 0x20 = 0xDC
	BOOST_CHECK(cpu.AF.h == 0xCD);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xCD - 0x40 = 0x8D
	BOOST_CHECK(cpu.AF.h == 0x8D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x8D + 0x80 = 0x0D
	BOOST_CHECK(cpu.AF.h == 0x0D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x0D - 0xF0 = 0x1D
	BOOST_CHECK(cpu.AF.h == 0x1D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 12);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SUB_A_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;
	cpu.HL.w = 0xFF;

	memorySmall.Write(0x0000, 0x96); //SUB A (HL)
	memorySmall.Write(0x0001, 0x96); //SUB A (HL)
	memorySmall.Write(0x0002, 0x96); //SUB A (HL)
	memorySmall.Write(0x0003, 0x96); //SUB A (HL)
	memorySmall.Write(0x0004, 0x96); //SUB A (HL)
	memorySmall.Write(0x0005, 0x96); //SUB A (HL)
	memorySmall.Write(0x0006, 0x96); //SUB A (HL)

	memorySmall.Write(cpu.HL.w, 0x08);
	cpu.RunCycle(cpuOwnershipId); // 0x04 - 0x08 = 0xFC
	BOOST_CHECK(cpu.AF.h == 0xFC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	memorySmall.Write(cpu.HL.w, 0x0F);
	cpu.RunCycle(cpuOwnershipId); // 0xFC - 0x0F = 0xED
	BOOST_CHECK(cpu.AF.h == 0xED);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(cpu.HL.w, 0x20);
	cpu.RunCycle(cpuOwnershipId); // 0xED - 0x20 = 0xDC
	BOOST_CHECK(cpu.AF.h == 0xCD);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(cpu.HL.w, 0x40);
	cpu.RunCycle(cpuOwnershipId); // 0xCD - 0x40 = 0x8D
	BOOST_CHECK(cpu.AF.h == 0x8D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(cpu.HL.w, 0x80);
	cpu.RunCycle(cpuOwnershipId); // 0x8D + 0x80 = 0x0D
	BOOST_CHECK(cpu.AF.h == 0x0D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(cpu.HL.w, 0xF0);
	cpu.RunCycle(cpuOwnershipId); // 0x0D - 0xF0 = 0x1D
	BOOST_CHECK(cpu.AF.h == 0x1D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0x80;

	memorySmall.Write(cpu.HL.w, 0x80);
	cpu.RunCycle(cpuOwnershipId); // 0x80 - 0x80 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SBC_A_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;
	cpu.BC.h = 0x08;
	cpu.BC.l = 0x0F;
	cpu.DE.h = 0x20;
	cpu.DE.l = 0x40;
	cpu.HL.h = 0x80;
	cpu.HL.l = 0xF0;

	memorySmall.Write(0x0000, 0x98); //SBC A B
	memorySmall.Write(0x0001, 0x99); //SBC A C
	memorySmall.Write(0x0002, 0x9A); //SBC A D
	memorySmall.Write(0x0003, 0x9B); //SBC A E
	memorySmall.Write(0x0004, 0x9C); //SBC A H
	memorySmall.Write(0x0005, 0x9D); //SBC A L
	memorySmall.Write(0x0006, 0x9F); //SBC A A

	cpu.RunCycle(cpuOwnershipId); // 0x04 - 0x08 = 0xFC
	BOOST_CHECK(cpu.AF.h == 0xFC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);


	cpu.RunCycle(cpuOwnershipId); // 0xFC - (0x0F + 1) = 0xEC
	BOOST_CHECK(cpu.AF.h == 0xEC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xEC - 0x20 = 0xCC
	BOOST_CHECK(cpu.AF.h == 0xCC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xCC - 0x40 = 0x8C
	BOOST_CHECK(cpu.AF.h == 0x8C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x8C + 0x80 = 0x0C
	BOOST_CHECK(cpu.AF.h == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x0C - 0xF0 = 0x1C
	BOOST_CHECK(cpu.AF.h == 0x1C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0x7F;

	cpu.RunCycle(cpuOwnershipId); // 0x7F - (0x7F + 1) = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SBC_A_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;

	memorySmall.Write(0x00, 0xDE); //SBC A 0x08
	memorySmall.Write(0x01, 0x08);
	memorySmall.Write(0x02, 0xDE); //SBC A 0x0F
	memorySmall.Write(0x03, 0x0F);
	memorySmall.Write(0x04, 0xDE); //SBC A 0x20
	memorySmall.Write(0x05, 0x20);
	memorySmall.Write(0x06, 0xDE); //SBC A 0x40
	memorySmall.Write(0x07, 0x40);
	memorySmall.Write(0x08, 0xDE); //SBC A 0x80
	memorySmall.Write(0x09, 0x80);
	memorySmall.Write(0x0A, 0xDE); //SBC A 0xF0
	memorySmall.Write(0x0B, 0xF0);


	cpu.RunCycle(cpuOwnershipId); // 0x04 - 0x08 = 0xFC
	BOOST_CHECK(cpu.AF.h == 0xFC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);


	cpu.RunCycle(cpuOwnershipId); // 0xFC - (0x0F + 1) = 0xEC
	BOOST_CHECK(cpu.AF.h == 0xEC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xEC - 0x20 = 0xCC
	BOOST_CHECK(cpu.AF.h == 0xCC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xCC - 0x40 = 0x8C
	BOOST_CHECK(cpu.AF.h == 0x8C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x8C + 0x80 = 0x0C
	BOOST_CHECK(cpu.AF.h == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x0C - 0xF0 = 0x1C
	BOOST_CHECK(cpu.AF.h == 0x1C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 12);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SBC_A_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x04;
	cpu.HL.w = 0xFF;

	memorySmall.Write(0x0000, 0x9E); //SBC A (HL)
	memorySmall.Write(0x0001, 0x9E); //SBC A (HL)
	memorySmall.Write(0x0002, 0x9E); //SBC A (HL)
	memorySmall.Write(0x0003, 0x9E); //SBC A (HL)
	memorySmall.Write(0x0004, 0x9E); //SBC A (HL)
	memorySmall.Write(0x0005, 0x9E); //SBC A (HL)
	memorySmall.Write(0x0006, 0x9E); //SBC A (HL)

	memorySmall.Write(cpu.HL.w, 0x08);
	cpu.RunCycle(cpuOwnershipId); // 0x04 - 0x08 = 0xFC
	BOOST_CHECK(cpu.AF.h == 0xFC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	memorySmall.Write(cpu.HL.w, 0x0F);
	cpu.RunCycle(cpuOwnershipId); // 0xFC - (0x0F + 1) = 0xEC
	BOOST_CHECK(cpu.AF.h == 0xEC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(cpu.HL.w, 0x20);
	cpu.RunCycle(cpuOwnershipId); // 0xEC - 0x20 = 0xCC
	BOOST_CHECK(cpu.AF.h == 0xCC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(cpu.HL.w, 0x40);
	cpu.RunCycle(cpuOwnershipId); // 0xCC - 0x40 = 0x8C
	BOOST_CHECK(cpu.AF.h == 0x8C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(cpu.HL.w, 0x80);
	cpu.RunCycle(cpuOwnershipId); // 0x8C + 0x80 = 0x0C
	BOOST_CHECK(cpu.AF.h == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	memorySmall.Write(cpu.HL.w, 0xF0);
	cpu.RunCycle(cpuOwnershipId); // 0x0C - 0xF0 = 0x1C
	BOOST_CHECK(cpu.AF.h == 0x1C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0x80;

	memorySmall.Write(cpu.HL.w, 0x7F);
	cpu.RunCycle(cpuOwnershipId); // 0x80 - (0x7F + 1) = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_AND_A_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0xFF;
	cpu.BC.h = 0x9E;
	cpu.BC.l = 0x1E;
	cpu.DE.h = 0xF4;
	cpu.DE.l = 0xDD;
	cpu.HL.h = 0x0F;
	cpu.HL.l = 0x98;

	memorySmall.Write(0x0000, 0xA0); //AND A B
	memorySmall.Write(0x0001, 0xA1); //AND A C
	memorySmall.Write(0x0002, 0xA2); //AND A D
	memorySmall.Write(0x0003, 0xA3); //AND A E
	memorySmall.Write(0x0004, 0xA4); //AND A H
	memorySmall.Write(0x0005, 0xA5); //AND A L
	memorySmall.Write(0x0006, 0xA7); //AND A A

	cpu.RunCycle(cpuOwnershipId); // 0xF & 0x9E = 0x9E
	BOOST_CHECK(cpu.AF.h == 0x9E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.RunCycle(cpuOwnershipId); // 0x9E & 0x1E = 0x1E
	BOOST_CHECK(cpu.AF.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x1E & 0xF4 = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x14 & 0xDD = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x14 & 0x0F = 0x04
	BOOST_CHECK(cpu.AF.h == 0x04);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x04 & 0x98 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x00 & 0x00 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_AND_A_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0xFF;

	memorySmall.Write(0x0000, 0xE6); //AND A 0x9E
	memorySmall.Write(0x0001, 0x9E); 
	memorySmall.Write(0x0002, 0xE6); //AND A 0x1E
	memorySmall.Write(0x0003, 0x1E);
	memorySmall.Write(0x0004, 0xE6); //AND A 0xF4
	memorySmall.Write(0x0005, 0xF4);
	memorySmall.Write(0x0006, 0xE6); //AND A 0xDD
	memorySmall.Write(0x0007, 0xDD);
	memorySmall.Write(0x0008, 0xE6); //AND A 0x0F
	memorySmall.Write(0x0009, 0x0F);
	memorySmall.Write(0x000A, 0xE6); //AND A 0x98
	memorySmall.Write(0x000B, 0x98);

	cpu.RunCycle(cpuOwnershipId); // 0xF & 0x9E = 0x9E
	BOOST_CHECK(cpu.AF.h == 0x9E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.RunCycle(cpuOwnershipId); // 0x9E & 0x1E = 0x1E
	BOOST_CHECK(cpu.AF.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x1E & 0xF4 = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x14 & 0xDD = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x14 & 0x0F = 0x04
	BOOST_CHECK(cpu.AF.h == 0x04);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x04 & 0x98 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 12);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_AND_A_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0xFF;

	memorySmall.Write(0x0000, 0xA6); //AND A (HL)
	memorySmall.Write(0x0001, 0xA6); //AND A (HL)
	memorySmall.Write(0x0002, 0xA6); //AND A (HL)
	memorySmall.Write(0x0003, 0xA6); //AND A (HL)
	memorySmall.Write(0x0004, 0xA6); //AND A (HL)
	memorySmall.Write(0x0005, 0xA6); //AND A (HL)

	memorySmall.Write(0x0010, 0x9E);
	memorySmall.Write(0x0011, 0x1E);
	memorySmall.Write(0x0012, 0xF4);
	memorySmall.Write(0x0013, 0xDD);
	memorySmall.Write(0x0014, 0x0D);
	memorySmall.Write(0x0015, 0x98);

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId); // 0xF & 0x9E = 0x9E
	BOOST_CHECK(cpu.AF.h == 0x9E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId); // 0x9E & 0x1E = 0x1E
	BOOST_CHECK(cpu.AF.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId); // 0x1E & 0xF4 = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId); // 0x14 & 0xDD = 0x14
	BOOST_CHECK(cpu.AF.h == 0x14);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId); // 0x14 & 0x0F = 0x04
	BOOST_CHECK(cpu.AF.h == 0x04);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId); // 0x04 & 0x98 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 6);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_OR_A_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x00;
	cpu.BC.h = 0x00;
	cpu.BC.l = 0x1E;
	cpu.DE.h = 0xF4;
	cpu.DE.l = 0xDD;
	cpu.HL.h = 0x0F;
	cpu.HL.l = 0x98;

	memorySmall.Write(0x0000, 0xB0); //OR A B
	memorySmall.Write(0x0001, 0xB1); //OR A C
	memorySmall.Write(0x0002, 0xB2); //OR A D
	memorySmall.Write(0x0003, 0xB3); //OR A E
	memorySmall.Write(0x0004, 0xB4); //OR A H
	memorySmall.Write(0x0005, 0xB5); //OR A L
	memorySmall.Write(0x0006, 0xB7); //OR A A

	cpu.RunCycle(cpuOwnershipId); // 0x00 | 0x00 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.RunCycle(cpuOwnershipId); // 0x00 | 0x1E = 0x1E
	BOOST_CHECK(cpu.AF.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x1E | 0xF4 = 0xFE
	BOOST_CHECK(cpu.AF.h == 0xFE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x14 | 0xDD = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xFF | 0x0F = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xFF | 0x98 = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xFF | 0xFF = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_OR_A_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x00;


	memorySmall.Write(0x0000, 0xF6); //OR A 0x9E
	memorySmall.Write(0x0001, 0x00);
	memorySmall.Write(0x0002, 0xF6); //OR A 0x1E
	memorySmall.Write(0x0003, 0x1E);
	memorySmall.Write(0x0004, 0xF6); //OR A 0xF4
	memorySmall.Write(0x0005, 0xF4);
	memorySmall.Write(0x0006, 0xF6); //OR A 0xDD
	memorySmall.Write(0x0007, 0xDD);
	memorySmall.Write(0x0008, 0xF6); //OR A 0x0F
	memorySmall.Write(0x0009, 0x0F);
	memorySmall.Write(0x000A, 0xF6); //OR A 0x98
	memorySmall.Write(0x000B, 0x98);

	cpu.RunCycle(cpuOwnershipId); // 0x00 | 0x00 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.RunCycle(cpuOwnershipId); // 0x00 | 0x1E = 0x1E
	BOOST_CHECK(cpu.AF.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x1E | 0xF4 = 0xFE
	BOOST_CHECK(cpu.AF.h == 0xFE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x14 | 0xDD = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xFF | 0x0F = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xFF | 0x98 = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 12);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_OR_A_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x00;

	memorySmall.Write(0x0000, 0xB6); //OR A (HL)
	memorySmall.Write(0x0001, 0xB6); //OR A (HL)
	memorySmall.Write(0x0002, 0xB6); //OR A (HL)
	memorySmall.Write(0x0003, 0xB6); //OR A (HL)
	memorySmall.Write(0x0004, 0xB6); //OR A (HL)
	memorySmall.Write(0x0005, 0xB6); //OR A (HL)

	memorySmall.Write(0x0010, 0x00);
	memorySmall.Write(0x0011, 0x1E);
	memorySmall.Write(0x0012, 0xF4);
	memorySmall.Write(0x0013, 0xDD);
	memorySmall.Write(0x0014, 0x0D);
	memorySmall.Write(0x0015, 0x98);

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId); // 0x00 | 0x00 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId); // 0x00 | 0x1E = 0x1E
	BOOST_CHECK(cpu.AF.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId); // 0x1E | 0xF4 = 0xFE
	BOOST_CHECK(cpu.AF.h == 0xFE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId); // 0x14 | 0xDD = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId); // 0xFF | 0x0F = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId); // 0xFF | 0x98 = 0xFF
	BOOST_CHECK(cpu.AF.h == 0xFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 6);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_XOR_A_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x00;
	cpu.BC.h = 0x00;
	cpu.BC.l = 0x1E;
	cpu.DE.h = 0xF4;
	cpu.DE.l = 0xDD;
	cpu.HL.h = 0x0F;
	cpu.HL.l = 0x98;

	memorySmall.Write(0x0000, 0xA8); //XOR A B
	memorySmall.Write(0x0001, 0xA9); //XOR A C
	memorySmall.Write(0x0002, 0xAA); //XOR A D
	memorySmall.Write(0x0003, 0xAB); //XOR A E
	memorySmall.Write(0x0004, 0xAC); //XOR A H
	memorySmall.Write(0x0005, 0xAD); //XOR A L
	memorySmall.Write(0x0006, 0xAF); //XOR A A

	cpu.RunCycle(cpuOwnershipId); // 0x00 ^ 0x00 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.RunCycle(cpuOwnershipId); // 0x00 ^ 0x1E = 0x1E
	BOOST_CHECK(cpu.AF.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x1E ^ 0xF4 = 0xEA
	BOOST_CHECK(cpu.AF.h == 0xEA);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xEA ^ 0xDD = 0x37
	BOOST_CHECK(cpu.AF.h == 0x37);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x37 ^ 0x0F = 0x38
	BOOST_CHECK(cpu.AF.h == 0x38);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x38 ^ 0x98 = 0xA0
	BOOST_CHECK(cpu.AF.h == 0xA0);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xA0 ^ 0xA0 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_XOR_A_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x00;


	memorySmall.Write(0x0000, 0xEE); //XOR A 0x9E
	memorySmall.Write(0x0001, 0x00);
	memorySmall.Write(0x0002, 0xEE); //XOR A 0x1E
	memorySmall.Write(0x0003, 0x1E);
	memorySmall.Write(0x0004, 0xEE); //XOR A 0xF4
	memorySmall.Write(0x0005, 0xF4);
	memorySmall.Write(0x0006, 0xEE); //XOR A 0xDD
	memorySmall.Write(0x0007, 0xDD);
	memorySmall.Write(0x0008, 0xEE); //XOR A 0x0F
	memorySmall.Write(0x0009, 0x0F);
	memorySmall.Write(0x000A, 0xEE); //XOR A 0x98
	memorySmall.Write(0x000B, 0x98);

	cpu.RunCycle(cpuOwnershipId); // 0x00 ^ 0x00 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.RunCycle(cpuOwnershipId); // 0x00 ^ 0x1E = 0x1E
	BOOST_CHECK(cpu.AF.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x1E ^ 0xF4 = 0xEA
	BOOST_CHECK(cpu.AF.h == 0xEA);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0xEA ^ 0xDD = 0x37
	BOOST_CHECK(cpu.AF.h == 0x37);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x37 ^ 0x0F = 0x38
	BOOST_CHECK(cpu.AF.h == 0x38);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId); // 0x38 ^ 0x98 = 0xA0
	BOOST_CHECK(cpu.AF.h == 0xA0);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 12);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_XOR_A_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x00;

	memorySmall.Write(0x0000, 0xAE); //XOR A (HL)
	memorySmall.Write(0x0001, 0xAE); //XOR A (HL)
	memorySmall.Write(0x0002, 0xAE); //XOR A (HL)
	memorySmall.Write(0x0003, 0xAE); //XOR A (HL)
	memorySmall.Write(0x0004, 0xAE); //XOR A (HL)
	memorySmall.Write(0x0005, 0xAE); //XOR A (HL)

	memorySmall.Write(0x0010, 0x00);
	memorySmall.Write(0x0011, 0x1E);
	memorySmall.Write(0x0012, 0xF4);
	memorySmall.Write(0x0013, 0xDD);
	memorySmall.Write(0x0014, 0x0F);
	memorySmall.Write(0x0015, 0x98);

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId); // 0x00 ^ 0x00 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId); // 0x00 ^ 0x1E = 0x1E
	BOOST_CHECK(cpu.AF.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId); // 0x1E ^ 0xF4 = 0xEA
	BOOST_CHECK(cpu.AF.h == 0xEA);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId); // 0xEA ^ 0xDD = 0x37
	BOOST_CHECK(cpu.AF.h == 0x37);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId); // 0x37 ^ 0x0F = 0x38
	BOOST_CHECK(cpu.AF.h == 0x38);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId); // 0x38 ^ 0x98 = 0xA0
	BOOST_CHECK(cpu.AF.h == 0xA0);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 6);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_CP_A_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	
	cpu.BC.h = 0x08;
	cpu.BC.l = 0x0F;
	cpu.DE.h = 0x20;
	cpu.DE.l = 0x40;
	cpu.HL.h = 0x80;
	cpu.HL.l = 0xF0;

	memorySmall.Write(0x0000, 0xB8); //CP A B
	memorySmall.Write(0x0001, 0xB9); //CP A C
	memorySmall.Write(0x0002, 0xBA); //CP A D
	memorySmall.Write(0x0003, 0xBB); //CP A E
	memorySmall.Write(0x0004, 0xBC); //CP A H
	memorySmall.Write(0x0005, 0xBD); //CP A L
	memorySmall.Write(0x0006, 0xBF); //CP A A

	cpu.AF.h = 0x04;
	cpu.RunCycle(cpuOwnershipId); // 0x04 - 0x08 = 0xFC
	BOOST_CHECK(cpu.AF.h == 0x04);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0xFC;
	cpu.RunCycle(cpuOwnershipId); // 0xFC - 0x0F = 0xED
	BOOST_CHECK(cpu.AF.h == 0xFC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0xED;
	cpu.RunCycle(cpuOwnershipId); // 0xED - 0x20 = 0xDC
	BOOST_CHECK(cpu.AF.h == 0xED);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0xDC;
	cpu.RunCycle(cpuOwnershipId); // 0xCD - 0x40 = 0x8D
	BOOST_CHECK(cpu.AF.h == 0xDC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0x8D;
	cpu.RunCycle(cpuOwnershipId); // 0x8D + 0x80 = 0x0D
	BOOST_CHECK(cpu.AF.h == 0x8D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0x0D;
	cpu.RunCycle(cpuOwnershipId); // 0x0D - 0xF0 = 0x1D
	BOOST_CHECK(cpu.AF.h == 0x0D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0x80;
	cpu.RunCycle(cpuOwnershipId); // 0x80 - 0x80 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x80);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_CP_A_n)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x00, 0xFE); //CP A 0x08
	memorySmall.Write(0x01, 0x08);
	memorySmall.Write(0x02, 0xFE); //CP A 0x0F
	memorySmall.Write(0x03, 0x0F);
	memorySmall.Write(0x04, 0xFE); //CP A 0x20
	memorySmall.Write(0x05, 0x20);
	memorySmall.Write(0x06, 0xFE); //CP A 0x40
	memorySmall.Write(0x07, 0x40);
	memorySmall.Write(0x08, 0xFE); //CP A 0x80
	memorySmall.Write(0x09, 0x80);
	memorySmall.Write(0x0A, 0xFE); //CP A 0xF0
	memorySmall.Write(0x0B, 0xF0);

	cpu.AF.h = 0x04;
	cpu.RunCycle(cpuOwnershipId); // 0x04 - 0x08 = 0xFC
	BOOST_CHECK(cpu.AF.h == 0x04);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0xFC;
	cpu.RunCycle(cpuOwnershipId); // 0xFC - 0x0F = 0xED
	BOOST_CHECK(cpu.AF.h == 0xFC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0xED;
	cpu.RunCycle(cpuOwnershipId); // 0xED - 0x20 = 0xDC
	BOOST_CHECK(cpu.AF.h == 0xED);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0xDC;
	cpu.RunCycle(cpuOwnershipId); // 0xCD - 0x40 = 0x8D
	BOOST_CHECK(cpu.AF.h == 0xDC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0x8D;
	cpu.RunCycle(cpuOwnershipId); // 0x8D + 0x80 = 0x0D
	BOOST_CHECK(cpu.AF.h == 0x8D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0x0D;
	cpu.RunCycle(cpuOwnershipId); // 0x0D - 0xF0 = 0x1D
	BOOST_CHECK(cpu.AF.h == 0x0D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 12);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_CP_A_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.HL.w = 0xFF;

	memorySmall.Write(0x0000, 0xBE); //CP A (HL)
	memorySmall.Write(0x0001, 0xBE); //CP A (HL)
	memorySmall.Write(0x0002, 0xBE); //CP A (HL)
	memorySmall.Write(0x0003, 0xBE); //CP A (HL)
	memorySmall.Write(0x0004, 0xBE); //CP A (HL)
	memorySmall.Write(0x0005, 0xBE); //CP A (HL)
	memorySmall.Write(0x0006, 0xBE); //CP A (HL)

	cpu.AF.h = 0x04;
	memorySmall.Write(cpu.HL.w, 0x08);
	cpu.RunCycle(cpuOwnershipId); // 0x04 - 0x08 = 0xFC
	BOOST_CHECK(cpu.AF.h == 0x04);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0xFC;
	memorySmall.Write(cpu.HL.w, 0x0F);
	cpu.RunCycle(cpuOwnershipId); // 0xFC - 0x0F = 0xED
	BOOST_CHECK(cpu.AF.h == 0xFC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0xED;
	memorySmall.Write(cpu.HL.w, 0x20);
	cpu.RunCycle(cpuOwnershipId); // 0xED - 0x20 = 0xDC
	BOOST_CHECK(cpu.AF.h == 0xED);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0xDC;
	memorySmall.Write(cpu.HL.w, 0x40);
	cpu.RunCycle(cpuOwnershipId); // 0xCD - 0x40 = 0x8D
	BOOST_CHECK(cpu.AF.h == 0xDC);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0x8D;
	memorySmall.Write(cpu.HL.w, 0x80);
	cpu.RunCycle(cpuOwnershipId); // 0x8D + 0x80 = 0x0D
	BOOST_CHECK(cpu.AF.h == 0x8D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.AF.h = 0x0D;
	memorySmall.Write(cpu.HL.w, 0xF0);
	cpu.RunCycle(cpuOwnershipId); // 0x0D - 0xF0 = 0x1D
	BOOST_CHECK(cpu.AF.h == 0x0D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.AF.h = 0x80;
	memorySmall.Write(cpu.HL.w, 0x80);
	cpu.RunCycle(cpuOwnershipId); // 0x80 - 0x80 = 0x00
	BOOST_CHECK(cpu.AF.h == 0x80);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_INC_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0xFF;
	cpu.BC.h = 0x08;
	cpu.BC.l = 0x0F;
	cpu.DE.h = 0x20;
	cpu.DE.l = 0x40;
	cpu.HL.h = 0x80;
	cpu.HL.l = 0xF0;

	memorySmall.Write(0x0000, 0x3C); //INC A
	memorySmall.Write(0x0001, 0x04); //INC B
	memorySmall.Write(0x0002, 0x0C); //INC C
	memorySmall.Write(0x0003, 0x14); //INC D
	memorySmall.Write(0x0004, 0x1C); //INC E
	memorySmall.Write(0x0005, 0x24); //INC H
	memorySmall.Write(0x0006, 0x2C); //INC L

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x09);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0x10);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x21);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x41);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x81);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0xF1);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_INC_$HL)
{
	MemoryElementSmall memorySmall;

	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0x34); //INC (HL)
	memorySmall.Write(0x0001, 0x34); //INC (HL)
	memorySmall.Write(0x0002, 0x34); //INC (HL)
	memorySmall.Write(0x0003, 0x34); //INC (HL)
	memorySmall.Write(0x0004, 0x34); //INC (HL)
	memorySmall.Write(0x0005, 0x34); //INC (HL)
	memorySmall.Write(0x0006, 0x34); //INC (HL)

	memorySmall.Write(0x0010, 0xFF);
	memorySmall.Write(0x0011, 0x08);
	memorySmall.Write(0x0012, 0x0F);
	memorySmall.Write(0x0013, 0x20);
	memorySmall.Write(0x0014, 0x40);
	memorySmall.Write(0x0015, 0x80);
	memorySmall.Write(0x0016, 0xF0);

	BYTE val = 0xFF;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x09);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x10);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x21);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x41);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x81);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xF1);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_DEC_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0xFF;
	cpu.BC.h = 0x08;
	cpu.BC.l = 0x0F;
	cpu.DE.h = 0x20;
	cpu.DE.l = 0x40;
	cpu.HL.h = 0x80;
	cpu.HL.l = 0x01;

	memorySmall.Write(0x0000, 0x3D); //DEC A
	memorySmall.Write(0x0001, 0x05); //DEC B
	memorySmall.Write(0x0002, 0x0D); //DEC C
	memorySmall.Write(0x0003, 0x15); //DEC D
	memorySmall.Write(0x0004, 0x1D); //DEC E
	memorySmall.Write(0x0005, 0x25); //DEC H
	memorySmall.Write(0x0006, 0x2D); //DEC L

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xFE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x07);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0x0E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x1F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x3F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x7F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_DEC_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0x35); //DEC (HL)
	memorySmall.Write(0x0001, 0x35); //DEC (HL)
	memorySmall.Write(0x0002, 0x35); //DEC (HL)
	memorySmall.Write(0x0003, 0x35); //DEC (HL)
	memorySmall.Write(0x0004, 0x35); //DEC (HL)
	memorySmall.Write(0x0005, 0x35); //DEC (HL)
	memorySmall.Write(0x0006, 0x35); //DEC (HL)

	memorySmall.Write(0x0010, 0xFF);
	memorySmall.Write(0x0011, 0x08);
	memorySmall.Write(0x0012, 0x0F);
	memorySmall.Write(0x0013, 0x20);
	memorySmall.Write(0x0014, 0x40);
	memorySmall.Write(0x0015, 0x80);
	memorySmall.Write(0x0016, 0x01);

	BYTE val = 0xFF;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xFE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x07);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x0E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x1F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x3F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x7F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 7);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_ADD_HL_ss)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.HL.w = 0x8A23;
	cpu.BC.w = 0x0605;
	cpu.DE.w = 0xFF00;
	cpu.SP.w = 0x00FF;

	memorySmall.Write(0x0000, 0x09); //ADD HL BC
	memorySmall.Write(0x0001, 0x19); //ADD HL DE
	memorySmall.Write(0x0002, 0x29); //ADD HL HL
	memorySmall.Write(0x0003, 0x39); //ADD HL SP

	cpu.ResetZ();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x9028);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x8F28);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.SetZ();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x1E50);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x1F4F);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_ADD_SP_e)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.SP.w = 0x0FFF;

	memorySmall.Write(0x0000, 0xE8);	//ADD SP e
	memorySmall.Write(0x0001, 0x23);	// 0x23
	memorySmall.Write(0x0002, 0xE8);	//ADD SP e
	memorySmall.Write(0x0003, 0xFF);	// -0x01
	memorySmall.Write(0x0004, 0xE8);	//ADD SP e
	memorySmall.Write(0x0005, 0x01);	// 0x01
	memorySmall.Write(0x0006, 0xE8);	//ADD SP e
	memorySmall.Write(0x0007, 0x80);	// -
	memorySmall.Write(0x0008, 0xE8);	//ADD SP e
	memorySmall.Write(0x0009, 0x80);	// -0x80

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.SP.w == 0x1022);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.SP.w == 0x1021);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.SetZ();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.SP.w == 0x1022);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.SP.w == 0x0FA2);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.SP.w = 0x0000;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.SP.w == 0xFF80);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 0x0A);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_INC_ss)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.BC.w = 0x0605;
	cpu.DE.w = 0xFF00;
	cpu.HL.w = 0x8A23;
	cpu.SP.w = 0x00FF;

	memorySmall.Write(0x0000, 0x03); //INC BC
	memorySmall.Write(0x0001, 0x13); //INC DE
	memorySmall.Write(0x0002, 0x23); //INC HL
	memorySmall.Write(0x0003, 0x33); //INC SP


	cpu.ResetZ();
	cpu.SetN();
	cpu.ResetH();
	cpu.SetCY();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.w == 0x0606);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.w == 0xFF01);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x8A24);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.SP.w == 0x0100);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_DEC_ss)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.BC.w = 0x0605;
	cpu.DE.w = 0xFF00;
	cpu.HL.w = 0x8A23;
	cpu.SP.w = 0x00FF;

	memorySmall.Write(0x0000, 0x0B); //DEC BC
	memorySmall.Write(0x0001, 0x1B); //DEC DE
	memorySmall.Write(0x0002, 0x2B); //DEC HL
	memorySmall.Write(0x0003, 0x3B); //DEC SP


	cpu.ResetZ();
	cpu.SetN();
	cpu.ResetH();
	cpu.SetCY();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.w == 0x0604);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.w == 0xFEFF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.w == 0x8A22);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.SP.w == 0x0FE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 1);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_RLCA)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0xA4;

	memorySmall.Write(0x0000, 0x07); //RLCA
	memorySmall.Write(0x0001, 0x07); //RLCA
	memorySmall.Write(0x0002, 0x07); //RLCA
	memorySmall.Write(0x0003, 0x07); //RLCA

	cpu.ResetZ();
	cpu.SetN();
	cpu.ResetH();
	cpu.SetCY();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x49);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x92);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x25);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x4A);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RLA)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0xA4;

	memorySmall.Write(0x0000, 0x17); //RLA
	memorySmall.Write(0x0001, 0x17); //RLA
	memorySmall.Write(0x0002, 0x17); //RLA
	memorySmall.Write(0x0003, 0x17); //RLA

	cpu.ResetZ();
	cpu.SetN();
	cpu.ResetH();
	cpu.SetCY();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x49);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x93);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x26);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x4D);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RRCA)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0xA4;

	memorySmall.Write(0x0000, 0x0F); //RRCA
	memorySmall.Write(0x0001, 0x0F); //RRCA
	memorySmall.Write(0x0002, 0x0F); //RRCA
	memorySmall.Write(0x0003, 0x0F); //RRCA

	cpu.ResetZ();
	cpu.SetN();
	cpu.ResetH();
	cpu.SetCY();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x52);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x29);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x94);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x4A);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RRA)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0xA4;

	memorySmall.Write(0x0000, 0x1F); //RCA
	memorySmall.Write(0x0001, 0x1F); //RCA
	memorySmall.Write(0x0002, 0x1F); //RCA
	memorySmall.Write(0x0003, 0x1F); //RCA

	cpu.ResetZ();
	cpu.SetN();
	cpu.ResetH();
	cpu.SetCY();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xD2);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x69);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x34);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x9A);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RLC_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RLC B
	memorySmall.Write(0x0001, 0x00);
	memorySmall.Write(0x0002, 0xCB); //RLC C
	memorySmall.Write(0x0003, 0x01);
	memorySmall.Write(0x0004, 0xCB); //RLC D
	memorySmall.Write(0x0005, 0x02);
	memorySmall.Write(0x0006, 0xCB); //RLC E
	memorySmall.Write(0x0007, 0x03);
	memorySmall.Write(0x0008, 0xCB); //RLC H
	memorySmall.Write(0x0009, 0x04);
	memorySmall.Write(0x000A, 0xCB); //RLC L
	memorySmall.Write(0x000B, 0x05);
	memorySmall.Write(0x000C, 0xCB); //RLC A
	memorySmall.Write(0x000D, 0x07);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();


	cpu.BC.h = 0x18;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x03;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x30);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xFB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x44);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x72);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x1F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0xE1);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x06);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RLC_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RLC (HL)
	memorySmall.Write(0x0001, 0x06);
	memorySmall.Write(0x0002, 0xCB); //RLC (HL)
	memorySmall.Write(0x0003, 0x06);
	memorySmall.Write(0x0004, 0xCB); //RLC (HL)
	memorySmall.Write(0x0005, 0x06);
	memorySmall.Write(0x0006, 0xCB); //RLC (HL)
	memorySmall.Write(0x0007, 0x06);
	memorySmall.Write(0x0008, 0xCB); //RLC (HL)
	memorySmall.Write(0x0009, 0x06);
	memorySmall.Write(0x000A, 0xCB); //RLC (HL)
	memorySmall.Write(0x000B, 0x06);
	memorySmall.Write(0x000C, 0xCB); //RLC (HL)
	memorySmall.Write(0x000D, 0x06);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();

	memorySmall.Write(0x0010, 0x18);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x03);

	BYTE val = 0x00;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x30);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xFB);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x44);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x72);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x1F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xE1);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x06);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RL_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RL B
	memorySmall.Write(0x0001, 0x10);
	memorySmall.Write(0x0002, 0xCB); //RL C
	memorySmall.Write(0x0003, 0x11);
	memorySmall.Write(0x0004, 0xCB); //RL D
	memorySmall.Write(0x0005, 0x12);
	memorySmall.Write(0x0006, 0xCB); //RL E
	memorySmall.Write(0x0007, 0x13);
	memorySmall.Write(0x0008, 0xCB); //RL H
	memorySmall.Write(0x0009, 0x14);
	memorySmall.Write(0x000A, 0xCB); //RL L
	memorySmall.Write(0x000B, 0x15);
	memorySmall.Write(0x000C, 0xCB); //RL A
	memorySmall.Write(0x000D, 0x17);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();


	cpu.BC.h = 0x18;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x03;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x30);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xFA);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x45);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x72);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0xE1);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x07);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RL_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RL (HL)
	memorySmall.Write(0x0001, 0x16);
	memorySmall.Write(0x0002, 0xCB); //RL (HL)
	memorySmall.Write(0x0003, 0x16);
	memorySmall.Write(0x0004, 0xCB); //RL (HL)
	memorySmall.Write(0x0005, 0x16);
	memorySmall.Write(0x0006, 0xCB); //RL (HL)
	memorySmall.Write(0x0007, 0x16);
	memorySmall.Write(0x0008, 0xCB); //RL (HL)
	memorySmall.Write(0x0009, 0x16);
	memorySmall.Write(0x000A, 0xCB); //RL (HL)
	memorySmall.Write(0x000B, 0x16);
	memorySmall.Write(0x000C, 0xCB); //RL (HL)
	memorySmall.Write(0x000D, 0x16);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();

	memorySmall.Write(0x0010, 0x18);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x03);

	BYTE val = 0x00;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x30);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xFA);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x45);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x72);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xE1);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x07);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RRC_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RRC B
	memorySmall.Write(0x0001, 0x08);
	memorySmall.Write(0x0002, 0xCB); //RRC C
	memorySmall.Write(0x0003, 0x09);
	memorySmall.Write(0x0004, 0xCB); //RRC D
	memorySmall.Write(0x0005, 0x0A);
	memorySmall.Write(0x0006, 0xCB); //RRC E
	memorySmall.Write(0x0007, 0x0B);
	memorySmall.Write(0x0008, 0xCB); //RRC H
	memorySmall.Write(0x0009, 0x0C);
	memorySmall.Write(0x000A, 0xCB); //RRC L
	memorySmall.Write(0x000B, 0x0D);
	memorySmall.Write(0x000C, 0xCB); //RRC A
	memorySmall.Write(0x000D, 0x0F);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();

	
	cpu.BC.h = 0x18;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x03;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xFE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x11);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x9C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0xC7);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x78);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x81);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RRC_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RRC (HL)
	memorySmall.Write(0x0001, 0x0E);
	memorySmall.Write(0x0002, 0xCB); //RRC (HL)
	memorySmall.Write(0x0003, 0x0E);
	memorySmall.Write(0x0004, 0xCB); //RRC (HL)
	memorySmall.Write(0x0005, 0x0E);
	memorySmall.Write(0x0006, 0xCB); //RRC (HL)
	memorySmall.Write(0x0007, 0x0E);
	memorySmall.Write(0x0008, 0xCB); //RRC (HL)
	memorySmall.Write(0x0009, 0x0E);
	memorySmall.Write(0x000A, 0xCB); //RRC (HL)
	memorySmall.Write(0x000B, 0x0E);
	memorySmall.Write(0x000C, 0xCB); //RRC (HL)
	memorySmall.Write(0x000D, 0x0E);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();

	memorySmall.Write(0x0010, 0x18);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x03);

	BYTE val = 0x00;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xFE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x11);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x9C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xC7);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x78);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x81);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RR_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RR B
	memorySmall.Write(0x0001, 0x18);
	memorySmall.Write(0x0002, 0xCB); //RR C
	memorySmall.Write(0x0003, 0x19);
	memorySmall.Write(0x0004, 0xCB); //RR D
	memorySmall.Write(0x0005, 0x1A);
	memorySmall.Write(0x0006, 0xCB); //RR E
	memorySmall.Write(0x0007, 0x1B);
	memorySmall.Write(0x0008, 0xCB); //RR H
	memorySmall.Write(0x0009, 0x1C);
	memorySmall.Write(0x000A, 0xCB); //RR L
	memorySmall.Write(0x000B, 0x1D);
	memorySmall.Write(0x000C, 0xCB); //RR A
	memorySmall.Write(0x000D, 0x1F);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();


	cpu.BC.h = 0x18;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x03;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0x7E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x91);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x1C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0xC7);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0xF8);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x01);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RR_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RR (HL)
	memorySmall.Write(0x0001, 0x1E);
	memorySmall.Write(0x0002, 0xCB); //RR (HL)
	memorySmall.Write(0x0003, 0x1E);
	memorySmall.Write(0x0004, 0xCB); //RR (HL)
	memorySmall.Write(0x0005, 0x1E);
	memorySmall.Write(0x0006, 0xCB); //RR (HL)
	memorySmall.Write(0x0007, 0x1E);
	memorySmall.Write(0x0008, 0xCB); //RR (HL)
	memorySmall.Write(0x0009, 0x1E);
	memorySmall.Write(0x000A, 0xCB); //RR (HL)
	memorySmall.Write(0x000B, 0x1E);
	memorySmall.Write(0x000C, 0xCB); //RR (HL)
	memorySmall.Write(0x000D, 0x1E);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();

	memorySmall.Write(0x0010, 0x18);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x03);

	BYTE val = 0x00;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x7E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x91);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x1C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xC7);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xF8);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x01);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SLA_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SLA B
	memorySmall.Write(0x0001, 0x20);
	memorySmall.Write(0x0002, 0xCB); //SLA C
	memorySmall.Write(0x0003, 0x21);
	memorySmall.Write(0x0004, 0xCB); //SLA D
	memorySmall.Write(0x0005, 0x22);
	memorySmall.Write(0x0006, 0xCB); //SLA E
	memorySmall.Write(0x0007, 0x23);
	memorySmall.Write(0x0008, 0xCB); //SLA H
	memorySmall.Write(0x0009, 0x24);
	memorySmall.Write(0x000A, 0xCB); //SLA L
	memorySmall.Write(0x000B, 0x25);
	memorySmall.Write(0x000C, 0xCB); //SLA A
	memorySmall.Write(0x000D, 0x27);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();


	cpu.BC.h = 0x18;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x80;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x30);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xFA);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x44);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x72);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0xE0);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SLA_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SLA (HL)
	memorySmall.Write(0x0001, 0x26);
	memorySmall.Write(0x0002, 0xCB); //SLA (HL)
	memorySmall.Write(0x0003, 0x26);
	memorySmall.Write(0x0004, 0xCB); //SLA (HL)
	memorySmall.Write(0x0005, 0x26);
	memorySmall.Write(0x0006, 0xCB); //SLA (HL)
	memorySmall.Write(0x0007, 0x26);
	memorySmall.Write(0x0008, 0xCB); //SLA (HL)
	memorySmall.Write(0x0009, 0x26);
	memorySmall.Write(0x000A, 0xCB); //SLA (HL)
	memorySmall.Write(0x000B, 0x26);
	memorySmall.Write(0x000C, 0xCB); //SLA (HL)
	memorySmall.Write(0x000D, 0x26);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();

	memorySmall.Write(0x0010, 0x18);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x80);

	BYTE val = 0x00;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x30);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xFA);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x44);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x72);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x1E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xE0);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SRA_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SRA B
	memorySmall.Write(0x0001, 0x28);
	memorySmall.Write(0x0002, 0xCB); //SRA C
	memorySmall.Write(0x0003, 0x29);
	memorySmall.Write(0x0004, 0xCB); //SRA D
	memorySmall.Write(0x0005, 0x2A);
	memorySmall.Write(0x0006, 0xCB); //SRA E
	memorySmall.Write(0x0007, 0x2B);
	memorySmall.Write(0x0008, 0xCB); //SRA H
	memorySmall.Write(0x0009, 0x2C);
	memorySmall.Write(0x000A, 0xCB); //SRA L
	memorySmall.Write(0x000B, 0x2D);
	memorySmall.Write(0x000C, 0xCB); //SRA A
	memorySmall.Write(0x000D, 0x2F);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();


	cpu.BC.h = 0x18;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x01;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xFE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x11);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x1C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0xC7);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0xF8);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SRA_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SRA (HL)
	memorySmall.Write(0x0001, 0x2E);
	memorySmall.Write(0x0002, 0xCB); //SRA (HL)
	memorySmall.Write(0x0003, 0x2E);
	memorySmall.Write(0x0004, 0xCB); //SRA (HL)
	memorySmall.Write(0x0005, 0x2E);
	memorySmall.Write(0x0006, 0xCB); //SRA (HL)
	memorySmall.Write(0x0007, 0x2E);
	memorySmall.Write(0x0008, 0xCB); //SRA (HL)
	memorySmall.Write(0x0009, 0x2E);
	memorySmall.Write(0x000A, 0xCB); //SRA (HL)
	memorySmall.Write(0x000B, 0x2E);
	memorySmall.Write(0x000C, 0xCB); //SRA (HL)
	memorySmall.Write(0x000D, 0x2E);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();

	memorySmall.Write(0x0010, 0x18);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x01);

	BYTE val = 0x00;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xFE);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x11);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x1C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xC7);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xF8);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SRL_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SRL B
	memorySmall.Write(0x0001, 0x38);
	memorySmall.Write(0x0002, 0xCB); //SRL C
	memorySmall.Write(0x0003, 0x39);
	memorySmall.Write(0x0004, 0xCB); //SRL D
	memorySmall.Write(0x0005, 0x3A);
	memorySmall.Write(0x0006, 0xCB); //SRL E
	memorySmall.Write(0x0007, 0x3B);
	memorySmall.Write(0x0008, 0xCB); //SRL H
	memorySmall.Write(0x0009, 0x3C);
	memorySmall.Write(0x000A, 0xCB); //SRL L
	memorySmall.Write(0x000B, 0x3D);
	memorySmall.Write(0x000C, 0xCB); //SRL A
	memorySmall.Write(0x000D, 0x3F);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();


	cpu.BC.h = 0x18;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x01;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0x7E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x11);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x1C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x47);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x78);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SRL_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SRL (HL)
	memorySmall.Write(0x0001, 0x3E);
	memorySmall.Write(0x0002, 0xCB); //SRL (HL)
	memorySmall.Write(0x0003, 0x3E);
	memorySmall.Write(0x0004, 0xCB); //SRL (HL)
	memorySmall.Write(0x0005, 0x3E);
	memorySmall.Write(0x0006, 0xCB); //SRL (HL)
	memorySmall.Write(0x0007, 0x3E);
	memorySmall.Write(0x0008, 0xCB); //SRL (HL)
	memorySmall.Write(0x0009, 0x3E);
	memorySmall.Write(0x000A, 0xCB); //SRL (HL)
	memorySmall.Write(0x000B, 0x3E);
	memorySmall.Write(0x000C, 0xCB); //SRL (HL)
	memorySmall.Write(0x000D, 0x3E);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();

	memorySmall.Write(0x0010, 0x18);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x01);

	BYTE val = 0x00;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x0C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x7E);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x11);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x1C);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x47);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x78);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SWAP_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SWAP B
	memorySmall.Write(0x0001, 0x30);
	memorySmall.Write(0x0002, 0xCB); //SWAP C
	memorySmall.Write(0x0003, 0x31);
	memorySmall.Write(0x0004, 0xCB); //SWAP D
	memorySmall.Write(0x0005, 0x32);
	memorySmall.Write(0x0006, 0xCB); //SWAP E
	memorySmall.Write(0x0007, 0x33);
	memorySmall.Write(0x0008, 0xCB); //SWAP H
	memorySmall.Write(0x0009, 0x34);
	memorySmall.Write(0x000A, 0xCB); //SWAP L
	memorySmall.Write(0x000B, 0x35);
	memorySmall.Write(0x000C, 0xCB); //SWAP A
	memorySmall.Write(0x000D, 0x37);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();


	cpu.BC.h = 0x18;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x00;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x81);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xDF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x22);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x93);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0xF8);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0x0F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SWAP_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SWAP (HL)
	memorySmall.Write(0x0001, 0x36);
	memorySmall.Write(0x0002, 0xCB); //SWAP (HL)
	memorySmall.Write(0x0003, 0x36);
	memorySmall.Write(0x0004, 0xCB); //SRL (HL)
	memorySmall.Write(0x0005, 0x36);
	memorySmall.Write(0x0006, 0xCB); //SWAP (HL)
	memorySmall.Write(0x0007, 0x36);
	memorySmall.Write(0x0008, 0xCB); //SWAP (HL)
	memorySmall.Write(0x0009, 0x36);
	memorySmall.Write(0x000A, 0xCB); //SWAP (HL)
	memorySmall.Write(0x000B, 0x36);
	memorySmall.Write(0x000C, 0xCB); //SWAP (HL)
	memorySmall.Write(0x000D, 0x36);


	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();

	memorySmall.Write(0x0010, 0x18);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x00);

	BYTE val = 0x00;

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x81);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);


	cpu.HL.w = 0x0011;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xDF);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0012;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x22);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0013;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x93);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0014;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xF8);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0015;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x0F);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.HL.w = 0x0016;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x00);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 14);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_BIT_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //BIT 0 B
	memorySmall.Write(0x0001, 0x40);
	memorySmall.Write(0x0002, 0xCB); //BIT 1 B
	memorySmall.Write(0x0003, 0x48);

	memorySmall.Write(0x0004, 0xCB); //BIT 1 C
	memorySmall.Write(0x0005, 0x49);
	memorySmall.Write(0x0006, 0xCB); //BIT 2 C
	memorySmall.Write(0x0007, 0x51);

	memorySmall.Write(0x0008, 0xCB); //BIT 2 D
	memorySmall.Write(0x0009, 0x52);
	memorySmall.Write(0x000A, 0xCB); //BIT 3 D
	memorySmall.Write(0x000B, 0x5A);

	memorySmall.Write(0x000C, 0xCB); //BIT 3 E
	memorySmall.Write(0x000D, 0x5B);
	memorySmall.Write(0x000E, 0xCB); //BIT 4 E
	memorySmall.Write(0x000F, 0x63);

	memorySmall.Write(0x0010, 0xCB); //BIT 4 H
	memorySmall.Write(0x0011, 0x64);
	memorySmall.Write(0x0012, 0xCB); //BIT 5 H
	memorySmall.Write(0x0013, 0x6C);

	memorySmall.Write(0x0014, 0xCB); //BIT 5 L
	memorySmall.Write(0x0015, 0x6D);
	memorySmall.Write(0x0016, 0xCB); //BIT 6 L
	memorySmall.Write(0x0017, 0x75);

	memorySmall.Write(0x0018, 0xCB); //BIT 6 A
	memorySmall.Write(0x0019, 0x77);
	memorySmall.Write(0x001A, 0xCB); //BIT 7 A
	memorySmall.Write(0x001B, 0x7F);

	memorySmall.Write(0x001C, 0xCB); //BIT 7 B
	memorySmall.Write(0x001D, 0x78);
	memorySmall.Write(0x001E, 0xCB); //BIT 2 B
	memorySmall.Write(0x001F, 0x50);

	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();


	cpu.BC.h = 0x81;
	cpu.BC.l = 0x40;
	cpu.DE.h = 0x20;
	cpu.DE.l = 0x10;
	cpu.HL.h = 0x08;
	cpu.HL.l = 0x04;
	cpu.AF.h = 0x02;

	//BIT 0 B
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//BIT 1 B
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//BIT 1 C
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//BIT 2 C
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//BIT 2 D
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//BIT 3 D
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//BIT 3 E
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	//BIT 4 E
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.SetCY();

	//BIT 4 H
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//BIT 5 H
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//BIT 5 L
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//BIT 6 L
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//BIT 6 A
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//BIT 7 A
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//BIT 7 B
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//BIT 1 B
	cpu.ResetH();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 1);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 32);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_BIT_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //BIT 0 (HL)
	memorySmall.Write(0x0001, 0x46);
	memorySmall.Write(0x0002, 0xCB); //BIT 1 (HL)
	memorySmall.Write(0x0003, 0x4E);
	memorySmall.Write(0x0004, 0xCB); //BIT 2 (HL)
	memorySmall.Write(0x0005, 0x56);
	memorySmall.Write(0x0006, 0xCB); //BIT 3 (HL)
	memorySmall.Write(0x0007, 0x5E);
	memorySmall.Write(0x0008, 0xCB); //BIT 4 (HL)
	memorySmall.Write(0x0009, 0x66);
	memorySmall.Write(0x000A, 0xCB); //BIT 5 (HL)
	memorySmall.Write(0x000B, 0x6E);
	memorySmall.Write(0x000C, 0xCB); //BIT 6 (HL)
	memorySmall.Write(0x000D, 0x76);
	memorySmall.Write(0x000E, 0xCB); //BIT 7 (HL)
	memorySmall.Write(0x000F, 0x7E);

	cpu.ResetZ();
	cpu.ResetN();
	cpu.ResetH();
	cpu.ResetCY();


	cpu.HL.w = 0x0010;
	memorySmall.Write(0x0010, 0xD2);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 0);

	cpu.SetCY();

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 0);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetZ() == 1);
	BOOST_CHECK(cpu.GetN() == 0);
	BOOST_CHECK(cpu.GetH() == 0);
	BOOST_CHECK(cpu.GetCY() == 1);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 16);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SET_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SET 0 B
	memorySmall.Write(0x0001, 0xC0);
	memorySmall.Write(0x0002, 0xCB); //SET 1 C
	memorySmall.Write(0x0003, 0xC9);
	memorySmall.Write(0x0004, 0xCB); //SET 2 D
	memorySmall.Write(0x0005, 0xD2);
	memorySmall.Write(0x0006, 0xCB); //SET 3 E
	memorySmall.Write(0x0007, 0xDB);
	memorySmall.Write(0x0008, 0xCB); //SET 4 H
	memorySmall.Write(0x0009, 0xE4);
	memorySmall.Write(0x000A, 0xCB); //SET 5 L
	memorySmall.Write(0x000B, 0xED);
	memorySmall.Write(0x000C, 0xCB); //SET 6 A
	memorySmall.Write(0x000D, 0xF7);
	memorySmall.Write(0x000E, 0xCB); //SET 7 B
	memorySmall.Write(0x000F, 0xF8);

	cpu.BC.h = 0x18;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x00;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x19);
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xFF);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x26);
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x39);
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x9F);
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0xF0);
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x40);
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x99);
	

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 16);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SET_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //SET 0 (HL)
	memorySmall.Write(0x0001, 0xC6);
	memorySmall.Write(0x0002, 0xCB); //SET 1 (HL)
	memorySmall.Write(0x0003, 0xCE);
	memorySmall.Write(0x0004, 0xCB); //SET 2 (HL)
	memorySmall.Write(0x0005, 0xD6);
	memorySmall.Write(0x0006, 0xCB); //SET 3 (HL)
	memorySmall.Write(0x0007, 0xDE);
	memorySmall.Write(0x0008, 0xCB); //SET 4 (HL)
	memorySmall.Write(0x0009, 0xE6);
	memorySmall.Write(0x000A, 0xCB); //SET 5 (HL)
	memorySmall.Write(0x000B, 0xEE);
	memorySmall.Write(0x000C, 0xCB); //SET 6 (HL)
	memorySmall.Write(0x000D, 0xF6);
	memorySmall.Write(0x000E, 0xCB); //SET 7 (HL)
	memorySmall.Write(0x000F, 0xFE);

	memorySmall.Write(0x0010, 0x18);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x00);
	memorySmall.Write(0x0017, 0x19);
	
	BYTE val = 0x00;

	cpu.HL.w = 0x10;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x19);

	cpu.HL.w = 0x11;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xFF);

	cpu.HL.w = 0x12;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x26);

	cpu.HL.w = 0x13;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x39);

	cpu.HL.w = 0x14;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x9F);

	cpu.HL.w = 0x15;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xF0);

	cpu.HL.w = 0x16;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x40);

	cpu.HL.w = 0x17;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x99);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 16);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RES_r)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RES 0 B
	memorySmall.Write(0x0001, 0x80);
	memorySmall.Write(0x0002, 0xCB); //RES 1 C
	memorySmall.Write(0x0003, 0x89);
	memorySmall.Write(0x0004, 0xCB); //RES 2 D
	memorySmall.Write(0x0005, 0x92);
	memorySmall.Write(0x0006, 0xCB); //RES 3 E
	memorySmall.Write(0x0007, 0x9B);
	memorySmall.Write(0x0008, 0xCB); //RES 4 H
	memorySmall.Write(0x0009, 0xA4);
	memorySmall.Write(0x000A, 0xCB); //RES 5 L
	memorySmall.Write(0x000B, 0xAD);
	memorySmall.Write(0x000C, 0xCB); //RES 6 A
	memorySmall.Write(0x000D, 0xB7);
	memorySmall.Write(0x000E, 0xCB); //RES 7 B
	memorySmall.Write(0x000F, 0xB8);

	cpu.BC.h = 0x98;
	cpu.BC.l = 0xFD;
	cpu.DE.h = 0x22;
	cpu.DE.l = 0x39;
	cpu.HL.h = 0x8F;
	cpu.HL.l = 0xF0;
	cpu.AF.h = 0x00;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x98);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.l == 0xFD);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.h == 0x22);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.DE.l == 0x31);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.h == 0x8F);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.HL.l == 0xD0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.BC.h == 0x18);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 16);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RES_$HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xCB); //RES 0 (HL)
	memorySmall.Write(0x0001, 0x86);
	memorySmall.Write(0x0002, 0xCB); //RES 1 (HL)
	memorySmall.Write(0x0003, 0x8E);
	memorySmall.Write(0x0004, 0xCB); //RES 2 (HL)
	memorySmall.Write(0x0005, 0x96);
	memorySmall.Write(0x0006, 0xCB); //RES 3 (HL)
	memorySmall.Write(0x0007, 0x9E);
	memorySmall.Write(0x0008, 0xCB); //RES 4 (HL)
	memorySmall.Write(0x0009, 0xA6);
	memorySmall.Write(0x000A, 0xCB); //RES 5 (HL)
	memorySmall.Write(0x000B, 0xAE);
	memorySmall.Write(0x000C, 0xCB); //RES 6 (HL)
	memorySmall.Write(0x000D, 0xB6);
	memorySmall.Write(0x000E, 0xCB); //RES 7 (HL)
	memorySmall.Write(0x000F, 0xBE);

	memorySmall.Write(0x0010, 0x98);
	memorySmall.Write(0x0011, 0xFD);
	memorySmall.Write(0x0012, 0x22);
	memorySmall.Write(0x0013, 0x39);
	memorySmall.Write(0x0014, 0x8F);
	memorySmall.Write(0x0015, 0xF0);
	memorySmall.Write(0x0016, 0x00);
	memorySmall.Write(0x0017, 0x19);

	BYTE val = 0x00;

	cpu.HL.w = 0x10;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x98);

	cpu.HL.w = 0x11;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xFD);

	cpu.HL.w = 0x12;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x22);

	cpu.HL.w = 0x13;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x31);

	cpu.HL.w = 0x14;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x8F);

	cpu.HL.w = 0x15;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0xD0);

	cpu.HL.w = 0x16;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x00);

	cpu.HL.w = 0x17;
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(cpu.HL.w, val);
	BOOST_CHECK(val == 0x19);


	//CHECK PC state
	BOOST_CHECK(cpu.PC == 16);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_JP_nn)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xC3); //JP 0x00AB
	memorySmall.Write(0x0001, 0xAB);
	memorySmall.Write(0x0002, 0x00);

	memorySmall.Write(0x00AB, 0xC3); //JP 0x00AB
	memorySmall.Write(0x00AC, 0xFF);
	memorySmall.Write(0x00AD, 0xFF);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x00AB);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0xFFFF);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_JP_cc_nn)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xC2); //JP NZ 0x0010 (YES)
	memorySmall.Write(0x0001, 0x10);
	memorySmall.Write(0x0002, 0x00);

	memorySmall.Write(0x0010, 0xC2); //JP NZ 0xFFFF (NO)
	memorySmall.Write(0x0011, 0xFF);
	memorySmall.Write(0x0012, 0xFF);

	memorySmall.Write(0x0013, 0xCA); //JP Z 0x0020 (YES)
	memorySmall.Write(0x0014, 0x20);
	memorySmall.Write(0x0015, 0x00);

	memorySmall.Write(0x0020, 0xCA); //JP Z 0xFFFF (NO)
	memorySmall.Write(0x0021, 0xFF);
	memorySmall.Write(0x0022, 0xFF);

	memorySmall.Write(0x0023, 0xD2); //JP NC 0x0030 (YES)
	memorySmall.Write(0x0024, 0x30);
	memorySmall.Write(0x0025, 0x00);

	memorySmall.Write(0x0030, 0xD2); //JP NC 0xFFFF (NO)
	memorySmall.Write(0x0031, 0xFF);
	memorySmall.Write(0x0032, 0xFF);

	memorySmall.Write(0x0033, 0xDA); //JP C 0x0040 (YES)
	memorySmall.Write(0x0034, 0x40);
	memorySmall.Write(0x0035, 0x00);

	memorySmall.Write(0x0040, 0xCA); //JP C 0xFFFF (NO)
	memorySmall.Write(0x0041, 0xFF);
	memorySmall.Write(0x0042, 0xFF);

	cpu.ResetZ();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0010);

	cpu.SetZ();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0013);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0020);

	cpu.ResetZ();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0023);

	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0030);

	cpu.SetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0033);

	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0040);

	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0043);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_JR_e)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0x18); //JR +129
	memorySmall.Write(0x0001, 0x7F);
	
	memorySmall.Write(0x0081, 0x18); //JR -126
	memorySmall.Write(0x0082, 0x80);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0081);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x003);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_JP_cc_e)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0x20); //JR NZ +129 (YES)
	memorySmall.Write(0x0001, 0x7F);
	
	memorySmall.Write(0x0081, 0x20); //JR NZ -126
	memorySmall.Write(0x0082, 0x80);
	
	memorySmall.Write(0x0003, 0x20); //JR NZ xxxx (NO)
	memorySmall.Write(0x0004, 0x80);

	memorySmall.Write(0x0005, 0x28); //JP Z +128 (YES)
	memorySmall.Write(0x0006, 0x7E);
	
	memorySmall.Write(0x0085, 0x28); //JP Z -120 (YES)
	memorySmall.Write(0x0086, 0x86);

	memorySmall.Write(0x000D, 0x30); //JP NC +100 (YES)
	memorySmall.Write(0x000E, 0x62);

	memorySmall.Write(0x0071, 0x38); //JP C +100 (YES)
	memorySmall.Write(0x0072, 0x62);

	cpu.ResetZ();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0081);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0003);

	cpu.SetZ();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0005);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0085);

	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x000D);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0071);

	cpu.SetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x00D5);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_JP_HL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x0000, 0xE9); //JP HL
	memorySmall.Write(0x0010, 0xE9); //JP HL
	memorySmall.Write(0x0020, 0xE9); //JP HL
	memorySmall.Write(0x0030, 0xE9); //JP HL
	memorySmall.Write(0x0080, 0xE9); //JP HL

	cpu.HL.w = 0x0010;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == cpu.HL.w);

	cpu.HL.w = 0x0020;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == cpu.HL.w);

	cpu.HL.w = 0x0030;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == cpu.HL.w);

	cpu.HL.w = 0x0080;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == cpu.HL.w);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_CALL_nn)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.SP.w = 0x00FF;

	memorySmall.Write(0x0000, 0xCD); //CALL 0x0010
	memorySmall.Write(0x0001, 0x10);
	memorySmall.Write(0x0002, 0x00);

	memorySmall.Write(0x0010, 0xCD); //CALL 0x0020
	memorySmall.Write(0x0011, 0x20);
	memorySmall.Write(0x0012, 0x00);

	memorySmall.Write(0x0020, 0xCD); //CALL 0x0030
	memorySmall.Write(0x0021, 0x30);
	memorySmall.Write(0x0022, 0x00);

	memorySmall.Write(0x0030, 0xCD); //CALL 0x0040
	memorySmall.Write(0x0031, 0x40);
	memorySmall.Write(0x0032, 0x00);

	
	BYTE valL = 0x00;
	BYTE valH = 0x00;

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFE, valH);
	memorySmall.Read(0xFD, valL);
	BOOST_CHECK(cpu.PC == 0x0010);
	BOOST_CHECK(cpu.SP.w == 0x00FD);
	BOOST_CHECK(valL == 0x03);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFC, valH);
	memorySmall.Read(0xFB, valL);
	BOOST_CHECK(cpu.PC == 0x0020);
	BOOST_CHECK(cpu.SP.w == 0x00FB);
	BOOST_CHECK(valL == 0x13);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFA, valH);
	memorySmall.Read(0xF9, valL);
	BOOST_CHECK(cpu.PC == 0x0030);
	BOOST_CHECK(cpu.SP.w == 0x00F9);
	BOOST_CHECK(valL == 0x23);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xF8, valH);
	memorySmall.Read(0xF7, valL);
	BOOST_CHECK(cpu.PC == 0x0040);
	BOOST_CHECK(cpu.SP.w == 0x00F7);
	BOOST_CHECK(valL == 0x33);
	BOOST_CHECK(valH == 0x00);

	MemoryController::Shared()->Clear();
}


BOOST_AUTO_TEST_CASE(CPUInstruction_CALL_cc_nn)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.SP.w = 0x00FF;

	memorySmall.Write(0x0000, 0xC4); //CALL NZ 0x0010
	memorySmall.Write(0x0001, 0x10);
	memorySmall.Write(0x0002, 0x00);

	memorySmall.Write(0x0010, 0xC4); //CALL NZ 0x0020
	memorySmall.Write(0x0011, 0x20);
	memorySmall.Write(0x0012, 0x00);

	memorySmall.Write(0x0013, 0xCC); //CALL Z 0x0030
	memorySmall.Write(0x0014, 0x30);
	memorySmall.Write(0x0015, 0x00);

	memorySmall.Write(0x0030, 0xCC); //CALL Z 0x0040
	memorySmall.Write(0x0031, 0x40);
	memorySmall.Write(0x0032, 0x00);

	memorySmall.Write(0x0033, 0xD4); //CALL NC 0x0050
	memorySmall.Write(0x0034, 0x50);
	memorySmall.Write(0x0035, 0x00);

	memorySmall.Write(0x0050, 0xD4); //CALL NC 0x0060
	memorySmall.Write(0x0051, 0x60);
	memorySmall.Write(0x0052, 0x00);

	memorySmall.Write(0x0053, 0xDC); //CALL CY 0x0070
	memorySmall.Write(0x0054, 0x70);
	memorySmall.Write(0x0055, 0x00);

	memorySmall.Write(0x0070, 0xDC); //CALL CY 0x0080
	memorySmall.Write(0x0071, 0x80);
	memorySmall.Write(0x0072, 0x00);


	BYTE valL = 0x00;
	BYTE valH = 0x00;

	cpu.ResetZ();
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFE, valH);
	memorySmall.Read(0xFD, valL);
	BOOST_CHECK(cpu.PC == 0x0010);
	BOOST_CHECK(cpu.SP.w == 0x00FD);
	BOOST_CHECK(valL == 0x03);
	BOOST_CHECK(valH == 0x00);

	cpu.SetZ();
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFE, valH);
	memorySmall.Read(0xFD, valL);
	BOOST_CHECK(cpu.PC == 0x0013);
	BOOST_CHECK(cpu.SP.w == 0x00FD);
	BOOST_CHECK(valL == 0x03);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFC, valH);
	memorySmall.Read(0xFB, valL);
	BOOST_CHECK(cpu.PC == 0x0030);
	BOOST_CHECK(cpu.SP.w == 0x00FB);
	BOOST_CHECK(valL == 0x16);
	BOOST_CHECK(valH == 0x00);

	cpu.ResetZ();
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFC, valH);
	memorySmall.Read(0xFB, valL);
	BOOST_CHECK(cpu.PC == 0x0033);
	BOOST_CHECK(cpu.SP.w == 0x00FB);
	BOOST_CHECK(valL == 0x16);
	BOOST_CHECK(valH == 0x00);




	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFA, valH);
	memorySmall.Read(0xF9, valL);
	BOOST_CHECK(cpu.PC == 0x0050);
	BOOST_CHECK(cpu.SP.w == 0x00F9);
	BOOST_CHECK(valL == 0x36);
	BOOST_CHECK(valH == 0x00);

	cpu.SetCY();
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFA, valH);
	memorySmall.Read(0xF9, valL);
	BOOST_CHECK(cpu.PC == 0x0053);
	BOOST_CHECK(cpu.SP.w == 0x00F9);
	BOOST_CHECK(valL == 0x36);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xF8, valH);
	memorySmall.Read(0xF7, valL);
	BOOST_CHECK(cpu.PC == 0x0070);
	BOOST_CHECK(cpu.SP.w == 0x00F7);
	BOOST_CHECK(valL == 0x56);
	BOOST_CHECK(valH == 0x00);

	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xF8, valH);
	memorySmall.Read(0xF7, valL);
	BOOST_CHECK(cpu.PC == 0x0073);
	BOOST_CHECK(cpu.SP.w == 0x00F7);
	BOOST_CHECK(valL == 0x56);
	BOOST_CHECK(valH == 0x00);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RET)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x00F7, 0x30);
	memorySmall.Write(0x00F8, 0x00);
	memorySmall.Write(0x00F9, 0x20);
	memorySmall.Write(0x00FA, 0x00);
	memorySmall.Write(0x00FB, 0x10);
	memorySmall.Write(0x00FC, 0x00);
	memorySmall.Write(0x00FD, 0x00);
	memorySmall.Write(0x00FE, 0x00);


	memorySmall.Write(0x0040, 0xC9); //RET
	memorySmall.Write(0x0030, 0xC9); //RET
	memorySmall.Write(0x0020, 0xC9); //RET
	memorySmall.Write(0x0010, 0xC9); //RET

	cpu.PC = 0x0040;
	cpu.SP.w = 0x00F7;

	/////
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0030);
	BOOST_CHECK(cpu.SP.w == 0x00F9);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0020);
	BOOST_CHECK(cpu.SP.w == 0x00FB);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0010);
	BOOST_CHECK(cpu.SP.w == 0x00FD);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0000);
	BOOST_CHECK(cpu.SP.w == 0x00FF);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RETI)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x00F7, 0x30);
	memorySmall.Write(0x00F8, 0x00);
	memorySmall.Write(0x00F9, 0x20);
	memorySmall.Write(0x00FA, 0x00);
	memorySmall.Write(0x00FB, 0x10);
	memorySmall.Write(0x00FC, 0x00);
	memorySmall.Write(0x00FD, 0x00);
	memorySmall.Write(0x00FE, 0x00);


	memorySmall.Write(0x0040, 0xD9); //RETI
	memorySmall.Write(0x0030, 0xD9); //RETI
	memorySmall.Write(0x0020, 0xD9); //RETI
	memorySmall.Write(0x0010, 0xD9); //RETI

	cpu.PC = 0x0040;
	cpu.SP.w = 0x00F7;
	cpu.IME = false;

	/////
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0030);
	BOOST_CHECK(cpu.SP.w == 0x00F9);
	BOOST_CHECK(cpu.IME == true);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0020);
	BOOST_CHECK(cpu.SP.w == 0x00FB);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0010);
	BOOST_CHECK(cpu.SP.w == 0x00FD);

	cpu.IME = false;

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0000);
	BOOST_CHECK(cpu.SP.w == 0x00FF);
	BOOST_CHECK(cpu.IME == true);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RET_cc)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	memorySmall.Write(0x00F7, 0x30);
	memorySmall.Write(0x00F8, 0x00);
	memorySmall.Write(0x00F9, 0x20);
	memorySmall.Write(0x00FA, 0x00);
	memorySmall.Write(0x00FB, 0x10);
	memorySmall.Write(0x00FC, 0x00);
	memorySmall.Write(0x00FD, 0x00);
	memorySmall.Write(0x00FE, 0x00);


	memorySmall.Write(0x0040, 0xC0); //RET NZ
	memorySmall.Write(0x0030, 0xC0); //RET NZ
	memorySmall.Write(0x0031, 0xC8); //RET Z
	memorySmall.Write(0x0020, 0xC8); //RET Z
	memorySmall.Write(0x0021, 0xD0); //RET NC
	memorySmall.Write(0x0010, 0xD0); //RET NC
	memorySmall.Write(0x0011, 0xD8); //RET CY
	memorySmall.Write(0x0000, 0xD8); //RET CY

	cpu.PC = 0x0040;
	cpu.SP.w = 0x00F7;

	/////
	cpu.ResetZ();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0030);
	BOOST_CHECK(cpu.SP.w == 0x00F9);

	cpu.SetZ();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0031);
	BOOST_CHECK(cpu.SP.w == 0x00F9);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0020);
	BOOST_CHECK(cpu.SP.w == 0x00FB);

	cpu.ResetZ();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0021);
	BOOST_CHECK(cpu.SP.w == 0x00FB);

	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0010);
	BOOST_CHECK(cpu.SP.w == 0x00FD);

	cpu.SetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0011);
	BOOST_CHECK(cpu.SP.w == 0x00FD);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0000);
	BOOST_CHECK(cpu.SP.w == 0x00FF);

	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.PC == 0x0001);
	BOOST_CHECK(cpu.SP.w == 0x00FF);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_RST_t)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.SP.w = 0x00FF;

	memorySmall.Write(0x0000, 0xCF); //RST 1
	memorySmall.Write(0x0008, 0xD7); //RST 2
	memorySmall.Write(0x0010, 0xDF); //RST 3
	memorySmall.Write(0x0018, 0xE7); //RST 4
	memorySmall.Write(0x0020, 0xEF); //RST 5
	memorySmall.Write(0x0028, 0xF7); //RST 6
	memorySmall.Write(0x0030, 0xFF); //RST 7
	memorySmall.Write(0x0038, 0xC7); //RST 0

	BYTE valL = 0x00;
	BYTE valH = 0x00;

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFE, valH);
	memorySmall.Read(0xFD, valL);
	BOOST_CHECK(cpu.PC == 0x0008);
	BOOST_CHECK(cpu.SP.w == 0x00FD);
	BOOST_CHECK(valL == 0x01);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFC, valH);
	memorySmall.Read(0xFB, valL);
	BOOST_CHECK(cpu.PC == 0x0010);
	BOOST_CHECK(cpu.SP.w == 0x00FB);
	BOOST_CHECK(valL == 0x09);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xFA, valH);
	memorySmall.Read(0xF9, valL);
	BOOST_CHECK(cpu.PC == 0x0018);
	BOOST_CHECK(cpu.SP.w == 0x00F9);
	BOOST_CHECK(valL == 0x11);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xF8, valH);
	memorySmall.Read(0xF7, valL);
	BOOST_CHECK(cpu.PC == 0x0020);
	BOOST_CHECK(cpu.SP.w == 0x00F7);
	BOOST_CHECK(valL == 0x19);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xF6, valH);
	memorySmall.Read(0xF5, valL);
	BOOST_CHECK(cpu.PC == 0x0028);
	BOOST_CHECK(cpu.SP.w == 0x00F5);
	BOOST_CHECK(valL == 0x21);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xF4, valH);
	memorySmall.Read(0xF3, valL);
	BOOST_CHECK(cpu.PC == 0x0030);
	BOOST_CHECK(cpu.SP.w == 0x00F3);
	BOOST_CHECK(valL == 0x29);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xF2, valH);
	memorySmall.Read(0xF1, valL);
	BOOST_CHECK(cpu.PC == 0x0038);
	BOOST_CHECK(cpu.SP.w == 0x00F1);
	BOOST_CHECK(valL == 0x31);
	BOOST_CHECK(valH == 0x00);

	cpu.RunCycle(cpuOwnershipId);
	memorySmall.Read(0xF0, valH);
	memorySmall.Read(0xEF, valL);
	BOOST_CHECK(cpu.PC == 0x0000);
	BOOST_CHECK(cpu.SP.w == 0x00EF);
	BOOST_CHECK(valL == 0x39);
	BOOST_CHECK(valH == 0x00);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_DAA)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	cpu.AF.h = 0x45;
	cpu.BC.h = 0x38;
	
	memorySmall.Write(0x0000, 0x80); //ADD A B
	memorySmall.Write(0x0001, 0x27); //DAA
	memorySmall.Write(0x0002, 0x90); //SUB A B
	memorySmall.Write(0x0003, 0x27); //DAA

	cpu.RunCycle(cpuOwnershipId);
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x83);
	BOOST_CHECK(cpu.GetCY() == false);

	cpu.RunCycle(cpuOwnershipId);
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x45);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_CPL)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();

	
	memorySmall.Write(0x0000, 0x2F); //CPL
	memorySmall.Write(0x0001, 0x2F); //CPL
	memorySmall.Write(0x0002, 0x2F); //CPL
	memorySmall.Write(0x0003, 0x2F); //CPL

	cpu.AF.h = 0x35;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0xCA);
	BOOST_CHECK(cpu.GetN() != 0);
	BOOST_CHECK(cpu.GetH() != 0);


	cpu.AF.h = 0xDD;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x22);

	cpu.AF.h = 0x90;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x6F);

	cpu.AF.h = 0x7A;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.AF.h == 0x85);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_NOP)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	memorySmall.Write(0x0000, 0x00); //NOP
	memorySmall.Write(0x0001, 0x00); //NOP
	memorySmall.Write(0x0002, 0x00); //NOP
	memorySmall.Write(0x0003, 0x00); //NOP

	cpu.RunCycle(cpuOwnershipId);
	cpu.RunCycle(cpuOwnershipId);
	cpu.RunCycle(cpuOwnershipId);
	cpu.RunCycle(cpuOwnershipId);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_CCF)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	memorySmall.Write(0x0000, 0x3F); //CCF
	memorySmall.Write(0x0001, 0x3F); //CCF
	memorySmall.Write(0x0002, 0x3F); //CCF
	memorySmall.Write(0x0003, 0x3F); //CCF

	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetCY() != 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetCY() == 0);
	
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetCY() != 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetCY() == 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_SCF)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	memorySmall.Write(0x0000, 0x37); //SCF
	memorySmall.Write(0x0001, 0x37); //SCF
	memorySmall.Write(0x0002, 0x37); //SCF
	memorySmall.Write(0x0003, 0x37); //SCF

	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetCY() != 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetCY() != 0);

	cpu.ResetCY();
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetCY() != 0);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.GetCY() != 0);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 4);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_DI)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	memorySmall.Write(0x0000, 0xF3); //DI
	
	cpu.IME = true;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.IME == false);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_EI)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	memorySmall.Write(0x0000, 0xFB); //DI

	cpu.IME = false;
	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.IME == true);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_HALT)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	memorySmall.Write(0x0000, 0x76); //HALT

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.haltMode == true);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 1);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_CASE(CPUInstruction_STOP)
{
	MemoryElementSmall memorySmall;
	
	MemoryController::Shared()->AppendMemoryElement(&memorySmall);

	CPU cpu;
	int cpuOwnershipId = cpu.GetOwnershipId();


	memorySmall.Write(0x0000, 0x10); //HALT
	memorySmall.Write(0x0001, 0x00);

	cpu.RunCycle(cpuOwnershipId);
	BOOST_CHECK(cpu.stopMode == true);

	//CHECK PC state
	BOOST_CHECK(cpu.PC == 0x02);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_SUITE_END()

#endif