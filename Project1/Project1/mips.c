#include "mips.h"
#include <stdio.h>

//用于生成mips代码

//相应函数实现对应的指令&寄存器转换为字符串
string instr2str(MIPS_INSTR instr) {
	switch(instr) {
	case add:	return "add";
	case sub:	return "sub";
	case mult:	return "mult";
	case div:	return "div";
	case li:	return "li";
	case la:	return "la";
	case lw:	return "lw";
	case lh:	return "la";
	case sw:	return "sw";
	case sh:	return "sh";
	case mflo:	return "mflo";
	case mfhi:	return "mfhi";
	case jal:	return "jal";
	case j:		return "j";
	case jr:	return "jr";
	case beq:	return "beq";
	case bne:	return "bne";
	case blez:	return "blez";
	case bgtz:	return "bgtz";
	case bltz:	return "bltz";
	case bgez:	return "bgez";
	case and:	return "and";
	case or :	return "or";
	case xor:	return "xor";
	case nor:	return "nor";
	case andi:	return "andi";
	case ori:	return "ori";
	case xori:	return "xori";
	case nori:	return "nori";
	case slt:	return "slt";
	case slti:	return "slti";
	case syscall:	return "syscall";
	default:	return NULL;
	}
}

string regs2str(MIPS_REGS reg) {
	switch (reg) {
	case zero:	return "$0";
	case at:	return "$at";
	case v0:	return "$v0";
	case v1:	return "$v1";
	case a0:	return "$a0";
	case a1:	return "$a1";
	case a2:	return "$a2";
	case a3:	return "$a3";
	case t0:	return "$t0";
	case t1:	return "$t1";
	case t2:	return "$t2";
	case t3:	return "$t3";
	case t4:	return "$t4";
	case t5:	return "$t5";
	case t6:	return "$t6";
	case t7:	return "$t7";
	case t8:	return "$t8";
	case t9:	return "$t9";
	case s0:	return "$s0";
	case s1:	return "$s1";
	case s2:	return "$s2";
	case s3:	return "$s3";
	case s4:	return "$s4";
	case s5:	return "$s5";
	case s6:	return "$s6";
	case s7:	return "$s7";
	case gp:	return "$gp";
	case sp:	return "$sp";
	case tp:	return "$tp";
	case ra:	return "$ra";
	default:	return NULL;
	}
}
string regs2num(MIPS_REGS reg) {
	switch (reg) {
	case zero:	return "$0";
	case at:	return "$1";
	case v0:	return "$2";
	case v1:	return "$3";
	case a0:	return "$4";
	case a1:	return "$5";
	case a2:	return "$6";
	case a3:	return "$7";
	case t0:	return "$8";
	case t1:	return "$9";
	case t2:	return "$10";
	case t3:	return "$11";
	case t4:	return "$12";
	case t5:	return "$13";
	case t6:	return "$14";
	case t7:	return "$15";
	case t8:	return "$24";
	case t9:	return "$25";
	case s0:	return "$16";
	case s1:	return "$17";
	case s2:	return "$18";
	case s3:	return "$19";
	case s4:	return "$20";
	case s5:	return "$21";
	case s6:	return "$22";
	case s7:	return "$23";
	case gp:	return "$28";
	case sp:	return "$29";
	case tp:	return "$30";
	case ra:	return "$31";
	default:	return NULL;
	}
}

//向文件输出mips语句
void gen_mips(FILE* fp, MIPS_CODE code) {
	string rs, rt, rd;
	int offset;
	string label_name;
	rs = regs2str(code.rs);
	rt = regs2str(code.rt);
	rd = regs2str(code.rd);
	offset = code.offset;
	strcpy(code.label, label_name);
	switch (code.instr) {
	case add:
	case sub:
	case and:
	case or:
	case xor:
	case nor:
	case slt: { 	//各种需要三个寄存器的
		fprintf(fp, "%s %s %s %s\n", instr2str(code.instr), rd, rs, rt);
		break;
	}
	case andi:
	case ori:
	case xori:
	case nori:
	case slti:{		//各种需要两个寄存器一个立即数的
		fprintf(fp, "%s %s %s %d\n", instr2str(code.instr), rt, rs, offset);
		break;
	}
	case beq:
	case bne:
		fprintf(fp, "%s %s %s %s\n", instr2str(code.instr), rs, rt, label_name);
		break;
	}
	case blez:
	case bgtz:
	case bltz:
	case bgez:{
		fprintf(fp, "%s %s %s\n", instr2str(code.instr), rs, label_name);
		break;
	}
	case lw:
	case sw:
	case lh:
	case sh:{
		fprintf(fp, "%s %s %d(%s)\n", instr2str(code.instr), rt, offset, rs);
		break;
	}

	case li:{
		fprintf(fp, "%s %s %d\n", instr2str(code.instr), rt, offset);
		break;
	}

	case la:{
		fprintf(fp, "%s %s %s\n", instr2str(code.instr), rt, label_name);
		break;
	}

	case jal:
	case j:{
		fprintf(fp, "%s %s\n", instr2str(code.instr), label_name);
		break;
	}

	case mult:
	case div:{
		fprintf(fp, "%s %s %s\n", instr2str(code.instr), rs, rt);
		break;
	}

	case mfhi:
	case mflo:{
		fprintf(fp, "%s %s\n" ,instr2str(code.instr), rd);
		break;
	}

	case syscall:{
		fprintf(fp, "syscall\n");
		break;
	}
	default:	return;
	}
}
