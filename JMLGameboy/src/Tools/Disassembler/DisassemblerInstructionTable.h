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

#ifndef JML_D_DISASSEMBLERINSTRUCTIONTABLE
#define JML_D_DISASSEMBLERINSTRUCTIONTABLE

#include "../../Metadata/DataTypes.h"
#include "../../Core/Memory/MemoryController.h"
#include "DisassemblerInstruction.h"

extern const char* INVALID_INSTRUCTION;
extern const char* INVALID_REG;
extern const char* INVALID_FLAG;

class DisassemblerInstructionTable
{
public:
	DisassemblerInstructionTable();
	DisassemblerInstruction Disassemble(MemoryController* memoryController, WORD address);


private:

	typedef DisassemblerInstruction(DisassemblerInstructionTable::*Instruction)(MemoryController* memoryController, WORD address, BYTE opcode);

	DisassemblerInstructionTable::Instruction instructions[256];
	DisassemblerInstructionTable::Instruction cbInstructions[256];

	const char* Get_r(BYTE code);
	const char* Get_qq(BYTE code);
	const char* Get_dd(BYTE code);
	const char* GetFlag_cc(BYTE code);

	//LD R, R'
	DisassemblerInstruction LD_R_R(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//LD R, n
	DisassemblerInstruction LD_R_n(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//LD R, (HL)
	DisassemblerInstruction LD_R_$HL(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//LD (HL), R
	DisassemblerInstruction LD_$HL_R(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//LD (HL), n
	DisassemblerInstruction LD_$HL_n(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD A, (BC)
	DisassemblerInstruction LD_A_$BC(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD A, (DE)
	DisassemblerInstruction LD_A_$DE(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD A, (C)
	DisassemblerInstruction LD_A_$C(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD (C), A
	DisassemblerInstruction LD_$C_A(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD A, (n)
	DisassemblerInstruction LD_A_$n(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD (n), A
	DisassemblerInstruction LD_$n_A(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD A, (nn)
	DisassemblerInstruction LD_A_$nn(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD (nn), A
	DisassemblerInstruction LD_$nn_A(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD A, (HLI)
	DisassemblerInstruction LD_A_$HLI(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD A, (HLD)
	DisassemblerInstruction LD_A_$HLD(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD (BC), A
	DisassemblerInstruction LD_$BC_A(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD (DE), A
	DisassemblerInstruction LD_$DE_A(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD (HLI), A
	DisassemblerInstruction LD_$HLI_A(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD (HLD), A
	DisassemblerInstruction LD_$HLD_A(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD dd, nn
	DisassemblerInstruction LD_dd_nn(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//LD SP, HL
	DisassemblerInstruction LD_SP_HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//PUSH qq
	DisassemblerInstruction PUSH_qq(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//POP qq
	DisassemblerInstruction POP_qq(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//LDHL SP, e
	DisassemblerInstruction LDHL_PS_e(MemoryController* memoryController, WORD address, BYTE opcode);

	//LD (nn), SP
	DisassemblerInstruction LD_$nn_SP(MemoryController* memoryController, WORD address, BYTE opcode);

	//ADD A, s ( r,n, (HL) )
	DisassemblerInstruction ADD_A_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction ADD_A_n(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction ADD_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//ADC A, s ( r,n, (HL) )
	DisassemblerInstruction ADC_A_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction ADC_A_n(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction ADC_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//SUB A, s ( r,n, (HL) )
	DisassemblerInstruction SUB_A_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction SUB_A_n(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction SUB_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//SBC A, s ( r,n, (HL) )
	DisassemblerInstruction SBC_A_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction SBC_A_n(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction SBC_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//AND A, s ( r,n, (HL) )
	DisassemblerInstruction AND_A_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction AND_A_n(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction AND_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//OR A, s ( r,n, (HL) )
	DisassemblerInstruction OR_A_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction OR_A_n(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction OR_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//XOR A, s ( r,n, (HL) )
	DisassemblerInstruction XOR_A_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction XOR_A_n(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction XOR_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//CP A, s ( r,n, (HL) )
	DisassemblerInstruction CP_A_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction CP_A_n(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction CP_A_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//INC r / (HL)
	DisassemblerInstruction INC_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction INC_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//DEC r / (HL)
	DisassemblerInstruction DEC_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction DEC_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//ADD HL, ss
	DisassemblerInstruction ADD_HL_ss(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//ADD SP, e
	DisassemblerInstruction ADD_SP_e(MemoryController* memoryController, WORD address, BYTE opcode);

	//INC ss
	DisassemblerInstruction INC_ss(MemoryController* memoryController, WORD address, BYTE opcode);

	//DEC ss
	DisassemblerInstruction DEC_ss(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//RLCA
	DisassemblerInstruction RLCA(MemoryController* memoryController, WORD address, BYTE opcode);

	//RLA
	DisassemblerInstruction RLA(MemoryController* memoryController, WORD address, BYTE opcode);

	//RRCA
	DisassemblerInstruction RRCA(MemoryController* memoryController, WORD address, BYTE opcode);

	//RRA
	DisassemblerInstruction RRA(MemoryController* memoryController, WORD address, BYTE opcode);

	//JP nn
	DisassemblerInstruction JP_nn(MemoryController* memoryController, WORD address, BYTE opcode);

	//JP cc, nn
	DisassemblerInstruction JP_cc_nn(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//JR e
	DisassemblerInstruction JR_e(MemoryController* memoryController, WORD address, BYTE opcode);

	//JR cc, e
	DisassemblerInstruction JR_cc_e(MemoryController* memoryController, WORD address, BYTE opcode);

	//JP HL
	DisassemblerInstruction JP_HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//CALL nn
	DisassemblerInstruction CALL_nn(MemoryController* memoryController, WORD address, BYTE opcode);

	//CALL cc, nn
	DisassemblerInstruction CALL_cc_nn(MemoryController* memoryController, WORD address, BYTE opcode);

	//RET
	DisassemblerInstruction RET(MemoryController* memoryController, WORD address, BYTE opcode);

	//RETI
	DisassemblerInstruction RETI(MemoryController* memoryController, WORD address, BYTE opcode);

	//RET cc
	DisassemblerInstruction RET_cc(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//RST t
	DisassemblerInstruction RST_t(MemoryController* memoryController, WORD address, BYTE opcode);
	
	//DAA
	DisassemblerInstruction DAA(MemoryController* memoryController, WORD address, BYTE opcode);

	//CPL
	DisassemblerInstruction CPL(MemoryController* memoryController, WORD address, BYTE opcode);

	//NOP
	DisassemblerInstruction NOP(MemoryController* memoryController, WORD address, BYTE opcode);

	//CCF
	DisassemblerInstruction CCF(MemoryController* memoryController, WORD address, BYTE opcode);

	//SCF
	DisassemblerInstruction SCF(MemoryController* memoryController, WORD address, BYTE opcode);

	//DI
	DisassemblerInstruction DI(MemoryController* memoryController, WORD address, BYTE opcode);

	//EI
	DisassemblerInstruction EI(MemoryController* memoryController, WORD address, BYTE opcode);

	//HALT
	DisassemblerInstruction HALT(MemoryController* memoryController, WORD address, BYTE opcode);

	//STOP
	DisassemblerInstruction STOP(MemoryController* memoryController, WORD address, BYTE opcode);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 0xBC PREFIX
	DisassemblerInstruction PREFIX_0xBC(MemoryController* memoryController, WORD address, BYTE opcode);

	//RLC
	DisassemblerInstruction RLC_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction RLC_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//RL
	DisassemblerInstruction RL_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction RL_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//RRC
	DisassemblerInstruction RRC_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction RRC_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//RR
	DisassemblerInstruction RR_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction RR_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//SLA
	DisassemblerInstruction SLA_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction SLA_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//SRA
	DisassemblerInstruction SRA_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction SRA_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//SRL
	DisassemblerInstruction SRL_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction SRL_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//SWAP
	DisassemblerInstruction SWAP_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction SWAP_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//BIT b, r
	DisassemblerInstruction BIT_b_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction BIT_b_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//SET b, r
	DisassemblerInstruction SET_b_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction SET_b_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//RES b, r
	DisassemblerInstruction RES_b_r(MemoryController* memoryController, WORD address, BYTE opcode);
	DisassemblerInstruction RES_b_$HL(MemoryController* memoryController, WORD address, BYTE opcode);

	//UNSUPPORTED
	DisassemblerInstruction UNSUPPORTED(MemoryController* memoryController, WORD address, BYTE opcode);

	void LoadInstructions();
};
#endif // !JML_D_DISASSEMBLERINSTRUCTIONTABLE
