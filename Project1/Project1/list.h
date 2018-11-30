#pragma once
#include <stdio.h>
#include "types.h"
#include "fourstate.h"

//这个头文件中声明了有关符号表的相关声明与结构

//列表的结构体声明

#define IS_VAR 10
#define IS_CONST 20

#define INIT 1
#define NOT_INIT -1

#define INSERT 1
#define EXIST 0

#define FIND 1
#define NOT_FIND -1

#define MAX_CONST_NUM 10
#define MAX_VAR_NUM 15

extern int now_hash;
extern struct FUNC* func_list;

//全局结构体

//(1)函数部分
typedef struct {	
	char* name;	//函数的名字
	int type;	//返回值的类型
	int hash;
	CODE4* first_code4;	//第一个4元式
	CODE4* last_code4;	//最后一个4元式
	CONST* cons;//函数需要的所有常量
	VAR* vars;	//函数需要的所有变量
	int args_num;//函数需要的参数个数
	int cons_num;	//函数的常量个数
	int vars_num;	//函数的变量个数	

}FUNC;

//(2)常量部分
typedef struct {
	char* name;		//常量名称
	int type;		//常量类型
	int hash;	//常量名称对应的hash值
	int value;		//常量值
}CONST;

//(3)变量部分
//实际上结构体与变量基本一致
typedef struct {
	char* name;		//变量名称
	int type;		//变量类型
	int hash;	//变量名称对应的hash值
	int value;		//变量值
	int init;		//变量是否被赋值
	int array_len;	//数组长度，如果不是数组则长度默认为0
	ARRAY* array;	//该变量数组所对应的数组表
}VAR;

//(4)数组部分
typedef struct {
	int value;		//该地址对应的数组值（字符被化为整型存储）
	int init;
	//Reserved
}ARRAY;

//(5)参数表部分
typedef struct {
	int type;
	char* name;
	int value;	//参数的值是有意义的
}ARGS;
//设置过程中把它省略了，并入到了var中

int LIST_SEARCH_VARS(VAR* vars_list, int hash, int var_num, VAR** ret_var);
int LIST_SEARCH_CONSTS(CONST* consts_list, int hash, int const_num, CONST** ret_const);
int LIST_INSERT_CONSTS(CONST* consts_list, CONST insert_const, int* const_num);
int LIST_INSERT_VARS(VAR* vars_list, VAR insert_var, int* var_num);
CONST* imp_const(int type, char* name, int value);
VAR* imp_var(int type, char* name, int value, int array_len);
int imp_func(int type, char* name, CONST* cons_list, VAR* vars_list, int cons_num, int vars_num);