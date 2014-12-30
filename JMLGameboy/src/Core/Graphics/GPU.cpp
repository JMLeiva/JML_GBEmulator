#include "GPU.h"
#include "../../Metadata/Configuration.h"
#include "../../Tools/Console.h"

#define CHARACTER_RAM_START_ADDRESS		0x8000
#define CHARACTER_RAM_END_ADDRESS		0x9800 //Exclusive

#define BG_DISPLAY_1_START_ADDRESS	0x9800
#define BG_DISPLAY_1_END_ADDRESS	0x9C00
#define BG_DISPLAY_2_START_ADDRESS	0x9C00
#define BG_DISPLAY_2_END_ADDRESS	0xA000

#define OAM_START_ADDRESS			0xFE00
#define OAM_END_ADDRESS				0xFEA0

#define LCDC_ADDRESS				0xFF40
#define STAT_ADDRESS				0xFF41
#define SCX_ADDRESS					0xFF43
#define SCY_ADDRESS					0xFF42
#define LY_ADDRESS					0xFF44
#define LYC_ADDRESS					0xFF45
#define BGP_ADDRESS					0xFF47
#define OBP0_ADDRESS				0xFF48
#define OBP1_ADDRESS				0xFF49
#define WX_ADDRESS					0xFF4B
#define WY_ADDRESS					0xFF4A

#define DMA_ADDRESS					0xFF46

#define CYCLES_SCANLINE_OAM			80
#define CYCLES_SCANLINE_VRAM		172
#define CYCLES_HORIZONTAL_BLANK		204
#define CYCLES_HORIZONTAL_LINE		CYCLES_SCANLINE_OAM + CYCLES_SCANLINE_VRAM + CYCLES_HORIZONTAL_BLANK

#define V_BLACK_LINE_START			144
#define V_BLACK_LINE_END			154

int SQUARE_SIZE = 4;
int WIDTH = 160;
int HEIGHT = 144;

GPU::GPU(CPU* cpu)
{
	this->cpu = cpu;

	Reset();

#ifndef UNIT_TEST_ON
	PALETTE_COLORS[0] = sf::Color(255, 255, 255);
	PALETTE_COLORS[1] = sf::Color(192, 192, 192);
	PALETTE_COLORS[2] = sf::Color(96, 96, 96);
	PALETTE_COLORS[3] = sf::Color(0, 0, 0);

	//Initialize SFML Window
	window = new sf::RenderWindow(sf::VideoMode(SQUARE_SIZE * WIDTH, SQUARE_SIZE * HEIGHT), "JML GBEmulator");

	//Create texture
	texture.create(WIDTH, HEIGHT);

	image.create(WIDTH, HEIGHT);

	texture.update(image);
	sprite.setTexture(texture);
	sprite.scale((float)SQUARE_SIZE, (float)SQUARE_SIZE);

	closed = false;
#endif
}

void GPU::Reset()
{
	gpuCycles = 0;
	LCDC = 0x91;
	STAT = 0x02; //Search OAM
	SCX = 0x00;
	SCY = 0x00;
	LYC = 0x00;
	WY = 0x00;
	WX = 0x00;

	pendingVBlankInterruption = false;

	//This Shouldn't Be Necessary
	for(int i = 0; i < 0x1800; i++)
	{
		characterRam[i] = 0;
	}

	//BG Display
	for(int i = 0; i < 0x400; i++)
	{
		bgDisplay1[i] = 0;
		bgDisplay2[i] = 0;
	}
	

	//OAM 
	for(int i = 0; i < 0x28; i++)
	{
		oamObjects[i].LCD_X = 0;
		oamObjects[i].LCD_Y = 0;
		oamObjects[i].CHAR_CODE = 0;
		oamObjects[i].ATTR_FLAG = 0;
	}
}

GPU::~GPU()
{

}

void GPU::Update()
{
#ifndef UNIT_TEST_ON
	while(window->pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			window->close();
			closed = true;
		}
		else
		{
			//ProcessInput(event);
		}

		// clear the window with black color
		window->clear(sf::Color::Black);

		// draw everything here...
		window->draw(sprite);

		// end the current frame
		window->display();
	}
#endif
}

