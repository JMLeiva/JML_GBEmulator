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

#include "CartidgeBuilder.h"
#include "../../Tools/Console.h"

char NINTENDO_LOGO[] ={0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};

CartidgeBuilder::CartidgeBuilder()
{

}

Cartidge* CartidgeBuilder::Build(const char* path)
{
	FILE* file = fopen(path, "rb");
	if(file == NULL)
	{
		WriteLineE("File %s not found", path);
		return NULL;
	}

	char* header = GetHeader(file);
	if(header == NULL)
	{
		return NULL;
	}

	//Logo
	if(!CheckLogoIntegrity(header)) return NULL;
	//CartidgeData
	if(!ReadCartidgeMetadata(header)) return NULL;

	//Type
	Cartidge* cartidge = GetCorrectCartidge(header);
	if(cartidge == NULL) return NULL;

	//ROM Size
	if(!SetRomSize(header, cartidge)) return NULL;

	//RAM Size
	if(!SetRamSize(header, cartidge)) return NULL;

	//CGB Flag
	if(!GetCBGFlag(header, cartidge)) return NULL;

	//SGB Flag
	if(!GetSBGFlag(header, cartidge)) return NULL;

	//Load Main Rom
	rewind(file);

	cartidge->file = file;

	delete[] header;

	cartidge->Initialize();

	return cartidge;
}

char* CartidgeBuilder::GetHeader(FILE* file)
{
	//Check file size
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);
	WriteLineI("File Size: %d \n", (int)size);

	if(size < 0x150)
	{
		WriteLineE("ROM is too small: %s bytes", (int)size);
		return NULL;
	}

	//First Read "header", I.E. first 0x150 bytes
	char* headerBuffer = new char[0x150];
	if(headerBuffer == NULL)
	{
		WriteLineE("Header buffer allocation error");
		return NULL;
	}

	//Copy "header" in the headerBuffer
	size_t headerSize = fread(headerBuffer, 1, 0x150, file);

	if(headerSize != 0x150)
	{
		WriteLineE("Header buffer read error");
		return NULL;
	}

	return headerBuffer;
}

bool CartidgeBuilder::CheckLogoIntegrity(const char* header)
{
	//Check NINENDO LOGO
	for(int i = 0; i < 48; i++)
	{
		if(header[0x104 + i] != NINTENDO_LOGO[i])
		{
			WriteLineE("NINTENDO LOGO error");
			return false;
		}
	}

	WriteLineI("NINTENDO LOGO is fine");

	//DestinationCode (NOT YET)
	// 00h - Japanese
	// 01h - Non - Japanese

	//014B - Old Licensee Code
	//Specifies the games company / publisher code in range 00 - FFh.A value of 33h signalizes that the New License Code in header bytes 0144 - 0145 is used instead. (Super GameBoy functions won't work if <> $33.)


	//014C - Mask ROM Version number

	//014D - Header Checksum
	BYTE headerCheckSum = header[0x014D];

	//Calculate checksum (???)
	DREG cs;
	cs.w = 0;

	for(int i = 0x134; i < 0x14D; i++)
	{
		cs.w -= header[i] - 1;
	}

	WriteLineI("Cartidge Header Checksume = %#x, Real CheckSum = %#x", headerCheckSum, cs.l);

	return true;
}

bool CartidgeBuilder::ReadCartidgeMetadata(const char* header)
{
	//Game Title
	char gameTitle[16];

	for(int i = 0; i < 16; i++)
	{
		char ch = header[0x134 + i];

		if(ch == 0)
		{
			break;
		}

		gameTitle[i] = ch;
	}

	WriteLineI("Game title: %s", gameTitle);

	//Manufacturer Code
	char manufacturerCode[] =
	{
		header[0x013F],
		header[0x0140],
		header[0x0141],
		header[0x0142],
	};

	WriteLineI("Manufacturer code: %s", manufacturerCode);

	return true;
}

