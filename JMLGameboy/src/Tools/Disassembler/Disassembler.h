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

#ifndef JML_D_DISASSEMBLER
#define JML_D_DISASSEMBLER

#include "../../Metadata/DataTypes.h"
#include "../../Core/Memory/MemoryController.h"
#include <vector>

struct DisassemberInstruction
{
	const char* instructionBase;
	bool hasParameter;
	WORD parameter;
	BYTE size;
};

struct EntryPoint
{
	WORD relativePosition;
	WORD realAddress;
};

class Disassembler
{
public:
	Disassembler(MemoryController* memoryController);
	DisassemberInstruction DisassembleAtRelative(WORD relativePosition);
	int GetCodeSize();

private:
	int codeSize;
	std::vector<EntryPoint*> entryPoints;
	MemoryController* memoryController;

	void InitialParse();
	bool IsAJump();
	WORD RelativeToRealPosition(WORD relativePosition);
	DisassemberInstruction DisassembleAtReal(WORD position);

	bool jumpIntructions[256];

	//INSTRUCTIONS TABLES
	typedef void (Disassembler::*InstructionParameter)(WORD position, DisassemberInstruction &instruction);

	const char* instructions[256];
	const char*  cbInstructions[256];

	Disassembler::InstructionParameter instructionsParameter[256];

	void LoadInstructions();
	void Get16(WORD position, DisassemberInstruction &instruction);
	void GetU8(WORD position, DisassemberInstruction &instruction);
	void GetS8(WORD position, DisassemberInstruction &instruction);
	void GetS8_FF00(WORD position, DisassemberInstruction &instruction);
	void GetDisplacementS8(WORD position, DisassemberInstruction &instruction);

};

#endif // !JML_D_DISASSEMBLER