void GPU::RunCycle(int cycleCount)
{
	if(!LCDC_DisplayOn())
	{
		return;
	}

	gpuCycles += cycleCount;

	switch(STAT_Mode())
	{
	case GPUState::RAM_ACCESS:
		if(gpuCycles >= CYCLES_HORIZONTAL_BLANK)
		{
			// Reset gpuCycles
			gpuCycles = 0;

			// Increment Line
			LY++;
			OnLYCHanged();
			
			if(LY == V_BLACK_LINE_START)
			{
				// Go to V-Blank
				STAT = (STAT & 0xFC) | VERTICAL_BLANK;
				pendingVBlankInterruption = true;
			}
			else
			{
				//Go to OAM Search
				STAT = (STAT & 0xFC) | SEARCHING_OAM;

				//Check If Interruption Needed
				if(this->STAT_InterruptSelection() == InterruptSelection::IS_MODE_10)
				{
					cpu->SetInterrupt(INTERRUPT_FLAG::IF_LCDC);
				}

#ifdef DEBUGGER_ON
				WriteLineV("Entering Search OAM GPU Mode");
#endif
			}			
		}
		break;
	case GPUState::VERTICAL_BLANK:

		// Set Interrupt Flag
		// Delay 24 CPU Cycles VBlank Interrupt
		// Some Games check if LY == 144 
		// CHECK IF THIS IS NECESSARY

		if(pendingVBlankInterruption && gpuCycles > 24)
		{
			cpu->SetInterrupt(INTERRUPT_FLAG::IF_V_BLANK);

			//Check If Interruption Needed
			if(this->STAT_InterruptSelection() == InterruptSelection::IS_MODE_01)
			{
				cpu->SetInterrupt(INTERRUPT_FLAG::IF_LCDC);
			}

			pendingVBlankInterruption = false;

			// Render Whole Imagen
			texture.update(image);
			sprite.setTexture(texture);
			//printf("\n\n\n-------------------------------------------------------------------\n");
		

#ifdef DEBUGGER_ON
			WriteLineV("Entering Vertical Blank GPU Mode");
#endif
		}

		if(gpuCycles >= CYCLES_HORIZONTAL_LINE)
		{
			// Reset gpuCycles
			gpuCycles = 0;

			// Increment Line
			LY++;

			if(LY >= V_BLACK_LINE_END)
			{
				// Go Back to OAM Search
				STAT = (STAT & 0xFC) | SEARCHING_OAM;

				// Reset LY
				LY = 0;

				//Check If Interruption Needed
				if(this->STAT_InterruptSelection() == InterruptSelection::IS_MODE_10)
				{
					cpu->SetInterrupt(INTERRUPT_FLAG::IF_LCDC);
				}

#ifdef DEBUGGER_ON
				WriteLineV("Entering Search OAM GPU Mode");
#endif
			}

			OnLYCHanged();
		}
		break;
	case GPUState::SEARCHING_OAM:

		// Read OAM ??

		if(gpuCycles >= CYCLES_SCANLINE_OAM)
		{
			// Reset gpuCycles
			gpuCycles = 0;

			// Go To VRAM Read
			STAT = (STAT & 0xFC) | TRANSFERING_DATA;


#ifdef DEBUGGER_ON
			WriteLineV("Entering Search Transfer Data GPU Mode");
#endif
		}
		break;
	case GPUState::TRANSFERING_DATA:
		if(gpuCycles >= CYCLES_SCANLINE_VRAM)
		{
			// Reset gpuCycles
			gpuCycles = 0;

			// Go To H Blank
			STAT = (STAT & 0xFC) | RAM_ACCESS;

			//Check If Interruption Needed
			if(this->STAT_InterruptSelection() == InterruptSelection::IS_MODE_00)
			{
				cpu->SetInterrupt(INTERRUPT_FLAG::IF_LCDC);
			}

#ifdef DEBUGGER_ON
			WriteLineV("Entering Horizontal Blak GPU Mode");
#endif
			//Check if Background Is On
			if(LCDC_BgOn())
			{
				// Check which tileset to use
				BYTE* bgDisplay = LCDC_BgAreaFlag() ? bgDisplay2 : bgDisplay1;
				WORD charOffset = LCDC_BgCharacterFlag() ? 0x0000 : 0x0800;

				BYTE logicY = LY >> 3; //LY / 8; Map Logic Coordinate Id
				BYTE characterYLine = LY & 0x07;//LY % 8; Tiles are 8x8, this is the y-line inside a tile

				WORD tileId =  logicY * 32;

				//TODO Use SCX & CSY
				
				//if(characterYLine == 0)
				//{
				//	printf("\n");
				//}

				for(BYTE x = 0; x < 32; x++)
				{
					BYTE tileAddress = bgDisplay[tileId+x] * 16;
					BYTE lineAddress = tileAddress + characterYLine * 2;

					

					//if(characterYLine == 0)
					//{
					//	printf("[%03d]", tileId + x);
					//
					//}

					
				
					BYTE line0 = characterRam[lineAddress + charOffset];
					BYTE line1 = characterRam[lineAddress + 1 + charOffset];

					// TEST IMPLEMENTATION
					for(BYTE i = 0; i < 8; i++)
					{
						BYTE renderX = x * 8 + i;
						BYTE renderY = LY;
						
						BYTE paletteIndex = (line1 >> (7 - i)) & 0x01;
						paletteIndex <<= 1;
						paletteIndex |= (line0 >> (7 - i)) & 0x01;

						if(renderX >= WIDTH || renderY >= HEIGHT)
						{
							return;
						}

						//TODO USER PALETTE REGISTER

						image.setPixel(renderX, renderY, PALETTE_COLORS[paletteIndex]);
					}
				}
			}
			else
			{
				//TODO Draw Blank Line
			}
			// TODO Render LINE
			// TODO Do the job "async" outside this if
		}
		break;
	}
}