Cartidge* CartidgeBuilder::GetCorrectCartidge(const char* header)
{
	// Temp, subclass in the future if necesary
	Cartidge* cartidge = new Cartidge();

	//Cartidge Type
	BYTE cartidgeTypeCode = header[0x0147];

	switch(cartidgeTypeCode)
	{
	case CartidgeType::BANDAI_TAMAS:
		cartidge->cartidgeType = CartidgeType::BANDAI_TAMAS;
		WriteLineI("CartidgeType: BANDAI TAMAS");
		break;
	case CartidgeType::HuC1_RAM_BATTERY:
		cartidge->cartidgeType = CartidgeType::HuC1_RAM_BATTERY;
		WriteLineI("CartidgeType: HuC1_RAM_BATTERY");
		break;
	case CartidgeType::HuC3:
		cartidge->cartidgeType = CartidgeType::HuC3;
		WriteLineI("CartidgeType: HuC3");
		break;
	case CartidgeType::MBC1:
		cartidge->cartidgeType = CartidgeType::MBC1;
		WriteLineI("CartidgeType: MBC1");
		break;
	case CartidgeType::MBC1_RAM:
		cartidge->cartidgeType = CartidgeType::MBC1_RAM;
		WriteLineI("CartidgeType: MBC1_RAM");
		break;
	case CartidgeType::MBC1_RAM_BATTERY:
		cartidge->cartidgeType = CartidgeType::MBC1_RAM_BATTERY;
		WriteLineI("CartidgeType: MBC1_RAM_BATTERY");
		break;
	case CartidgeType::MBC2:
		cartidge->cartidgeType = CartidgeType::MBC2;
		WriteLineI("CartidgeType: MBC2");
		break;
	case CartidgeType::MBC2_BATTERY:
		cartidge->cartidgeType = CartidgeType::MBC2_BATTERY;
		WriteLineI("CartidgeType: MBC2_BATTERY");
		break;
	case CartidgeType::MBC3:
		cartidge->cartidgeType = CartidgeType::MBC3;
		WriteLineI("CartidgeType: MBC3");
		break;
	case CartidgeType::MBC3_RAM:
		cartidge->cartidgeType = CartidgeType::MBC3_RAM;
		WriteLineI("CartidgeType: MBC3_RAM");
		break;
	case CartidgeType::MBC3_RAM_BATTERY:
		cartidge->cartidgeType = CartidgeType::MBC3_RAM_BATTERY;
		WriteLineI("CartidgeType: MBC3_RAM_BATTERY");
		break;
	case CartidgeType::MBC3_TIMER_BATTERY:
		cartidge->cartidgeType = CartidgeType::MBC3_TIMER_BATTERY;
		WriteLineI("CartidgeType: MBC3_TIMER_BATTERY");
		break;
	case CartidgeType::MBC4:
		cartidge->cartidgeType = CartidgeType::MBC4;
		WriteLineI("CartidgeType: MBC4");
		break;
	case CartidgeType::MBC4_RAM:
		cartidge->cartidgeType = CartidgeType::MBC4_RAM;
		WriteLineI("CartidgeType: MBC4_RAM");
		break;
	case CartidgeType::MBC4_RAM_BATTERY:
		cartidge->cartidgeType = CartidgeType::MBC4_RAM_BATTERY;
		WriteLineI("CartidgeType: MBC4_RAM_BATTERY");
		break;
	case CartidgeType::MBC5:
		cartidge->cartidgeType = CartidgeType::MBC5;
		WriteLineI("CartidgeType: MBC5");
		break;
	case CartidgeType::MBC5_RAM:
		cartidge->cartidgeType = CartidgeType::MBC5_RAM;
		WriteLineI("CartidgeType: MBC5_RAM");
		break;
	case CartidgeType::MBC5_RAM_BATTERY:
		cartidge->cartidgeType = CartidgeType::MBC5_RAM_BATTERY;
		WriteLineI("CartidgeType: MBC5_RAM_BATTERY");
		break;
	case CartidgeType::MBC5_RUMBLE:
		cartidge->cartidgeType = CartidgeType::MBC5_RUMBLE;
		WriteLineI("CartidgeType: MBC5_RUMBLE");
		break;
	case CartidgeType::MBC5_RUMBLE_RAM:
		cartidge->cartidgeType = CartidgeType::MBC5_RUMBLE_RAM;
		WriteLineI("CartidgeType: MBC5_RUMBLE_RAM");
		break;
	case CartidgeType::MBC5_RUMBLE_RAM_BATTERY:
		cartidge->cartidgeType = CartidgeType::MBC5_RUMBLE_RAM_BATTERY;
		WriteLineI("CartidgeType: MBC5_RUMBLE_RAM_BATTERY");
		break;
	case CartidgeType::MMM01:
		cartidge->cartidgeType = CartidgeType::MMM01;
		WriteLineI("CartidgeType: MMM01");
		break;
	case CartidgeType::MMM01_RAM:
		cartidge->cartidgeType = CartidgeType::MMM01_RAM;
		WriteLineI("CartidgeType: MMM01_RAM");
		break;
	case CartidgeType::MMM01_RAM_BATTERY:
		cartidge->cartidgeType = CartidgeType::MMM01_RAM_BATTERY;
		WriteLineI("CartidgeType: MMM01_RAM_BATTERY");
		break;
	case CartidgeType::POCKET_CAMERA:
		cartidge->cartidgeType = CartidgeType::POCKET_CAMERA;
		WriteLineI("CartidgeType: POCKET_CAMERA");
		break;
	case CartidgeType::ROM_ONLY:
		cartidge->cartidgeType = CartidgeType::ROM_ONLY;
		WriteLineI("CartidgeType: ROM_ONLY");
		break;
	case CartidgeType::ROM_RAM:
		cartidge->cartidgeType = CartidgeType::ROM_RAM;
		WriteLineI("CartidgeType: ROM_RAM");
		break;
	case CartidgeType::ROM_RAM_BATTERY:
		cartidge->cartidgeType = CartidgeType::ROM_RAM_BATTERY;
		WriteLineI("CartidgeType: ROM_RAM_BATTERY");
		break;
	default:
		WriteLineI("UNKNOWN Cartidge Type %#x", cartidgeTypeCode);
		return NULL;
	}

	return cartidge;
}

