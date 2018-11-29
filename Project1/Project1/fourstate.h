#pragma once

//四元式相关的结构

typedef enum {
	add = 1, sub, mult, div,
	eq, neq, bgt, beq, lst, les,
	assign,
	get_addr,
	write_stack, read_stack,
	jump, jump_ifnz, jump_ifz, jump_link, jump_backto,
	read_char, read_int,
	write_char, write_int, write_string,
	set_label, set_func
}STATE4_OP;

typedef enum {
	t0 = 0, t1,	//used in item's add/sub
	t2, t3, t4, t5, t6, t7
}CODE4_TEMPREG;

typedef struct CODE4{
	STATE4_OP op;
	char* rs;
	char* rt;
	char* rd;
	char* label;
	struct CODE4* next_code4;
}CODE4;
//transfer funcs
char* int2str(int a);
char* var2str(char* var);
char* const2str(char* cons);

//store func
void INSERT_4_STATE(FUNC* func, STATE4_OP op, char* rs, char* rt, char* rd, char* label);
char* string_add2(char* a, char* b);