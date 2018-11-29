#pragma once
//������������mips����

//mips���й�ָ��
typedef enum MIPS_INSTRUCTIONS {
	add = 1, sub, mult, div,	//����ָ��
	li, la, lw, lh,	//ȡָ��
	sw, sh,	//��ָ��
	mflo, mfhi,	//�˳���Ԫȡָ��
	jal, j, jr,	//��ת���ָ��
	beq, bne, blez, bgtz, bltz, bgez,	//�Ƚ���תָ��
	and, or , xor, nor, andi, ori, xori, nori,	//�߼�����ָ��
	slt, slti,	//�Ƚ�ָ��
	syscall,	//ϵͳ����ָ��
}MIPS_INSTR;

//mips�йؼĴ���
typedef enum MIPS_REGISTERS {
	zero = 0, //0�żĴ���
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

//mips����Ĵ洢�ṹ
typedef struct {
	MIPS_INSTR instr;	//��Ӧ��mipsָ��
	MIPS_REGS rs, rt, rd;	//��Ӧ��rs, rt, rd
	int offset;			//���ڶ�ȡ����Ҫ���ص�ƫ��
	string label;
	struct MIPS_CODE* next_code;
}MIPS_CODE;

//mips����ķֳ���ṹ
typedef struct {
	string label_name;
	MIPS_CODE* first_code;
}MIPS_PART;