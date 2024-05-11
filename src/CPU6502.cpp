#include <cstdint>
#include "CPU6502.h"
#include "bus.h"

CPU6502::CPU6502() {
    // opcode instruction matrix
    using a = CPU6502;
    search = {
        // MSD\LSD          0x0                           0x1                            0x2                            0x3                          0x4                            0x5                          0x6                            0x7                           0x8                          0x9                          0xA                           0xB                             0xC                                 0xD                             0xE                             0xF
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    /* 0x0 */ {"BRK", &a::BRK, &a::IMP, 7}, {"ORA", &a::ORA, &a::IND_X, 6}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::XXX, &a::IMP, 3},  {"ORA", &a::ORA, &a::ZP, 3},   {"ASL", &a::ASL, &a::ZP, 5},   {"???", &a::XXX, &a::IMP, 5}, {"PHP", &a::PHP, &a::IMP, 2}, {"ORA", &a::ORA, &a::IMM, 2},   {"ASL", &a::ASL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 4},   {"ORA", &a::ORA, &a::ABS, 4},   {"ASL", &a::ASL, &a::ABS, 6},   {"???", &a::XXX, &a::IMP, 6},
    /* 0x1 */ {"BPL", &a::BPL, &a::REL, 2}, {"ORA", &a::ORA, &a::IND_Y, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::XXX, &a::IMP, 4},  {"ORA", &a::ORA, &a::ZP_X, 4}, {"ASL", &a::ASL, &a::ZP_X, 6}, {"???", &a::XXX, &a::IMP, 6}, {"CLC", &a::CLC, &a::IMP, 2}, {"ORA", &a::ORA, &a::ABS_Y, 4}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4},   {"ORA", &a::ORA, &a::ABS_X, 4}, {"ASL", &a::ASL, &a::ABS_X, 7}, {"???", &a::XXX, &a::IMP, 7},
    /* 0x2 */ {"JSR", &a::JSR, &a::ABS, 6}, {"AND", &a::AND, &a::IND_X, 6}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"BIT", &a::BIT, &a::ZP, 3},   {"AND", &a::AND, &a::ZP, 3},   {"ROL", &a::ROL, &a::ZP, 5},   {"???", &a::XXX, &a::IMP, 5}, {"PLP", &a::PLP, &a::IMP, 4}, {"AND", &a::AND, &a::IMM, 2},   {"ROL", &a::ROL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"BIT", &a::BIT, &a::ABS, 4},   {"AND", &a::AND, &a::ABS, 4},   {"ROL", &a::ROL, &a::ABS, 6},   {"???", &a::XXX, &a::IMP, 6},
    /* 0x3 */ {"BMI", &a::BMI, &a::REL, 2}, {"AND", &a::AND, &a::IND_Y, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},  {"AND", &a::AND, &a::ZP_X, 4}, {"ROL", &a::ROL, &a::ZP_X, 6}, {"???", &a::XXX, &a::IMP, 6}, {"SEC", &a::SEC, &a::IMP, 2}, {"AND", &a::AND, &a::ABS_Y, 4}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4},   {"AND", &a::AND, &a::ABS_X, 4}, {"ROL", &a::ROL, &a::ABS_X, 7}, {"???", &a::XXX, &a::IMP, 7},
    /* 0x4 */ {"RTI", &a::RTI, &a::IMP, 6}, {"EOR", &a::EOR, &a::IND_X, 6}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 3},  {"EOR", &a::EOR, &a::ZP, 3},   {"LSR", &a::LSR, &a::ZP, 5},   {"???", &a::XXX, &a::IMP, 5}, {"PHA", &a::PHA, &a::IMP, 3}, {"EOR", &a::EOR, &a::IMM, 2},   {"LSR", &a::LSR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"JMP", &a::JMP, &a::ABS, 3},   {"EOR", &a::EOR, &a::ABS, 4},   {"LSR", &a::LSR, &a::ABS, 6},   {"???", &a::XXX, &a::IMP, 6},
    /* 0x5 */ {"BVC", &a::BVC, &a::REL, 2}, {"EOR", &a::EOR, &a::IND_Y, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},  {"EOR", &a::EOR, &a::ZP_X, 4}, {"LSR", &a::LSR, &a::ZP_X, 6}, {"???", &a::XXX, &a::IMP, 6}, {"CLI", &a::CLI, &a::IMP, 2}, {"EOR", &a::EOR, &a::ABS_Y, 4}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4},   {"EOR", &a::EOR, &a::ABS_X, 4}, {"LSR", &a::LSR, &a::ABS_X, 7}, {"???", &a::XXX, &a::IMP, 7},
    /* 0x6 */ {"RTS", &a::RTS, &a::IMP, 6}, {"ADC", &a::ADC, &a::IND_X, 6}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 3},  {"ADC", &a::ADC, &a::ZP, 3},   {"ROR", &a::ROR, &a::ZP, 5},   {"???", &a::XXX, &a::IMP, 5}, {"PLA", &a::PLA, &a::IMP, 4}, {"ADC", &a::ADC, &a::IMM, 2},   {"ROR", &a::ROR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"JMP", &a::JMP, &a::IND, 5},   {"ADC", &a::ADC, &a::ABS, 4},   {"ROR", &a::ROR, &a::ABS, 6},   {"???", &a::XXX, &a::IMP, 6},
    /* 0x7 */ {"BVS", &a::BVS, &a::REL, 2}, {"ADC", &a::ADC, &a::IND_Y, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},  {"ADC", &a::ADC, &a::ZP_X, 4}, {"ROR", &a::ROR, &a::ZP_X, 6}, {"???", &a::XXX, &a::IMP, 6}, {"SEI", &a::SEI, &a::IMP, 2}, {"ADC", &a::ADC, &a::ABS_Y, 4}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4},   {"ADC", &a::ADC, &a::ABS_X, 4}, {"ROR", &a::ROR, &a::ABS_X, 7}, {"???", &a::XXX, &a::IMP, 7},
    /* 0x8 */ {"???", &a::NOP, &a::IMP, 2}, {"STA", &a::STA, &a::IND_X, 6}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6}, {"STY", &a::STY, &a::ZP, 3},   {"STA", &a::STA, &a::ZP, 3},   {"STX", &a::STX, &a::ZP, 3},   {"???", &a::XXX, &a::IMP, 3}, {"DEY", &a::DEY, &a::IMP, 2}, {"???", &a::NOP, &a::IMP, 2},   {"TXA", &a::TXA, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"STY", &a::STY, &a::ABS, 4},   {"STA", &a::STA, &a::ABS, 4},   {"STX", &a::STX, &a::ABS, 4},   {"???", &a::XXX, &a::IMP, 4},
    /* 0x9 */ {"BCC", &a::BCC, &a::REL, 2}, {"STA", &a::STA, &a::IND_Y, 6}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6}, {"STY", &a::STY, &a::ZP_X, 4}, {"STA", &a::STA, &a::ZP_X, 4}, {"STX", &a::STX, &a::ZP_Y, 4}, {"???", &a::XXX, &a::IMP, 4}, {"TYA", &a::TYA, &a::IMP, 2}, {"STA", &a::STA, &a::ABS_Y, 5}, {"TXS", &a::TXS, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5}, {"???", &a::NOP, &a::IMP, 5},   {"STA", &a::STA, &a::ABS_X, 5}, {"???", &a::XXX, &a::IMP, 5},   {"???", &a::XXX, &a::IMP, 5},
    /* 0xA */ {"LDY", &a::LDY, &a::IMM, 2}, {"LDA", &a::LDA, &a::IND_X, 6}, {"LDX", &a::LDX, &a::IMM, 2}, {"???", &a::XXX, &a::IMP, 6}, {"LDY", &a::LDY, &a::ZP, 3},   {"LDA", &a::LDA, &a::ZP, 3},   {"LDX", &a::LDX, &a::ZP, 3},   {"???", &a::XXX, &a::IMP, 3}, {"TAY", &a::TAY, &a::IMP, 2}, {"LDA", &a::LDA, &a::IMM, 2},   {"TAX", &a::TAX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"LDY", &a::LDY, &a::ABS, 4},   {"LDA", &a::LDA, &a::ABS, 4},   {"LDX", &a::LDX, &a::ABS, 4},   {"???", &a::XXX, &a::IMP, 4},
    /* 0xB */ {"BCS", &a::BCS, &a::REL, 2}, {"LDA", &a::LDA, &a::IND_Y, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5}, {"LDY", &a::LDY, &a::ZP_X, 4}, {"LDA", &a::LDA, &a::ZP_X, 4}, {"LDX", &a::LDX, &a::ZP_Y, 4}, {"???", &a::XXX, &a::IMP, 4}, {"CLV", &a::CLV, &a::IMP, 2}, {"LDA", &a::LDA, &a::ABS_Y, 4}, {"TSX", &a::TSX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 4}, {"LDY", &a::LDY, &a::ABS_X, 4}, {"LDA", &a::LDA, &a::ABS_X, 4}, {"LDX", &a::LDX, &a::ABS_Y, 4}, {"???", &a::XXX, &a::IMP, 4},
    /* 0xC */ {"CPY", &a::CPY, &a::IMM, 2}, {"CMP", &a::CMP, &a::IND_X, 6}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"CPY", &a::CPY, &a::ZP, 3},   {"CMP", &a::CMP, &a::ZP, 3},   {"DEC", &a::DEC, &a::ZP, 5},   {"???", &a::XXX, &a::IMP, 5}, {"INY", &a::INY, &a::IMP, 2}, {"CMP", &a::CMP, &a::IMM, 2},   {"DEX", &a::DEX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"CPY", &a::CPY, &a::ABS, 4},   {"CMP", &a::CMP, &a::ABS, 4},   {"DEC", &a::DEC, &a::ABS, 6},   {"???", &a::XXX, &a::IMP, 6},
    /* 0xD */ {"BNE", &a::BNE, &a::REL, 2}, {"CMP", &a::CMP, &a::IND_Y, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::XXX, &a::IMP, 4},  {"CMP", &a::CMP, &a::ZP_X, 4}, {"DEC", &a::DEC, &a::ZP_X, 6}, {"???", &a::XXX, &a::IMP, 6}, {"CLD", &a::CLD, &a::IMP, 2}, {"CMP", &a::CMP, &a::ABS_Y, 4}, {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4},   {"CMP", &a::CMP, &a::ABS_X, 4}, {"DEC", &a::DEC, &a::ABS_X, 7}, {"???", &a::XXX, &a::IMP, 7},
    /* 0xE */ {"CPX", &a::CPX, &a::IMM, 2}, {"SBC", &a::SBC, &a::IND_X, 6}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"CPX", &a::CPX, &a::ZP, 3},   {"SBC", &a::SBC, &a::ZP, 3},   {"INC", &a::INC, &a::ZP, 5},   {"???", &a::XXX, &a::IMP, 5}, {"INX", &a::INX, &a::IMP, 2}, {"SBC", &a::SBC, &a::IMM, 2},   {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::SBC, &a::IMP, 2}, {"CPX", &a::CPX, &a::ABS, 4},   {"SBC", &a::SBC, &a::ABS, 4},   {"INC", &a::INC, &a::ABS, 6},   {"???", &a::XXX, &a::IMP, 6},
    /* 0xF */ {"BEQ", &a::BEQ, &a::REL, 2}, {"SBC", &a::SBC, &a::IND_Y, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},  {"SBC", &a::SBC, &a::ZP_X, 4}, {"INC", &a::INC, &a::ZP_X, 6}, {"???", &a::XXX, &a::IMP, 6}, {"SED", &a::SED, &a::IMP, 2}, {"SBC", &a::SBC, &a::ABS_Y, 4}, {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4},   {"SBC", &a::SBC, &a::ABS_X, 4}, {"INC", &a::INC, &a::ABS_X, 7}, {"???", &a::XXX, &a::IMP, 7},
    };

}

