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

#ifndef JML_CPU
#define JML_CPU

#include "../../Metadata/Configuration.h"
#include "../../Metadata/DataTypes.h"
#include "../Memory/MemoryController.h"

class MainMemory;

enum INTERRUPT_FLAG
{
	IF_V_BLANK				=	0x01,
	IF_LCDC				=	0x02,
	IF_TIMER_OVERFLOW		=	0x04,
	IF_SERIAL_IO_TRANSFER	=	0x08,
	IF_P10_P13_LOW			=	0x10,
};

enum INTERRUPT_ADDRESS
{
	IA_V_BLANK				=	0x40,
	IA_LCDC					=	0x48,
	IA_TIMER_OVERFLOW		=	0x50,
	IA_SERIAL_IO_TRANSFER	=	0x58,
	IA_P10_P13_LOW			=	0x60,
};

class CPU : public MemoryElement
{
	friend class CPURegistersItemModel;
	friend class DisassemblerStringListModel;
public:
	CPU();
	~CPU();

	void Reset();

	bool Write(const WORD &address, const BYTE &value);
	bool Read(const WORD &address, BYTE &out);
	
	void SetInterrupt(INTERRUPT_FLAG flag);

	static int TARGET_CPU_SPEED;

#ifdef DEBUGGER_ON
	int GetOwnershipId();
	void ReleaseOwnership();
	BYTE RunCycle(int ownerId);
	int intructionCounter[256];
	int instructionCBCounter[256];
	unsigned long emulationCycleCount;
	unsigned long realCycleCount;
#else
	BYTE RunCycle();
#endif

#ifdef UNIT_TEST_ON
public:
#else
private:
#endif
	//Controller data
	//BYTE P1;

	//Interrupt registers

	/*
	Address 0xFFFF was mapped to a special register referred 
	to as the Interrupt Enable Register. It was used to enable/disable 
	interrupts.
	*/

	bool IME;
	BYTE IF;
	BYTE IE;

	bool haltMode;
	bool stopMode;

	BYTE stackRam[0x7F]; //127

	void LoadInstructions();
	void CheckInterrupt();
	void StartInterrupt(INTERRUPT_FLAG flag);
	void PushPC();

	BYTE opcode;

	WORD PC;
	DREG SP;

	DREG AF;
	DREG BC;
	DREG DE;
	DREG HL;

	//Register From Code
	BYTE& Get_r(BYTE code);
	DREG& Get_dd(BYTE code);
	DREG& Get_qq(BYTE code);
	bool  GetFlag_cc(BYTE code);

	//Flags
	void SetZ();
	void ResetZ();
	void SetN();
	void ResetN();
	void SetH();
	void ResetH();
	void SetCY();
	void ResetCY();
	char GetZ();
	char GetN();
	char GetH();
	char GetCY();

	//Operations With That Affects Flags
	void Add8AndChangeFlags(BYTE& dest, const BYTE& s);
	void Add16AndChangeFlags(WORD& dest, const WORD& s);
	void Sub8AndChangeFlags(BYTE& dest, const BYTE& s);

	void AddSigned8To16AndChangeFlags(WORD& dest, const char& s);

	//INSTRUCTIONS TABLES
	typedef unsigned char(CPU::*Instruction)(void);
	CPU::Instruction instructions[256];
	CPU::Instruction cbInstructions[256];

	//INSTRUCTIONS
	//8-Bit Transfer & I/O

	//LD R, R'
	unsigned char LD_R_R();
	//unsigned char LD_A_A();	unsigned char LD_A_B();	unsigned char LD_A_C();	unsigned char LD_A_D();	unsigned char LD_A_E();	unsigned char LD_A_H();	unsigned char LD_A_L(); 
	//unsigned char LD_B_A();	unsigned char LD_B_B();	unsigned char LD_B_C();	unsigned char LD_B_D();	unsigned char LD_B_E();	unsigned char LD_B_H();	unsigned char LD_B_L();
	//unsigned char LD_C_A();	unsigned char LD_C_B();	unsigned char LD_C_C();	unsigned char LD_C_D();	unsigned char LD_C_E();	unsigned char LD_C_H();	unsigned char LD_C_L();
	//unsigned char LD_D_A();	unsigned char LD_D_B();	unsigned char LD_D_C();	unsigned char LD_D_D();	unsigned char LD_D_E();	unsigned char LD_D_H();	unsigned char LD_D_L();
	//unsigned char LD_E_A();	unsigned char LD_E_B();	unsigned char LD_E_C();	unsigned char LD_E_D();	unsigned char LD_E_E();	unsigned char LD_E_H();	unsigned char LD_E_L();
	//unsigned char LD_H_A();	unsigned char LD_H_B();	unsigned char LD_H_C();	unsigned char LD_H_D();	unsigned char LD_H_E();	unsigned char LD_H_H();	unsigned char LD_H_L();
	//unsigned char LD_L_A();	unsigned char LD_L_B();	unsigned char LD_L_C();	unsigned char LD_L_D();	unsigned char LD_L_E();	unsigned char LD_L_H();	unsigned char LD_L_L();

