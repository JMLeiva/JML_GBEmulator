#include "WorkingRAM.h"

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