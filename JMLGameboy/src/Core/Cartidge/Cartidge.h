#ifndef JML_CARTIDGE
#define JML_CARTIDGE

#include "../Memory/MemoryElement.h"
#include <vector>
#include <array>

#define ROM_SIZE 0x4000

enum CBGFlag
{
	CBG_UNSUPPORTED = 0x00,
	CBG_COMPATIBLE  = 0x80,
	CBG_EXCLUSIVE	= 0xC0
};

enum SBGFlag
{
	SBG_UNSUPPORTED = 0x00,
	SGB_SUPPORTED	= 0x03
};

enum CartidgeType
{
	ROM_ONLY				= 0x00,
	MBC1					= 0x01,
	MBC1_RAM				= 0x02,
	MBC1_RAM_BATTERY		= 0x03,
	MBC2					= 0x05,
	MBC2_BATTERY			= 0x06,
	ROM_RAM					= 0x08,
	ROM_RAM_BATTERY			= 0x09,
	MMM01					= 0x0B,
	MMM01_RAM				= 0x0C,
	MMM01_RAM_BATTERY		= 0x0D,
	MBC3_TIMER_BATTERY		= 0x0F,
	MBC3					= 0x11,
	MBC3_RAM				= 0x12,
	MBC3_RAM_BATTERY		= 0x13,
	MBC4					= 0x15,
	MBC4_RAM				= 0x16,
	MBC4_RAM_BATTERY		= 0x17,
	MBC5					= 0x19,
	MBC5_RAM				= 0x1A,
	MBC5_RAM_BATTERY		= 0x1B,
	MBC5_RUMBLE				= 0x1C,
	MBC5_RUMBLE_RAM			= 0x1D,
	MBC5_RUMBLE_RAM_BATTERY	= 0x1E,
	POCKET_CAMERA			= 0xFC,
	BANDAI_TAMAS			= 0xFD,
	HuC3					= 0xFE,
	HuC1_RAM_BATTERY		= 0xFF
};

enum ROMBankSize
{
	SIZE_1		= 0x00,		//32 KByte
	SIZE_4		= 0x01,		//64 KByte
	SIZE_8		= 0x02,		//128 KByte
	SIZE_16	= 0x03,		//256 KByte
	SIZE_32	= 0x04,		//512 KByte
	SIZE_64	= 0x05,		//1 MByte - only 63 banks used by MBC1
	SIZE_128	= 0x06,		//2 MByte - only 125 banks used by MBC1
	SIZE_256	= 0x07,		//4 MByte
	SIZE_72	= 0x52,		//1.1 MByte
	SIZE_80	= 0x53,		//1.2 MByte
	SIZE_96	= 0x54		//1.5 MByte
	
};

enum RAMBankSize
{
	SIZE_NONE	= 0x00,		//None
	SIZE_2K		= 0x01,		//2 KByte
	SIZE_8K		= 0x02,		//8 KByte
	SIZE_32K	= 0x03,		//32 KByte (4 banks of 8KBytes each)
};

class Cartidge : public MemoryElement
{
	friend class CartidgeBuilder;

public:
	Cartidge();
	~Cartidge();

	bool Write(const WORD &address, const BYTE &value);
	bool Read(const WORD &address, BYTE &out);

private:
	void Initialize();


	std::vector<BYTE* > romBanks;
	std::vector<BYTE* > ramBanks;

	unsigned char currentBank;

	CBGFlag cbgFlag;
	SBGFlag sbgFlag;
	CartidgeType cartidgeType;

	FILE* file;
};


#endif //JML_CARTIDGE