#include "CPU.h"

#include <cstdio>
#include <iostream>
using namespace std;

#define TODO printf("Not implemented yet.")

CPU::CPU() { }

CPU::CPU(Memory* memory) {
	this->memory = memory;
	P = {0, 0, 1, 0, 0, 1, 0, 0};
	
	A = 0x00;
	X = 0x00;
	Y = 0x00;

	memory->write(0x4017, 0x00);
	memory->write(0x4015, 0x00);

	for (Word addr = 0x4000; addr <= 0x400F; addr++) {
		memory->write(addr, 0x00);
	}

	for (Word addr = 0x4010; addr <= 0x4013; addr++) {
		memory->write(addr, 0x00);
	}

	//PC = memory->read16(0xFFFC);
	PC = 0xC000;
	SP = 0xFD;

	initializeInstructionTable();
}

void CPU::step() {
	// Fetch
	Byte opcode = memory->read(PC++);

	// Decode
	Instr instr = InstrTable[opcode];
	
	//Execute
	Address data = (this->*instr.addr)();
	(this->*instr.code)(data);
}

void CPU::reset() {
	P.I = 1;
	memory->write(0x4015, 0);

	//PC = memory->read16(0xFFFC);
	PC = 0xC000;
	SP = 0xFD;
}

void CPU::printState() {
	printf("%04hX  %02hhX %02hhX %02hhX\t %s\t\t\t",
			PC,	memory->read(PC), memory->read(PC + 1),	memory->read(PC + 2),instructionNames[memory->read(PC)]);
	printf("A:%02hhX X:%02hhX Y:%02hhX P:%02hhX SP:%02hhX - \n", A, X, Y, serializeStatus(), SP);
	fflush(stdout);
}

void CPU::stackPush(Byte b) {
	memory->write(0x0100 + SP, b);
	if (SP == 0x00) SP = 0xFF;
	else SP--;
}

Byte CPU::stackPop() {
	if (SP == 0xFF) SP = 0x00;
	else SP++;
	return memory->read(0x0100 + SP);
}

Byte CPU::serializeStatus() {
	// If something breaks inexplicably, this is the cause.
	//return ((Byte*)&P)[0];
	return (P.N << 7) | (P.V << 6) | (P._ << 5) | (P.B << 4) | (P.D << 3) | (P.I << 2) | (P.Z << 1) | P.C;
}

void CPU::deserializeStatus(Byte status) {
	// If something breaks inexplicably, this is the cause.
	//((Byte*)&P)[0] = status;
	P.N = (status & 0x80) ? 1 : 0;
	P.V = (status & 0x40) ? 1 : 0;
	P._ = 1;
	P.B = 0;
	P.D = (status & 0x08) ? 1 : 0;
	P.I = (status & 0x04) ? 1 : 0;
	P.Z = (status & 0x02) ? 1 : 0;
	P.C = (status & 0x01) ? 1 : 0;
}

