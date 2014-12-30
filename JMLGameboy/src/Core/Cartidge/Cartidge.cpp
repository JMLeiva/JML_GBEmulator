#include "Cartidge.h"
#include "../../Tools/Console.h"
#include "../../Metadata/Configuration.h"


#define STARTING_CARTIDGE_ADDRESS	0x0000
#define ENDING_CARTIDGE_ADDRESS		0x7FFF

Cartidge::Cartidge()
{
	currentBank = 0;
}

Cartidge::~Cartidge()
{
	fclose(file);
}

bool Cartidge::Write(const WORD &address, const BYTE &value)
{
	if(address < STARTING_CARTIDGE_ADDRESS)
	{
		return false;
	}

	if(address > ENDING_CARTIDGE_ADDRESS)
	{
		return false;
	}

	return false;
}

bool Cartidge::Read(const WORD &address, BYTE &out)
{
	if(address < STARTING_CARTIDGE_ADDRESS)
	{
		return false;
	}

	if(address > ENDING_CARTIDGE_ADDRESS)
	{
		return false;
	}

	if(address < ROM_SIZE)
	{
		out = romBanks[0][address];
	}
	else
	{
		out = romBanks[currentBank + 1][address];
	}

	return true;
}


void Cartidge::Initialize()
{
	//Load Initial Banks

	//Bank 0
	fread(romBanks[0], sizeof(BYTE), ROM_SIZE, file);
	BYTE* bTest = romBanks[0];

	WriteLineE("TEST");
	//Bank 1 (First Mapped??)
	// TODO

	//RAM ??
}