void GPU::OnLYCHanged()
{
	if(LY == LYC)
	{
		//UPDATE STAT
		STAT |= 0x04;

		if(this->STAT_InterruptSelection() == InterruptSelection::IS_MODE_LCY_MATCH)
		{
			cpu->SetInterrupt(INTERRUPT_FLAG::IF_LCDC);
		}
	}
	else
	{
		//UPDATE STAT
		STAT &= 0xFB; //~0x04
	}
}

bool GPU::Closed()
{
	return closed;
}

bool GPU::Write(const WORD &address, const BYTE &value)
{
	if(address == LCDC_ADDRESS)
	{
		if(this->LCDC_DisplayOn() && (value & 0x80))
		{
			// Writing a value of 0 to bit 7 of the LCDC register when its value is 1 
			// stops the LCD controller, and the value of
			// register LY immediately becomes 0.
			LY = 0x00;
		}


		LCDC = value;

		// TODO
		// Turn LCD On/Off

		return true;
	}
	else if(address == STAT_ADDRESS)
	{
		STAT = value;
		return true;
	}
	else if(address == SCX_ADDRESS)
	{
		SCX = value;
		return true;
	}
	else if(address == SCY_ADDRESS)
	{
		SCY = value;
		return true;
	}
	else if(address == LYC_ADDRESS)
	{
		LYC = value;
		return true;
	}
	else if(address == BGP_ADDRESS)
	{
		BGP = value;
		// Bit 7 - 6 - Shade for Color Number 3
		// Bit 5 - 4 - Shade for Color Number 2
		// Bit 3 - 2 - Shade for Color Number 1
		// Bit 1 - 0 - Shade for Color Number 0

		return true;
	}
	else if(address == OBP0_ADDRESS)
	{
		OBP0 = value;
		// Bit 7 - 6 - Shade for Color Number 3
		// Bit 5 - 4 - Shade for Color Number 2
		// Bit 3 - 2 - Shade for Color Number 1
		// Bit 1 - 0 - Unused (sprite data 00 is transparent)

		return true;
	}
	else if(address == OBP1_ADDRESS)
	{
		OBP1 = value;
		// Bit 7 - 6 - Shade for Color Number 3
		// Bit 5 - 4 - Shade for Color Number 2
		// Bit 3 - 2 - Shade for Color Number 1
		// Bit 1 - 0 - Unused (sprite data 00 is transparent)

		return true;
	}
	else if(address == WX_ADDRESS)
	{
		WX = value;
		
#ifdef DEBUGGER_ON
		if(value <= 7 || value >= 166)
		{
			WriteLineW("WARNING: Writing %d otuside [7, 166] to WX", value);
		}

#endif

		return true;
	}
	else if(address == WY_ADDRESS)
	{
		WY = value;

#ifdef DEBUGGER_ON
		if(value >= 143)
		{
			WriteLineW("WARNING: Writing %d otuside [0, 143] to Wy", value);
		}

#endif

		return true;
	}
	else if(address == DMA_ADDRESS)
	{
		//DMA Transfer
		BYTE transferAddress = address;
		for(BYTE oamId = 0; oamId < 160; oamId++)
		{
			oamObjects[oamId].LCD_Y		= MemoryController::Shared()->ReadMemory(transferAddress);
			oamObjects[oamId].LCD_X		= MemoryController::Shared()->ReadMemory(transferAddress+1);
			oamObjects[oamId].CHAR_CODE = MemoryController::Shared()->ReadMemory(transferAddress+2);
			oamObjects[oamId].ATTR_FLAG	= MemoryController::Shared()->ReadMemory(transferAddress+3);

			transferAddress += 4;
		}
		
		return true;
	}
	else if(address >= CHARACTER_RAM_START_ADDRESS && address < CHARACTER_RAM_END_ADDRESS)
	{
		//Write to CHARACTER RAM
		if(STAT_Mode() != GPUState::TRANSFERING_DATA)
		{
			characterRam[address - CHARACTER_RAM_START_ADDRESS] = value;
		}
#ifdef DEBUGGER_ON
		else
		{ 
			WriteLineE("ERROR: Writing to CharacterRam while Wrong GPU State");
			//TODO CHECH IF THIS SHOULD BE JUST UNHANDLED
		}
#endif
		return true;
	}
	else if(address >= BG_DISPLAY_1_START_ADDRESS && address < BG_DISPLAY_1_END_ADDRESS)
	{
		//Write to BG Display 1
		if(STAT_Mode() != GPUState::TRANSFERING_DATA)
		{
			bgDisplay1[address - BG_DISPLAY_1_START_ADDRESS] = value;
		}
#ifdef DEBUGGER_ON
		else
		{
			WriteLineE("ERROR: Writing to BGDisplay1Ram while Wrong GPU State");
			//TODO CHECH IF THIS SHOULD BE JUST UNHANDLED
		}
#endif
		return true;
	}
	else if(address >= BG_DISPLAY_2_START_ADDRESS && address < BG_DISPLAY_2_END_ADDRESS)
	{
		//Write to BG Display 2
		if(STAT_Mode() != GPUState::TRANSFERING_DATA)
		{
			bgDisplay2[address - BG_DISPLAY_2_START_ADDRESS] = value;
		}
#ifdef DEBUGGER_ON
		else
		{
			WriteLineE("ERROR: Writing to BGDisplay2Ram while Wrong GPU State");
			//TODO CHECH IF THIS SHOULD BE JUST UNHANDLED
		}
#endif
		return true;
	}
	else if(address >= OAM_START_ADDRESS && address < OAM_END_ADDRESS)
	{
		//Write OAM
		BYTE statMode = STAT_Mode();

		if(statMode != GPUState::TRANSFERING_DATA && statMode != GPUState::SEARCHING_OAM)
		{
			BYTE objIndex = (address >> 2) & 0x003F;
			BYTE objProp = address & 0x03;

			switch(objProp)
			{
			case 0x00:
				oamObjects[objIndex].LCD_Y = value;
				break;
			case 0x01:
				oamObjects[objIndex].LCD_X = value;
				break;
			case 0x10:
				oamObjects[objIndex].CHAR_CODE = value;
				break;
			case 0x11:
				oamObjects[objIndex].ATTR_FLAG = value;
				break;
			}
		}
#ifdef DEBUGGER_ON
		else
		{
			WriteLineE("ERROR: Writing to OAM while Wrong GPU State");
			//TODO CHECH IF THIS SHOULD BE JUST UNHANDLED
		}
#endif
		return true;
	}

	return false;
}

