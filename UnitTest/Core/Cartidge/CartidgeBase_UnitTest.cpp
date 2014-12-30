#include "../../stdafx.h"
#include "Metadata\Configuration.h"
#include "Core\Cartidge\Cartidge.h"
#include "Core\Cartidge\CartidgeBuilder.h"
#include "Core\Memory\MemoryController.h"

#ifdef UNIT_TEST_ON


BOOST_AUTO_TEST_SUITE(CARTIDGE_BASE_Suite)


BOOST_AUTO_TEST_CASE(CartidgeBuilderTest)
{
	char* PATH = "C:/Users/Juan/Documents/JMLGameboy/JMLGameboy/ROMs/game.gb";
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