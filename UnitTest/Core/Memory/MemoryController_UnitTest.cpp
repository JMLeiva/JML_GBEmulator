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