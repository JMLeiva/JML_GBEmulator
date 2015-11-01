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

#include "Joypad.h"
#include "../../Tools/Console.h"

//#ifndef UNIT_TEST_ON
#include <SFML\Window\Keyboard.hpp>
//#endif



#define P1_ADDRESS 0xFF00

Joypad::Joypad()
{
	leftInput = 0x0F;
	rightInput = 0x0F;

	column = LEFT_COLUMN;
}

Joypad::~Joypad()
{

}

bool Joypad::Write(const WORD &address, const BYTE &value)
{
	if(address == P1_ADDRESS)
	{
		column = value & 0x30;

#ifdef DEBUGGER_ON
		if(value & 0xCF != 0)
		{
			WriteLineW("WARNING: Writing to P1 outside output bits");
		}

		if(column == 0xC0)
		{
			WriteLineW("WARNING: Writing to P1, enabling both columns at the same time");
		}

		if(column == 0x00)
		{
			WriteLineW("WARNING: Writing to P1, disabling both columns at the same time");
		}
#endif

		return true;
	}

	return false;
}

bool Joypad::Read(const WORD &address, BYTE &out)
{
	if(address == P1_ADDRESS)
	{
		if( (column & ColumnEnum::LEFT_COLUMN) == 0)
		{
			out = leftInput | column;
		}
		else if( (column & ColumnEnum::RIGHT_COLUMN) == 0)
		{
			out = rightInput | column;
		}
		else
		{
			out = 0x0F;
		}

		return true;
	}

	return false;
}

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 

void Joypad::HandleEvent(sf::Event event)
{
	//leftInput = 

	if(event.type == sf::Event::KeyPressed)
	{
		if(event.key.code == sf::Keyboard::Right)
		{
			leftInput &= P_NOT_MASK_10;
		}
		
		if(event.key.code == sf::Keyboard::Left)
		{
			leftInput &= P_NOT_MASK_11;
		}

		if(event.key.code == sf::Keyboard::Up)
		{
			leftInput &= P_NOT_MASK_12;
		}

		if(event.key.code == sf::Keyboard::Down)
		{
			leftInput &= P_NOT_MASK_13;
		}
			

		if(event.key.code == sf::Keyboard::Z)
		{
			rightInput &= P_NOT_MASK_10;
		}
		
		if(event.key.code == sf::Keyboard::X)
		{
			rightInput &= P_NOT_MASK_11;
		}

		if(event.key.code == sf::Keyboard::Space)
		{
			rightInput &= P_NOT_MASK_12;
		}

		if(event.key.code == sf::Keyboard::Return)
		{
			rightInput &= P_NOT_MASK_13;
		}

		WriteLineC("Left "BYTETOBINARYPATTERN, BYTETOBINARY(leftInput));
		WriteLineC("Right "BYTETOBINARYPATTERN, BYTETOBINARY(rightInput));
	}
	else if(event.type == sf::Event::KeyReleased)
	{
		if(event.key.code == sf::Keyboard::Right)
		{
			leftInput |= P_MASK_10;
		}

		if(event.key.code == sf::Keyboard::Left)
		{
			leftInput |= P_MASK_11;
		}

		if(event.key.code == sf::Keyboard::Up)
		{
			leftInput |= P_MASK_12;
		}

		if(event.key.code == sf::Keyboard::Down)
		{
			leftInput |= P_MASK_13;
		}


		if(event.key.code == sf::Keyboard::Z)
		{
			rightInput |= P_MASK_10;
		}

		if(event.key.code == sf::Keyboard::X)
		{
			rightInput |= P_MASK_11;
		}

		if(event.key.code == sf::Keyboard::Space)
		{
			rightInput |= P_MASK_12;
		}

		if(event.key.code == sf::Keyboard::Return)
		{
			rightInput |= P_MASK_13;
		}

		WriteLineC("Left "BYTETOBINARYPATTERN, BYTETOBINARY(leftInput));
		WriteLineC("Right "BYTETOBINARYPATTERN, BYTETOBINARY(rightInput));
	}
}