CPU6502::~CPU6502() {

}

uint8_t CPU6502::read(uint16_t addr) {
    return bus->read(addr, false);
}

void CPU6502::write(uint16_t addr, uint8_t data) {
    bus->write(addr, data);
}

void CPU6502::clock() {
    if (cycles = 0) {
        opcode = read(pc); // get opcode -> rteurns 1 byte
        pc++;

        // get starting number of cycles
        cycles = search[opcode].cycles;

        uint8_t additionalCycles = (this->*search[opcode].addrmode)(); //opcode to get function -- if function needs additional clock cycles [fetch]
        uint8_t additionalCycles2 =(this->*search[opcode].operate)(); // perform operation

        // if both needs additional clock cycles
        cycles += (additionalCycles & additionalCycles2);

        SetFlag(U, true);

    }

    cycles--;
}

// FLAG -----------------------------------------
// returns the specific bit of the status register
uint8_t CPU6502::GetFlag(STATUS_FLAGS f) {
    return((status & f) > 0) ? 1 : 0;
}

// sets/clear a specific bit of the status register 
void CPU6502::SetFlag(STATUS_FLAGS f, bool b) {
    if (b) {
        status |= f;
    }
    else {
        status &= ~f;
    }
}

// Addressing Modes ----------------------------
// Implied 
uint8_t CPU6502::IMP() {
    fetched = a;
    return 0;
}

