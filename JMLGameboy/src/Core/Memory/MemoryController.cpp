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

#include "MemoryController.h"
#include "../../Tools/Console.h"
#include <stdexcept>


#define BOOTSTRA_DISABLE_ADDRESS 0xFF50

int MEMORY_ELEMENTS_INCREASE_STEP = 8;


MemoryController* MemoryController::_instance;

MemoryController* MemoryController::Shared()
{
	if(_instance == NULL)
	{
		_instance = new MemoryController();
	}

	return _instance;
}

MemoryController::MemoryController()
{
	memoryElementsCount = 0;
	memoryElementsIncreaseCount = 1;
	memoryElements = new MemoryElement*[MEMORY_ELEMENTS_INCREASE_STEP];

#if BOOTSTRAP_ENABLED
	boostrapEnabled = true;
#else
	boostrapEnabled = false;
#endif
	
}

MemoryController::~MemoryController()
{

}

void MemoryController::AppendMemoryElement(MemoryElement* memoryElement)
{
	int currentCapacity = memoryElementsIncreaseCount * MEMORY_ELEMENTS_INCREASE_STEP;

	if (currentCapacity <= memoryElementsCount+1)
	{
		memoryElementsIncreaseCount++;
		MemoryElement** tempMemoryElements = new MemoryElement*[MEMORY_ELEMENTS_INCREASE_STEP * memoryElementsIncreaseCount];

		for (int i = 0; i < memoryElementsCount; i++)
		{
			tempMemoryElements[i] = memoryElements[i];
		}

		delete[] memoryElements;
		memoryElements = tempMemoryElements;
	}

	memoryElements[memoryElementsCount] = memoryElement;
	memoryElementsCount++;
}

void MemoryController::Clear()
{
	delete[] memoryElements;

	memoryElementsCount = 0;
	memoryElementsIncreaseCount = 1;
	memoryElements = new MemoryElement*[MEMORY_ELEMENTS_INCREASE_STEP];
}

BYTE MemoryController::ReadMemory(const WORD& address)
{
	BYTE readByte = 0x00;

	if(boostrapEnabled)
	{
		if(memoryBootstrap.Read(address, readByte) )
		{
			return readByte;
		}
	}

	for (int i = 0; i < memoryElementsCount; i++)
	{
		if (memoryElements[i]->Read(address, readByte))
		{
			return readByte;
		}
	}

	//Wrong Memory Read
	//WriteLineW("Trying to Read ummaped address %d", address);
	//throw std::runtime_error("Trying to Read ummaped address");
	return 0x00;
}

void MemoryController::WriteMemory(const WORD& address, const BYTE& value)
{
	if(boostrapEnabled && address == BOOTSTRA_DISABLE_ADDRESS && value > 0)
	{
		boostrapEnabled = false;
		return;
	}

	for (int i = 0; i < memoryElementsCount; i++)
	{
		if (memoryElements[i]->Write(address, value))
		{
			return;
		}
	}

	//Wrong Memory Read
	//WriteLineW("Trying to Write ummaped address %d", address);
	//throw std::runtime_error("Trying to Write ummaped address");
}