bool CartidgeBuilder::SetRomSize(const char* header, Cartidge* cartidge)
{
	//ROM SIZE
	BYTE romBanksNumberCode = header[0x0148];
	unsigned short romBanksNumber = 0;

	switch(romBanksNumberCode)
	{
	case ROMBankSize::SIZE_1:
		romBanksNumber = 1;
		WriteLineI("ROM Bank Size: 1 (No Rom Banking), 32 KByte");
		break;
	case ROMBankSize::SIZE_4:
		romBanksNumber = 4;
		WriteLineI("ROM Bank Size: 4, 64 KByte");
		break;
	case ROMBankSize::SIZE_8:
		romBanksNumber = 8;
		WriteLineI("ROM Bank Size: 8, 128 KByte");
		break;
	case ROMBankSize::SIZE_16:
		romBanksNumber = 1;
		WriteLineI("ROM Bank Size: 16, 256 KByte");
		break;
	case ROMBankSize::SIZE_32:
		romBanksNumber = 1;
		WriteLineI("ROM Bank Size: 32, 512 KByte");
		break;
	case ROMBankSize::SIZE_64:
		romBanksNumber = 1;
		WriteLineI("ROM Bank Size: 64, 1 MByte");
		break;
	case ROMBankSize::SIZE_128:
		romBanksNumber = 1;
		WriteLineI("ROM Bank Size: 128, 2 MByte");
		break;
	case ROMBankSize::SIZE_256:
		romBanksNumber = 1;
		WriteLineI("ROM Bank Size: 256, 4 MByte");
		break;
	case ROMBankSize::SIZE_72:
		romBanksNumber = 1;
		WriteLineI("ROM Bank Size: 72, 1.1 MByte");
		break;
	case ROMBankSize::SIZE_80:
		romBanksNumber = 1;
		WriteLineI("ROM Bank Size: 80, 1.2 MByte");
		break;
	case ROMBankSize::SIZE_96:
		romBanksNumber = 1;
		WriteLineI("ROM Bank Size: 96, 1.5 MByte");
		break;
	default:
		WriteLineI("UNKNOWN ROM Banking  code %#x", romBanksNumberCode);
		return false;
	}

	for(int i = 0; i < romBanksNumber; i++)
	{
		cartidge->romBanks.push_back(new BYTE[ROM_SIZE]);
	}

	return true;
}

