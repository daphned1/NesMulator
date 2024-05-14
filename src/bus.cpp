#include "bus.h"

Bus::Bus() {
    // clear ram
    //for (auto &i : cRam) {
    //    i = 0x00;
    //}

    // connect cpu to bus
    cpu.connect(this);
    
}

Bus::~Bus() {

}

uint8_t Bus::cRead(uint16_t address, bool read) {
    uint8_t data = 0x00;
    if (cart->cRead(address, data)) {
        // cartridge access range
    }
    else if (address >= 0x0000 && address <= 0x1FFF) { //8kb range
        data = cRam[address & 0x07FF];
    }
    else if (address >= 0x2000 && address <= 0x3FFF) {
        data = ppu.cRead(address & 0x0007, read);
    }
    else if (address >= 0x4016 && address <= 0x4017) {
        // return the msb
        data = (ctrller_state[address & 0x0001] & 0x80) > 0;
        ctrller_state[address & 0x0001] <<= 1;
    }

    return data;
}

void Bus::cWrite(uint16_t address, uint8_t val) {
    if (cart->cWrite(address, val)) {
        // cartridge decides 
        // ram doesn't get written to
        // ppu doesn't get updated
    }
    // full range 
    else if (address >= 0x0000 && address <= 0x1FFF) { //8kb range
        cRam[address & 0x07FF] = val;
    }
    else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu.cWrite(address & 0x0007, val);
    }
    else if (address >= 0x4016 && address <= 0x4017) {
        // takes a snapshot of the controller input
        // crtller_state = shift register
        ctrller_state[address & 0x0001] = controller[address & 0x0001];
    }
}

void Bus::insertCart(const std::shared_ptr<cartridge>& cartr) {
    this->cart = cartr;
    ppu.connectCart(cartr);
}

void Bus::reset() {
    cart->reset();
    cpu.reset();
    ppu.reset();
    systemClcCounter = 0;
}

void Bus::clock()
{
    ppu.clock();
    // cpu clock runs 3x slower than ppu
    if (systemClcCounter % 3 == 0) {
        cpu.clock();
    }

    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nonmaskable();
    }

    systemClcCounter++;
}
