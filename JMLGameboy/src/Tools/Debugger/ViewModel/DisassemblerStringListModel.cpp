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

#include "DissasemblerStringListModel.h"
#include "../../Console.h"
#include "../../../Metadata/DataTypes.h"

DisassemblerStringListModel::DisassemblerStringListModel(CPU* cpu, Disassembler* disassembler) : QItemSelectionModel(this)
{
	this->cpu = cpu;
	this->disassembler = disassembler;
}

int DisassemblerStringListModel::rowCount(const QModelIndex & parent) const
{
	return disassembler->GetCodeSize();
}

QModelIndex DisassemblerStringListModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column);
}

QVariant DisassemblerStringListModel::data(const QModelIndex & index, int role) const
{
	if(!index.isValid())
		return QVariant();


	if(role == Qt::DisplayRole)
		return GetVariantForPosition((WORD)index.row());
	else
		return QVariant();
}

QVariant DisassemblerStringListModel::GetVariantForPosition(WORD position) const
{
	DisassemberInstruction instruction = disassembler->DisassembleAtRelative(position);

	QString resultQstring;

	if(!instruction.hasParameter)
	{
		resultQstring = QString("%1%2			%3").arg("0x").arg(position, 4, 16, QLatin1Char('0')).arg(instruction.instructionBase);
	}
	else
	{
		int width = 4;// instruction.paramterSize * 2;



		QString parameterStr = QString("%1%2").arg("0x").arg(instruction.parameter, width, 16, QLatin1Char('0'));
		QString instrString = QString(instruction.instructionBase).arg(parameterStr);
		resultQstring = QString("%1%2			%3").arg("0x").arg(position, 4, 16, QLatin1Char('0')).arg(instrString);
	}
	return QVariant(resultQstring);
}

Qt::ItemFlags DisassemblerStringListModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QModelIndex DisassemblerStringListModel::currentIndex() const
{
	return createIndex(cpu->PC, 0);
}

void DisassemblerStringListModel::Refresh()
{
	QModelIndex index = createIndex(cpu->PC, 0);

	select(index, Current | Select);
	emit currentChanged(index, index);
}