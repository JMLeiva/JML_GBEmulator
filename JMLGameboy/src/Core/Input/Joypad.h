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
#ifndef JML_JOYPAD
#define JML_JOYPAD

#include "../Memory/MemoryElement.h"
#include "../../Metadata/Configuration.h"

#ifndef UNIT_TEST_ON
#include <SFML\Window\Event.hpp>
#endif

enum P_MASK
{
	P_MASK_10 = 0x01,
	P_MASK_11 = 0x02,
	P_MASK_12 = 0x04,
	P_MASK_13 = 0x08,
	P_MASK_14 = 0x10,
	P_MASK_15 = 0x20,
};

enum ColumnEnum
{
	LEFT_COLUMN = 0x10,
	RIGHT_COLUMN = 0x20,
};

class Joypad : public MemoryElement
{
public:
	Joypad();
	~Joypad();

	bool Write(const WORD &address, const BYTE &value);
	bool Read(const WORD &address, BYTE &out);

	void HandleEvent(sf::Event event);

private:
	BYTE leftInput;
	BYTE rightInput;

	BYTE column;
	
};

#endif // JML_JOYPAD