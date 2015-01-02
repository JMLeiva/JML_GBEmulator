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