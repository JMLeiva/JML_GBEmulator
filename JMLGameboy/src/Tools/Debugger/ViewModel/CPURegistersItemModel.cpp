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

#include "CPURegistersItemModel.h"
#include "../Globals.h"
#include "../DebuggerTools.h"

#define REG_COUNT 9

//CPURegistersItemModel::CPURegistersItemModel(CPU* cpu, QObject * parent) : QStandardItemModel(5, 2, parent)
CPURegistersItemModel::CPURegistersItemModel(CPU* cpu) : QStandardItemModel(REG_COUNT, 3)
{
	this->cpu = cpu;

	setHorizontalHeaderItem(0, new QStandardItem("Hex"));
	setHorizontalHeaderItem(1, new QStandardItem("Dec"));
	setHorizontalHeaderItem(2, new QStandardItem("Bin"));

	setVerticalHeaderItem(0, new QStandardItem(REG_NAME_PC));
	setVerticalHeaderItem(1, new QStandardItem(REG_NAME_SP));
	setVerticalHeaderItem(2, new QStandardItem(REG_NAME_AF));
	setVerticalHeaderItem(3, new QStandardItem(REG_NAME_BC));
	setVerticalHeaderItem(4, new QStandardItem(REG_NAME_DE));
	setVerticalHeaderItem(5, new QStandardItem(REG_NAME_HL));
	setVerticalHeaderItem(6, new QStandardItem(REG_NAME_IF));
	setVerticalHeaderItem(7, new QStandardItem(REG_NAME_IE));
	setVerticalHeaderItem(8, new QStandardItem(REG_NAME_IME));

	for(int i = 0; i < REG_COUNT; i++)
	{
		setItem(i, 0, new QStandardItem("0x0000"));
		setItem(i, 1, new QStandardItem("00000"));
		setItem(i, 2, new QStandardItem("00000000"));
	}
}

void CPURegistersItemModel::Refresh()
{
	short registers[] = {cpu->PC, cpu->SP.w, cpu->AF.w, cpu->BC.w, cpu->DE.w, cpu->HL.w, cpu->IF, cpu->IE, cpu->IME};
	char sizes[] = {16, 16, 16, 16, 16, 16, 8, 8, 1};

	for(int i = 0; i < REG_COUNT; i++)
	{
		item(i, 0)->setText(getRegFormattedQString(registers[i], sizes[i]/4, 16, "0x"));
		item(i, 1)->setText(getRegFormattedQString(registers[i], sizes[i]/3, 10));
		item(i, 2)->setText(getRegFormattedQString(registers[i], sizes[i], 2));
	}
}