	//LD R, n
	unsigned char LD_R_n();
	//unsigned char LD_A_n();	unsigned char LD_B_n();	unsigned char LD_C_n();	unsigned char LD_D_n();	unsigned char LD_E_n();	unsigned char LD_H_n();	unsigned char LD_L_n();

	//LD R, (HL)
	unsigned char LD_R_$HL();
	//unsigned char LD_A_$HL();unsigned char LD_B_$HL();unsigned char LD_C_$HL(); unsigned char LD_D_$HL();unsigned char LD_E_$HL();unsigned char LD_H_$HL();unsigned char LD_L_$HL();

	//LD (HL), R
	unsigned char LD_$HL_R();
	//unsigned char LD_$HL_A();unsigned char LD_$HL_B();unsigned char LD_$HL_C();unsigned char LD_$HL_D();unsigned char LD_$HL_E();unsigned char LD_$HL_H();unsigned char LD_$HL_L();

	//LD (HL), n
	unsigned char LD_$HL_n();

	//LD A, (BC)
	unsigned char LD_A_$BC();

	//LD A, (DE)
	unsigned char LD_A_$DE();

	//LD A, (C)
	unsigned char LD_A_$C();

	//LD (C), A
	unsigned char LD_$C_A();

	//LD A, (n)
	unsigned char LD_A_$n(); 

	//LD (n), A
	unsigned char LD_$n_A();

	//LD A, (nn)
	unsigned char LD_A_$nn();

	//LD (nn), A
	unsigned char LD_$nn_A();

	//LD A, (HLI)
	unsigned char LD_A_$HLI(); 

	//LD A, (HLD)
	unsigned char LD_A_$HLD();

	//LD (BC), A
	unsigned char LD_$BC_A();

	//LD (DE), A
	unsigned char LD_$DE_A();

	//LD (HLI), A
	unsigned char LD_$HLI_A();

	//LD (HLD), A
	unsigned char LD_$HLD_A();

	//LD dd, nn
	unsigned char LD_dd_nn(); 
	//unsigned char LD_BC_nn();	unsigned char LD_DE_nn();	unsigned char LD_HL_nn(); unsigned char LD_SP_nn();

	//LD SP, HL
	unsigned char LD_SP_HL();

	//PUSH qq
	unsigned char PUSH_qq();
	//unsigned char PUSH_BC();	unsigned char PUSH_DE();	unsigned char PUSH_HL();	unsigned char PUSH_AF();

	//POP qq
	unsigned char POP_qq();
	//unsigned char POP_BC();	unsigned char POP_DE();	unsigned char POP_HL();	unsigned char POP_AF();

	//LDHL SP, e
	unsigned char LDHL_SP_e();

	//LD (nn), SP
	unsigned char LD_$nn_SP();

	//ADD A, s ( r,n, (HL) )
	unsigned char ADD_A_r();
	//unsigned char ADD_A_A();	unsigned char ADD_A_B();	unsigned char ADD_A_C();	unsigned char ADD_A_D();	unsigned char ADD_A_E();	unsigned char ADD_A_H();	unsigned char ADD_A_L();
	unsigned char ADD_A_n(); 
	unsigned char ADD_A_$HL();

	//ADC A, s ( r,n, (HL) )
	unsigned char ADC_A_r();
	//unsigned char ADC_A_A();	unsigned char ADC_A_B();	unsigned char ADC_A_C();	unsigned char ADC_A_D();	unsigned char ADC_A_E();	unsigned char ADC_A_H();	unsigned char ADC_A_L();
	unsigned char ADC_A_n();
	unsigned char ADC_A_$HL();

	//SUB A, s ( r,n, (HL) )
	unsigned char SUB_A_r();
	//unsigned char SUB_A_A();	unsigned char SUB_A_B();	unsigned char SUB_A_C();	unsigned char SUB_A_D();	unsigned char SUB_A_E();	unsigned char SUB_A_H();	unsigned char SUB_A_L();
	unsigned char SUB_A_n();
	unsigned char SUB_A_$HL();

