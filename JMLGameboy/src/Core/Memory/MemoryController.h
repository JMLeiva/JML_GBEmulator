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