#include "CPU6502.h"
#include "bus.h"

// Constructor
CPU6502::CPU6502() {
	// Assembles the translation table. It's big, it's ugly, but it yields a convenient way
	// to emulate the 6502. I'm certain there are some "code-golf" strategies to reduce this
	// but I've deliberately kept it verbose for study and alteration

	// It is 16x16 entries. This gives 256 instructions. It is arranged to that the bottom
	// 4 bits of the instruction choose the column, and the top 4 bits choose the row.

	// For convenience to get function pointers to members of this class, I'm using this
	// or else it will be much much larger :D

	// The table is one big initialiser list of initialiser lists...
	using a = CPU6502;
	search =
	{
	// MSD\LSD          0x0                           0x1                            0x2                            0x3                               0x4                            0x5                          0x6                            0x7                               0x8                              0x9                             0xA                           0xB                             0xC                                 0xD                             0xE                             0xF
	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/* 0x0 */	{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IND_X, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 }, { "ORA", &a::ORA, &a::ZP, 3 },  { "ASL", &a::ASL, &a::ZP, 5 },  { "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },  { "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },  { "ORA", &a::ORA, &a::ABS, 4 },  { "ASL", &a::ASL, &a::ABS, 6 },  { "???", &a::XXX, &a::IMP, 6 },
		/* 0x1 */	{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IND_Y, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 }, { "ORA", &a::ORA, &a::ZP_X, 4 },{ "ASL", &a::ASL, &a::ZP_X, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABS_Y, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },  { "ORA", &a::ORA, &a::ABS_X, 4 },{ "ASL", &a::ASL, &a::ABS_X, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		/* 0x2 */	{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IND_X, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP, 3 },  { "AND", &a::AND, &a::ZP, 3 },  { "ROL", &a::ROL, &a::ZP, 5 },  { "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },  { "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },  { "AND", &a::AND, &a::ABS, 4 },  { "ROL", &a::ROL, &a::ABS, 6 },  { "???", &a::XXX, &a::IMP, 6 },
		/* 0x3 */	{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IND_Y, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 }, { "AND", &a::AND, &a::ZP_X, 4 },{ "ROL", &a::ROL, &a::ZP_X, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABS_Y, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },  { "AND", &a::AND, &a::ABS_X, 4 },{ "ROL", &a::ROL, &a::ABS_X, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		/* 0x4 */	{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IND_X, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 }, { "EOR", &a::EOR, &a::ZP, 3 },  { "LSR", &a::LSR, &a::ZP, 5 },  { "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },  { "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },  { "EOR", &a::EOR, &a::ABS, 4 },  { "LSR", &a::LSR, &a::ABS, 6 },  { "???", &a::XXX, &a::IMP, 6 },
		/* 0x5 */	{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IND_Y, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 }, { "EOR", &a::EOR, &a::ZP_X, 4 },{ "LSR", &a::LSR, &a::ZP_X, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABS_Y, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },  { "EOR", &a::EOR, &a::ABS_X, 4 },{ "LSR", &a::LSR, &a::ABS_X, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		/* 0x6 */	{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IND_X, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 }, { "ADC", &a::ADC, &a::ZP, 3 },  { "ROR", &a::ROR, &a::ZP, 5 },  { "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },  { "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },  { "ADC", &a::ADC, &a::ABS, 4 },  { "ROR", &a::ROR, &a::ABS, 6 },  { "???", &a::XXX, &a::IMP, 6 },
		/* 0x7 */	{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IND_Y, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 }, { "ADC", &a::ADC, &a::ZP_X, 4 },{ "ROR", &a::ROR, &a::ZP_X, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABS_Y, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },  { "ADC", &a::ADC, &a::ABS_X, 4 },{ "ROR", &a::ROR, &a::ABS_X, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		/* 0x8 */	{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IND_X, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP, 3 },  { "STA", &a::STA, &a::ZP, 3 },  { "STX", &a::STX, &a::ZP, 3 },  { "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },  { "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },  { "STA", &a::STA, &a::ABS, 4 },  { "STX", &a::STX, &a::ABS, 4 },  { "???", &a::XXX, &a::IMP, 4 },
		/* 0x9 */	{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IND_Y, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP_X, 4 },{ "STA", &a::STA, &a::ZP_X, 4 },{ "STX", &a::STX, &a::ZP_Y, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABS_Y, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },  { "STA", &a::STA, &a::ABS_X, 5 },{ "???", &a::XXX, &a::IMP, 5 },  { "???", &a::XXX, &a::IMP, 5 },
		/* 0xA */	{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IND_X, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP, 3 },  { "LDA", &a::LDA, &a::ZP, 3 },  { "LDX", &a::LDX, &a::ZP, 3 },  { "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },  { "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },  { "LDA", &a::LDA, &a::ABS, 4 },  { "LDX", &a::LDX, &a::ABS, 4 },  { "???", &a::XXX, &a::IMP, 4 },
		/* 0xB */	{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IND_Y, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZP_X, 4 },{ "LDA", &a::LDA, &a::ZP_X, 4 },{ "LDX", &a::LDX, &a::ZP_Y, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABS_Y, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABS_X, 4 },{ "LDA", &a::LDA, &a::ABS_X, 4 },{ "LDX", &a::LDX, &a::ABS_Y, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		/* 0xC */	{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IND_X, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP, 3 },  { "CMP", &a::CMP, &a::ZP, 3 },  { "DEC", &a::DEC, &a::ZP, 5 },  { "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },  { "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },  { "CMP", &a::CMP, &a::ABS, 4 },  { "DEC", &a::DEC, &a::ABS, 6 },  { "???", &a::XXX, &a::IMP, 6 },
		/* 0xD */	{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IND_Y, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 }, { "CMP", &a::CMP, &a::ZP_X, 4 },{ "DEC", &a::DEC, &a::ZP_X, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABS_Y, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },  { "CMP", &a::CMP, &a::ABS_X, 4 },{ "DEC", &a::DEC, &a::ABS_X, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		/* 0xE */	{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IND_X, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP, 3 },  { "SBC", &a::SBC, &a::ZP, 3 },  { "INC", &a::INC, &a::ZP, 5 },  { "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },  { "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },  { "SBC", &a::SBC, &a::ABS, 4 },  { "INC", &a::INC, &a::ABS, 6 },  { "???", &a::XXX, &a::IMP, 6 },
		/* 0xF */	{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IND_Y, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 }, { "SBC", &a::SBC, &a::ZP_X, 4 },{ "INC", &a::INC, &a::ZP_X, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABS_Y, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },  { "SBC", &a::SBC, &a::ABS_X, 4 },{ "INC", &a::INC, &a::ABS_X, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

CPU6502::~CPU6502() {
	// destructor
}


uint8_t CPU6502::read(uint16_t a) {
	return bus->cRead(a, false);
}

// Writes a byte to the bus at the specified address
void CPU6502::write(uint16_t a, uint8_t d) {
	bus->cWrite(a, d);
}

// reset: configures cpu to known state
void CPU6502::reset() {
	// Get addr to set pc to
	addrAbsolute = 0xFFFC;
	uint16_t lo = read(addrAbsolute + 0);
	uint16_t hi = read(addrAbsolute + 1);

	// Set pc
	pc = (hi << 8) | lo;

	// Reset internal registers
	a = 0;
	x = 0;
	y = 0;
	sp = 0xFD;
	status = 0x00 | U;

	// Clear internal helper variables
	addrRelative = 0x0000;
	addrAbsolute = 0x0000;
	fetched = 0x00;

	// Reset takes time
	cycles = 8;
}


// interrupt
void CPU6502::interruptRequest() {
	// If interrupts are allowed
	if (GetFlag(I) == 0)
	{
		// Push the program counter to the stack. It's 16-bits
		// two writes because it's 16 bits
		write(0x0100 + sp, (pc >> 8) & 0x00FF);
		sp--;
		write(0x0100 + sp, pc & 0x00FF);
		sp--;

		// push status register to stack
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(0x0100 + sp, status);
		sp--;

		// get new pc from hardcoded address
		addrAbsolute = 0xFFFE;
		uint16_t lo = read(addrAbsolute + 0);
		uint16_t hi = read(addrAbsolute + 1);
		pc = (hi << 8) | lo;

		// IRQs take time
		cycles = 7;
	}
}


// nonmaskable interrupts
void CPU6502::nonmaskable() {
	// write current pc
	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	// set status register -> push to stack
	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	write(0x0100 + sp, status);
	sp--;

	addrAbsolute = 0xFFFA;
	uint16_t lo = read(addrAbsolute + 0);
	uint16_t hi = read(addrAbsolute + 1);
	pc = (hi << 8) | lo;

	cycles = 8;
}

// Perform one clock cycles worth of emulation
void CPU6502::clock() {
	if (cycles == 0)
	{
		// Read next instruction byte. 
		opcode = read(pc);

		// set the unused status flag bit to 1
		SetFlag(U, true);

		// Increment pc to read opcode byte
		pc++;

		// Get Starting number of cycles
		cycles = search[opcode].cycles;

		//opcode to get function -- if function needs additional clock cycles [fetch]
		uint8_t additional_cycle1 = (this->*search[opcode].addrmode)();

		// Perform operation
		uint8_t additional_cycle2 = (this->*search[opcode].operate)();

		// The addressmode and opcode may have altered the number
		// of cycles this instruction requires before its completed
		cycles += (additional_cycle1 & additional_cycle2);

		// Always set the unused status flag bit to 1
		SetFlag(U, true);
	}

	// Increment global clock count (for debugging)
	clock_count++;

	// Decrement the number of cycles remaining for this instruction
	cycles--;
}





// FLAG -----------------------------------------
// returns the specific bit of the status register
uint8_t CPU6502::GetFlag(STATUS_FLAGS f) {
	return ((status & f) > 0) ? 1 : 0;
}

// sets/clear a specific bit of the status register 
void CPU6502::SetFlag(STATUS_FLAGS f, bool v) {
	if (v)
		status |= f;
	else
		status &= ~f;
}





// Addressing Modes ----------------------------
// Implied
uint8_t CPU6502::IMP() {
	fetched = a;
	return 0;
}


// Immediate
// The instruction expects the next byte to be used as a value
uint8_t CPU6502::IMM() {
	addrAbsolute = pc++;
	return 0;
}



// Zero Page 
// byte of data interested in can be found in page 0
uint8_t CPU6502::ZP() {
	addrAbsolute = read(pc);
	pc++;
	addrAbsolute &= 0x00FF;
	return 0;
}



// Zero Page with X offset
uint8_t CPU6502::ZP_X() {
	// addr supplied w/ instruction is added to register x (useful to iterate through regions)
	addrAbsolute = (read(pc) + x);
	pc++;
	addrAbsolute &= 0x00FF;
	return 0;
}


// Zero Page with Y offset
uint8_t CPU6502::ZP_Y() {
	// addr supplied w/ instruction is added to register y (useful to iterate through regions)
	addrAbsolute = (read(pc) + y);
	pc++;
	addrAbsolute &= 0x00FF;
	return 0;
}


// Relative 
uint8_t CPU6502::REL() {
	addrRelative = read(pc);
	pc++;
	// needs to be signed 
	if (addrRelative & 0x80)
		addrRelative |= 0xFF00;
	return 0;
}


// Absolute 
// forming a 16-bit address 
uint8_t CPU6502::ABS() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addrAbsolute = (hi << 8) | lo; // form a 16 bit address word

	return 0;
}


// Addressing with X offset
uint8_t CPU6502::ABS_X() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addrAbsolute = (hi << 8) | lo; // form a 16 bit address word
	addrAbsolute += x; // contents of x register is added to the supplied 2 byte address

	if ((addrAbsolute & 0xFF00) != (hi << 8))
		return 1; //need to indicate an additional clock cycle if it went to a different page (overflow)
	else
		return 0;
}


// Addressing with Y offset
uint8_t CPU6502::ABS_Y() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addrAbsolute = (hi << 8) | lo;
	addrAbsolute += y;

	if ((addrAbsolute & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// Indirect
uint8_t CPU6502::IND() {
	// assemble 16-bit address that stores another address 
	uint16_t ptr_lo = read(pc);
	pc++;
	uint16_t ptr_hi = read(pc);
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF) {
		addrAbsolute = (read(ptr & 0xFF00) << 8) | read(ptr + 0); // Simulate page boundary hardware bug
	}
	else {
		// read 16-bit data at the original address 
		addrAbsolute = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}


// Indirect X
uint8_t CPU6502::IND_X() {
	uint16_t t = read(pc);
	pc++;

	//offset location by x register to get the actual 16-bit address for the instruction
	uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

	addrAbsolute = (hi << 8) | lo;

	return 0;
}


// Indirect Y
uint8_t CPU6502::IND_Y() {
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addrAbsolute = (hi << 8) | lo;
	addrAbsolute += y;

	// possible page boundary
	if ((addrAbsolute & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}


uint8_t CPU6502::fetch() {
	if (!(search[opcode].addrmode == &CPU6502::IMP))
		fetched = read(addrAbsolute);
	return fetched;
}





// Instructions ---------------------------------------------------------

// add
uint8_t CPU6502::ADC() {
	// Grab the data that we are adding to the accumulator
	fetch();

	// Add is performed in 16-bit domain for emulation to capture any carry bit, which will exist in bit 8 of the 16-bit word
	temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);

	// The carry flag out exists in the high byte bit 0
	SetFlag(C, temp > 255);

	// The Zero flag is set if the result is 0
	SetFlag(Z, (temp & 0x00FF) == 0);

	// Overflow --------------------------------------
	// A  M  R  |  V  |  A ^ R  |  A ^ M  |  ~(A^M)  |
	// 0  0  0  |  0  |    0    |    0    |     1    |
	// 0  0  1  |  1  |    1    |    0    |     1    |
	// 0  1  0  |  0  |    0    |    1    |     0    |
	// 0  1  1  |  0  |    1    |    1    |     0    |
	// 1  0  0  |  0  |    1    |    1    |     0    |
	// 1  0  1  |  0  |    0    |    1    |     0    |
	// 1  1  0  |  1  |    1    |    0    |     1    |
	// 1  1  1  |  0  |    0    |    0    |     1    |
	// V (overflow) = ~(A^M) & (A^R)
	SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

	// The negative flag is set to the most significant bit of the result
	SetFlag(N, temp & 0x80);

	// Load the result into the accumulator (it's 8-bit dont forget!)
	a = temp & 0x00FF;

	// This instruction has the potential to require an additional clock cycle
	return 1;
}


// subtraction
// A = A - M - (1 - C)  ->  A = A + -1 * (M - (1 - C))  ->  A = A + (-M + 1 + C)
uint8_t CPU6502::SBC() {
	fetch();

	// Operating in 16-bit domain to capture carry out

	// We can invert the bottom 8 bits with bitwise xor
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

	// Notice this is exactly the same as addition from here!
	temp = (uint16_t)a + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, ((temp & 0x00FF) == 0));
	SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	SetFlag(N, temp & 0x0080);
	a = temp & 0x00FF;
	return 1;
}

// Bitwise Logic AND
// Function:    A = A & M
// Flags Out:   N, Z
uint8_t CPU6502::AND() {
	fetch();
	a = a & fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}


// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t CPU6502::ASL()
{
	fetch();
	temp = (uint16_t)fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x80);
	if (search[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addrAbsolute, temp & 0x00FF);
	return 0;
}


// Instruction: Branch if Carry Clear
// Function:    if(C == 0) pc = address 
uint8_t CPU6502::BCC()
{
	if (GetFlag(C) == 0)
	{
		cycles++;
		addrAbsolute = pc + addrRelative;

		if ((addrAbsolute & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addrAbsolute;
	}
	return 0;
}


// Instruction: Branch if Carry Set
// Function:    if(C == 1) pc = address
uint8_t CPU6502::BCS()
{
	if (GetFlag(C) == 1)
	{
		cycles++;
		addrAbsolute = pc + addrRelative;

		if ((addrAbsolute & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addrAbsolute;
	}
	return 0;
}


// Instruction: Branch if Equal
// Function:    if(Z == 1) pc = address
uint8_t CPU6502::BEQ()
{
	if (GetFlag(Z) == 1)
	{
		cycles++;
		addrAbsolute = pc + addrRelative;

		if ((addrAbsolute & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addrAbsolute;
	}
	return 0;
}

uint8_t CPU6502::BIT()
{
	fetch();
	temp = a & fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(V, fetched & (1 << 6));
	return 0;
}


// Instruction: Branch if Negative
// Function:    if(N == 1) pc = address
uint8_t CPU6502::BMI()
{
	if (GetFlag(N) == 1)
	{
		cycles++;
		addrAbsolute = pc + addrRelative;

		if ((addrAbsolute & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addrAbsolute;
	}
	return 0;
}


// Instruction: Branch if Not Equal
// Function:    if(Z == 0) pc = address
uint8_t CPU6502::BNE()
{
	if (GetFlag(Z) == 0)
	{
		cycles++;
		addrAbsolute = pc + addrRelative;

		if ((addrAbsolute & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addrAbsolute;
	}
	return 0;
}


// Instruction: Branch if Positive
// Function:    if(N == 0) pc = address
uint8_t CPU6502::BPL()
{
	if (GetFlag(N) == 0)
	{
		cycles++;
		addrAbsolute = pc + addrRelative;

		if ((addrAbsolute & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addrAbsolute;
	}
	return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t CPU6502::BRK()
{
	pc++;

	SetFlag(I, 1);
	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	SetFlag(B, 1);
	write(0x0100 + sp, status);
	sp--;
	SetFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}


// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) pc = address
uint8_t CPU6502::BVC()
{
	if (GetFlag(V) == 0)
	{
		cycles++;
		addrAbsolute = pc + addrRelative;

		if ((addrAbsolute & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addrAbsolute;
	}
	return 0;
}


// Instruction: Branch if Overflow Set
// Function:    if(V == 1) pc = address
uint8_t CPU6502::BVS()
{
	if (GetFlag(V) == 1)
	{
		cycles++;
		addrAbsolute = pc + addrRelative;

		if ((addrAbsolute & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addrAbsolute;
	}
	return 0;
}


// Instruction: Clear Carry Flag
// Function:    C = 0
uint8_t CPU6502::CLC()
{
	SetFlag(C, false);
	return 0;
}


// Instruction: Clear Decimal Flag
// Function:    D = 0
uint8_t CPU6502::CLD()
{
	SetFlag(D, false);
	return 0;
}


// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t CPU6502::CLI()
{
	SetFlag(I, false);
	return 0;
}


// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t CPU6502::CLV()
{
	SetFlag(V, false);
	return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CMP()
{
	fetch();
	temp = (uint16_t)a - (uint16_t)fetched;
	SetFlag(C, a >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 1;
}


// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CPX()
{
	fetch();
	temp = (uint16_t)x - (uint16_t)fetched;
	SetFlag(C, x >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CPY()
{
	fetch();
	temp = (uint16_t)y - (uint16_t)fetched;
	SetFlag(C, y >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEC()
{
	fetch();
	temp = fetched - 1;
	write(addrAbsolute, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEX()
{
	x--;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}


// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEY()
{
	y--;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}


// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t CPU6502::EOR()
{
	fetch();
	a = a ^ fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}


// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t CPU6502::INC()
{
	fetch();
	temp = fetched + 1;
	write(addrAbsolute, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t CPU6502::INX()
{
	x++;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}


// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t CPU6502::INY()
{
	y++;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}


// Instruction: Jump To Location
// Function:    pc = address
uint8_t CPU6502::JMP()
{
	pc = addrAbsolute;
	return 0;
}


// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t CPU6502::JSR()
{
	pc--;

	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	pc = addrAbsolute;
	return 0;
}


// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t CPU6502::LDA()
{
	fetch();
	a = fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}


// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t CPU6502::LDX()
{
	fetch();
	x = fetched;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 1;
}


// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t CPU6502::LDY()
{
	fetch();
	y = fetched;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 1;
}

uint8_t CPU6502::LSR()
{
	fetch();
	SetFlag(C, fetched & 0x0001);
	temp = fetched >> 1;
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (search[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addrAbsolute, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::NOP()
{
	// Sadly not all NOPs are equal, Ive added a few here
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
	// and will add more based on game compatibility, and ultimately
	// I'd like to cover all illegal opcodes too
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t CPU6502::ORA()
{
	fetch();
	a = a | fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}


// Instruction: Push Accumulator to Stack
// Function:    A -> stack
uint8_t CPU6502::PHA()
{
	write(0x0100 + sp, a);
	sp--;
	return 0;
}


// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t CPU6502::PHP()
{
	write(0x0100 + sp, status | B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	sp--;
	return 0;
}


// Instruction: Pop Accumulator off Stack
// Function:    A <- stack
// Flags Out:   N, Z
uint8_t CPU6502::PLA()
{
	sp++;
	a = read(0x0100 + sp);
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}


// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t CPU6502::PLP()
{
	sp++;
	status = read(0x0100 + sp);
	SetFlag(U, 1);
	return 0;
}

uint8_t CPU6502::ROL()
{
	fetch();
	temp = (uint16_t)(fetched << 1) | GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (search[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addrAbsolute, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::ROR()
{
	fetch();
	temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	if (search[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addrAbsolute, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::RTI()
{
	sp++;
	status = read(0x0100 + sp);
	status &= ~B;
	status &= ~U;

	sp++;
	pc = (uint16_t)read(0x0100 + sp);
	sp++;
	pc |= (uint16_t)read(0x0100 + sp) << 8;
	return 0;
}

uint8_t CPU6502::RTS()
{
	sp++;
	pc = (uint16_t)read(0x0100 + sp);
	sp++;
	pc |= (uint16_t)read(0x0100 + sp) << 8;

	pc++;
	return 0;
}




// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t CPU6502::SEC()
{
	SetFlag(C, true);
	return 0;
}


// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t CPU6502::SED()
{
	SetFlag(D, true);
	return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t CPU6502::SEI()
{
	SetFlag(I, true);
	return 0;
}


// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t CPU6502::STA()
{
	write(addrAbsolute, a);
	return 0;
}


// Instruction: Store X Register at Address
// Function:    M = X
uint8_t CPU6502::STX()
{
	write(addrAbsolute, x);
	return 0;
}


// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t CPU6502::STY()
{
	write(addrAbsolute, y);
	return 0;
}


// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t CPU6502::TAX()
{
	x = a;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t CPU6502::TAY()
{
	y = a;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}


// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t CPU6502::TSX()
{
	x = sp;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t CPU6502::TXA()
{
	a = x;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t CPU6502::TXS()
{
	sp = x;
	return 0;
}


// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t CPU6502::TYA()
{
	a = y;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}


// This function captures illegal opcodes
uint8_t CPU6502::XXX()
{
	return 0;
}





///////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS

bool CPU6502::complete()
{
	return cycles == 0;
}

// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> CPU6502::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
		{
			std::string s(d, '0');
			for (int i = d - 1; i >= 0; i--, n >>= 4)
				s[i] = "0123456789ABCDEF"[n & 0xF];
			return s;
		};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->cRead(addr, true); addr++;
		sInst += search[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (search[opcode].addrmode == &CPU6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (search[opcode].addrmode == &CPU6502::IMM)
		{
			value = bus->cRead(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (search[opcode].addrmode == &CPU6502::ZP)
		{
			lo = bus->cRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (search[opcode].addrmode == &CPU6502::ZP_X)
		{
			lo = bus->cRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (search[opcode].addrmode == &CPU6502::ZP_Y)
		{
			lo = bus->cRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (search[opcode].addrmode == &CPU6502::IND_X)
		{
			lo = bus->cRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (search[opcode].addrmode == &CPU6502::IND_Y)
		{
			lo = bus->cRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (search[opcode].addrmode == &CPU6502::ABS)
		{
			lo = bus->cRead(addr, true); addr++;
			hi = bus->cRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (search[opcode].addrmode == &CPU6502::ABS_X)
		{
			lo = bus->cRead(addr, true); addr++;
			hi = bus->cRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (search[opcode].addrmode == &CPU6502::ABS_Y)
		{
			lo = bus->cRead(addr, true); addr++;
			hi = bus->cRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (search[opcode].addrmode == &CPU6502::IND)
		{
			lo = bus->cRead(addr, true); addr++;
			hi = bus->cRead(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (search[opcode].addrmode == &CPU6502::REL)
		{
			value = bus->cRead(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}

// End of File - Jx9
