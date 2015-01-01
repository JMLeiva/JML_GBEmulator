#ifndef JML_GPU
#define JML_GPU

#include "../Memory/MemoryElement.h"
#include "../../Metadata/Configuration.h"
#include "../CPU/CPU.h"

#ifndef UNIT_TEST_ON
#include <SFML\Graphics.hpp>
#endif

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
};

class GPU : public MemoryElement
{
public:
	GPU(CPU* cpu);
	~GPU();

	void Reset();
	bool Closed();

	bool Write(const WORD &address, const BYTE &value);
	bool Read(const WORD &address, BYTE &out);

	void RunCycle(int cycleCount);

#ifndef UNIT_TEST_ON
private:
	sf::Color PALETTE_COLORS[4];
#endif

	unsigned int gpuCycles;
	unsigned int eventCycles;

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
	inline bool LCDC_Obj8x8()			{ return (LCDC & 0x04) != 0x00; };
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

	//Helper Functions
	void RenderLine();
	void RenderScreen();

	//SFML
	bool closed;
	void UpdateEvents();
#ifndef UNIT_TEST_ON
	sf::Image image;
	sf::RenderWindow* window;
	sf::Event event;
	sf::Texture texture;
	sf::Sprite sprite;
#endif

};

#endif //JML_GPU