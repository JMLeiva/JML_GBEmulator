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

#ifndef JML_D_DISASSEMBLERINSTRUCTION
#define JML_D_DISASSEMBLERINSTRUCTION

#include "../../Metadata/DataTypes.h"
#include <vector>

struct DisassemblerInstruction
{
	BYTE size;
	WORD address;
	BYTE bytes[3];
	char disass[32];
	std::vector<WORD> referencesAddresses;
};

#endif //JML_D_DISASSEMBLERINSTRUCTION