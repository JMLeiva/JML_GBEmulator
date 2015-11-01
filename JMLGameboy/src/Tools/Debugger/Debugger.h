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

#ifndef JML_DEBUGGER
#define JML_DEBUGGER

#include "../../Core/CPU/CPU.h"
#include "../Disassembler/Disassembler.h"
#include "ViewModel\CPURegistersItemModel.h"
#include "ViewModel\DissasemblerStringListModel.h"
#include "ViewModel\InputRegistersItemModel.h"


class Debugger
{
public:
	Debugger();
	void Initialize(CPU *cpu, int argc, char** argv);
	~Debugger();

	CPURegistersItemModel* GetCPURegistersItemModel();
	DisassemblerStringListModel* GetDisassemberStringListModel();
	InputRegistersItemModel* GetInputRegistersItemModel();

	void Attach();
	void DeAttach();
	void StepInto();
	void Update();

private:
	CPU* cpu;
	int ownershipID;
	CPURegistersItemModel* cpuRegistersItemModel;
	DisassemblerStringListModel* disassemberStringListModel;
	InputRegistersItemModel* inputRegistersItemModel;

	Disassembler* disassembler;


	void Refresh();
};

#endif //JML_DEBUGGER