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

#include "../../stdafx.h"
#include "..\Memory\MemoryElementTest.h"
#include "Core\CPU\CPU.h"
#include "Metadata\Configuration.h"

#ifdef UNIT_TEST_ON


BOOST_AUTO_TEST_SUITE(CPU_FLAGS_Suite)


BOOST_AUTO_TEST_CASE(CPUFlags)
{
	MemoryElementDummy memoryDummy;
	MemoryController::Shared()->AppendMemoryElement(&memoryDummy);

	CPU cpu;

	cpu.SetZ();
	BOOST_CHECK(((cpu.AF.l >> 7) & 0x01) == 0x01);
	cpu.ResetZ();
	BOOST_CHECK(((cpu.AF.l >> 7) & 0x01) == 0x00);

	cpu.SetN();
	BOOST_CHECK(((cpu.AF.l >> 6) & 0x01) == 0x01);
	cpu.ResetN();
	BOOST_CHECK(((cpu.AF.l >> 6) & 0x01) == 0x00);

	cpu.SetH();
	BOOST_CHECK(((cpu.AF.l >> 5) & 0x01) == 0x01);
	cpu.ResetH();
	BOOST_CHECK(((cpu.AF.l >> 5) & 0x01) == 0x00);

	cpu.SetCY();
	BOOST_CHECK(((cpu.AF.l >> 4) & 0x01) == 0x01);
	cpu.ResetCY();
	BOOST_CHECK(((cpu.AF.l >> 4) & 0x01) == 0x00);

	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_SUITE_END()

#endif