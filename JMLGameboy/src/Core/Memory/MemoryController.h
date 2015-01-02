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

#ifndef JML_MOMORY_CONTROLLER
#define JML_MEMORY_CONTROLLER

#include "MemoryElement.h"
#include "../../Metadata/Configuration.h"

#define SPACE_SIZE 0xFFFF;


class MemoryController
{
public:
	
	//Singleton
	static MemoryController* Shared();
	//!Singleton


	~MemoryController();

	void AppendMemoryElement(MemoryElement* memoryElement);
	void Clear();

	BYTE ReadMemory(const WORD& address);
	void WriteMemory(const WORD& address, const BYTE& value);

#ifdef UNIT_TEST_ON
public:
#else
private:
#endif
	//Singleton
	static MemoryController* _instance;
	//!Singleton

	MemoryElement** memoryElements;
	int memoryElementsCount;
	int memoryElementsIncreaseCount;

private:
	MemoryController();
};

#endif JML_MEMORY_ADDRESS