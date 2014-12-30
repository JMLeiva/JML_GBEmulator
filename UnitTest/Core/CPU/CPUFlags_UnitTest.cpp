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