bool CartidgeBuilder::SetRamSize(const char* header, Cartidge* cartidge)
{
	//RAM SIZE
	BYTE ramBanksNumberCode = header[0x0149];

	int ramBanksNumber;
	int ramBankSize;

	switch(ramBanksNumberCode)
	{
	case RAMBankSize::SIZE_NONE:
		ramBanksNumber = 0;
		WriteLineI("RAM Bank Size: NONE");
		break;
	case RAMBankSize::SIZE_2K:
		ramBanksNumber = 1;
		ramBankSize = 2048;
		WriteLineI("RAM Bank Size: 2KByte");
		break;
	case RAMBankSize::SIZE_8K:
		ramBanksNumber = 1;
		ramBankSize = 8096;
		WriteLineI("RAM Bank Size: 8K Byte");
		break;
	case RAMBankSize::SIZE_32K:
		ramBanksNumber = 4;
		ramBankSize = 8096;
		WriteLineI("RAM Bank Size: 32K Byte (4 banks of 8KBytes each)");
		break;
	default:
		WriteLineI("UNKNOWN RAM Banking  code %#x", ramBanksNumberCode);
		return false;
	}

	for(int i = 0; i < ramBanksNumber; i++)
	{
		cartidge->ramBanks.push_back(new BYTE[ramBankSize]);
	}

	return true;
}

bool CartidgeBuilder::GetCBGFlag(const char* header, Cartidge* cartidge)
{
	//GBC Flag
	BYTE cbgCode = header[0x0143];

	switch(cbgCode)
	{
	case CBGFlag::CBG_COMPATIBLE:
		cartidge->cbgFlag = CBGFlag::CBG_COMPATIBLE;
		WriteLineI("CBG COMPATIBLE -> NOT SUPPORTED YET");
		return false;
		break;
	case CBGFlag::CBG_EXCLUSIVE:
		cartidge->cbgFlag = CBGFlag::CBG_EXCLUSIVE;
		WriteLineI("CBG EXCLUSIVE -> NOT SUPPORTED YET");
		return false;
		break;
	default:
		cartidge->cbgFlag = CBGFlag::CBG_UNSUPPORTED;
		WriteLineI("CBG UNSUPPORTED");
		break;
	}

	return true;
}

bool CartidgeBuilder::GetSBGFlag(const char* header, Cartidge* cartidge)
{

	//SBGFlag
	BYTE sbgCode = header[0x0146];

	switch(sbgCode)
	{
	case SBGFlag::SBG_UNSUPPORTED:
		cartidge->sbgFlag = SBGFlag::SBG_UNSUPPORTED;
		WriteLineI("SBG UNSUPPORTED");
		break;
	case SBGFlag::SGB_SUPPORTED:
		cartidge->sbgFlag = SBGFlag::SGB_SUPPORTED;
		WriteLineI("SBG SUPPORTED -> NOT SUPPORTED YET");
		return false;
		break;
	default:
		WriteLineI("UNKNOWN SBG FLAG %#x", sbgCode);
		return false;
		break;
	}

	return true;
}