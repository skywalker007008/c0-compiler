#include <stdio.h>
#include "list.h"

//list.c中包含了所有用于生成符号表的函数

FUNC func_list;
int func_num;
//LIST有关函数

//(1)在变量表中搜索
//如果在列表中找到了该变量，那么就返回将该变量的VAR存储到*ret_var中，并返回FIND
//如果没找到，返回一个正数(temp_pl)表示可能的插入位置
int LIST_SEARCH_VARS(VAR* vars_list, double hash, int var_num, VAR** ret_var) {
	int find_pl;
	int min_pl = 1;
	int max_pl = var_num;
	int mid_pl = (min_pl + max_pl) / 2;
	VAR* temp_var;
	while (mid_pl != min_pl) {
		temp_var = (vars_list + mid_pl);
		if (hash < temp_var->hash) {
			max_pl = mid_pl;
		}
		else if (hash > temp_var->hash) {
			min_pl = mid_pl;
		}
		else if (hash == temp_var->hash) {
			*ret_var = temp_var;
			return FIND;
		}
		mid_pl = (min_pl + max_pl) / 2;
	}
	*ret_var = NULL;
	return max_pl;
}

//(2)插入到变量表中
//如果插入成功则返回INSERT，否则返回EXIST表示该变量已经存在
int LIST_INSERT_VARS(VAR* vars_list, VAR insert_var, int var_num, int insert_pl) {
	int i;
	VAR* temp_var = NULL;
	for (i = var_num + 1; i > insert_pl; i--) {
		*(vars_list + i) = *(vars_list + i - 1);
	}
	*(vars_list + insert_pl) = insert_var;
	return INSERT;
}
//由于不存在删除操作，因此这部分没必要写。
//常量的操作十分类似
//(1)在常量表中搜索
int LIST_SEARCH_CONSTS(CONST* consts_list, double hash, int const_num, CONST** ret_const) {
	int find_pl;
	int min_pl = 1;
	int max_pl = const_num;
	int mid_pl = (min_pl + max_pl) / 2;
	CONST* temp_const;
	while (mid_pl != min_pl) {
		temp_const = (consts_list + mid_pl);
		if (hash < temp_const->hash) {
			max_pl = mid_pl;
		}
		else if (hash > temp_const->hash) {
			min_pl = mid_pl;
		}
		else if (hash == temp_const->hash) {
			*ret_const = temp_const;
			return FIND;
		}
		mid_pl = (min_pl + max_pl) / 2;
	}
	*ret_const = NULL;
	return max_pl;
}

//(2)插入到变量表中
//如果插入成功则返回INSERT，否则返回EXIST表示该变量已经存在
int LIST_INSERT_CONSTS(CONST* consts_list, CONST insert_const, int const_num, int insert_pl) {
	int i;
	CONST* temp_const = NULL;
	for (i = const_num + 1; i > insert_pl; i--) {
		*(consts_list + i) = *(consts_list + i - 1);
	}
	*(consts_list + insert_pl) = insert_const;
	return INSERT;
}

//有关对VAR,CONST,FUNC的更新
//
//新增一个const的信息
CONST* imp_const(int type, string name, int value){
	CONST* temp_const;
	temp_const = (CONST*)malloc(sizeof(CONST));
	temp_const->type = type;
	temp_const->name = name;
	temp_const->value = value;
	temp_const->hash = hash(name);
	return temp_const;
}
//新增一个var的信息
VAR* imp_var(int type, string name, int value, int array_len){
	VAR* temp_var;
	temp_var = (VAR*)malloc(sizeof(VAR));
	temp_var->type = type;
	temp_var->name = name;
	temp_var->value = value;
	temp_var->hash = hash(name);
	temp_var->init = NO_INIT;
	temp_var->array_len = array_len;
	if(array_len == 0)
		array = NULL;
	else
		array = (ARRAY*)malloc(array_len * sizeof(int));	//size: ARRAT = int

	return temp_var;
}
//函数信息的登陆
int imp_func(int type, string name, CONST* cons_list, VAR* vars_list, int cons_num, int vars_num){
	//先找一遍有没有被定义过这个函数
	int i = 0;
	FUNC* temp_func;
	for(i = 0;i < func_num;i++){
		if(strcmp(name, func_list[i].name) == 1)
			return REDEFINE;
	}
	temp_func = (FUNC*)malloc(sizeof(FUNC));
	temp_func -> name = name;
	temp_func -> cons = cons_list;
	temp_func -> vars = vars_list;
	temp_func -> cons_num = cons_num;
	temp_func -> vars_num = vars_num;
	temp_func -> type = type;
	func_list[func_num] = temp_func;
	return DONE;
}



