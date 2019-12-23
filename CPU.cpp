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

	PC = memory->read16(0xFFFC);
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

	PC = memory->read16(0xFFFC);
	SP = 0xFD;
}

void CPU::initializeInstructionTable() {
	for (int i = 0; i < 0xFF; i++) {
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

	InstrTable[0x0A] = (Instr){&CPU::ADDR_ACC,  &CPU::OP_ASL, 2};
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
	InstrTable[0xB4] = (Instr){&CPU::ADDR_ZPY,  &CPU::OP_LDY, 4};
	InstrTable[0xAC] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_LDY, 4};
	InstrTable[0xBC] = (Instr){&CPU::ADDR_ABSY, &CPU::OP_LDY, 4};

	InstrTable[0x4A] = (Instr){&CPU::ADDR_ACC,  &CPU::OP_LSR, 2};
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

	InstrTable[0x2A] = (Instr){&CPU::ADDR_ACC,  &CPU::OP_ROL, 2};
	InstrTable[0x26] = (Instr){&CPU::ADDR_ZP,   &CPU::OP_ROL, 5};
	InstrTable[0x36] = (Instr){&CPU::ADDR_ZPX,  &CPU::OP_ROL, 6};
	InstrTable[0x2E] = (Instr){&CPU::ADDR_ABS,  &CPU::OP_ROL, 6};
	InstrTable[0x2E] = (Instr){&CPU::ADDR_ABSX, &CPU::OP_ROL, 7};

	InstrTable[0x6A] = (Instr){&CPU::ADDR_ACC,  &CPU::OP_ROR, 2};
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

}

// ADDRESSING MODES

Address CPU::ADDR_ACC() {
	// Not used
	return 0;
}

Address CPU::ADDR_IMM() {
	return this->PC++;
}

Address CPU::ADDR_ABS() {
	Address addr = this->PC;
	this->PC += 2;
	return addr;
}

Address CPU::ADDR_ZP() {
	Address addr = this->PC++;
	return addr;
}

Address CPU::ADDR_ZPX() {
	return this->X + memory->read(this->PC++);
}

Address CPU::ADDR_ZPY() {
	return this->Y + memory->read(this->PC++);
}

Address CPU::ADDR_ABSX() {
	Address addr = this->X + memory->read16(this->PC);
	this->PC += 2;
	return addr;
}

Address CPU::ADDR_ABSY() {
	Address addr = this->Y + memory->read16(this->PC);
	this->PC += 2;
	return addr;
}

Address CPU::ADDR_IMPL() {
	// Not used.
	return 0;
}

Address CPU::ADDR_REL() {
	SignedByte offset = memory->read(this->PC++);
	return this->PC + offset;
}

Address CPU::ADDR_INDX() {
	return memory->read16((this->X + this->PC++) & 0xFF);
}

Address CPU::ADDR_INDY() {
	return memory->read(this->PC++) + this->Y;
}

Address CPU::ADDR_ABSI() {
	Address addr = memory->read(this->PC);
	this->PC += 2;
	return addr;
}


// OPCODES

void CPU::OP_ADC(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_AND(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_ASL(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BCC(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BCS(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BEQ(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BIT(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BMI(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BNE(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BPL(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BRK(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BVC(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_BVS(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_CLC(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_CLD(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_CLI(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_CLV(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_CMP(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_CPX(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_CPY(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_DEC(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_DEX(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_DEY(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_EOR(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_INC(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_INX(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_INY(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_JMP(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_JSR(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_LDA(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_LDX(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_LDY(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_LSR(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_NOP(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_ORA(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_PHA(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_PHP(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_PLA(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_PLP(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_ROL(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_ROR(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_RTI(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_RTS(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_SBC(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_SEC(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_SED(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_SEI(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_STA(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_STX(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_STY(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_TAX(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_TAY(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_TSX(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_TXA(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_TXS(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_TYA(Address data) {
	UNUSED(data);
	TODO;
}

void CPU::OP_ILL(Address data) {
	UNUSED(data);
	printf("ILLEGAL INSTRUCTION.\n");
}