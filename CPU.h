#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

#include "Memory.h"
#include "Globals.h"

struct Status {
	Bit N : 1;	// negative flag
	Bit V : 1;	// overflow flag
	Bit _ : 1;	// unused (always 1)
	Bit B : 1;	// indicates if BRK was executed
	Bit D : 1;	// decimal mode (ignored)
	Bit I : 1;	// interrupt disable
	Bit Z : 1;	// zero flag
	Bit C : 1;	// carry flag
};

class CPU {
public:	
	CPU();
	CPU(Memory* memory);

	void step(); 
	void reset();

	void printState();

private:
	Memory* memory;

	Byte A;		// accumulator
	Byte X;		// index register X
	Byte Y;		// index register Y
	Status P;	// status register
	Byte SP;	// stack pointer
	Word PC;	// program counter

	void stackPush(Byte b);
	Byte stackPop();

	Byte serializeStatus();
	void deserializeStatus(Byte status);

	struct Instr {
		Address (CPU::*addr)();
		void (CPU::*code)(Address);
		Byte cycles;
	};

	Instr InstrTable[0xFF];
	void initializeInstructionTable();

	// ADDRESSING MODES

	Address ADDR_ACC();		// ACCUMULATOR
	Address ADDR_IMM();		// IMMEDIATE
	Address ADDR_ABS();		// ABSOLUTE
	Address ADDR_ZP();		// ZERO PAGE 
	Address ADDR_ZPX();		// INDEXED ZERO PAGE X
	Address ADDR_ZPY();		// INDEXED ZERO PAGE Y
	Address ADDR_ABSX();	// INDEXED ABSOLUTE X
	Address ADDR_ABSY();	// INDEXED ABSOLUTE Y
	Address ADDR_IMPL();	// IMPLIED
	Address ADDR_REL();		// RELATIVE
	Address ADDR_INDX();	// INDEXED INDIRECT X
	Address ADDR_INDY();	// INDIRECT INDEXED Y
	Address ADDR_ABSI();	// ABSOLUTE INDIRECT 

	// OPCODES
	void OP_ADC(Address data);
	void OP_AND(Address data);
	void OP_ASL(Address data);
	void OP_ASLA(Address data);

	void OP_BCC(Address data);
	void OP_BCS(Address data);
	void OP_BEQ(Address data);
	void OP_BIT(Address data);
	void OP_BMI(Address data);
	void OP_BNE(Address data);
	void OP_BPL(Address data);
	void OP_BRK(Address data);
	void OP_BVC(Address data);
	void OP_BVS(Address data);

	void OP_CLC(Address data);
	void OP_CLD(Address data);
	void OP_CLI(Address data);
	void OP_CLV(Address data);
	void OP_CMP(Address data);
	void OP_CPX(Address data);
	void OP_CPY(Address data);

	void OP_DEC(Address data);
	void OP_DEX(Address data);
	void OP_DEY(Address data);

	void OP_EOR(Address data);

	void OP_INC(Address data);
	void OP_INX(Address data);
	void OP_INY(Address data);

	void OP_JMP(Address data);
	void OP_JSR(Address data);

	void OP_LDA(Address data);
	void OP_LDX(Address data);
	void OP_LDY(Address data);
	void OP_LSR(Address data);
	void OP_LSRA(Address data);

	void OP_NOP(Address data);

	void OP_ORA(Address data);

	void OP_PHA(Address data);
	void OP_PHP(Address data);
	void OP_PLA(Address data);
	void OP_PLP(Address data);

	void OP_ROL(Address data);
	void OP_ROLA(Address data);
	void OP_ROR(Address data);
	void OP_RORA(Address data);
	void OP_RTI(Address data);
	void OP_RTS(Address data);

	void OP_SBC(Address data);
	void OP_SEC(Address data);
	void OP_SED(Address data);
	void OP_SEI(Address data);
	void OP_STA(Address data);
	void OP_STX(Address data);
	void OP_STY(Address data);

	void OP_TAX(Address data);
	void OP_TAY(Address data);
	void OP_TSX(Address data);
	void OP_TXA(Address data);
	void OP_TXS(Address data);
	void OP_TYA(Address data);

	void OP_ILL(Address data);
};

