#include <iostream>
#include <sstream>

#include "bus.h"
#include "CPU6502.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"



class Demo_olc6502 : public olc::PixelGameEngine
{
public:
	Demo_olc6502() { sAppName = "olc6502 Demonstration"; }

private: 
	std::shared_ptr<cartridge> cart;
	Bus nes;
	std::map<uint16_t, std::string> mapAsm;

	bool emulationRun = false;
	float residualT = 0.0f;

	uint8_t selectedPalette = 0x00; // allow user to choose which palette is being used to draw the pattern table

	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
	{
		int nRamX = x, nRamY = y;
		for (int row = 0; row < nRows; row++)
		{
			std::string sOffset = "$" + hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++)
			{
				sOffset += " " + hex(nes.cRead(nAddr, true), 2);
				nAddr += 1;
			}
			DrawString(nRamX, nRamY, sOffset);
			nRamY += 10;
		}
	}

	void DrawCpu(int x, int y)
	{
		std::string status = "STATUS: ";
		DrawString(x, y, "STATUS:", olc::WHITE);
		DrawString(x + 64, y, "N", nes.cpu.status & CPU6502::N ? olc::GREEN : olc::RED);
		DrawString(x + 80, y, "V", nes.cpu.status & CPU6502::V ? olc::GREEN : olc::RED);
		DrawString(x + 96, y, "-", nes.cpu.status & CPU6502::U ? olc::GREEN : olc::RED);
		DrawString(x + 112, y, "B", nes.cpu.status & CPU6502::B ? olc::GREEN : olc::RED);
		DrawString(x + 128, y, "D", nes.cpu.status & CPU6502::D ? olc::GREEN : olc::RED);
		DrawString(x + 144, y, "I", nes.cpu.status & CPU6502::I ? olc::GREEN : olc::RED);
		DrawString(x + 160, y, "Z", nes.cpu.status & CPU6502::Z ? olc::GREEN : olc::RED);
		DrawString(x + 178, y, "C", nes.cpu.status & CPU6502::C ? olc::GREEN : olc::RED);
		DrawString(x, y + 10, "PC: $" + hex(nes.cpu.pc, 4));
		DrawString(x, y + 20, "A: $" + hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
		DrawString(x, y + 30, "X: $" + hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
		DrawString(x, y + 40, "Y: $" + hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
		DrawString(x, y + 50, "Stack P: $" + hex(nes.cpu.sp, 4));
	}

	void DrawCode(int x, int y, int nLines)
	{
		auto it_a = mapAsm.find(nes.cpu.pc);
		int nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			DrawString(x, nLineY, (*it_a).second, olc::CYAN);
			while (nLineY < (nLines * 10) + y)
			{
				nLineY += 10;
				if (++it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}

		it_a = mapAsm.find(nes.cpu.pc);
		nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			while (nLineY > y)
			{
				nLineY -= 10;
				if (--it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}
	}

	bool OnUserCreate()
	{
		// load cartridge
		cart = std::make_shared<cartridge>("../games/nestest.nes");

		if (!cart->ImageValid()) {
			return false;
		}

		//insert into nes
		nes.insertCart(cart);

		// extract disassembly
		mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

		// reset nes
		nes.reset();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		Clear(olc::DARK_BLUE);

		// controller
		nes.controller[0] = 0x00;
		nes.controller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;

		if (emulationRun) {
			if (residualT > 0.0f) {
				residualT -= fElapsedTime;
			}
			else {
				residualT += (1.0f / 60.0f) - fElapsedTime;
				do {
					nes.clock();
				} while (!nes.ppu.frameComplete);
				nes.ppu.frameComplete = false;
			}
		}
		else {

			// emulate code step by step
			if (GetKey(olc::Key::C).bPressed) {
				// clock enough times to execute a whole CPU instruction
				do {
					nes.clock();
				} while (!nes.cpu.complete());

				// cpu clock runs slower than system clock -> may have completed for additional system clock cycles
				// drain it out
				do {
					nes.clock();
				} while (nes.cpu.complete());
			}

			// emulate the whole frame
			if (GetKey(olc::Key::F).bPressed) {
				// clock enough times to draw a frame
				do {
					nes.clock();
				} while (!nes.ppu.frameComplete);

				// use residual clock cycles to complete current instruction
				do {
					nes.clock();
				} while (!nes.cpu.complete());
				// reset frame completion flag
				nes.ppu.frameComplete = false;
			}
		}
		if (GetKey(olc::Key::R).bPressed) {
			nes.reset();
		}

		if (GetKey(olc::Key::SPACE).bPressed) {
			emulationRun = !emulationRun;
		}

		if (GetKey(olc::Key::P).bPressed) {
			// increments and wrap it around
			(++selectedPalette) &= 0x07;
		}

		DrawCpu(516, 2);
		DrawCode(516, 72, 26);

		const int swatchSize = 6;
		for (int p = 0; p < 8; p++) { // for each palette
			for (int s = 0; s < 4; s++) { // for each index
				FillRect(516 + p * (swatchSize * 5) + s * swatchSize, 340, swatchSize, swatchSize, nes.ppu.getColorFromPalette(p, s));
			}
		}

		DrawRect(516 + selectedPalette * (swatchSize * 5) - 1, 339, (swatchSize * 4), swatchSize, olc::WHITE);

		DrawSprite(516, 348, &nes.ppu.GetPatternTable(0, selectedPalette));
		DrawSprite(648, 348, &nes.ppu.GetPatternTable(1, selectedPalette));

		DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);

		// draw ids (for testing)
		//for (uint8_t y = 0; y < 30; y++) {
		//	for (uint8_t x = 0; x < 32; x++) {
		//		DrawString(x * 16, y * 16, hex((uint32_t)nes.ppu.nameTable[0][y * 32 + x], 2));
		//	}
		//}

		return true;
	}
};





int main()
{
	Demo_olc6502 demo;
	demo.Construct(780, 480, 2, 2);
	demo.Start();
	return 0;
}