#include "MemoryController.h"
#include "../../Tools/Console.h"
#include <stdexcept>


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

	for (int i = 0; i < memoryElementsCount; i++)
	{
		if (memoryElements[i]->Read(address, readByte))
		{
			return readByte;
		}
	}

	//Wrong Memory Read
	WriteLineC("Trying to Read ummaped address %d", address);
	throw std::runtime_error("Trying to Read ummaped address");
	return 0x00;
}

void MemoryController::WriteMemory(const WORD& address, const BYTE& value)
{
	if(address == 0x99cf)
	{
		int a = 0;
	}

	for (int i = 0; i < memoryElementsCount; i++)
	{
		if (memoryElements[i]->Write(address, value))
		{
			return;
		}
	}

	//Wrong Memory Read
	WriteLineC("Trying to Write ummaped address %d", address);
	throw std::runtime_error("Trying to Write ummaped address");
}