	//SBC A, s ( r,n, (HL) )
	unsigned char SBC_A_r();
	//unsigned char SBC_A_A();	unsigned char SBC_A_B();	unsigned char SBC_A_C();	unsigned char SBC_A_D();	unsigned char SBC_A_E();	unsigned char SBC_A_H();	unsigned char SBC_A_L();
	unsigned char SBC_A_n();
	unsigned char SBC_A_$HL();

	//AND A, s ( r,n, (HL) )
	unsigned char AND_A_r();
	//unsigned char AND_A_A();	unsigned char AND_A_B();	unsigned char AND_A_C();	unsigned char AND_A_D();	unsigned char AND_A_E();	unsigned char AND_A_H();	unsigned char AND_A_L();
	unsigned char AND_A_n();
	unsigned char AND_A_$HL();

	//OR A, s ( r,n, (HL) )
	unsigned char OR_A_r();
	//unsigned char OR_A_A();	unsigned char OR_A_B();	unsigned char OR_A_C();	unsigned char OR_A_D();	unsigned char OR_A_E();	unsigned char OR_A_H();	unsigned char OR_A_L();
	unsigned char OR_A_n();
	unsigned char OR_A_$HL();

	//XOR A, s ( r,n, (HL) )
	unsigned char XOR_A_r();
	//unsigned char XOR_A_A();	unsigned char XOR_A_B();	unsigned char XOR_A_C();	unsigned char XOR_A_D();	unsigned char XOR_A_E();	unsigned char XOR_A_H();	unsigned char XOR_A_L();
	unsigned char XOR_A_n();
	unsigned char XOR_A_$HL();

	//CP A, s ( r,n, (HL) )
	unsigned char CP_A_r();
	//unsigned char CP_A_A();	unsigned char CP_A_B();	unsigned char CP_A_C();	unsigned char CP_A_D();	unsigned char CP_A_E();	unsigned char CP_A_H();	unsigned char CP_A_L();
	unsigned char CP_A_n();
	unsigned char CP_A_$HL();

	//INC r / (HL)
	unsigned char INC_r();
	//unsigned char INC_A();	unsigned char INC_B();	unsigned char INC_C();	unsigned char INC_D();	unsigned char INC_E();	unsigned char INC_H();	unsigned char INC_L();
	unsigned char INC_$HL();

	//DEC r / (HL)
	unsigned char DEC_r();
	//unsigned char DEC_A();	unsigned char DEC_B();	unsigned char DEC_C();	unsigned char DEC_D();	unsigned char DEC_E();	unsigned char DEC_H();	unsigned char DEC_L();
	unsigned char DEC_$HL();

	//ADD HL, ss
	unsigned char ADD_HL_ss();
	//unsigned char ADD_HL_BC();	unsigned char ADD_HL_DE();	unsigned char ADD_HL_HL();	unsigned char ADD_HL_SP();

	//ADD SP, e
	unsigned char ADD_SP_e();

	//INC ss
	unsigned char INC_ss();
	//unsigned char INC_BC();	unsigned char INC_DE();	unsigned char INC_HL();	unsigned char INC_SP();

	//DEC ss
	unsigned char DEC_ss();
	//unsigned char DEC_BC();	unsigned char DEC_DE();	unsigned char DEC_HL();	unsigned char DEC_SP();

	//RLCA
	unsigned char RLCA();

	//RLA
	unsigned char RLA();

	//RRCA
	unsigned char RRCA();

	//RRA
	unsigned char RRA();

	//JP nn
	unsigned char JP_nn();

	//JP cc, nn
	unsigned char JP_cc_nn();
	//unsigned char JP_NZ_nn();	unsigned char JP_Z_nn();	unsigned char JP_NC_nn();	unsigned char JP_C_nn();

	//JR e
	unsigned char JR_e();

	//JR cc, e
	unsigned char JR_cc_e();
	//unsigned char JP_NZ_e();	unsigned char JP_Z_e();	unsigned char JP_NC_e();	unsigned char JP_C_e();

	//JP HL
	unsigned char JP_HL();

	//CALL nn
	unsigned char CALL_nn();

	//CALL cc, nn
	unsigned char CALL_cc_nn();
	//unsigned char CALL_NZ_nn();	unsigned char CALL_Z_nn();	unsigned char CALL_NC_nn();	unsigned char CALL_C_nn();

	//RET
	unsigned char RET();

	//RETI
	unsigned char RETI();

	//RET cc
	unsigned char RET_cc();
	//unsigned char RET_NZ();	unsigned char RET_Z();	unsigned char RET_NC();	unsigned char RET_C();