// Immediate 
// expects next byte to be used as a value 
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
    addrAbsolute = (read(pc) + x); // addr supplied w/ instruction is added to register x (useful to iterate through regions)
    pc++;
    addrAbsolute &= 0x00FF;
    return 0;
}

// Zero Page with Y offset
uint8_t CPU6502::ZP_Y() {
    addrAbsolute = (read(pc) + y); // addr supplied w/ instruction is added to register y (useful to iterate through regions)
    pc++;
    addrAbsolute &= 0x00FF;
    return 0;
}

// Absolute 
// forming a 16-bit address 
uint8_t CPU6502::ABS() {
    uint16_t lo = read(pc);
    pc ++;
    uint16_t hi = read(pc);
    pc++;

    addrAbsolute = (hi << 8) | lo; // form a 16 bit address word
    return 0;
}

// Addressing with X offset
uint8_t CPU6502::ABS_X() {
    uint16_t lo = read(pc);
    pc ++;
    uint16_t hi = read(pc);
    pc++;

    addrAbsolute = (hi << 8) | lo; // form a 16 bit address word
    addrAbsolute += x; // contents of x register is added to the supplied 2 byte address

    if ((addrAbsolute & 0xFF00) != (hi << 8)) {
        return 1; //need to indicate an additional clock cycle if it went to a different page (overflow)
    }
    return 0;
}

