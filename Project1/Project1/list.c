#include <stdio.h>
#include "list.h"


//list.c中包含了所有用于生成符号表的函数

FUNC* func_list;
int func_num;
int now_hash;	//hash值相关

int hash() {
	int i = now_hash;
	now_hash++;
	return i;
}

int LIST_SEARCH_CONSTS(CONST* consts_list, int hash, int const_num, CONST** ret_const){
	CONST* temp_const;
	int min = 0;
	int max = const_num - 1;
	int mid = (min + max) / 2;
	while(1){
		if((consts_list + mid) -> hash == hash){
			*ret_const = consts_list + mid;
			return FIND;
		}
		else if((consts_list + mid) -> hash < hash){
			min = mid;
		}
		else
			max = mid;

		mid = (max + min) / 2;
		if(mid == min)
			return NOT_FIND;
	}
}
int LIST_SEARCH_VARS(VAR* vars_list, int hash, int var_num, VAR** ret_var){
	VAR* temp_var;
	int min = 0;
	int max = var_num - 1;
	int mid = (min + max) / 2;
	while(1){
		if((vars_list + mid) -> hash == hash){
			*ret_var = vars_list + mid;
			return FIND;
		}
		else if((vars_list + mid) -> hash < hash){
			min = mid;
		}
		else
			max = mid;

		mid = (max + min) / 2;
		if(mid == min)
			return NOT_FIND;
	}
}
int LIST_INSERT_CONSTS(CONST* consts_list, CONST insert_const, int* const_num) {
	int i;
	*(consts_list + *const_num) = insert_const;
	*const_num = *const_num + 1;
	return INSERT;
}
int LIST_INSERT_VARS(VAR* vars_list, VAR insert_var, int* var_num) {
	int i;
	*(vars_list + *var_num) = insert_var;
	*var_num = *var_num + 1;
	return INSERT;
}

//有关对VAR,CONST,FUNC的更新
//
//新增一个const的信息
CONST* imp_const(int type, char* name, int value){
	CONST* temp_const;
	temp_const = (CONST*)malloc(sizeof(CONST));
	temp_const->type = type;
	temp_const->name = name;
	temp_const->value = value;
	temp_const->hash = hash();
	return temp_const;
}
//新增一个var的信息
VAR* imp_var(int type, char* name, int value, int array_len){
	VAR* temp_var;
	temp_var = (VAR*)malloc(sizeof(VAR));
	temp_var->type = type;
	temp_var->name = name;
	temp_var->value = value;
	temp_var->hash = hash();
	temp_var->init = NOT_INIT;
	temp_var->array_len = array_len;
	if(array_len == 0)
		temp_var->array = NULL;
	else
		temp_var->array = (ARRAY*)malloc(array_len * sizeof(int));	

	return temp_var;
}
//函数信息的登陆
FUNC* imp_func(int type, char* name, CONST* cons_list, VAR* vars_list, int cons_num, int vars_num){
	//先找一遍有没有被定义过这个函数
	int i = 0;
	FUNC* temp_func;
	for(i = 0;i < func_num;i++){
		if(strcmp(name, func_list[i].name) == 1)
			return REDEFINE;
	}
	temp_func = (FUNC*)malloc(sizeof(FUNC));
	temp_func -> name = name;
	temp_func -> hash = hash();
	temp_func -> cons = cons_list;
	temp_func -> vars = vars_list;
	temp_func -> cons_num = cons_num;
	temp_func -> vars_num = vars_num;
	temp_func -> type = type;
	*(func_list + func_num) = temp_func;
	return temp_func;
}

FUNC* INIT_FUNC(int type, char* name) {
	FUNC* func = func_list + func_num;
	func->args_num = 0;
	func->cons = (CONST*)malloc(MAX_CONST_NUM * sizeof(CONST));
	func->cons_num = 0;
	func->type = type;
	func->name = name;
	func->vars = (VAR*)malloc(MAX_VAR_NUM * sizeof(VAR));
	func->vars_num = 0;
	func->first_code4 = NULL;
	func->last_code4 = NULL;
	func->hash = hash();
	return func;
}