void CPU::initializeInstructionTable() {
	for (int i = 0; i <= 0xFF; i++) {
		InstrTable[i] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_ILL, 1};
	}

	InstrTable[0x69] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_ADC, 2};
	InstrTable[0x65] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_ADC, 3};
	InstrTable[0x75] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_ADC, 4};
	InstrTable[0x6D] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_ADC, 4};
	InstrTable[0x7D] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_ADC, 4};
	InstrTable[0x79] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_ADC, 4};
	InstrTable[0x61] = (Instr){&CPU::ADDR_INDX, &CPU::OP_ADC, 6};
	InstrTable[0x71] = (Instr){&CPU::ADDR_INDY, &CPU::OP_ADC, 5};

	InstrTable[0x29] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_AND, 2};
	InstrTable[0x25] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_AND, 3};
	InstrTable[0x35] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_AND, 4};
	InstrTable[0x2D] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_AND, 4};
	InstrTable[0x3D] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_AND, 4};
	InstrTable[0x39] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_AND, 4};
	InstrTable[0x21] = (Instr){&CPU::ADDR_INDX, &CPU::OP_AND, 6};
	InstrTable[0x31] = (Instr){&CPU::ADDR_INDY, &CPU::OP_AND, 5};

	InstrTable[0x0A] = (Instr){&CPU::ADDR_ACC,  &CPU::OP_ASLA, 2};
	InstrTable[0x06] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_ASL, 5};
	InstrTable[0x16] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_ASL, 6};
	InstrTable[0x0E] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_ASL, 6};
	InstrTable[0x1E] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_ASL, 7};

	InstrTable[0x90] = (Instr){&CPU::ADDR_REL,  &CPU::OP_BCC, 2};
	InstrTable[0xB0] = (Instr){&CPU::ADDR_REL,  &CPU::OP_BCS, 2};
	InstrTable[0xF0] = (Instr){&CPU::ADDR_REL,  &CPU::OP_BEQ, 2};
	InstrTable[0x30] = (Instr){&CPU::ADDR_REL,  &CPU::OP_BMI, 2};
	InstrTable[0xD0] = (Instr){&CPU::ADDR_REL,  &CPU::OP_BNE, 2};
	InstrTable[0x10] = (Instr){&CPU::ADDR_REL,  &CPU::OP_BPL, 2};
	InstrTable[0x50] = (Instr){&CPU::ADDR_REL,  &CPU::OP_BVC, 2};
	InstrTable[0x70] = (Instr){&CPU::ADDR_REL,  &CPU::OP_BVS, 2};

	InstrTable[0x24] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_BIT, 3};
	InstrTable[0x2C] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_BIT, 4};

	InstrTable[0x00] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_BRK, 7};

	InstrTable[0x18] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_CLC, 2};
	InstrTable[0xD8] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_CLD, 2};
	InstrTable[0x58] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_CLI, 2};
	InstrTable[0xB8] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_CLV, 2};

	InstrTable[0xC9] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_CMP, 2};
	InstrTable[0xC5] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_CMP, 3};
	InstrTable[0xD5] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_CMP, 4};
	InstrTable[0xCD] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_CMP, 4};
	InstrTable[0xDD] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_CMP, 4};
	InstrTable[0xD9] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_CMP, 4};
	InstrTable[0xC1] = (Instr){&CPU::ADDR_INDX, &CPU::OP_CMP, 6};
	InstrTable[0xD1] = (Instr){&CPU::ADDR_INDY, &CPU::OP_CMP, 5};

	InstrTable[0xE0] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_CPX, 2};
	InstrTable[0xE4] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_CPX, 3};
	InstrTable[0xEC] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_CPX, 4};
	InstrTable[0xC0] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_CPY, 2};
	InstrTable[0xC4] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_CPY, 3};
	InstrTable[0xCC] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_CPY, 4};

	InstrTable[0xC6] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_DEC, 5};
	InstrTable[0xD6] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_DEC, 6};
	InstrTable[0xCE] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_DEC, 6};
	InstrTable[0xDE] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_DEC, 7};

	InstrTable[0xCA] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_DEX, 2};
	InstrTable[0x88] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_DEY, 2};

	InstrTable[0x49] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_EOR, 2};
	InstrTable[0x45] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_EOR, 3};
	InstrTable[0x55] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_EOR, 4};
	InstrTable[0x4D] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_EOR, 4};
	InstrTable[0x5D] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_EOR, 4};
	InstrTable[0x59] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_EOR, 4};
	InstrTable[0x41] = (Instr){&CPU::ADDR_INDX, &CPU::OP_EOR, 6};
	InstrTable[0x51] = (Instr){&CPU::ADDR_INDY, &CPU::OP_EOR, 5};

	InstrTable[0xE6] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_INC, 5};
	InstrTable[0xF6] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_INC, 6};
	InstrTable[0xEE] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_INC, 6};
	InstrTable[0xFE] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_INC, 7};

	InstrTable[0xE8] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_INX, 2};
	InstrTable[0xC8] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_INY, 2};

	InstrTable[0x4C] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_JMP, 3};
	InstrTable[0x6C] = (Instr){&CPU::ADDR_ABSI, &CPU::OP_JMP, 5};

	InstrTable[0x20] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_JSR, 6};

	InstrTable[0xA9] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_LDA, 2};
	InstrTable[0xA5] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_LDA, 3};
	InstrTable[0xB5] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_LDA, 4};
	InstrTable[0xAD] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_LDA, 4};
	InstrTable[0xBD] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_LDA, 4};
	InstrTable[0xB9] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_LDA, 4};
	InstrTable[0xA1] = (Instr){&CPU::ADDR_INDX, &CPU::OP_LDA, 6};
	InstrTable[0xB1] = (Instr){&CPU::ADDR_INDY, &CPU::OP_LDA, 5};

	InstrTable[0xA2] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_LDX, 2};
	InstrTable[0xA6] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_LDX, 3};
	InstrTable[0xB6] = (Instr){&CPU::ADDR_ZPY,  &CPU::OP_LDX, 4};
	InstrTable[0xAE] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_LDX, 4};
	InstrTable[0xBE] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_LDX, 4};

	InstrTable[0xA0] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_LDY, 2};
	InstrTable[0xA4] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_LDY, 3};
	InstrTable[0xB4] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_LDY, 4};
	InstrTable[0xAC] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_LDY, 4};
	InstrTable[0xBC] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_LDY, 4};

	InstrTable[0x4A] = (Instr){&CPU::ADDR_ACC,  &CPU::OP_LSRA, 2};
	InstrTable[0x46] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_LSR, 5};
	InstrTable[0x56] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_LSR, 6};
	InstrTable[0x4E] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_LSR, 6};
	InstrTable[0x5E] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_LSR, 7};

	InstrTable[0xEA] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_NOP, 2};

	InstrTable[0x09] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_ORA, 2};
	InstrTable[0x05] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_ORA, 3};
	InstrTable[0x15] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_ORA, 4};
	InstrTable[0x0D] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_ORA, 4};
	InstrTable[0x1D] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_ORA, 4};
	InstrTable[0x19] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_ORA, 4};
	InstrTable[0x01] = (Instr){&CPU::ADDR_INDX, &CPU::OP_ORA, 6};
	InstrTable[0x11] = (Instr){&CPU::ADDR_INDY, &CPU::OP_ORA, 5};

	InstrTable[0x48] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_PHA, 3};
	InstrTable[0x08] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_PHP, 3};

	InstrTable[0x68] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_PLA, 4};
	InstrTable[0x28] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_PLP, 4};

	InstrTable[0x2A] = (Instr){&CPU::ADDR_ACC,  &CPU::OP_ROLA, 2};
	InstrTable[0x26] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_ROL, 5};
	InstrTable[0x36] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_ROL, 6};
	InstrTable[0x2E] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_ROL, 6};
	InstrTable[0x3E] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_ROL, 7};

	InstrTable[0x6A] = (Instr){&CPU::ADDR_ACC,  &CPU::OP_RORA, 2};
	InstrTable[0x66] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_ROR, 5};
	InstrTable[0x76] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_ROR, 6};
	InstrTable[0x6E] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_ROR, 6};
	InstrTable[0x7E] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_ROR, 7};

	InstrTable[0x40] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_RTI, 6};
	InstrTable[0x60] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_RTS, 6};

	InstrTable[0xE9] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_SBC, 2};
	InstrTable[0xE5] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_SBC, 3};
	InstrTable[0xF5] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_SBC, 4};
	InstrTable[0xED] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_SBC, 4};
	InstrTable[0xFD] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_SBC, 4};
	InstrTable[0xF9] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_SBC, 4};
	InstrTable[0xE1] = (Instr){&CPU::ADDR_INDX, &CPU::OP_SBC, 6};
	InstrTable[0xF1] = (Instr){&CPU::ADDR_INDY, &CPU::OP_SBC, 5};

	InstrTable[0x38] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_SEC, 2};
	InstrTable[0xF8] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_SED, 2};
	InstrTable[0x78] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_SEI, 2};

	InstrTable[0x85] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_STA, 3};
	InstrTable[0x95] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_STA, 4};
	InstrTable[0x8D] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_STA, 4};
	InstrTable[0x9D] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_STA, 4};
	InstrTable[0x99] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_STA, 4};
	InstrTable[0x81] = (Instr){&CPU::ADDR_INDX, &CPU::OP_STA, 6};
	InstrTable[0x91] = (Instr){&CPU::ADDR_INDY, &CPU::OP_STA, 5};

	InstrTable[0x86] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_STX, 3};
	InstrTable[0x96] = (Instr){&CPU::ADDR_ZPY,  &CPU::OP_STX, 4};
	InstrTable[0x8E] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_STX, 4};

	InstrTable[0x84] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_STY, 3};
	InstrTable[0x94] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_STY, 4};
	InstrTable[0x8C] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_STY, 4};

	InstrTable[0xAA] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_TAX, 2};
	InstrTable[0xA8] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_TAY, 2};
	InstrTable[0xBA] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_TSX, 2};
	InstrTable[0x8A] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_TXA, 2};
	InstrTable[0x9A] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_TXS, 2};
	InstrTable[0x98] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_TYA, 2};

	// UNDOCUMENTED INSTRUCTIONS
	InstrTable[0x0B] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_AAC, 2};
	InstrTable[0x2B] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_AAC, 2};

	InstrTable[0x87] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_AAX, 3};
	InstrTable[0x97] = (Instr){&CPU::ADDR_ZPY,  &CPU::OP_AAX, 4};
	InstrTable[0x83] = (Instr){&CPU::ADDR_INDX, &CPU::OP_AAX, 6};
	InstrTable[0x8F] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_AAX, 4};

	InstrTable[0x6B] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_ARR, 2};

	InstrTable[0x4B] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_ASR, 2};

	InstrTable[0xAB] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_ATX, 2};

	InstrTable[0x9F] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_AXA, 5};
	InstrTable[0x93] = (Instr){&CPU::ADDR_INDY, &CPU::OP_AXA, 6};

	InstrTable[0xCB] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_AXS, 2};

	InstrTable[0xC7] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_DCP, 5};
	InstrTable[0xD7] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_DCP, 6};
	InstrTable[0xCF] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_DCP, 6};
	InstrTable[0xDF] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_DCP, 7};
	InstrTable[0xDB] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_DCP, 7};
	InstrTable[0xC3] = (Instr){&CPU::ADDR_INDX, &CPU::OP_DCP, 8};
	InstrTable[0xD3] = (Instr){&CPU::ADDR_INDY, &CPU::OP_DCP, 8};

	InstrTable[0x04] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_DOP, 3};
	InstrTable[0x14] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_DOP, 4};
	InstrTable[0x34] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_DOP, 4};
	InstrTable[0x44] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_DOP, 3};
	InstrTable[0x54] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_DOP, 4};
	InstrTable[0x64] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_DOP, 3};
	InstrTable[0x74] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_DOP, 4};
	InstrTable[0x80] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_DOP, 2};
	InstrTable[0x82] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_DOP, 2};
	InstrTable[0x89] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_DOP, 2};
	InstrTable[0xC2] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_DOP, 2};
	InstrTable[0xD4] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_DOP, 4};
	InstrTable[0xE2] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_DOP, 3};
	InstrTable[0xF4] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_DOP, 4};

	InstrTable[0xE7] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_ISC, 5};
	InstrTable[0xF7] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_ISC, 6};
	InstrTable[0xEF] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_ISC, 6};
	InstrTable[0xFF] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_ISC, 7};
	InstrTable[0xFB] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_ISC, 7};
	InstrTable[0xE3] = (Instr){&CPU::ADDR_INDX, &CPU::OP_ISC, 8};
	InstrTable[0xF3] = (Instr){&CPU::ADDR_INDY, &CPU::OP_ISC, 8};

	InstrTable[0x02] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0x12] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0x22] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0x32] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0x42] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0x52] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0x62] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0x72] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0x92] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0xB2] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0xD2] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};
	InstrTable[0xF2] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_KIL, 1};

	InstrTable[0xBB] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_LAR, 4};

	InstrTable[0xA7] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_LAX, 3};
	InstrTable[0xB7] = (Instr){&CPU::ADDR_ZPY,  &CPU::OP_LAX, 4};
	InstrTable[0xAF] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_LAX, 4};
	InstrTable[0xBF] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_LAX, 4};
	InstrTable[0xA3] = (Instr){&CPU::ADDR_INDX, &CPU::OP_LAX, 6};
	InstrTable[0xB3] = (Instr){&CPU::ADDR_INDY, &CPU::OP_LAX, 5};

	InstrTable[0x1A] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_NOP, 2};
	InstrTable[0x3A] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_NOP, 2};
	InstrTable[0x5A] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_NOP, 2};
	InstrTable[0x7A] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_NOP, 2};
	InstrTable[0xDA] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_NOP, 2};
	InstrTable[0xFA] = (Instr){&CPU::ADDR_IMPL, &CPU::OP_NOP, 2};

	InstrTable[0x27] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_RLA, 5};
	InstrTable[0x37] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_RLA, 6};
	InstrTable[0x2F] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_RLA, 6};
	InstrTable[0x3F] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_RLA, 7};
	InstrTable[0x3B] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_RLA, 7};
	InstrTable[0x23] = (Instr){&CPU::ADDR_INDX, &CPU::OP_RLA, 8};
	InstrTable[0x33] = (Instr){&CPU::ADDR_INDY, &CPU::OP_RLA, 8};

	InstrTable[0x67] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_RRA, 5};
	InstrTable[0x77] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_RRA, 6};
	InstrTable[0x6F] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_RRA, 6};
	InstrTable[0x7F] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_RRA, 7};
	InstrTable[0x7B] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_RRA, 7};
	InstrTable[0x63] = (Instr){&CPU::ADDR_INDX, &CPU::OP_RRA, 8};
	InstrTable[0x73] = (Instr){&CPU::ADDR_INDY, &CPU::OP_RRA, 8};

	InstrTable[0xEB] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_SBC, 2};

	InstrTable[0x07] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_SLO, 5};
	InstrTable[0x17] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_SLO, 6};
	InstrTable[0x0F] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_SLO, 6};
	InstrTable[0x1F] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_SLO, 7};
	InstrTable[0x1B] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_SLO, 7};
	InstrTable[0x03] = (Instr){&CPU::ADDR_INDX, &CPU::OP_SLO, 8};
	InstrTable[0x13] = (Instr){&CPU::ADDR_INDY, &CPU::OP_SLO, 8};

	InstrTable[0x47] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_SRE, 5};
	InstrTable[0x57] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_SRE, 6};
	InstrTable[0x4F] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_SRE, 6};
	InstrTable[0x5F] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_SRE, 7};
	InstrTable[0x5B] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_SRE, 7};
	InstrTable[0x43] = (Instr){&CPU::ADDR_INDX, &CPU::OP_SRE, 8};
	InstrTable[0x53] = (Instr){&CPU::ADDR_INDY, &CPU::OP_SRE, 8};

	InstrTable[0x9E] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_SXA, 5};

	InstrTable[0x9C] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_SYA, 5};

	InstrTable[0x0C] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_TOP, 4};
	InstrTable[0x1C] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_TOP, 4};
	InstrTable[0x3C] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_TOP, 4};
	InstrTable[0x5C] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_TOP, 4};
	InstrTable[0x7C] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_TOP, 4};
	InstrTable[0xDC] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_TOP, 4};
	InstrTable[0xFC] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_TOP, 4};

	InstrTable[0x8B] = (Instr){&CPU::ADDR_IMM,  &CPU::OP_XAA, 2};

	InstrTable[0x9B] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_XAS, 5};
}

