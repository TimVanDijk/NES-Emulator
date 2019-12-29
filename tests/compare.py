instructionNames = [
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
	"SED impl",	"SBC abs,Y",	"ILL",			"ILL",		"ILL",		"SBC abs,X",	"INC abs,X",	"ILL"]

with open('tests/nestest.log') as f:
	target = f.read().split('\n')
	tmp = []
	for line in target:
		try:
			addr_opcode = line[:8]
			instr = instructionNames[int(line[6:8], 16)]
			instr += ' '*(9-len(instr))
			regs = 'A:' + line.split('A:')[1].split(' PPU')[0]
			tmp += [addr_opcode + ' ' + instr + '  ' + regs]
		except:
			break
	target = tmp

with open('tests/output.log') as f:
	actual = f.read().split('\n')
	tmp = []
	for line in actual:
		try:
			addr_opcode = line[:8]
			instr = instructionNames[int(line[6:8], 16)]
			instr += ' '*(9-len(instr))
			regs = line.split('\t')[4].split(' -')[0]
			tmp += [addr_opcode + ' ' + instr + '  ' + regs]
		except:
			if "ILL" in line:
				tmp += [line]
			break
	actual = tmp

print("Actual/Target: {} / {}".format(len(actual), len(target)))
for i in range(min(len(target), len(actual))):
	a = actual[i]
	t = target[i]
	
	if a != t:
		for j in range(max(0, i-11), i):
			print("[" + str(j) + "] Trace:    " + target[j])
			
		print("")
		print("[" + str(i) + "] Expected: " + target[i])
		print("[" + str(i) + "] Actual:   " + actual[i])

		break

print("Done")
