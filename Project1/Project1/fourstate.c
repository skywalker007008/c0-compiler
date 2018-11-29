#include "fourstate.h"
#include "mips.h"
#include "list.h"
#include "grammar.h"
#include <string.h>

//INSERT 4_state into func list

void INSERT_4_STATE(FUNC* func, STATE4_OP op, char* rs, char* rt, char* rd, char* label) {
	CODE4* temp_code4;
	temp_code4 = (CODE4*)malloc(sizeof(CODE4));
	//strcpy(temp_code4->rs, rs);
	//strcpy(temp_code4->rt, rt);
	//strcpy(temp_code4->rd, rd);
	//strcpy(temp_code4->label, label);
	temp_code4->rs = rs;
	temp_code4->rt = rt;
	temp_code4->rd = rd;
	temp_code4->label = label;
	//等号的话是一个有关地址赋值的操作
	//这里要调一下，外面封装不放心得在这弄一下吧我觉得
	//倒也不用
	temp_code4->next_code4 = NULL;

	if (func->first_code4 == NULL) {
		func->first_code4 = temp_code4;
		func->last_code4 = temp_code4;
	}
	else {
		func->last_code4->next_code4 = temp_code4;
		func->last_code4 = temp_code4;
	}
}

char* int2str(int a) {
	char c[10]="int";
	char* i;
	itoa(a, i, 10);
	strcat(c, i);
	return c;
}
char* var2str(char* var) {
	char c[20] = "var";
	strcat(c, var);
	return c;
}
char* const2str(char* cons) {
	char c[20] = "const";
	strcat(c, cons);
	return c;
}
char* string_add2(char* a, char* b) {
	char* c;
	int la, lb;
	la = strlen(a);
	lb = strlen(b);
	c = (char*)malloc((la + lb + 1) * sizeof(char));
	strcpy(c, a);
	strcat(c, "_");
	strcat(c, b);
	return c;
}