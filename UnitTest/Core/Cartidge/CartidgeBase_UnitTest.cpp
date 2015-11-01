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
#include "Metadata\Configuration.h"
#include "Core\Cartidge\Cartidge.h"
#include "Core\Cartidge\CartidgeBuilder.h"
#include "Core\Memory\MemoryController.h"

#ifdef UNIT_TEST_ON


BOOST_AUTO_TEST_SUITE(CARTIDGE_BASE_Suite)


BOOST_AUTO_TEST_CASE(CartidgeBuilderTest)
{
	char* PATH = "C:/Users/Juan/Documents/C++/JML_GBEmulator/ROMs/game.gb";
	Cartidge* cartidge = CartidgeBuilder().Build(PATH);

	BOOST_CHECK(cartidge != NULL);

	MemoryController::Shared()->AppendMemoryElement(cartidge);

	BYTE out;

	out = MemoryController::Shared()->ReadMemory(0x0150);
	BOOST_CHECK(out == 0x76);

	out = MemoryController::Shared()->ReadMemory(0x0151);
	BOOST_CHECK(out == 0x00);

	out = MemoryController::Shared()->ReadMemory(0x0152);
	BOOST_CHECK(out == 0x18);

	out = MemoryController::Shared()->ReadMemory(0x0153);
	BOOST_CHECK(out == 0xfc);

	out = MemoryController::Shared()->ReadMemory(0x0154);
	BOOST_CHECK(out == 0xd9);
	
	MemoryController::Shared()->Clear();
}

BOOST_AUTO_TEST_SUITE_END()

#endif