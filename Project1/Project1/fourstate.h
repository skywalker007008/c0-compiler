#pragma once

//四元式相关的结构

/*static address stores each temp_regs name*/
//extern static char temp_regs[10][3] = { "t0","t1","t2","t3","t4","t5","t6","t7","t8","t9" };

typedef enum {
	add = 1, sub, mult, div,
	eq, neq, bgt, beq, lst, leq,
	assign, assign_const, assign_var,
	get_addr, write_addr,
	var_declare,
	save_temp, reload_temp,
	write_stack, read_stack,
	jump, jump_ifnz, jump_ifz, jump_link, jump_backto,
	read_char, read_int,
	write_char, write_int, write_string,
	set_label, set_func
}STATE4_OP;

typedef enum {
	t0 = 0, t1,	//used in item's add/sub
	t2, t3, t4, t5, t6, t7,
	t8, t9	//used in load&store in stack
}CODE4_TEMPREG;

typedef struct CODE4{
	STATE4_OP op;
	char* rs;
	char* rt;
	char* rd;
	char* label;
	struct CODE4* next_code4;
}CODE4;
//transfer-type funcs
char* int2str(int a);
char* var2str(char* var);
char* const2str(char* cons);
char* string_add2(char* a, char* b);
char* int2reg(int a);

//store func
void INSERT_4_STATE(FUNC* func, STATE4_OP op, char* rs, char* rt, char* rd, char* label);
