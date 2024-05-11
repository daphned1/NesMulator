#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Memory {
    static constexpr u32 MAX_MEMORY = 64 * 1024;
    Byte Data[MAX_MEMORY];

    void Initialize() {
        for (auto &i : Data) {
            i = 0x00;
        }
    }
};

struct CPU {
    Word PC; //program counter
    Word SP; //stack pointer 

    // registers
    Byte A;
    Byte X;
    Byte Y;

    //status flags 
    Byte C : 1;     // carry
    Byte Z : 1;     // zero 
    Byte I : 1;     // interrupt
    Byte D : 1;     // decimal 
    Byte B : 1;     // break
    Byte V : 1;     // overflow
    Byte N : 1;     // negative

    void Reset(Memory &memory) {
        PC = 0xFFFC;
        C = Z = I = D = B = V = N = 0;
        SP = 0x0100;
        A = 0;
        X = 0;
        Y = 0;
        memory.Initialize();
    }
};

int main() {
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);
    //cpu.Execute(memory);
    return 0;
}