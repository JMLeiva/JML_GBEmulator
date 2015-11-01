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

#include "CartidgeMBC1.h"
#include "../../Tools/Console.h"
#include "../../Metadata/Configuration.h"

#define RAMCS_GATE_START_ADDRESS	0x0000
#define RAMCS_GATE_END_ADDRESS		0x2000

#define ROM_BANK_SELECTION_START_ADDRESS	0x2000
#define ROM_BANK_SELECTION_END_ADDRESS		0x4000

//TODO
//#define 

CartidgeMBC1::CartidgeMBC1() : Cartidge()
{
	
}

CartidgeMBC1::~CartidgeMBC1()
{
	
}

bool CartidgeMBC1::Write(const WORD &address, const BYTE &value)
{
	if(address >= 0x8000)
	{
		return false;
	}

	if(address >= ROM_BANK_SELECTION_START_ADDRESS && address < ROM_BANK_SELECTION_END_ADDRESS)
	{
		currentBank = value;
		//WriteLineE("Current Bank: %d", currentBank);
		return true;
	}

	return false;
}