// ADDRESSING MODES

Address CPU::ADDR_ACC() {
	// Not used
	return 0;
}

Address CPU::ADDR_IMM() {
	return PC++;
}

Address CPU::ADDR_ABS() {
	Address addr = memory->read16(PC);
	PC += 2;
	return addr;
}

Address CPU::ADDR_ZP() {
	return memory->read(PC++);
}

Address CPU::ADDR_ZPX() {
	return (X + memory->read(PC++)) % 256;
}

Address CPU::ADDR_ZPY() {
	return (Y + memory->read(PC++)) % 256;
}

Address CPU::ADDR_ABSX() {
	Address addr = X + memory->read16(PC);
	PC += 2;
	return addr;
}

Address CPU::ADDR_ABSY() {
	Address addr = Y + memory->read16(PC);
	PC += 2;
	return addr;
}

Address CPU::ADDR_IMPL() {
	// Not used.
	return 0;
}

Address CPU::ADDR_REL() {
	SignedByte offset = memory->read(PC++);
	return PC + offset;
}

Address CPU::ADDR_INDX() {
	Address lo;
	Address hi;
	
	lo = (memory->read(PC++) + X) & 0xFF;
	hi = (lo + 1) & 0xFF;

	return  memory->read(lo) + (memory->read(hi) << 8);
}

