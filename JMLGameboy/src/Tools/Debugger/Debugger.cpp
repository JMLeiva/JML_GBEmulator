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

#include "Debugger.h"
#include <QtWidgets\qapplication.h>
#include "UI/DebuggerMainWindow.h"

Debugger::Debugger()
{

}

void Debugger::Initialize(CPU *cpu, int argc, char** argv)
{
	QApplication app(argc, argv);

	this->cpu = cpu;
	ownershipID = cpu->GetOwnershipId();
	disassembler = new Disassembler(MemoryController::Shared());
	
	cpuRegistersItemModel = new CPURegistersItemModel(cpu);
	disassemberStringListModel = new DisassemblerStringListModel(cpu, disassembler);
	inputRegistersItemModel = new InputRegistersItemModel(MemoryController::Shared());

	DebuggerMainWindow mainWindow(this);
	mainWindow.showMaximized();

	//QPushButton pushButton();

	app.exec();
}


void Debugger::Update()
{
	// Unused for now
}

void Debugger::Attach()
{
	ownershipID = cpu->GetOwnershipId();
}

void Debugger::DeAttach()
{
	cpu->ReleaseOwnership();
}

void Debugger::StepInto()
{
	cpu->RunCycle(ownershipID);
	Refresh();
}

CPURegistersItemModel* Debugger::GetCPURegistersItemModel()
{
	return cpuRegistersItemModel;
}

DisassemblerStringListModel* Debugger::GetDisassemberStringListModel()
{
	return disassemberStringListModel;
}

InputRegistersItemModel* Debugger::GetInputRegistersItemModel()
{
	return inputRegistersItemModel;
}

void Debugger::Refresh()
{
	cpuRegistersItemModel->Refresh();
	disassemberStringListModel->Refresh();
	inputRegistersItemModel->Refresh();
}

Debugger::~Debugger()
{
	delete cpuRegistersItemModel;
	delete disassemberStringListModel;
}
