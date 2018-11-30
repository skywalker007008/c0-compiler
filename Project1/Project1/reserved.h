#pragma once
//各种之前写错不用了的东西放在这里

typedef struct {
	list_t* first;
	int list_num;
}list;
//链表的结构体声明

typedef struct {
	list_t* prev;
	list_t* next;
}list_t;
//如此要求，list_t的声明必须放在首位，
//这样才能保证发生list指向操作时，可以指到对应结构体

//如果采用的是连续存储空间，则不需要考虑这个链表的问题
//相反，链表不适用于二分查找，插入与查找操作都会很慢。

//形参结构体
typedef struct {
	int type;	//形参类型
	int isarray;	//形参是否是数组类型（saved，实际上文法不允许）
	char* name;	//该形参的名称
	arg* next_arg;	//下一个形参地址
}arg;

//全局结构体
//(1)函数部分
typedef struct {
	list_t point;	//链表指针（saved）
	char* name;		//函数的名字
	arg* args;	//函数需要的所有变量
	int arg_num;	//函数的变量个数
}FUNC;
//(2)常量部分
typedef struct {
	list_t point;
	int type;		//常量类型
	FUNC* func；		//声明的所在函数（全局量情况为MAIN）
		char* name;		//常量名称
	int value;		//常量值
}CONST;
//(3)变量部分
//实际上结构体与变量基本一致
typedef struct {
	list_t point;
	int type;		//变量类型
	FUNC* func；		//声明的所在函数（全局量情况为MAIN）
		char* name;		//变量名称
	int value;		//变量值
}VARS;

//定义相关函数去找/插/改相关的列表项

void LIST_SEARCH(int type, void* store_str, int store_num) {

	if (type == VARS)
}


//插入表时的另一种Imp_const风格,封装效果好
int imp_const(int type, symbol_str* now_symbol, FUNC func) {
	CONST** temp_const;
	CONST* store_const;
	store_const = imp_const(intsy, now_symbol->symbol_name, now_symbol->symbol_num);
	i = LIST_SEARCH_CONSTS(func->cons, store_const->hash, func->cons_num, temp_const);
	if (i == FIND) {
		error(args_redefined);
		return ERROR;
	}
	LIST_INSERT_CONSTS(func->cons, store_const->hash, func->cons_num, i);
	return 1;
}

//LIST有关函数

//(1)在变量表中搜索
//如果在列表中找到了该变量，那么就返回将该变量的VAR存储到*ret_var中，并返回FIND
//如果没找到，返回一个正数(temp_pl)表示可能的插入位置
/*int LIST_SEARCH_VARS(VAR* vars_list, double hash, int var_num, VAR** ret_var) {
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
*/

//(1)在变量表中查找
//思路很简单，线性的找这个名字就行了
int LIST_SEARCH_VARS(VAR* vars_list, char* name, int var_num, VAR** ret_var){
	VAR* temp_var;
	int i;
	for(i = 0;i < var_num;i++){
		temp_var = vars_list + i;
		if(strcmp(temp_var->name, name) == 0){
			*ret_var = temp_var;
			return FIND;
		}
	}
	return NOT_FIND;
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
/*int LIST_SEARCH_CONSTS(CONST* consts_list, double hash, int const_num, CONST** ret_const) {
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
}*/

//(1)在常量表搜索
int LIST_SEARCH_CONSTS(CONST* consts_list, char* name, int const_num, CONST** ret_const){
	CONST* temp_const;
	int i;
	for(i = 0;i < const_num;i++){
		temp_const = consts_list + i;
		if(strcmp(temp_const->name, name) == 0){
			*ret_const = temp_const;
			return FIND;
		}
	}
	return NOT_FIND;
}

//(2)插入常量表中
//如果插入成功则返回INSERT，否则返回EXIST表示该常量已经存在
int LIST_INSERT_CONSTS(CONST* consts_list, CONST insert_const, int const_num, int insert_pl) {
	int i;
	CONST* temp_const = NULL;
	for (i = const_num + 1; i > insert_pl; i--) {
		*(consts_list + i) = *(consts_list + i - 1);
	}
	*(consts_list + insert_pl) = insert_const;
	return INSERT;
}
