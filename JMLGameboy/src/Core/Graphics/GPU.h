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

#ifndef JML_GPU
#define JML_GPU

#include "../Memory/MemoryElement.h"
#include "../../Metadata/Configuration.h"
#include "../CPU/CPU.h"

//#ifndef UNIT_TEST_ON
#include <SFML\Graphics.hpp>
//#endif

enum GPUState
{
	RAM_ACCESS			= 0x00,
	VERTICAL_BLANK		= 0x01,
	SEARCHING_OAM		= 0x02,
	TRANSFERING_DATA	= 0x03,
};

enum InterruptSelectionMask
{
	ISM_MODE_00			= 0x08,
	ISM_MODE_01			= 0x10,
	ISM_MODE_10			= 0x20,
	ISM_MODE_LCY_MATCH	= 0x40,
};

enum InterruptSelection
{
	IS_MODE_00,
	IS_MODE_01,
	IS_MODE_10,
	IS_MODE_LCY_MATCH,
};

struct GraphicObject
{
	BYTE LCD_Y;
	BYTE LCD_X;
	BYTE CHAR_CODE;
	BYTE ATTR_FLAG;

	inline bool Palette1()	{ return (ATTR_FLAG & 0x10) != 0; }
	inline bool FlipX()		{ return (ATTR_FLAG & 0x20) != 0; }
	inline bool FlipY()		{ return (ATTR_FLAG & 0x40) != 0; }
	inline bool UnderBG()	{ return (ATTR_FLAG & 0x80) != 0; }

};

class GPU : public MemoryElement
{
public:
	GPU(CPU* cpu);
	~GPU();

	void Reset();

	bool Write(const WORD &address, const BYTE &value);
	bool Read(const WORD &address, BYTE &out);

	void RunCycle(int cycleCount);

//#ifndef UNIT_TEST_ON
	sf::RenderWindow* GetWindow(){return window; };
//#endif

#ifndef UNIT_TEST_ON
private:
#endif

//#ifndef UNIT_TEST_ON
	sf::Color PALETTE_COLORS[4];
//#endif

	unsigned int gpuCycles;

	//Registers
	BYTE LCDC;	// LCD Control Regiter
	BYTE STAT;	// LCDC Flag Status 
	BYTE SCX;	// Scroll X
	BYTE SCY;	// Scroll Y
	BYTE LY;	// LDCD Y-Coordinate
	BYTE LYC;	// LY Compare
	BYTE BGP;	// BG Palette Data (Only DMG, and GBC DMG Mode)
	BYTE OBP0;	// Object Palette 0 Data (Only DMG, and GBC DMG Mode)
	BYTE OBP1;	// Object Palette 1 Data (Only DMG, and GBC DMG Mode)
	BYTE WX;	// Window X Coordinate
	BYTE WY;	// Window X Coordinate


	//Character RAM
	BYTE characterRam[0x1800]; //BYTE[6144]

	//BG Display
	BYTE bgDisplay1[0x400]; //BYTE[1024]
	BYTE bgDisplay2[0x400]; //BYTE[1024]

	//OAM 
	GraphicObject oamObjects[0x28]; //GraphicObject[40]

	//LCDC DATA
	inline bool LCDC_BgOn()				{ return (LCDC & 0x01) != 0x00; };
	inline bool LCDC_ObjOn()			{ return (LCDC & 0x02) != 0x00; };
	inline bool LCDC_Obj8x16()			{ return (LCDC & 0x04) != 0x00; };
	inline bool LCDC_BgAreaFlag()		{ return (LCDC & 0x08) != 0x00; };
	inline bool LCDC_BgCharacterFlag()	{ return (LCDC & 0x10) != 0x00; };
	inline bool LCDC_WindowingOn()		{ return (LCDC & 0x20) != 0x00; };
	inline bool LCDC_WindowAreaLow()	{ return (LCDC & 0x40) != 0x00; };
	inline bool LCDC_DisplayOn()		{ return (LCDC & 0x80) != 0x00; };
	
	//STAT DATA
	inline GPUState	STAT_Mode()			{ return (GPUState)(STAT & 0x03); };
	inline bool STAT_LYCMatch()			{ return (STAT & 0x04) != 0x00; };
	inline InterruptSelection STAT_InterruptSelection()
	{
		if(STAT & InterruptSelectionMask::ISM_MODE_00) return InterruptSelection::IS_MODE_00;
		if(STAT & InterruptSelectionMask::ISM_MODE_01) return InterruptSelection::IS_MODE_01;
		if(STAT & InterruptSelectionMask::ISM_MODE_10) return InterruptSelection::IS_MODE_10;
		if(STAT & InterruptSelectionMask::ISM_MODE_LCY_MATCH) return InterruptSelection::IS_MODE_LCY_MATCH;
	};

	CPU* cpu;

	//Misc
	bool pendingVBlankInterruption;
	void OnLYCHanged();

	//Helper 
	BYTE bgLineMask[160];

	void RenderLine();
	void RenderBGLine();
	void RenderOBJLine();
	void RenderScreen();


	//SFML

//#ifndef UNIT_TEST_ON
	sf::RenderWindow* window;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
//#endif

};

#endif //JML_GPU