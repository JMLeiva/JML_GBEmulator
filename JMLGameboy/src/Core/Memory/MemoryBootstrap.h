#ifndef JML_MEMORY_BOOTSTRAP
#define JML_MEMORY_BOOTSTRAP

#include "MemoryElement.h"

#define BOOTSTRAP_START 0x00
#define BOOTSTRAP_END 0xFF

class MemoryBootstrap : public MemoryElement
{
public:
	MemoryBootstrap();
	~MemoryBootstrap();
	bool Write(const WORD &address, const BYTE &value);
	bool Read(const WORD &address, BYTE &out);

private:
	BYTE memory[BOOTSTRAP_END - BOOTSTRAP_START];
};

#endif //JML_MEMORY_BOOTSTRAP