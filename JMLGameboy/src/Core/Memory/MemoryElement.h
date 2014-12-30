#ifndef JML_MEMORY_ELEMENT
#define JML_MEMORY_ELEMENT

#include "../../Metadata/DataTypes.h"

class MemoryElement
{
public:
	virtual bool Write(const WORD &address, const BYTE &value) = 0;
	virtual bool Read(const WORD &address, BYTE &out) = 0;
};

#endif //JML_MEMORY_ELEMENT