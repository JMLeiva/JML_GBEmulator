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

#include "WorkingRAM.h"
#include "../../Tools/Console.h"

#define WORKING_RAM_START_ADDRESS 0xC000
#define WORKING_RAM_END_ADDRESS 0xE000

#define WORKING_ECHO_RAM_START_ADDRESS 0xC000
#define WORKING_ECHO_RAM_END_ADDRESS 0xFE00

WorkingRam::WorkingRam()
{

}

WorkingRam::~WorkingRam()
{

}

bool WorkingRam::Write(const WORD &address, const BYTE &value)
{
	if(address >= WORKING_RAM_START_ADDRESS && address < WORKING_RAM_END_ADDRESS)
	{
		ram[address - WORKING_RAM_START_ADDRESS] = value;
		return true;
	}

	//Echo RAM
	/*
	if(address >= WORKING_ECHO_RAM_START_ADDRESS && address < WORKING_ECHO_RAM_END_ADDRESS)
	{
		ram[address - WORKING_ECHO_RAM_START_ADDRESS] = value;
		return true;
	}*/

	return false;
}

bool WorkingRam::Read(const WORD &address, BYTE &out)
{
	if(address >= WORKING_RAM_START_ADDRESS && address < WORKING_RAM_END_ADDRESS)
	{
		out = ram[address - WORKING_RAM_START_ADDRESS];
		return true;
	}

	//Echo RAM
	/*
	if(address >= WORKING_ECHO_RAM_START_ADDRESS && address < WORKING_ECHO_RAM_END_ADDRESS)
	{
		out = ram[address - WORKING_ECHO_RAM_START_ADDRESS];
		return true;
	}
	*/

	return false;
}