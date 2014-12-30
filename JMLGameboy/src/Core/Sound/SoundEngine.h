#ifndef JML_SOUND_ENGINE
#define JML_SOUND_ENGINE

#include "../Memory/MemoryElement.h"

class SoundEngine : public MemoryElement
{
public:
	SoundEngine();
	~SoundEngine();

	bool Write(const WORD &address, const BYTE &value);
	bool Read(const WORD &address, BYTE &out);
};

#endif //JML_SOUND_ENGINE