bool GPU::Read(const WORD &address, BYTE &out)
{
	if(address == LCDC_ADDRESS)
	{
		out = LCDC;
		return true;
	}
	else if(address == STAT_ADDRESS)
	{
		out = STAT;
		return true;
	}
	else if(address == SCX_ADDRESS)
	{
		out = SCX;
		return true;
	}
	else if(address == SCY_ADDRESS)
	{
		out = SCY;
		return true;
	}
	else if(address == LY_ADDRESS)
	{
		out = LY;
		return true;
	}
	else if(address == LYC_ADDRESS)
	{
		out = LYC;
		return true;
	}
	else if(address == BGP_ADDRESS)
	{
		out = BGP;
		// Bit 7 - 6 - Shade for Color Number 3
		// Bit 5 - 4 - Shade for Color Number 2
		// Bit 3 - 2 - Shade for Color Number 1
		// Bit 1 - 0 - Shade for Color Number 0

		return true;
	}
	else if(address == OBP0_ADDRESS)
	{
		out = OBP0;
		// Bit 7 - 6 - Shade for Color Number 3
		// Bit 5 - 4 - Shade for Color Number 2
		// Bit 3 - 2 - Shade for Color Number 1
		// Bit 1 - 0 - Unused (sprite data 00 is transparent)

		return true;
	}
	else if(address == OBP1_ADDRESS)
	{
		out = OBP1;
		// Bit 7 - 6 - Shade for Color Number 3
		// Bit 5 - 4 - Shade for Color Number 2
		// Bit 3 - 2 - Shade for Color Number 1
		// Bit 1 - 0 - Unused (sprite data 00 is transparent)

		return true;
	}
	else if(address == WX_ADDRESS)
	{
		out = WX;
		return true;
	}
	else if(address == WY_ADDRESS)
	{
		out = WY;
		return true;
	}
	else if(address >= CHARACTER_RAM_START_ADDRESS && address < CHARACTER_RAM_END_ADDRESS)
	{
		if(STAT_Mode() != GPUState::TRANSFERING_DATA)
		{
			out = characterRam[address - CHARACTER_RAM_START_ADDRESS];
		}
#ifdef DEBUGGER_ON
		else
		{
			WriteLineE("ERROR: Reading CharacterRam while Wrong GPU State");
			//TODO CHECH IF THIS SHOULD BE JUST UNHANDLED
		}
#endif
		return true;
	}
	else if(address >= BG_DISPLAY_1_START_ADDRESS && address < BG_DISPLAY_1_END_ADDRESS)
	{
		if(STAT_Mode() != GPUState::TRANSFERING_DATA)
		{
			out = bgDisplay1[address - BG_DISPLAY_1_START_ADDRESS];
		}
#ifdef DEBUGGER_ON
		else
		{
			WriteLineE("ERROR: Reading BGDisplay1Ram while Wrong GPU State");
			//TODO CHECH IF THIS SHOULD BE JUST UNHANDLED
		}
#endif
		return true;
	}
	else if(address >= BG_DISPLAY_2_START_ADDRESS && address < BG_DISPLAY_2_END_ADDRESS)
	{
		if(STAT_Mode() != GPUState::TRANSFERING_DATA)
		{
			out = bgDisplay2[address - BG_DISPLAY_2_START_ADDRESS];
		}
#ifdef DEBUGGER_ON
		else
		{
			WriteLineE("ERROR: Reading BGDisplay2Ram while Wrong GPU State");
			//TODO CHECH IF THIS SHOULD BE JUST UNHANDLED
		
		}
#endif
		return true;
	}
	else if(address >= OAM_START_ADDRESS && address < OAM_END_ADDRESS)
	{
		//Read OAM
		BYTE statMode = STAT_Mode();

		if(statMode != GPUState::TRANSFERING_DATA && statMode != GPUState::SEARCHING_OAM)
		{
			BYTE objIndex = (address >> 2) & 0x003F;
			BYTE objProp = address & 0x03;

			switch(objProp)
			{
			case 0x00:
				out = oamObjects[objIndex].LCD_Y;
				break;
			case 0x01:
				out = oamObjects[objIndex].LCD_X;
				break;
			case 0x10:
				out = oamObjects[objIndex].CHAR_CODE;
				break;
			case 0x11:
				out = oamObjects[objIndex].ATTR_FLAG;
				break;
			}
		}
#ifdef DEBUGGER_ON
		else
		{
			WriteLineE("ERROR: Reading OAM while Wrong GPU State");
			//TODO CHECH IF THIS SHOULD BE JUST UNHANDLED
		}
#endif

		return true;
	}

	return false;
}