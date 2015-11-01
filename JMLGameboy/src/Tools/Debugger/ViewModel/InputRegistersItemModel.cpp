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

//#include "../../../Core/Memory/MemoryController.h"
#include "InputRegistersItemModel.h"
#include "../Globals.h"
#include "../DebuggerTools.h"

#define REG_COUNT 1

InputRegistersItemModel::InputRegistersItemModel(MemoryController* memoryController) : QStandardItemModel(REG_COUNT, 8)
{
	this->memoryController = memoryController;

	setHorizontalHeaderItem(0, new QStandardItem("-"));
	setHorizontalHeaderItem(1, new QStandardItem("-"));
	setHorizontalHeaderItem(2, new QStandardItem("ULDR"));
	setHorizontalHeaderItem(3, new QStandardItem("ABSS"));
	setHorizontalHeaderItem(4, new QStandardItem("D/START"));
	setHorizontalHeaderItem(5, new QStandardItem("U/SELECT"));
	setHorizontalHeaderItem(6, new QStandardItem("L/B"));
	setHorizontalHeaderItem(7, new QStandardItem("R/A"));

	setVerticalHeaderItem(0, new QStandardItem(IREG_NAME_P1));

	for(int i = 0; i < 8; i++)
	{
		setItem(0, i, new QStandardItem("0"));
	}
}

void InputRegistersItemModel::Refresh()
{
	BYTE p1 = memoryController->ReadMemory(0xFF00);

	for(int i = 7; i >= 0; i--)
	{
		BYTE bit = p1 & 0x0001;
		item(0, i)->setText(getRegFormattedQString(bit, 1, 2));
		bit >>= 1;
	}
}