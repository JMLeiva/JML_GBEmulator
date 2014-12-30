/*
Area 0xC000-0xCFFF and 0xD000-0xDFFF 
were used to map the Working RAM (WRAM). 
This RAM was internal to the Game Boy, 
as opposed to the RAM banks presented by the cartridges. 
This area indeed consisted of two permanently-mapped banks internal 
to the Game Boy. It was used by the games to store temporary data; 
effectively, a working RAM.

The Game Boy also presented a special working RAM space 
at addresses 0xFF80-0xFFFE. It was used arbitrarily 
by games for storing temporary values.
*/

#ifndef JML_WORKING_RAM
#define JML_WORKING_RAM

#include "../Memory/MemoryElement.h"

class WorkingRam : public MemoryElement
{
public:
	WorkingRam();
	~WorkingRam();

	bool Write(const WORD &address, const BYTE &value);
	bool Read(const WORD &address, BYTE &out);

private:
	BYTE ram[0x2000]; //8K
};

#endif // JML_WORKING_RAM