	//RST t
	unsigned char RST_t();
	//unsigned char RST_0();	unsigned char RST_1();	unsigned char RST_2();	unsigned char RST_3(); 
	//unsigned char RST_4();	unsigned char RST_5();	unsigned char RST_6();	unsigned char RST_7();

	//DAA
	unsigned char DAA();

	//CPL
	unsigned char CPL();

	//NOP
	unsigned char NOP();

	//CCF
	unsigned char CCF();

	//SCF
	unsigned char SCF();

	//DI
	unsigned char DI(); 

	//EI
	unsigned char EI();

	//HALT
	unsigned char HALT();

	//STOP
	unsigned char STOP();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 0xBC PREFIX
	unsigned char PREFIX_0xBC();
	
	//RLC
	unsigned char RLC_r();
	// unsigned char RLC_A();	unsigned char RLC_B();	unsigned char RLC_C();	unsigned char RLC_D();	unsigned char RLC_E();	unsigned char RLC_H();	unsigned char RLC_L();
	unsigned char RLC_$HL(); 

	//RL
	unsigned char RL_r();
	// unsigned char RL_A();	unsigned char RL_B();	unsigned char RL_C();	unsigned char RL_D();	unsigned char RL_E();	unsigned char RL_H();	unsigned char RL_L();
	unsigned char RL_$HL();

	//RRC
	unsigned char RRC_r();
	// unsigned char RRC_A();	unsigned char RRC_B();	unsigned char RRC_C();	unsigned char RRC_D();	unsigned char RRC_E();	unsigned char RRC_H();	unsigned char RRC_L();
	unsigned char RRC_$HL();

	//RR
	unsigned char RR_r();
	// unsigned char RR_A();	unsigned char RR_B();	unsigned char RR_C();	unsigned char RR_D();	unsigned char RR_E();	unsigned char RR_H();	unsigned char RR_L();
	unsigned char RR_$HL();

	//SLA
	unsigned char SLA_r();
	// unsigned char SLA_A();	unsigned char SLA_B();	unsigned char SLA_C();	unsigned char SLA_D();	unsigned char SLA_E();	unsigned char SLA_H();	unsigned char SLA_L();
	unsigned char SLA_$HL();

	//SRA
	unsigned char SRA_r();
	// unsigned char SRA_A();	unsigned char SRA_B();	unsigned char SRA_C();	unsigned char SRA_D();	unsigned char SRA_E();	unsigned char SRA_H();	unsigned char SRA_L();
	unsigned char SRA_$HL();

	//SRL
	unsigned char SRL_r();
	// unsigned char SRL_A();	unsigned char SRL_B();	unsigned char SRL_C();	unsigned char SRL_D();	unsigned char SRL_E();	unsigned char SRL_H();	unsigned char SRL_L();
	unsigned char SRL_$HL();

	//SWAP
	unsigned char SWAP_r();
	// unsigned char SWAP_A();	unsigned char SWAP_B();	unsigned char SWAP_C();	unsigned char SWAP_D();	unsigned char SWAP_E();	unsigned char SWAP_H();	unsigned char SWAP_L();
	unsigned char SWAP_$HL();

	//BIT b, r
	unsigned char BIT_b_r();
	// unsigned char BIT_0_A();	unsigned char BIT_0_B();	unsigned char BIT_0_C();	unsigned char BIT_0_D();	unsigned char BIT_0_E();	unsigned char BIT_0_H();	unsigned char BIT_0_L();
	// unsigned char BIT_1_A();	unsigned char BIT_1_B();	unsigned char BIT_1_C();	unsigned char BIT_1_D();	unsigned char BIT_1_E();	unsigned char BIT_1_H();	unsigned char BIT_1_L();
	// unsigned char BIT_2_A();	unsigned char BIT_2_B();	unsigned char BIT_2_C();	unsigned char BIT_2_D();	unsigned char BIT_2_E();	unsigned char BIT_2_H();	unsigned char BIT_2_L();
	// unsigned char BIT_3_A();	unsigned char BIT_3_B();	unsigned char BIT_3_C();	unsigned char BIT_3_D();	unsigned char BIT_3_E();	unsigned char BIT_3_H();	unsigned char BIT_3_L();
	// unsigned char BIT_4_A();	unsigned char BIT_4_B();	unsigned char BIT_4_C();	unsigned char BIT_4_D();	unsigned char BIT_4_E();	unsigned char BIT_4_H();	unsigned char BIT_4_L();
	// unsigned char BIT_5_A();	unsigned char BIT_5_B();	unsigned char BIT_5_C();	unsigned char BIT_5_D();	unsigned char BIT_5_E();	unsigned char BIT_5_H();	unsigned char BIT_5_L();
	// unsigned char BIT_6_A();	unsigned char BIT_6_B();	unsigned char BIT_6_C();	unsigned char BIT_6_D();	unsigned char BIT_6_E();	unsigned char BIT_6_H();	unsigned char BIT_6_L();
	// unsigned char BIT_7_A();	unsigned char BIT_7_B();	unsigned char BIT_7_C();	unsigned char BIT_7_D();	unsigned char BIT_7_E();	unsigned char BIT_7_H();	unsigned char BIT_7_L();
	unsigned char BIT_b_$HL();

