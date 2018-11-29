#pragma once
//用于生成最后的mips代码

//mips的有关指令
typedef enum MIPS_INSTRUCTIONS {
	add = 1, sub, mult, div,	//运算指令
	li, la, lw, lh,	//取指令
	sw, sh,	//存指令
	mflo, mfhi,	//乘除单元取指令
	jal, j, jr,	//跳转相关指令
	beq, bne, blez, bgtz, bltz, bgez,	//比较跳转指令
	and, or , xor, nor, andi, ori, xori, nori,	//逻辑运算指令
	slt, slti,	//比较指令
	syscall,	//系统调用指令
}MIPS_INSTR;

//mips有关寄存器
typedef enum MIPS_REGISTERS {
	zero = 0, //0号寄存器
	at,	//reserved
	v0, v1,	//return value
	a0, a1, a2, a3,	//args for func
	t0, t1, t2, t3, t4, t5, t6, t7,	//temp_registers
	s0, s1, s2, s3, s4, s5, s6, s7,	//global_registers
	t8, t9,	//temp_registers
	gp,	//global pointer
	sp,	//stack pointer
	tp, //frame pointer
	ra,	//return address
}MIPS_REGS;

//mips代码的存储结构
typedef struct {
	MIPS_INSTR instr;	//对应的mips指令
	MIPS_REGS rs, rt, rd;	//对应的rs, rt, rd
	int offset;			//用于读取所需要加载的偏移
	string label;
	struct MIPS_CODE* next_code;
}MIPS_CODE;

//mips代码的分程序结构
typedef struct {
	string label_name;
	MIPS_CODE* first_code;
}MIPS_PART;