// Addressing with Y offset
uint8_t CPU6502::ABS_Y() {
    uint16_t lo = read(pc);
    pc ++;
    uint16_t hi = read(pc);
    pc++;

    addrAbsolute = (hi << 8) | lo; // form a 16 bit address word
    addrAbsolute += y; // contents of y register is added to the supplied 2 byte address

    if ((addrAbsolute & 0xFF00) != (hi << 8)) {
        return 1; //need to indicate an additional clock cycle if it went to a different page (overflow)
    }
    return 0;
}

// Indirect 
uint8_t CPU6502:: IND() {
    // assemble 16-bit address that stores another address 
    uint16_t ptrLo = read(pc);
    pc++;
    uint16_t ptrHi = read(pc);
    pc++;

    uint16_t ptr = (ptrHi << 8) | ptrLo;

    // simulate page boundary bug
    if (ptrLo == 0x00FF) {
        addrAbsolute = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    }
    else {
        // read 16-bit data at the original address 
        addrAbsolute = (read(ptr + 1) << 8) | read(ptr + 0);
    }

    return 0;
}

// Indirect X
uint8_t CPU6502::IND_X() {
    uint16_t loc = read(pc);
    pc ++;

    //offset location by x register to get the actual 16-bit address for the instruction
    uint16_t lo = read((uint16_t)(loc + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((uint16_t)(loc + (uint16_t)x + 1) & 0x00FF);

    addrAbsolute = (hi << 8) | lo;

    return 0;
}

// Indirect Y
uint8_t CPU6502::IND_Y() {
    uint16_t loc = read(pc);
    pc++;

    uint16_t lo = read(loc & 0x00FF);
    uint16_t hi = read((loc + 1) & 0x00FF);

    addrAbsolute = (hi << 8) | lo;
    addrAbsolute += y;

    // possible page boundary
    if ((addrAbsolute & 0xFF00) != (hi << 8)) {
        return 1;
    }
    return 0;
}

// Relative 
uint8_t CPU6502::REL() {
    addrRelative = read(pc);
    pc++;
    // needs to be signed 
    if (addrRelative & 0x80) {
        addrRelative |= 0xFF00; 
    }
    return 0;
}

// Instructions ---------------------------------------------------------

uint8_t CPU6502::fetch() {
    // fetch all data from addressing modes besides implied (nothing to fetch)
    if (!(search[opcode].addrmode == &CPU6502::IMP)) {
        fetched = read(addrAbsolute);
    }
    return fetched;
}

uint8_t CPU6502::AND() {
    fetch(); // fetched
    a = a & fetched; //computed
    
    // update status register 
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Branch if carry set
uint8_t CPU6502::BCS() {
    if (GetFlag(C) == 1) {
        cycles++; // add 1 clock cycle if branch occurs to same page 
        addrAbsolute = pc + addrRelative; // set abs to pc + offset (relative addr)
         
        // if passes page boundary, it needs an additional clock cycle 
        if ((addrAbsolute & 0xFF00) != (pc & 0xFF00)) {
            cycles ++;
        }
        pc = addrAbsolute;
    }
    return 0;
}

// Branch if carry clear 
uint8_t CPU6502::BCC() {
    if (GetFlag(C) == 0) {
        cycles++; // add 1 clock cycle if branch occurs to same page 
        addrAbsolute = pc + addrRelative; // set abs to pc + offset (relative addr)
         
        // if passes page boundary, it needs an additional clock cycle 
        if ((addrAbsolute & 0xFF00) != (pc & 0xFF00)) {
            cycles ++;
        }
        pc = addrAbsolute;
    }
    return 0;
}

// Branch if equal
uint8_t CPU6502::BEQ() {
    if (GetFlag(Z) == 1) {
        cycles++; // add 1 clock cycle if branch occurs to same page 
        addrAbsolute = pc + addrRelative; // set abs to pc + offset (relative addr)
         
        // if passes page boundary, it needs an additional clock cycle 
        if ((addrAbsolute & 0xFF00) != (pc & 0xFF00)) {
            cycles ++;
        }
        pc = addrAbsolute;
    }
    return 0;
}

// Branch if Negative 
uint8_t CPU6502::BMI() {
    if (GetFlag(N) == 1) {
        cycles++; // add 1 clock cycle if branch occurs to same page 
        addrAbsolute = pc + addrRelative; // set abs to pc + offset (relative addr)
         
        // if passes page boundary, it needs an additional clock cycle 
        if ((addrAbsolute & 0xFF00) != (pc & 0xFF00)) {
            cycles ++;
        }
        pc = addrAbsolute;
    }
    return 0;
}

// Branch if not equal
uint8_t CPU6502::BNE() {
    if (GetFlag(Z) == 0) {
        cycles++; // add 1 clock cycle if branch occurs to same page 
        addrAbsolute = pc + addrRelative; // set abs to pc + offset (relative addr)
         
        // if passes page boundary, it needs an additional clock cycle 
        if ((addrAbsolute & 0xFF00) != (pc & 0xFF00)) {
            cycles ++;
        }
        pc = addrAbsolute;
    }
    return 0;
}

// Branch if positive 
uint8_t CPU6502::BPL() {
    if (GetFlag(N) == 0) {
        cycles++; // add 1 clock cycle if branch occurs to same page 
        addrAbsolute = pc + addrRelative; // set abs to pc + offset (relative addr)
         
        // if passes page boundary, it needs an additional clock cycle 
        if ((addrAbsolute & 0xFF00) != (pc & 0xFF00)) {
            cycles ++;
        }
        pc = addrAbsolute;
    }
    return 0;
}

// Branch if overflow clear 
uint8_t CPU6502::BVC() {
    if (GetFlag(V) == 0) {
        cycles++; // add 1 clock cycle if branch occurs to same page 
        addrAbsolute = pc + addrRelative; // set abs to pc + offset (relative addr)
         
        // if passes page boundary, it needs an additional clock cycle 
        if ((addrAbsolute & 0xFF00) != (pc & 0xFF00)) {
            cycles ++;
        }
        pc = addrAbsolute;
    }
    return 0;
}

// Branch if overflow set
uint8_t CPU6502::BVS() {
    if (GetFlag(V) == 1) {
        cycles++; // add 1 clock cycle if branch occurs to same page 
        addrAbsolute = pc + addrRelative; // set abs to pc + offset (relative addr)
         
        // if passes page boundary, it needs an additional clock cycle 
        if ((addrAbsolute & 0xFF00) != (pc & 0xFF00)) {
            cycles ++;
        }
        pc = addrAbsolute;
    }
    return 0;
}

uint8_t CPU6502::BIT() {
    fetch();
    uint8_t temp = a & fetched;
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, fetched & (1 << 7)); // set bit 7 to negative
    SetFlag(V, fetched & (1 << 6)); // set bit 6 to overflow
    return 0;

}

// Break
uint8_t CPU6502::BRK() {
    pc++;

    SetFlag(I, 1);
    write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp --;
    write(0x0100 + sp, pc & 0x00FF);
    sp --;

    SetFlag(B, 1);
    write(0x0100 + sp, status);
    sp--;
    SetFlag(B, 0);

    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}

// Set carry flag
uint8_t CPU6502::SEC() {
    SetFlag(C, true);
    return 0;
}

// Set decimal flag
uint8_t CPU6502::SED() {
    SetFlag(D, true);
    return 0;
}

// Set interrupt flag
uint8_t CPU6502::SEI() {
    SetFlag(I, true);
    return 0;
}

// Clear carry flag
uint8_t CPU6502::CLC() {
    SetFlag(C, false);
    return 0;
}

// Clear decimal flag
uint8_t CPU6502::CLD() {
    SetFlag(D, false);
    return 0;
}

// Clear interrupt flag
uint8_t CPU6502::CLI() {
    SetFlag(I, false);
    return 0;
}

// Clear overflow flag
uint8_t CPU6502::CLV() {
    SetFlag(V, false);
    return 0;
}

// Store accumulator at address
uint8_t CPU6502::STA() {
    write(addrAbsolute, a);
    return 0;
}

// Store x register at address
uint8_t CPU6502::STX() {
    write(addrAbsolute, x);
    return 0;
}

// Store y register at address
uint8_t CPU6502::STY() {
    write(addrAbsolute, y);
    return 0;
}

// transfer accumulator to x register
uint8_t CPU6502::TAX() {
    x = a;
    SetFlag(Z, x = 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// transfer accumulator to y register
uint8_t CPU6502::TAY() {
    y = a;
    SetFlag(Z, y = 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// transfer stack ptr to x register
uint8_t CPU6502::TSX() {
    x = sp;
    SetFlag(Z, x = 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// transfer x register to accumulator
uint8_t CPU6502::TXA() {
    a = x;
    SetFlag(Z, a = 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

// transfer x register to stack ptr
uint8_t CPU6502::TXS() {
    sp = x;
    return 0;
}

// transfer y register to accumulator
uint8_t CPU6502::TYA() {
    a = y;
    SetFlag(Z, a = 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

// Compare accumulator : C <- A >= M
uint8_t CPU6502::CMP() {
    fetch();
    uint16_t temp = (uint16_t)a - (uint16_t)fetched; // get the comparison
    SetFlag(C, a >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 1;
}

// compare x register : C <- X >= M
uint8_t CPU6502::CPX() {
    fetch();
    uint16_t temp = (uint16_t)x - (uint16_t)fetched;
    SetFlag(C, x >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

// compare y register : C <- Y >= M
uint8_t CPU6502::CPY() {
    fetch();
    uint16_t temp = (uint16_t)y - (uint16_t)fetched;
    SetFlag(C, y >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

// decrement value at memory location : M = M -1
uint8_t CPU6502::DEC() {
    fetch();
    uint8_t temp = fetched - 1;
    write(addrAbsolute, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

// decrement x register : X = X -1
uint8_t CPU6502::DEX() {
    x--;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// decrement y register : X = X -1
uint8_t CPU6502::DEY() {
    y--;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// bitwise logic XOR : A ⊕ M
uint8_t CPU6502::EOR() {
    fetch();
    a = a ^ fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// bitwise logic OR :  A = A | M
uint8_t CPU6502::ORA() {
    fetch();
    a = a | fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// increment value at memory location
uint8_t CPU6502::INC() {
    fetch();
    uint8_t temp = fetched + 1;
    write(addrAbsolute, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

// increment x register : X = X + 1
uint8_t CPU6502::INX() {
    x++;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// increment y register : y = y + 1
uint8_t CPU6502::INY() {
    y++;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// jump
uint8_t CPU6502::JMP() {
    pc = addrAbsolute;
    return 0;
}

// jump to subroutine
uint8_t CPU6502::JSR() {
    pc --;
    write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp --;
    write(0x0100 + sp, pc & 0x00FF);
    sp --;

    pc = addrAbsolute;
    return 0;
}

// load the accumulator
uint8_t CPU6502::LDA() {
    fetch();
    a = fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// load the x register
uint8_t CPU6502::LDX() {
    fetch();
    x = fetched;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 1;
}

// load the y register
uint8_t CPU6502::LDY() {
    fetch();
    y = fetched;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 1;
}

// logical shift right
uint8_t CPU6502::LSR() {
    fetch();
    SetFlag(C, fetched & 0x0001);
    uint8_t temp = fetched >> 1;
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x80);

    if (search[opcode].addrmode = &CPU6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addrAbsolute, temp &0x00FF);
    }

    return 0;
}

// no op
uint8_t CPU6502::NOP() {
    switch(opcode) {
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

// rotate left
uint8_t CPU6502::ROL() {
    fetch();
    uint16_t temp = (uint16_t)(fetched >> 1) | GetFlag(C);
    SetFlag(C, temp & 0xFF00);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);

    if (search[opcode].addrmode = &CPU6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addrAbsolute, temp &0x00FF);
    }

    return 0;
}

// rotate right
uint8_t CPU6502::ROR() {
    fetch();
    uint16_t temp = (uint16_t)(GetFlag(C) >> 7) | (fetched >> 1);
    SetFlag(C, fetched & 0x01);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);

    if (search[opcode].addrmode = &CPU6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addrAbsolute, temp &0x00FF);
    }

    return 0;
}

// addition : A = A + M + C
uint8_t CPU6502::ADC() {
    fetch();
    uint16_t temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C); // 16-bit to check carry bit
    SetFlag(C, temp > 255);
    SetFlag(Z, (temp & 0x00FF) == 0);
    SetFlag(N, temp & 0x80); // the most significant bit of the low byte of result 
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
    a = temp & 0x00FF; // store result in accumulator
    return 1;
}

// subtraction
uint8_t CPU6502::SBC() {
    fetch();
    uint16_t val = ((uint16_t)fetched) ^ 0x00FF; // invert the bits of data 
    uint16_t temp = (uint16_t)a + val + (uint16_t)GetFlag(C);
    SetFlag(C, temp > 255);
    SetFlag(Z, (temp & 0x00FF) == 0);
    SetFlag(N, temp & 0x0080); // the most significant bit of the low byte of result 
    SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ val) & 0x0080);
    a = temp & 0x00FF;
    return 1;
}

// Arithmetic Shift Left : A = C <- (A << 1) <-0
uint8_t CPU6502::ASL() {
    fetch();
    uint16_t temp = (uint16_t)fetched << 1; // 16-bit

    SetFlag(C, (temp & 0xFF00) > 0);
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x80);

    // fetch all from addr mode except implied 
    if (search[opcode].addrmode == &CPU6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addrAbsolute, temp & 0x00FF);
    }
    return 0;
}

// ILLEGAL OPCODE
uint8_t CPU6502::XXX() {
    return 0;
}


// Stack ------------------------------------------
// pushes accumulator to stack
uint8_t CPU6502::PHA() {
    write(0x0100 + sp, a); // access bus (0x0100 is base location)
    sp--;
    return 0;
}

// pop accumulator from stack
uint8_t CPU6502::PLA() {
    sp++;
    a = read(0x0100 + sp); // read from bus
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

// push status register to stack 
uint8_t CPU6502::PHP() {
    write(0x0100 + sp, status | B | U); // access bus (0x0100 is base location)
    SetFlag(B, 0);
    SetFlag(U, 0);
    sp--;
    return 0;
}

// pop register from stack
uint8_t CPU6502::PLP() {
    sp++;
    status = read(0x0100 + sp);
    SetFlag(U, 1);
    return 0;
}

// reset: configures cpu to known state
void CPU6502::reset() {
    a = 0;
    x = 0;
    y = 0;
    sp = 0xFD;
    status = 0x00 | U;

    addrAbsolute = 0xFFFC;
    uint16_t lo = read(addrAbsolute + 0);
    uint16_t hi = read(addrAbsolute + 1);
    pc = (hi << 8) | lo;

    addrRelative = 0x0000;
    addrAbsolute = 0x0000;
    fetched = 0x00;
    cycles = 8;
}

// interrupt
void CPU6502::interruptRequest() {
    if (GetFlag(I) == 0) {
        // current pc
        // two writes because it's 16 bits
        write(0x0100 + sp, (pc >> 8) & 0x00FF); 
        sp --;
        write(0x0100 + sp, pc & 0x00FF); 
        sp --; 

        // status register
        SetFlag(B, 0);
        SetFlag(U, 1);
        SetFlag(I, 1);
        write(0x0100 + sp, status); // write status to stack
        sp--;

        //hardcode address to get the new pc
        addrAbsolute = 0xFFFE;
        uint16_t lo = read(addrAbsolute + 0);
        uint16_t hi = read(addrAbsolute + 1);
        pc = (hi << 8) | lo;

        cycles = 7;
    }
}

void CPU6502::nonmaskable() {
    // write current pc
    write(0x0100 + sp, (pc << 8) & 0x00FF);
    sp --;
    write(0x0100 + sp, pc & 0x00FF);
    sp --;

    // set status register
    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    write(0x0100 + sp, status);
    sp--;

    addrAbsolute = 0xFFFA;
    uint16_t lo = read(addrAbsolute + 0);
    uint16_t hi = read(addrAbsolute + 1);
    pc = (hi >> 8) | lo;

    cycles = 8;
}

// return interrupt : restores state of processor before interrupt
uint8_t CPU6502::RTI() {
    sp++;
    status = read(0x0100 + sp);
    status &= ~B;
    status &= ~U;

    // read status register and previous pc
    sp++;
    pc = (uint16_t)read(0x0100 + sp);
    sp++;
    pc |= (uint16_t)read(0x0100 + sp) << 8;
    return 0;
}

// return subroutine 
uint8_t CPU6502::RTS() {
    // read status register and previous pc
    sp++;
    pc = (uint16_t)read(0x0100 + sp);
    sp++;
    pc |= (uint16_t)read(0x0100 + sp) << 8;
    pc++;
    return 0;
}

// disassemble
bool CPU6502::complete() {
    return cycles == 0;
}

std::map<uint16_t, std::string> CPU6502::disassemble(uint16_t nStart, uint16_t nStop) {
    uint32_t addr = nStart;
    uint8_t value = 0x00, lo = 0x00, hi = 0x00;
    std::map<uint16_t, std::string> mapLines;
    uint16_t line_addr = 0;

    auto hex = [](uint32_t n, uint8_t d) {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4) {
            s[i] = "0123456789ABCDEF"[n & 0xF];
        }
        return s;
    };

    while (addr <= (uint32_t)nStop) {
        line_addr = addr;

        std::string sInst = "$" + hex(addr, 4) + ": ";
        
        uint8_t opcode = bus->read(addr, true); addr++;
        sInst += search[opcode].name + " ";

        // get operands from desired locations
        if (search[opcode].addrmode == &CPU6502::IMP) {
            sInst += " {IMP}";
        }
        else if (search[opcode].addrmode == &CPU6502::IMM) {
            value = bus->read(addr, true); 
            addr++;
            sInst += "#$" + hex(value, 2) + " {IMM}";
        }
        else if (search[opcode].addrmode == &CPU6502::ZP) {
            lo = bus->read(addr, true); 
            addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + " {ZP}";
        }
        else if (search[opcode].addrmode == &CPU6502::ZP_X) {
            lo = bus->read(addr, true); 
            addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + " , X {ZPX}";
        }
        else if (search[opcode].addrmode == &CPU6502::ZP_Y) {
            lo = bus->read(addr, true); 
            addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + " , Y {ZPY}";
        }
        else if (search[opcode].addrmode == &CPU6502::IND_X) {
            lo = bus->read(addr, true); 
            addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + ", X) {IND_X}";
        }
        else if (search[opcode].addrmode == &CPU6502::IND_Y) {
            lo = bus->read(addr, true); 
            addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + "), Y {IND_Y}";
        }
        else if (search[opcode].addrmode == &CPU6502::ABS) {
            lo = bus->read(addr, true); 
            addr++;
            hi = bus->read(addr, true); 
            addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
        }
        else if (search[opcode].addrmode == &CPU6502::ABS_X) {
            lo = bus->read(addr, true); 
            addr++;
            hi = bus->read(addr, true); 
            addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABS_X}";
        }
        else if (search[opcode].addrmode == &CPU6502::ABS_Y) {
            lo = bus->read(addr, true); 
            addr++;
            hi = bus->read(addr, true); 
            addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABS_Y}";
        }
        else if (search[opcode].addrmode == &CPU6502::IND) {
            lo = bus->read(addr, true); 
            addr++;
            hi = bus->read(addr, true); 
            addr++;
            sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
        }
        else if (search[opcode].addrmode == &CPU6502::REL) {
            value = bus->read(addr, true);
            addr++;
            sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
        }

        mapLines[line_addr] = sInst;
    }
    return mapLines;
}