	//SET b, r
	unsigned char SET_b_r();
	// unsigned char SET_0_A();	unsigned char SET_0_B();	unsigned char SET_0_C();	unsigned char SET_0_D();	unsigned char SET_0_E();	unsigned char SET_0_H();	unsigned char SET_0_L();
	// unsigned char SET_1_A();	unsigned char SET_1_B();	unsigned char SET_1_C();	unsigned char SET_1_D();	unsigned char SET_1_E();	unsigned char SET_1_H();	unsigned char SET_1_L();
	// unsigned char SET_2_A();	unsigned char SET_2_B();	unsigned char SET_2_C();	unsigned char SET_2_D();	unsigned char SET_2_E();	unsigned char SET_2_H();	unsigned char SET_2_L();
	// unsigned char SET_3_A();	unsigned char SET_3_B();	unsigned char SET_3_C();	unsigned char SET_3_D();	unsigned char SET_3_E();	unsigned char SET_3_H();	unsigned char SET_3_L();
	// unsigned char SET_4_A();	unsigned char SET_4_B();	unsigned char SET_4_C();	unsigned char SET_4_D();	unsigned char SET_4_E();	unsigned char SET_4_H();	unsigned char SET_4_L();
	// unsigned char SET_5_A();	unsigned char SET_5_B();	unsigned char SET_5_C();	unsigned char SET_5_D();	unsigned char SET_5_E();	unsigned char SET_5_H();	unsigned char SET_5_L();
	// unsigned char SET_6_A();	unsigned char SET_6_B();	unsigned char SET_6_C();	unsigned char SET_6_D();	unsigned char SET_6_E();	unsigned char SET_6_H();	unsigned char SET_6_L();
	// unsigned char SET_7_A();	unsigned char SET_7_B();	unsigned char SET_7_C();	unsigned char SET_7_D();	unsigned char SET_7_E();	unsigned char SET_7_H();	unsigned char SET_7_L();
	unsigned char SET_b_$HL();

	//RES b, r
	unsigned char RES_b_r();
	// unsigned char RES_0_A();	unsigned char RES_0_B();	unsigned char RES_0_C();	unsigned char RES_0_D();	unsigned char RES_0_E();	unsigned char RES_0_H();	unsigned char RES_0_L();
	// unsigned char RES_1_A();	unsigned char RES_1_B();	unsigned char RES_1_C();	unsigned char RES_1_D();	unsigned char RES_1_E();	unsigned char RES_1_H();	unsigned char RES_1_L();
	// unsigned char RES_2_A();	unsigned char RES_2_B();	unsigned char RES_2_C();	unsigned char RES_2_D();	unsigned char RES_2_E();	unsigned char RES_2_H();	unsigned char RES_2_L();
	// unsigned char RES_3_A();	unsigned char RES_3_B();	unsigned char RES_3_C();	unsigned char RES_3_D();	unsigned char RES_3_E();	unsigned char RES_3_H();	unsigned char RES_3_L();
	// unsigned char RES_4_A();	unsigned char RES_4_B();	unsigned char RES_4_C();	unsigned char RES_4_D();	unsigned char RES_4_E();	unsigned char RES_4_H();	unsigned char RES_4_L();
	// unsigned char RES_5_A();	unsigned char RES_5_B();	unsigned char RES_5_C();	unsigned char RES_5_D();	unsigned char RES_5_E();	unsigned char RES_5_H();	unsigned char RES_5_L();
	// unsigned char RES_6_A();	unsigned char RES_6_B();	unsigned char RES_6_C();	unsigned char RES_6_D();	unsigned char RES_6_E();	unsigned char RES_6_H();	unsigned char RES_6_L();
	// unsigned char RES_7_A();	unsigned char RES_7_B();	unsigned char RES_7_C();	unsigned char RES_7_D();	unsigned char RES_7_E();	unsigned char RES_7_H();	unsigned char RES_7_L();
	unsigned char RES_b_$HL();



	//UNSUPPORTED
	unsigned char UNSUPPORTED();
};


#endif //JML_CPU