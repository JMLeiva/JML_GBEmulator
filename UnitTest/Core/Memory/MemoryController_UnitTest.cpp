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
#include "MemoryElementTest.h"
#include "Core\Memory\MemoryController.h"
#include "Metadata\Configuration.h"


#ifdef UNIT_TEST_ON

BOOST_AUTO_TEST_SUITE(MemoryController_Suite)

BOOST_AUTO_TEST_CASE(MemoryController_AppendMemoryElement)
{
	const int adds = 44;

	MemoryElementDummy memDummy[adds];

	for (int i = 0; i < adds; i++)
	{
		memDummy[i] = MemoryElementDummy();
		MemoryController::Shared()->AppendMemoryElement(&memDummy[i]);
	}

	BOOST_REQUIRE(MemoryController::Shared()->memoryElementsCount == adds);
}

BOOST_AUTO_TEST_SUITE_END()

#endif