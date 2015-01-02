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

#include "Cartidge.h"
#include "../../Tools/Console.h"
#include "../../Metadata/Configuration.h"


#define STARTING_CARTIDGE_ADDRESS	0x0000
#define ENDING_CARTIDGE_ADDRESS		0x7FFF

Cartidge::Cartidge()
{
	currentBank = 0;
}

Cartidge::~Cartidge()
{
	fclose(file);
}

bool Cartidge::Write(const WORD &address, const BYTE &value)
{
	if(address < STARTING_CARTIDGE_ADDRESS)
	{
		return false;
	}

	if(address > ENDING_CARTIDGE_ADDRESS)
	{
		return false;
	}

	return false;
}

bool Cartidge::Read(const WORD &address, BYTE &out)
{
	if(address < STARTING_CARTIDGE_ADDRESS)
	{
		return false;
	}

	if(address > ENDING_CARTIDGE_ADDRESS)
	{
		return false;
	}

	if(address < ROM_SIZE)
	{
		out = romBanks[0][address];
	}
	else
	{
		out = romBanks[currentBank + 1][address];
	}

	return true;
}


void Cartidge::Initialize()
{
	//Load Initial Banks

	//Bank 0
	fread(romBanks[0], sizeof(BYTE), ROM_SIZE, file);
	BYTE* bTest = romBanks[0];

	WriteLineE("TEST");
	//Bank 1 (First Mapped??)
	// TODO

	//RAM ??
}