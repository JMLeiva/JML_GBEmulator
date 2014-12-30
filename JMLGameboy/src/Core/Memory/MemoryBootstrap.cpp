#include "MemoryBootstrap.h"
#include "../../Tools/Console.h"

#include <stdexcept>



MemoryBootstrap::MemoryBootstrap()
{

}


MemoryBootstrap::~MemoryBootstrap()
{

}

bool MemoryBootstrap::Write(const WORD &address, const BYTE &value)
{
	if (address > BOOTSTRAP_END) return false;

	WriteLineC("Trying to write on Bootstrap ROM at address %d", address);
	throw std::runtime_error("Cannot write to Bootstrap ROM");

	return false;
}

bool MemoryBootstrap::Read(const WORD &address, BYTE &out)
{
	if (address > BOOTSTRAP_END) return false;

	out = memory[address];

	return true;
}