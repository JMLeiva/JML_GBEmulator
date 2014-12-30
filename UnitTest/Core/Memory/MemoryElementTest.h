#ifndef JML_MEMORY_ELEMENT_TEST
#define JML_MEMORY_ELEMENT_TEST

#include "Core\Memory\MemoryElement.h"

class MemoryElementDummy : public MemoryElement
{
public:
	MemoryElementDummy(){}
	~MemoryElementDummy(){}
	bool Write(const WORD &address, const BYTE &value)
	{
		return false;
	}

	bool Read(const WORD &address, BYTE &out)
	{
		return false;
	}
};

class MemoryElementSmall : public MemoryElement
{
public:
	BYTE memory[0x100];

	MemoryElementSmall()
	{
		for (int i = 0; i < 0xFF; i++)
		{
			memory[i] = 0x00;
		}
	}

	~MemoryElementSmall(){}
	bool Write(const WORD &address, const BYTE &value)
	{
		if (address > 0xFF)
		{
			return false;
		}

		memory[address] = value;
		return true;
	}

	bool Read(const WORD &address, BYTE &out)
	{
		if (address > 0xFF)
		{
			return false;
		}

		out = memory[address];
		return true;
	}
};

class MemoryElementFull : public MemoryElement
{
public:
	BYTE memory[0xFFFF];

	MemoryElementFull()
	{
		for (int i = 0; i < 0xFFFF; i++)
		{
			memory[i] = 0x00;
		}
	}

	~MemoryElementFull(){}
	bool Write(const WORD &address, const BYTE &value)
	{
		memory[address] = value;
		return true; 
	}

	bool Read(const WORD &address, BYTE &out)
	{ 
		out = memory[address];
		return true; 
	}
};

#endif //JML_MEMORY_ELEMENT_TEST