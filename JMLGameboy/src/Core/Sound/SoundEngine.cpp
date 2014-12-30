#include "SoundEngine.h"

#define SOUND_START_ADDRESS 0xFF10
#define SOUND_END_ADDRESS	0xFF40

SoundEngine::SoundEngine()
{
}


SoundEngine::~SoundEngine()
{

}
		
bool SoundEngine::Write(const WORD &address, const BYTE &value)
{
	if(address >= SOUND_START_ADDRESS && address < SOUND_END_ADDRESS)
	{
		return true;
	}
	
	return false;
}

bool SoundEngine::Read(const WORD &address, BYTE &out)
{
	if(address >= SOUND_START_ADDRESS && address < SOUND_END_ADDRESS)
	{
		out = 0x0000;
		return true;
	}

	return false;
}