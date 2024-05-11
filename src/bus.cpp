#include "bus.h"

Bus::Bus() {
    // clear ram
    for (auto &i : ram) {
        i = 0x00;
    }

    // connect cpu to bus
    cpu.connect(this);
    
}

Bus::~Bus() {

}

uint8_t Bus::read(uint16_t address, bool read) {
    if (address >= 0x0000 && address < 0xFFFF) {
        return ram[address];
    }

    return 0x00; 
}

void Bus::write(uint16_t address, uint8_t val) {
    // full range 
    if (address >= 0x0000 && address < 0xFFFF) {
        ram[address] = val;
    }
}