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

#include "DebuggerMainWindow.h"
#include "../ViewModel/CPURegistersItemModel.h"
#include <QtUiTools\QtUiTools>

//#include "DebuggerTools.h"

const char* ATTACH_STR		= "Attach";
const char* DEATTACH_STR	= "Deatach";
const char* PC_LABEL		= "PC: ";
const char* I_LABEL			= "I: ";
const char* V_LABEL_PREFIX	= "V";



DebuggerMainWindow::DebuggerMainWindow(Debugger *debugger) : QMainWindow()
{
	//CPU
	this->debugger = debugger;
	//SetupLayout
	
	SetupLayout();
}

void DebuggerMainWindow::SetupLayout()
{
	// Initialize Window
	QUiLoader loader;

	QFile file("Resources/DebuggerMainWindow.ui");
	file.open(QFile::ReadOnly);

	QWidget *formWidget = loader.load(&file, this);
	file.close();

	this->setFixedSize(formWidget->size());

	// Setup Controls
	QTableView* registersTableView = formWidget->findChild<QTableView*>("registersTable");
	registersTableView->setModel(debugger->GetCPURegistersItemModel());
	registersTableView->setColumnWidth(2, 120);

	QTableView* inputRegistersTableview = formWidget->findChild<QTableView*>("inputRegistersTable");
	inputRegistersTableview->setModel(debugger->GetInputRegistersItemModel());
	inputRegistersTableview->setColumnWidth(0, 12);
	inputRegistersTableview->setColumnWidth(1, 12);
	inputRegistersTableview->setColumnWidth(2, 48);
	inputRegistersTableview->setColumnWidth(3, 48);
	inputRegistersTableview->setColumnWidth(4, 56);
	inputRegistersTableview->setColumnWidth(5, 56);
	inputRegistersTableview->setColumnWidth(6, 32);
	inputRegistersTableview->setColumnWidth(7, 32);


	QListView* dissamblerListView = formWidget->findChild<QListView*>("dissamblerListView");
	dissamblerListView->setModel(debugger->GetDisassemberStringListModel());
	dissamblerListView->setSelectionModel(debugger->GetDisassemberStringListModel());

	QPushButton* stepIntoButton = formWidget->findChild<QPushButton*>("stepIntoButton");
	connect(stepIntoButton, SIGNAL(released()), this, SLOT(OnStepIntoPressed()));

	attachPressed = true;
	QPushButton* attachButton = formWidget->findChild<QPushButton*>("attachButton");
	connect(attachButton, SIGNAL(released()), this, SLOT(OnAttachDeatach()));
}

void DebuggerMainWindow::OnStepIntoPressed()
{
	debugger->StepInto();
}
	/*
	nextCycleButton = formWidget->findChild<QPushButton*>("nextCycleButton");
	connect(nextCycleButton, SIGNAL(released()), this, SLOT(OnNextCycle()));

	attachDeatachButton = formWidget->findChild<QPushButton*>("attachButton");
	connect(attachDeatachButton, SIGNAL(released()), this, SLOT(OnAttachDeatach()));

	memoryListView = formWidget->findChild<QListView*>("memoryListView");
	memoryModel = new QStandardItemModel(this);
	memoryListView->setModel(memoryModel);
	attachProgressBar = formWidget->findChild<QProgressBar*>("attachProgressBar");
	attachProgressBar->setVisible(false);

	gfxMemoryListView = formWidget->findChild<QListView*>("gfxMemoryListView");
	gfxMemoryModel = new QStandardItemModel(this);
	gfxMemoryListView->setModel(gfxMemoryModel);

	pcLabel = formWidget->findChild<QLabel*>("pcLabel");
	iLabel = formWidget->findChild<QLabel*>("iLabel");

	goToPcButton = formWidget->findChild<QPushButton*>("goToPcButton");
	connect(goToPcButton, SIGNAL(released()), this, SLOT(OnGoToPc()));

	dumpGfxMemoryButton = formWidget->findChild<QPushButton*>("dumpGfxMemoryButton");
	connect(dumpGfxMemoryButton, SIGNAL(released()), this, SLOT(FillGfxMemoryList()));

	vLabels = new QLabel*[16];

	for(int i = 0; i < 16; i++)
	{
		QString vName = QString("v%1Label").arg(i);
		vLabels[i] = formWidget->findChild<QLabel*>(vName);
	}
	*/

	//FillMemoryList();
	//UpdateLayout();



void DebuggerMainWindow::OnAttachDeatach()
{
	attachPressed = !attachPressed;

	if(attachPressed)
	{
		debugger->Attach();
		//attachProgressBar->setVisible(true);
	}
	else
	{
		debugger->DeAttach();
	}
}