const char instructionNames[256][10] = {
	"BRK impl",	"ORA X,ind", 	"ILL",			"ILL",		"ILL",		"ORA zpg",		"ASL zpg",		"ILL",
	"PHP impl",	"ORA #",		"ASL A",		"ILL",		"ILL",		"ORA abs",		"ASL abs",		"ILL",
	"BPL rel",	"ORA ind,Y",	"ILL",			"ILL",		"ILL",		"ORA zpg,X",	"ASL zpg,X",	"ILL",
	"CLC impl",	"ORA abs,Y",	"ILL",			"ILL",		"ILL",		"ORA abs,X",	"ASL abs,X",	"ILL",
	"JSR abs",	"AND X,ind",	"ILL",			"ILL",		"BIT zpg",	"AND zpg",		"ROL zpg",		"ILL",
	"PLP impl",	"AND #",		"ROL A",		"ILL",		"BIT abs",	"AND abs",		"ROL abs",		"ILL",
	"BMI rel",	"AND ind,Y",	"ILL",			"ILL",		"ILL",		"AND zpg,X",	"ROL zpg,X",	"ILL",
	"SEC impl",	"AND abs,Y",	"ILL",			"ILL",		"ILL",		"AND abs,X",	"ROL abs,X",	"ILL",
	"RTI impl",	"EOR X,ind",	"ILL",			"ILL",		"ILL",		"EOR zpg",		"LSR zpg",		"ILL",	
	"PHA impl",	"EOR #",		"LSR A",		"ILL",		"JMP abs",	"EOR abs",		"LSR abs",		"ILL",
	"BVC rel",	"EOR ind,Y",	"ILL",			"ILL",		"ILL",		"EOR zpg,X",	"LSR zpg,X",	"ILL",
	"CLI impl",	"EOR abs,Y",	"ILL",			"ILL",		"ILL",		"EOR abs,X",	"LSR abs,X",	"ILL",
	"RTS impl",	"ADC X,ind",	"ILL",			"ILL",		"ILL",		"ADC zpg",		"ROR zpg",		"ILL",
	"PLA impl",	"ADC #",		"ROR A",		"ILL",		"JMP ind",	"ADC abs",		"ROR abs",		"ILL",
	"BVS rel",	"ADC ind,Y",	"ILL",			"ILL",		"ILL",		"ADC zpg,X",	"ROR zpg,X",	"ILL",
	"SEI impl",	"ADC abs,Y",	"ILL",			"ILL",		"ILL",		"ADC abs,X",	"ROR abs,X",	"ILL",
	"ILL",		"STA X,ind",	"ILL",			"ILL",		"STY zpg",	"STA zpg",		"STX zpg",		"ILL",
	"DEY impl",	"ILL",			"TXA impl",		"ILL",		"STY abs",	"STA abs",		"STX abs",		"ILL",
	"BCC rel",	"STA ind,Y",	"ILL",			"ILL",		"STY zpg,X","STA zpg,X",	"STX zpg,Y",	"ILL",
	"TYA impl",	"STA abs,Y",	"TXS impl",		"ILL",		"ILL",		"STA abs,X",	"ILL",			"ILL",	
	"LDY #",	"LDA X,ind",	"LDX #",		"ILL",		"LDY zpg",	"LDA zpg",		"LDX zpg",		"ILL",
	"TAY impl",	"LDA #",		"TAX impl",		"ILL",		"LDY abs",	"LDA abs",		"LDX abs",		"ILL",	
	"BCS rel",	"LDA ind,Y",	"ILL",			"ILL",		"LDY zpg,X","LDA zpg,X",	"LDX zpg,Y",	"ILL",	
	"CLV impl",	"LDA abs,Y",	"TSX impl",		"ILL",		"LDY abs,X","LDA abs,X",	"LDX abs,Y",	"ILL",	
	"CPY #",	"CMP X,ind",	"ILL",			"ILL",		"CPY zpg",	"CMP zpg",		"DEC zpg", 		"ILL",	
	"INY impl",	"CMP #",		"DEX impl",		"ILL",		"CPY abs",	"CMP abs",		"DEC abs",		"ILL",
	"BNE rel",	"CMP ind,Y",	"ILL",			"ILL",		"ILL",		"CMP zpg,X",	"DEC zpg,X",	"ILL",	
	"CLD impl",	"CMP abs,Y",	"ILL",			"ILL",		"ILL",		"CMP abs,X",	"DEC abs,X",	"ILL",	
	"CPX #",	"SBC X,ind",	"ILL",			"ILL",		"CPX zpg",	"SBC zpg",		"INC zpg",		"ILL",
	"INX impl",	"SBC #",		"NOP impl",		"ILL",		"CPX abs",	"SBC abs",		"INC abs",		"ILL",	
	"BEQ rel",	"SBC ind,Y",	"ILL",			"ILL",		"ILL",		"SBC zpg,X",	"INC zpg,X",	"ILL",	
	"SED impl",	"SBC abs,Y",	"ILL",			"ILL",		"ILL",		"SBC abs,X",	"INC abs,X",	"ILL"
};



#endif