Address CPU::ADDR_INDY() {
	Address lo;
	Address hi;
	
	lo = memory->read(PC++);
	hi = (lo + 1) % 256;

	return  memory->read(lo) + (memory->read(hi) << 8) + Y;
}

Address CPU::ADDR_ABSI() {
	Address addr = memory->read16(PC);
	PC += 2;

	// Indirect JMP bug
	Address lo = memory->read(addr);
	Address hi = memory->read((addr & 0xFF00) + ((addr + 1) & 0x00FF));

	return lo + (hi << 8);
}


// OPCODES

void CPU::OP_ADC(Address data) {
	// Add the two values along with the carry bit.
	// This might result in an overflow, hence the Word variable.
	Byte val = memory->read(data);
	Word res = A + val + P.C;

	P.C = (res & 0x0100) ? 1 : 0;
	P.Z = ((res & 0xFF) == 0) ? 1 : 0;
	P.N = (res & 0x80) ? 1 : 0;
	P.V = !((A^val) & 0x80) && ((A^res) & 0x80) ? 1 : 0;

	A = (res & 0xFF);
}

void CPU::OP_AND(Address data) {
	A &= memory->read(data);

	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_ASL(Address data) {
	Byte val = memory->read(data);
	P.C = (val & 0x80) ? 1 : 0;

	val <<= 1;
	memory->write(data, val);

	P.Z = (val == 0) ? 1 : 0;
	P.N = (val & 0x80) ? 1 : 0;
}

void CPU::OP_ASLA(Address data) {
	UNUSED(data);

	P.C = (A & 0x80) ? 1 : 0;
	A <<= 1;
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_BCC(Address data) {
	if (!P.C)
		PC = data;
}

void CPU::OP_BCS(Address data) {
	if (P.C)
		PC = data;
}

void CPU::OP_BEQ(Address data) {
	if (P.Z)
		PC = data;
}

void CPU::OP_BIT(Address data) {
	Byte val = memory->read(data);

	P.Z = ((A & val) == 0) ? 1 : 0;

	P.V = (val & 0x40) ? 1 : 0;
	P.N = (val & 0x80) ? 1 : 0;
}

void CPU::OP_BMI(Address data) {
	if (P.N)
		PC = data;
}

void CPU::OP_BNE(Address data) {
	if (!P.Z)
		PC = data;
}

void CPU::OP_BPL(Address data) {
	if (!P.N)
		PC = data;
}

void CPU::OP_BRK(Address data) {
	UNUSED(data);

	//https://retrocomputing.stackexchange.com/questions/12291/what-are-uses-of-the-byte-after-brk-instruction-on-6502
	PC++;
	stackPush(PC >> 8);
	stackPush(PC & 0xFF);

	stackPush(serializeStatus());
	P.B = 1;

	PC = memory->read16(0xFFFE);
	
}

void CPU::OP_BVC(Address data) {
	if (!P.V)
		PC = data;
}

void CPU::OP_BVS(Address data) {
	if (P.V)
		PC = data;
}

void CPU::OP_CLC(Address data) {
	UNUSED(data);
	
	P.C = 0;
}

void CPU::OP_CLD(Address data) {
	UNUSED(data);
	
	P.D = 0;
}

void CPU::OP_CLI(Address data) {
	UNUSED(data);
	
	P.I = 0;
}

void CPU::OP_CLV(Address data) {
	UNUSED(data);
	
	P.V = 0;
}

void CPU::OP_CMP(Address data) {
	Byte val = memory->read(data);
	P.C = (A >= val) ? 1 : 0;
	P.Z = (A == val) ? 1 : 0;
	P.N = (A - val) & 0x80 ? 1 : 0;
}

void CPU::OP_CPX(Address data) {
	Byte val = memory->read(data);
	P.C = (X >= val) ? 1 : 0;
	P.Z = (X == val) ? 1 : 0;
	P.N = (X - val) & 0x80 ? 1 : 0;
}

void CPU::OP_CPY(Address data) {
	Byte val = memory->read(data);
	P.C = (Y >= val) ? 1 : 0;
	P.Z = (Y == val) ? 1 : 0;
	P.N = (Y - val) & 0x80 ? 1 : 0;
}

void CPU::OP_DEC(Address data) {
	Byte val = (memory->read(data) - 1) % 256;
	memory->write(data, val);
	P.Z = (val == 0) ? 1 : 0;
	P.N = (val & 0x80) ? 1 : 0;
}

void CPU::OP_DEX(Address data) {
	UNUSED(data);
	
	X = (X - 1) % 256;
	P.Z = (X == 0) ? 1 : 0;
	P.N = (X & 0x80) ? 1 : 0;
}

void CPU::OP_DEY(Address data) {
	UNUSED(data);
	
	Y = (Y - 1) % 256;
	P.Z = (Y == 0) ? 1 : 0;
	P.N = (Y & 0x80) ? 1 : 0;
}

void CPU::OP_EOR(Address data) {
	A ^= memory->read(data);
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_INC(Address data) {
	Byte val = (memory->read(data) + 1) % 256;
	memory->write(data, val);
	P.Z = (val == 0) ? 1 : 0;
	P.N = (val & 0x80) ? 1 : 0;
}

void CPU::OP_INX(Address data) {
	UNUSED(data);
	
	X = (X + 1) % 256;
	P.Z = (X == 0) ? 1 : 0;
	P.N = (X & 0x80) ? 1 : 0;
}

void CPU::OP_INY(Address data) {
	UNUSED(data);
	
	Y = (Y + 1) % 256;
	P.Z = (Y == 0) ? 1 : 0;
	P.N = (Y & 0x80) ? 1 : 0;
}

void CPU::OP_JMP(Address data) {
	UNUSED(data);
	
	PC = data;
}

void CPU::OP_JSR(Address data) {
	PC--;
	stackPush(PC >> 8);
	stackPush(PC & 0xFF);
	PC = data;
}

void CPU::OP_LDA(Address data) {
	A = memory->read(data);
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_LDX(Address data) {
	X = memory->read(data);
	P.Z = (X == 0) ? 1 : 0;
	P.N = (X & 0x80) ? 1 : 0;
}

void CPU::OP_LDY(Address data) {
	Y = memory->read(data);
	P.Z = (Y == 0) ? 1 : 0;
	P.N = (Y & 0x80) ? 1 : 0;
}

void CPU::OP_LSR(Address data) {
	Byte val = memory->read(data);

	P.C = (val & 0x01) ? 1 : 0;

	val >>= 1;
	memory->write(data, val);

	P.Z = (val == 0) ? 1 : 0;
	P.N = (val & 0x80) ? 1 : 0;

}

void CPU::OP_LSRA(Address data) {
	UNUSED(data);

	P.C = (A & 0x01) ? 1 : 0;

	A >>= 1;

	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_NOP(Address data) {
	UNUSED(data);
}

void CPU::OP_ORA(Address data) {
	A |= memory->read(data);

	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_PHA(Address data) {
	UNUSED(data);
	stackPush(A);
}

void CPU::OP_PHP(Address data) {
	UNUSED(data);
	stackPush(serializeStatus() | 0x10);
}

void CPU::OP_PLA(Address data) {
	UNUSED(data);

	A = stackPop();
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_PLP(Address data) {
	UNUSED(data);
	deserializeStatus(stackPop());
	P._ = 1;
}

void CPU::OP_ROL(Address data) {
	Byte old = memory->read(data);
	Byte res = (old << 1) | P.C;
	memory->write(data, res);

	P.C = (old & 0x80) ? 1 : 0;
	P.Z = (res == 0) ? 1 : 0;
	P.N = (res & 0x80) ? 1 : 0;
}

void CPU::OP_ROLA(Address data) {
	UNUSED(data);

	Byte res = (A << 1) | P.C;
	P.C = (A & 0x80) ? 1 : 0;

	A = res;
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_ROR(Address data) {
	Byte old = memory->read(data);
	Byte res = (old >> 1) | (P.C << 7);
	memory->write(data, res);

	P.C = (old & 0x01) ? 1 : 0;
	P.Z = (res == 0) ? 1 : 0;
	P.N = (res & 0x80) ? 1 : 0;
}

void CPU::OP_RORA(Address data) {
	UNUSED(data);

	Byte res = (A >> 1) | (P.C << 7);
	P.C = (A & 0x01) ? 1 : 0;

	A = res;
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_RTI(Address data) {
	UNUSED(data);

	deserializeStatus(stackPop());

	Byte lo, hi;
	lo = stackPop();
	hi = stackPop();

	PC = (hi << 8) | lo;
}

void CPU::OP_RTS(Address data) {
	UNUSED(data);

	Byte lo, hi;
	lo = stackPop();
	hi = stackPop();

	PC = ((hi << 8) | lo) + 1;
}

void CPU::OP_SBC(Address data) {
	// SBC is ADC but with the bits of the argument flipped.
	Byte val = ~memory->read(data);
	Word res = A + val + P.C;

	P.C = (res & 0x0100) ? 1 : 0;
	P.Z = ((res & 0xFF) == 0) ? 1 : 0;
	P.N = (res & 0x80) ? 1 : 0;
	P.V = !((A^val) & 0x80) && ((A^res) & 0x80) ? 1 : 0;

	A = (res & 0xFF);
}

void CPU::OP_SEC(Address data) {
	UNUSED(data);
	P.C = 1;
}

void CPU::OP_SED(Address data) {
	UNUSED(data);
	P.D = 1;
}

void CPU::OP_SEI(Address data) {
	UNUSED(data);
	P.I = 1;
}

void CPU::OP_STA(Address data) {
	memory->write(data, A);
}

void CPU::OP_STX(Address data) {
	memory->write(data, X);
}

void CPU::OP_STY(Address data) {
	memory->write(data, Y);
}

void CPU::OP_TAX(Address data) {
	UNUSED(data);
	X = A;
	P.Z = (X == 0) ? 1 : 0;
	P.N = (X & 0x80) ? 1 : 0;
}

void CPU::OP_TAY(Address data) {
	UNUSED(data);
	Y = A;
	P.Z = (Y == 0) ? 1 : 0;
	P.N = (Y & 0x80) ? 1 : 0;
}

void CPU::OP_TSX(Address data) {
	UNUSED(data);
	X = SP;
	P.Z = (X == 0) ? 1 : 0;
	P.N = (X & 0x80) ? 1 : 0;
}

void CPU::OP_TXA(Address data) {
	UNUSED(data);
	A = X;
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_TXS(Address data) {
	UNUSED(data);
	SP = X;
}

void CPU::OP_TYA(Address data) {
	UNUSED(data);
	A = Y;
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_ILL(Address data) {
	UNUSED(data);
	printf("ILLEGAL INSTRUCTION.\n");
}

// UNDOCUMENTED
void CPU::OP_AAC(Address data) {
	A = A & memory->read(data);

	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
	P.C = (A & 0x80) ? 1 : 0;
}

void CPU::OP_AAX(Address data) {
	Byte res = A & X;
	memory->write(data, res);

	//P.N = (res & 0x80) ? 1 : 0;
	//P.Z = (res == 0) ? 1 : 0;
}

void CPU::OP_ARR(Address data) {
	A = A & memory->read(data);
	A = (A >> 1) | (P.C << 7);

	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;

	P.C = (A & 0x40) ? 1 : 0;
	P.V = (((A & 0x20) != 0) ^ ((A & 0x40) != 0));
}

void CPU::OP_ASR(Address data) {
	A = A & memory->read(data);

	P.C = (A & 0x01) ? 1 : 0;

	A >>= 1;

	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_ATX(Address data) {
	A = A & memory->read(data);
	X = A;

	P.N = (X & 0x80) ? 1 : 0;
	P.Z = (X == 0) ? 1 : 0;
}

void CPU::OP_AXA(Address data) {
	Byte val = A & X & 7;
	memory->write(data, val);
	P.N = (val & 0x80) ? 1 : 0;
	P.Z = (val == 0) ? 1 : 0;
}

void CPU::OP_AXS(Address data) {
	Byte val = memory->read(data);
	X = X & A;
	P.C = (X < val) ? 1 : 0;
	X = X - val;

	P.Z = (X == 0) ? 1 : 0;
	P.N = (X & 0x80) ? 1 : 0;
}

void CPU::OP_DCP(Address data) {
	Byte val = (memory->read(data) - 1) % 256;
	memory->write(data, val);
	P.C = (A >= val) ? 1 : 0;
	P.Z = (A == val) ? 1 : 0;
	P.N = (A - val) & 0x80 ? 1 : 0;
}

void CPU::OP_DOP(Address data) {
	UNUSED(data);
}

void CPU::OP_ISC(Address data) {
	Byte val = (memory->read(data) + 1) & 0xFF;
	memory->write(data, val);
	Word res = A - val - (1 - P.C);

	P.Z = (res == 0) ? 1 : 0;
	P.N = (res & 0x80) ? 1 : 0;
	P.V = (((A ^ val) & (A ^ res) & 0x80) != 0);
	P.C = ((res & 0x100) == 0) ? 1 : 0;

	A = res & 0xFF;
}


void CPU::OP_KIL(Address data) {
	UNUSED(data);

	// Freeze up program counter by setting PC to current instruction
	PC = PC - 1;
}

void CPU::OP_LAR(Address data) {
	SP = SP & memory->read(data);
	A = SP;
	X = SP;

	P.N = (SP & 0x80) ? 1 : 0;
	P.Z = (SP == 0) ? 1 : 0;
}

void CPU::OP_LAX(Address data) {
	Byte val = memory->read(data);
	A = val;
	X = val;

	P.N = (A & 0x80) ? 1 : 0;
	P.Z = (A == 0) ? 1 : 0;
}

void CPU::OP_RLA(Address data) {
	Byte old = memory->read(data);
	Byte res = (old << 1) | P.C;
	memory->write(data, res);
	A = A & res;

	P.C = (old & 0x80) ? 1 : 0;
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_RRA(Address data) {
	Byte old = memory->read(data);
	Byte val = (old >> 1) | (P.C << 7);
	memory->write(data, val);
	P.C = (old & 0x01) ? 1 : 0;

	Word res = A + val + P.C;

	P.C = (res & 0x0100) ? 1 : 0;
	P.Z = ((res & 0xFF) == 0) ? 1 : 0;
	P.N = (res & 0x80) ? 1 : 0;
	P.V = !((A^val) & 0x80) && ((A^res) & 0x80) ? 1 : 0;

	A = (res & 0xFF);
}

void CPU::OP_SLO(Address data) {
	Byte val = memory->read(data);
	P.C = (val & 0x80) ? 1 : 0;

	val <<= 1;
	memory->write(data, val);

	A = A | val;

	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

void CPU::OP_SRE(Address data) {
	Byte val = memory->read(data);

	P.C = (val & 0x01) ? 1 : 0;

	val >>= 1;
	memory->write(data, val);

	A ^= val;
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

//TODO
void CPU::OP_SXA(Address data) {
	UNUSED(data);
}

//TODO
void CPU::OP_SYA(Address data) {
	UNUSED(data);
}

void CPU::OP_TOP(Address data) {
	UNUSED(data);
}

void CPU::OP_XAA(Address data) {
	Byte magic = 0xEE;
	A = (A | magic) & X & data;
	P.Z = (A == 0) ? 1 : 0;
	P.N = (A & 0x80) ? 1 : 0;
}

//TODO
void CPU::OP_XAS(Address data) {
	UNUSED(data);
}