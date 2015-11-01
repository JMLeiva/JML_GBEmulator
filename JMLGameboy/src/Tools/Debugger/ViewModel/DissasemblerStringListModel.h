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

#ifndef JML_D_DISASSEMBLERSTRINGLSITMODEL
#define JML_D_DISASSEMBLERSTRINGLSITMODEL

#include <QtWidgets\QtWidgets>
#include <QtCore\qvariant.h>
#include <QtCore\qabstractitemmodel.h>
#include "../../../Core/CPU/CPU.h"
#include "../../Disassembler/Disassembler.h"

class DisassemblerStringListModel : public QAbstractListModel, public QItemSelectionModel
{
public:
	DisassemblerStringListModel(CPU* cpu, Disassembler* disassembler);// , QObject * parent);
	void Refresh();
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;

	QModelIndex currentIndex() const;

private:
	CPU* cpu;
	Disassembler* disassembler;
	QVariant GetVariantForPosition(WORD position) const;
};

#endif // !JML_D_DISASSEMBLERSTRINGLSITMODEL
