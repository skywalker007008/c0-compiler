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