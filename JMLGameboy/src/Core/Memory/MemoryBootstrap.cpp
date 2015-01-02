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

#include "MemoryBootstrap.h"
#include "../../Tools/Console.h"

#include <stdexcept>



MemoryBootstrap::MemoryBootstrap()
{

}


MemoryBootstrap::~MemoryBootstrap()
{

}

bool MemoryBootstrap::Write(const WORD &address, const BYTE &value)
{
	if (address > BOOTSTRAP_END) return false;

	WriteLineC("Trying to write on Bootstrap ROM at address %d", address);
	throw std::runtime_error("Cannot write to Bootstrap ROM");

	return false;
}

bool MemoryBootstrap::Read(const WORD &address, BYTE &out)
{
	if (address > BOOTSTRAP_END) return false;

	out = memory[address];

	return true;
}