#include "grammar.h"
#include "types.h"
#include "define.h"
#include "symbol.h"
#include <stdio.h>
#include <string.h>
#include "rule.h"
#include "list.h"
#include "fourstate.h"


//语法+符号表登陆

/*Define of ret value*/

#define MATCH 1
#define BUGS -1

/*Global vars*/

struct symbol_str *now_symbol, *last_symbol;	//record of this symbol and the last symbol
int sym_pl;		//the place in the symbol_list
int sym;	//current symbol just read

/*Flag on whether it should stop*/
int gram_end;

/*File Point to make outprint*/
FILE *fp_gram;
FILE *fp_code4;
FILE *fp_mips;


/*Global structure stores vars or const*/
FUNC global_func;
CONST global_const;
VAR global_var;

/*Global vars about func*/
FUNC* now_func;	//可以用来做函数调用时的优化使用
int func_num;

/* This file contains functions about the process of grammar analysis. */

void read_symbol() {
	/* Read the next symbol in the list.*/

	last_symbol = now_symbol;
	now_symbol = now_symbol->next_symbol;	//update symbol information
	if (now_symbol == NULL) {
		fprintf(fp_gram, "The file has been read to the end.\n");
		gram_end = 1;
		sym = MAX_SYM_NUM + 1;	//Nothing match;
		return;
	}
	sym_pl++;
	sym = now_symbol -> symbol_type;
	
}

/*主程序的判别函数*/

int is_program(int pl) {				//If it is the program.
	int judge;
	int temp_sym;
	int start_line,end_line;
	struct symbol_str * temp_symbol;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;
	if (sym == consy) {	//进入常量说明递归子程序
		judge = is_const_option(sym_pl, &global_func);
		if (judge != MATCH)
			error();
	}
	if(sym == intsy || sym == charsy){	//要判断一下是变量说明还是有返回值函数定义
		temp_symbol = now_symbol->next_symbol;
		temp_sym = temp_symbol->symbol_type;	//提前读入一个，应该是ident类型
		if (temp_sym != ident)
			error();
		temp_symbol = temp_symbol->next_symbol;
		temp_sym = temp_symbol->symbol_type;	//再提前读一个，到这里就可以分开了。
		if (temp_sym == lparent || temp_sym == lbrack) {	//进入函数定义了
			judge = is_ret_func(sym_pl);
			if (judge != MATCH)
				error();
		}
		else if (temp_sym == split || temp_sym == lbrace) {	//意味着读到了一个变量定义。
			judge = is_var_option(sym_pl, &global_func);
			if (judge != MATCH)
				error();
		}
		else
			error();
		sym = now_symbol->symbol_type;
		if (sym == intsy || sym == charsy) {	//这里应该开始就不会有变量定义了，那就一定是有返回函数的定义
			judge = is_ret_func(sym_pl);
			if (judge != MATCH)
				error();
		}
		else if (sym == voidsy) {		//开始声明主函数或者是无返回函数
			temp_symbol = now_symbol->next_symbol;
			temp_sym = temp_symbol->symbol_type;
			if (temp_sym == mainsy) {	//开始声明main函数
				judge = is_main(sym_pl);
				if (judge != MATCH)
					error();
				return MATCH;
			}
			else if (temp_sym == ident) {	//开始声明无返回函数
				judge = is_void_func(sym_pl);
				if (judge != MATCH)
					error();
			}
			else
				error();
		}
		else
			error();
		//如果函数没有返回，那么就运行到这里，开始启动模块的循环检测。
		while (1) {
			sym = now_symbol->symbol_type;
			if (sym == intsy || sym == charsy) {	//这里应该开始就不会有变量定义了，那就一定是有返回函数的定义
				judge = is_ret_func(sym_pl);
				if (judge != MATCH){
					error();
				}
			}
			else if (sym == voidsy) {		//开始声明主函数或者是无返回函数
				temp_symbol = now_symbol->next_symbol;
				temp_sym = temp_symbol->symbol_type;
				if (temp_sym == mainsy) {	//开始声明main函数
					judge = is_main(sym_pl);
					if (judge != MATCH){
						error(); 
					}
					end_line = last_symbol->line;
					fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is a program.\n", start_line, end_line, pl, sym_pl - 1);
					return MATCH;
				}
				else if (temp_sym == ident) {	//开始声明无返回函数
					judge = is_void_func(sym_pl);
					if (judge != MATCH){
						error();
					}
				}
				else{
					error();
				}
			}
			else{
				error();
			}
		}
	}
	//如果走到了这里，那意味着压根就没有变量声明
	//由于进入模块相同，因此使用的代码应相同
	else if (sym == voidsy) {		//开始声明主函数或者是无返回函数
		temp_symbol = now_symbol->next_symbol;
		temp_sym = temp_symbol->symbol_type;
		if (temp_sym == mainsy) {	//开始声明main函数
			judge = is_main(sym_pl);
			if (judge != MATCH){
				error();
			}
			end_line = last_symbol->line;
			fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is a program.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		else if (temp_sym == ident) {	//开始声明无返回函数
			judge = is_void_func(sym_pl);
			if (judge != MATCH){
				error();
			}
		}
		else{
			error();
		}
	}
	else{
		error();
	}
	//如果函数没有返回，那么就运行到这里，开始启动模块的循环检测。
	while (1) {
		sym = now_symbol->symbol_type;
		if (sym == intsy || sym == charsy) {	//这里应该开始就不会有变量定义了，那就一定是有返回函数的定义
			judge = is_var_option(sym_pl);
			if (judge != MATCH){
				error();
			}
		}
		else if (sym == voidsy) {		//开始声明主函数或者是无返回函数
			temp_symbol = now_symbol->next_symbol;
			temp_sym = temp_symbol->symbol_type;
			if (temp_sym == mainsy) {	//开始声明main函数
				judge = is_main(sym_pl);
				if (judge != MATCH){
					error();
				}
				end_line = last_symbol->line;
				fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is a program.\n", start_line, end_line, pl, sym_pl - 1);
				return MATCH;
			}
			else if (temp_sym == ident) {	//开始声明无返回函数
				judge = is_void_func(sym_pl);
				if (judge != MATCH){
					error();
				}
			}
			else{
				error();
			}
		}
		else{
			error();
		}
	}
	//到这里这个函数定义应该就完事了。
	//fprintf(fp_gram, "%d ~ %d\tThis is a program", pl, sym_pl);
	//read_symbol();
}


/*常量说明的判别函数*/

int is_const_option(int pl, FUNC* func) {			//If it is const's option sentence.
	int judge;
	int start_line, end_line;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;
	if (sym != consy){	//开头如果不是const，那就是有问题
		error();
	}
	read_symbol();
	judge = is_const_define(sym_pl, func);	//正常进行，这里应该是常量定义
	if (judge != MATCH){
		error();
	}
	
	sym = now_symbol->symbol_type;	//需要更新一下当前读到的symbol形式
	if (sym != split)	//应该分号结束一个定义
		error();
	read_symbol();
	sym = now_symbol->symbol_type;
	if (sym != consy) {	//常量定义结束了
		end_line = last_symbol->line;
		fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is const option.\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	else {	//还有其他定义，还没完……
		
		while (1) {
			read_symbol();
			judge = is_const_define(sym_pl, func);	//正常进行，这里应该是常量定义
			if (judge != MATCH){
				error();
			}
			sym = now_symbol->symbol_type;	//需要更新一下当前读到的symbol形式
			if (sym != split)	//应该分号结束一个定义
				error();
			read_symbol();
			sym = now_symbol->symbol_type;
			if (sym != consy) {	//常量定义结束了
				end_line = last_symbol->line;
				fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is const option.\n", start_line, end_line, pl, sym_pl - 1);
				return MATCH;
			}
		}
	}
	return 0;
}
int is_const_define(int pl, FUNC* func){			//If it is const's definition sentence.
	//const_define中会读取到标识符的名字,数值并存储
	//常量分为两类，int型与char型。两类的判断模式完全不同
	int judge;
	int i, num;
	string name;
	CONST** temp_const;
	CONST* store_const;
	int start_line, end_line;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;
	if(sym == intsy){	//第一个定义的常量是一个int
		while(1){	//实际上，从这里就开始进入循环体了，没必要先写一次的样子
			read_symbol();
			sym = now_symbol->symbol_type;
			if (sym != ident) {	//下一项应该是标识符
				error();
			}
			name = now_symbol->symbol_name;
			read_symbol();
			sym = now_symbol->symbol_type;
			if (sym != assignsy) {	//下一项应该是等号
				error();
			}
			

			read_symbol();
			sym = now_symbol->symbol_type;
			if (sym != addsy && sym != minussy && sym != uintsy) {	//整数的first集
				error();
			}
			judge = is_integer(sym_pl, &num);
			if (judge != MATCH) {
				error();
			}
			//插入表项
			store_const = imp_const(intsy, name, num);
			i = LIST_SEARCH_CONSTS(func->cons, store_const->hash, func->cons_num, temp_const);
			if (i == FIND) {
				error(args_redefined);
			}
			LIST_INSERT_CONSTS(func->cons, store_const->hash, func->cons_num, i);
			INSERT_4_STATE(func, assign, int2str(num), NULL, name, NULL);	//	插一条常量赋值进去
			//插入结束
			//reserved.h中有封装版本

			sym = now_symbol->symbol_type;
			if (sym != comma) {	//没有逗号，意味着可以出去了
				end_line = last_symbol->line;
				fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is an int definition.\n", start_line, end_line, pl, sym_pl - 1);
				return MATCH;
			}
		}
	}
	else if(sym == charsy){	//第一个定义的是char
		while (1) {
			read_symbol();
			sym = now_symbol->symbol_type;
			if (sym != ident) {
				error();
			}
			name = now_symbol->symbol_name;
			read_symbol();
			sym = now_symbol->symbol_type;
			if (sym != assignsy) {
				error();
			}
			read_symbol();
			sym = now_symbol->symbol_type;
			if (sym != alphasy) {
				error();
			}
			num = now_symbol->symbol_num;

			//插入表项
			store_const = imp_const(charsy, name, num);
			i = LIST_SEARCH_CONSTS(func->cons, store_const->hash, func->cons_num, temp_const);
			if (i == FIND) {
				error(args_redefined);
			}
			LIST_INSERT_CONSTS(func->cons, store_const->hash, func->cons_num, i);
			//插入结束
			//reserved.h中有封装版本
			INSERT_4_STATE(func, assign, int2str((int)num), 0, name, NULL);	//	插一条常量赋值进去
			read_symbol();
			sym = now_symbol->symbol_type;
			if (sym != comma) {
				end_line = last_symbol->line;
				fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is a char definition.\n", start_line, end_line, pl, sym_pl - 1);
				return MATCH;
			}
		}
	}
}

/*声明头部判断*/

int is_head_declare(int pl, int* type, string* name) {	//type返回函数的返回值类型
	int judge;
	int start_line, end_line;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;
	if (sym != intsy && sym != charsy) {
		error();
	}
	*type = sym;
	read_symbol();
	sym = now_symbol->symbol_type;
	if (sym != ident) {
		error();
	}
	strcpy(name, now_symbol->symbol_name);
	end_line = now_symbol->line;
	fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is a head declaration.\n", start_line, end_line, pl, sym_pl);
	read_symbol();
	sym = now_symbol->symbol_type;
	return MATCH;
}

/*变量说明*/

int is_var_option(int pl, FUNC* func){			//If it is var's option
	int judge;
	int start_line, end_line;
	struct symbol_str* temp_symbol;
	int temp_sym;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;
	while(1){	//一开始就直接进入循环
		judge = is_var_define(sym_pl);
		if (judge != MATCH) {
			error();
		}
		if (sym != split) {
			error();
		}
		read_symbol();
		sym = now_symbol->symbol_type;
		if (sym != intsy && sym != charsy) {
			end_line = last_symbol->line;
			fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is a var option.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		//接下来要根据前读来判断，是还在定义变量，还是要出去了
		//int/char ident;
		//只要不满足，那就直接退出
		temp_symbol = now_symbol->next_symbol;
		temp_sym = temp_symbol->symbol_type;
		if (temp_sym != ident) {
			end_line = last_symbol->line;
			fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is a var option.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		temp_symbol = temp_symbol->next_symbol;
		temp_sym = temp_symbol->symbol_type;
		if (temp_sym != split) {
			end_line = last_symbol->line;
			fprintf(fp_gram, "Line %d ~ %d:\tword from %d ~ %d\tThis is a var option.\n", start_line, end_line, pl, sym_pl - 1);
		}
		//不断重复读入，实际上可以优化一下
	}
}
int is_var_define(int pl, FUNC* func){			//If it is var's definition
	//int judge;
	int start_line,end_line;
	int type;
	string name;
	VAR** temp_var;
	VAR* store_var;
	int array_len = 0;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;
	if(sym != intsy && sym != charsy){
		error();
	}
	type = sym;
	read_symbol();
	sym = now_symbol->symbol_type;
	while(1){
		if(sym != ident){
			error();
		}
		strcpy(name, now_symbol->symbol_name);
		read_symbol();
		if(sym == lbrace){	//读入到了一个中括号，数组模式
			read_symbol();
			if(sym != uintsy){
				error();
			}
			array_len = now_symbol->symbol_num;
			read_symbol();
			if(sym != rbrace){
				error();
			}	
			read_symbol();
		}
		else {
			array_len = 0;
		}
		//插入表项
		store_var = imp_var(type, name, 0, array_len);	//这个0是变量值的信息
		i = LIST_SEARCH_VARS(func->vars, store_var->hash, func->vars_num, temp_var);
		if (i == FIND) {
			error(args_redefined);
		}
		LIST_INSERT_VARS(func->vars, store_var->hash, func->vars_num, i);
		//插入结束
		//reserved.h中有封装版本
		//紧接着了，都是要判断逗号了。
		if(sym != comma){
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a var's definition.\n",start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		read_symbol();
	}
}

/*有返回函数定义声明*/

int is_ret_func(int pl){
	int judge;
	int start_line, end_line;
	int type;
	string name;
	FUNC* func;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;
	
	if(sym != intsy && sym != charsy){
		error();
	}
	//type = sym;
	judge = is_head_declare(sym_pl, &type, &name);
	if(judge != MATCH){
		error();
	}
	func = INIT_FUNC(type, name);
	if(sym == lparent){	//有参数表
		read_symbol();
		judge = is_args_list(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		if(sym != rparent){
			error();
		}
		read_symbol();
	}
	//准备判断{
	if(sym != lbrack){
		error();
	}
	if (LIST_SEARCH_FUNCS(func) == FIND) {
		error(func_redefined);
	}
	else if (strcmp(name, "main") == 1) {
		error(main_redefined);
	}
	LIST_INSERT_FUNCS(func);
	read_symbol();
	judge = is_com_sentence(sym_pl, func);
	if(judge != MATCH){
		error();
	}
	if(sym != rbrack){
		error();
	}
	end_line = now_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a ret-func declaration.\n", start_line, end_line, pl, sym_pl);
	read_symbol();
	func_num++;
	return MATCH;
}

/*无返回值函数定义*/

int is_void_func(int pl){	//基本和有返回值函数定义差不多。
	int judge;
	int start_line, end_line;
	string name;
	FUNC **func
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;

	if(sym != voidsy){
		error();
	}
	read_symbol();
	if(sym != ident){
		error();
	}
	read_symbol();
	strcpy(name, now_symbol->symbol_name);
	func = INIT_FUNC(voidsy, name);
	if(sym == lparent){	//有参数表
		read_symbol();
		judge = is_args_list(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		if(sym != rparent){
			error();
		}
		read_symbol();
	}
	//准备判断{
	if(sym != lbrack){
		error();
	}
	read_symbol();
	judge = is_com_sentence(sym_pl, func);
	if(judge != MATCH){
		error();
	}
	if(sym != rbrack){
		error();
	}
	end_line = now_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a void-func declaration.\n", start_line, end_line, pl, sym_pl);
	read_symbol();
	func_num++;
	return MATCH;
}

/*判断主函数*/

int is_main(int pl){
	int judge;
	int start_line, end_line;
	FUNC* func;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;

	if(sym != voidsy){
		error();
	}
	read_symbol();
	if(sym != mainsy){
		error();
	}
	func = INIT_FUNC(voidsy, "main");
	
	read_symbol();
	if(sym != lparent){
		error();
	}
	read_symbol();
	if(sym != rparent){
		error();
	}
	if (LIST_SEARCH_FUNCS(func) == FIND) {
		error(main_redefined);
	}
	LIST_INSERT_FUNCS(func);
	read_symbol();
	if(sym != lbrack){
		error();
	}
	read_symbol();
	judge = is_com_sentence(sym_pl);
	if(judge != MATCH){
		error();
	}
	if(sym != rbrack){
		error();
	}
	end_line = now_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is the main function.\n", start_line, end_line, pl, sym_pl);
	read_symbol();
	func_num++;
	return MATCH;
}

/*整数判断*/

int is_integer(int pl, int* num){
	//int judge;
	int start_line, end_line;
	int is_minus = 1;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;

	if(sym == addsy || sym == minussy){
		is_minus = sym == minussy ? -1 : 1;
		read_symbol();
	} 
	if(sym != uintsy){
		error();
	}
	*num = now_symbol->symbol_num * is_minus;
	end_line = now_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is an integer.\n", start_line, end_line, pl, sym_pl);
	read_symbol();
	return MATCH;
}

/*常量判断
//由于非终结符不可达，因此是多于项，删除……
*/

/*复合语句*/

int is_com_sentence(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;

	if(sym == consy){	//有一个常量的声明过程
		//read_symbol();
		judge = is_const_option(sym_pl, func);
		if(judge != MATCH){
			error();
		}
	}
	if(sym == intsy || sym == charsy){	//有一个变量声明的过程
		//read_symbol();
		judge = is_var_option(sym_pl, func);
		if(judge != MATCH){
			error();
		}
	}
	judge = is_sent_list(sym_pl, func);
	if(judge != MATCH){
		error();
	}
	end_line = last_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is composed sentences.\n", start_line, end_line, pl, sym_pl - 1);
	return MATCH;
}

/*赋值语句*/

int is_assign(int pl, FUNC* func){
	int judge;
	int isarray = 0;
	int start_line, end_line;
	char name[ID_MAX_LENGTH];
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;
	VAR* temp_var;

	if(sym != ident){
		error();
	}
	strcpy(name, now_symbol->symbol_name);
	if (LIST_SEARCH_VARS(func->vars, hash(name), func->vars_num, &temp_var) != FIND) {
		if (LIST_SEARCH_VARS(global_func.vars, hash(now_symbol->symbol_name), global_func.vars_num, &temp_var) != FIND) {
			error(args_undefined);
		}
	}
	if (temp_var->type != intsy) {
		error(char_assigned);
	}
	read_symbol();
	if(sym == lbrace){	//数组形式，先处理一下
		//这样需要先读取一个有关数据栈位置的问题
		read_symbol();
		judge = is_statement(sym_pl, func);
		INSERT_4_STATE(func, assign, "v0", NULL, "t0", NULL);
		//这里，v0是存储表达式结果的最后一步
		//在这里实际上可以……优化一下，有些时候根本不需要一次又一次的v0 = v0
		//甚至可能这里是一个常数项，都会一次又一次的重复
		if(judge != MATCH){
			error();
		}
		if(sym != rbrace){
			error();
		}
		read_symbol();
		INSERT_4_STATE(func, get_addr, name, "t8", "t0", NULL);	//读取了数组name的对应下标
		//其实可以直接用上面的v0
		is_array = 1;
	}
	//处理完毕
	if(sym != assignsy){
		error();
	}
	read_symbol();
	judge = is_statement(sym_pl, func);
	if (is_array == 0) {
		INSERT_4_STATE(func, assign, "v0", NULL, name, NULL);	//	非数组直接赋值
	}
	else {
		INSERT_4_STATE(func, write_stack, "v0", NULL, "t8", NULL);	//数组形式的话，写到对应空间里
	}
	if(judge != MATCH){
		error();
	}
	end_line = last_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is an assign sentence.\n",start_line, end_line, pl, sym_pl - 1);
	return MATCH;
}

/*条件语句*/

int is_if_sentence(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;

	if(sym != ifsy){
		error();
	}
	read_symbol();
	if(sym != lparent){
		error();
	}
	read_symbol();
	judge = is_condition(sym_pl, func);
	//v0是条件的结果
	INSERT_4_STATE(func, jumpifn, "v0", NULL, NULL, "else");	//添加一个有关条件跳转的四元式
	if(judge != MATCH){
		error();
	}
	if(sym != rparent){
		error();
	}
	read_symbol();
	judge = is_sentence(sym_pl, func);
	if(judge != MATCH){
		error();
	}
	if(sym != elsesy){	//没有else可以结束了
		INSERT_4_STATE(func, jump, NULL, NULL, NULL, "end");	//if的终结
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a condition sentence(without else).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	read_symbol();
	INSERT_4_STATE(func, set_label, NULL, NULL, NULL, "else");	//设置一个else的标记点
	judge = is_sentence(sym_pl, func);
	if(judge != MATCH){
		error();
	}
	//理论上……else不需要end，但是if需要啊，所以还是要加end标签，但不需要加jump end的四元式
	INSERT_4_STATE(func, set_label, NULL, NULL, NULL, "end");	//设置end标签
	end_line = last_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a condition sentence(with else).\n", start_line, end_line, pl, sym_pl - 1);
	return MATCH;
}

/*函数调用语句
//由于有无返回值这里定义模式相同，就都用一个说了
*/

int is_func_usage(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	char name[ID_MAX_LENGTH];
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;
	//这里会比较麻烦，因为涉及一个保存现场的问题

	//由于func_usage只在sentence中出现调用问题，因此在sentence里面保存现场应该会比较好
	//因子中的func_usage被拆开了，所以只在这里做会有问题
	//这个问题应该留在上层解决，不在这里处理了
	if(sym != ident){
		error();
	}
	strcpy(name, now_symbol->symbol_name);	//函数名字
	read_symbol();
	if(sym != lparent){	//没有参数
		//这里似乎要比一下参数个数的样子
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a func_usage sentence(without args).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	//有参数的话
	read_symbol();
	judge = is_value_args_list(sym_pl, func);
	//在这里吧，应该就……处理一下每个参数的值，要交给value_args_list解决了

	//或者参数个数在这里比一下也可以
	if(judge != MATCH){
		error();
	}
	if(sym != rparent){
		error();
	}
	//临结束前，需要做恢复现场的工作
	//既然保存交给上层了，那恢复也交给上层吧
	//跳转不能丢上去
	INSERT_4_STATE(func, jump_link, NULL, NULL, NULL, name);
	end_line = now_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a func_usage sentence(with args).\n", start_line, end_line, pl, sym_pl);
	read_symbol();
	return MATCH;

}

/*值参数表*/

int is_value_args_list(int pl, FUNC* func) {
	int judge;
	int start_line, end_line;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;

	judge = is_statement(sym_pl, func);
	//这里要判断类型是否符合
	//可以使用is_statement的返回值
	if (judge != MATCH) {
		error();
	}
	//比较合格之后，将这个值存储到函数的对应的全局空间中
	INSERT_4_STATE(func, write_stack, "v0", NULL, "sp", NULL);
	//对应的指针需要发生偏移
	INSERT_4_STATE(func, add, "sp", "sp", "int4", NULL);
	while (1) {
		if (sym == comma) {
			read_symbol();
			judge = is_statement(sym_pl, func);
			INSERT_4_STATE(func, write_stack, "v0", NULL, "sp", NULL);
			INSERT_4_STATE(func, add, "sp", "sp", "int4", NULL);
			//same as above
			if (judge != MATCH) {
				error();
			}
		}
		else {
			end_line = last_symbol->line;
			fprintf(fp_gram, "Line %d ~ %d:\t%d ~ %d:\tThis is a value_args_list.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
	}
}

/*语句列*/

int is_sent_list(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;
	while(1){
		if(sym != ifsy && sym != printsy && sym != scansy && sym != ident && sym != returnsy && sym != lbrack && sym != forsy && sym != whilesy && sym != split){	//不在语句的FIRST集中
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence_list.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		judge = is_sentence(sym_pl, func);
		if(judge != MATCH){
			error();
		}
	}
}

/*读语句*/

int is_scanf(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	int type;
	VAR** temp_var;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;

	if(sym != scansy){
		error();
	}
	read_symbol();
	if(sym != lparent){
		error();
	}
	read_symbol();
	//进入while(1),即多个变量使用的地方
	while(1){
		if(sym != ident){
			error();
		}
		if (LIST_SEARCH_VARS(func->vars, hash(now_symbol->symbol_name), func->vars_num, temp_var) == FIND) {
			error(args_undefined);
		}
		else if (LIST_SEARCH_VARS(global_func.vars, hash(now_symbol->symbol_name), global_func.vars_num, temp_var) == FIND) {
			error(args_undefined);
		}	
		type = now_symbol->symbol_type;
		if (type == charsy) {
			INSERT_4_STATE(func, read_char, NULL, NULL, now_symbol->symbol_name, NULL);
		}
		else if (type == intsy) {
			INSERT_4_STATE(func, read_int, NULL, NULL, now_symbol->symbol_name, NULL);
		}
		read_symbol();
		if(sym != comma){
			break;
		}
		read_symbol();
	}
	if(sym != rparent){
		error();
	}
	end_line = now_symbol->line;
	fprintf(fp_gram, "Line %d ~ %d:\t%d ~ %d:\tThis is a scanf sentence.\n", start_line, end_line, pl, sym_pl);
	read_symbol();
	return MATCH;
}

/*写语句*/

int is_printf(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;

	if(sym != printsy){
		error();
	}
	read_symbol();
	if(sym != lparent){
		error();
	}
	read_symbol();
	if(sym == strsy){	//先读入了一个字符串
		INSERT_4_STATE(func, write_string, NULL, NULL, NULL, now_symbol->symbol_name);
		read_symbol();
		if(sym != comma){
			if(sym != rparent){
				error();
			}
			end_line = now_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a printf sentence(only string).\n", start_line, end_line, pl, sym_pl);
			read_symbol();
			return MATCH;
		}
		else{	//有表达式要处理
			read_symbol();
		}
	}
	judge = is_statement(sym_pl, func);
	//实际上什么情况都有可能出现
	//后期优化需要考虑一下有关only var的问题
	INSERT_4_STATE(func, assign, "v0", NULL, "t1", NULL);
	INSERT_4_STATE(func, write_int, "t0", NULL, NULL, NULL);

	if(judge != MATCH){
		error();
	}
	if(sym != rparent){
		error();
	}
	end_line = now_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a printf sentence(with statement).\n", start_line, end_line, pl, sym_pl);
	read_symbol();
	return MATCH;
}

/*返回语句*/

int is_return(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;

	if(sym != returnsy){
		error();
	}
	//return的话就是一个jr ra的问题了
	read_symbol();
	if(sym != lparent){	//这就打算结束了
		if (func->type != voidsy) {
			error(return_no_args);
		}
		//无参数就在这里jr ra
		INSERT_4_STATE(func, jump_backto, "ra", NULL, NULL, NULL);
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a return sentence(without value).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	read_symbol();
	judge = is_statement(sym_pl, func);
	//这里的v0值我觉得不用动吧
	INSERT_4_STATE(func, jump_backto, "ra", NULL, NULL, NULL);
	if (return_type != func->type) {
		error(return_type_mismatch);
	}
	if(judge != MATCH){
		error();
	}
	if(sym != rparent){
		error();
	}
	read_symbol();
	end_line = last_symbol->line;
	fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a return sentence.\n", start_line, end_line, pl, sym_pl - 1);
	return MATCH;
}

/*循环语句*/

int is_circle_sentence(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	sym= now_symbol->symbol_type;
	start_line = now_symbol->line;

	if(sym == whilesy){	//while循环体
		read_symbol();
		if(sym != lparent){
			error();
		}
		read_symbol();
		judge = is_condition(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		if(sym != rparent){
			error();
		}
		read_symbol();
		judge = is_sentence(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a while-circle.\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	if(sym == forsy){	//for循环体
		read_symbol();
		if(sym != lparent){
			error();
		}
		read_symbol();
		if(sym != ident){
			error();
		}
		read_symbol();
		if(sym != assignsy){
			error();
		}
		read_symbol();
		judge = is_statement(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		if(sym != split){
			error();
		}
		read_symbol();
		judge = is_condition(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		if(sym != split){
			error();
		}
		read_symbol();
		if(sym != ident){
			error();
		}
		read_symbol();
		if(sym != assignsy){
			error();
		}
		read_symbol();
		if(sym != ident){
			error();
		}
		read_symbol();
		if(sym != addsy && sym != minussy){
			error();
		}
		read_symbol();
		if(sym != uintsy){
			error();
		}
		read_symbol();
		if(sym != rparent){
			error();
		}
		read_symbol();
		judge = is_sentence(sym_pl, minussy);
		if(judge != MATCH){
			error();
		}
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a if-circle sentence.\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	else {
		error();
	}
	return 0;
}

/*语句*/

int is_sentence(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	struct symbol_str* temp_symbol;
	int temp_sym;
	sym = now_symbol->symbol_type;
	start_line = now_symbol -> line;

	if(sym == ifsy){	//条件语句
		judge = is_if_sentence(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(if).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	if(sym == whilesy || sym == forsy){
		judge = is_circle_sentence(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(circle).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	if(sym == scansy){
		judge = is_scanf(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		//read_symbol();
		if(sym != split){
			error();
		}
		read_symbol();
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(scanf).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	if(sym == printsy){
		judge = is_printf(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		//read_symbol();
		if(sym != split){
			error();
		}
		read_symbol();
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(printf).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	if(sym == returnsy){
		judge = is_return(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		if(sym != split){
			error();
		}
		read_symbol();
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(return).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	if(sym == split){	//空语句
		read_symbol();
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(empty).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	if(sym == lbrack){
		read_symbol();
		judge = is_sent_list(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		if(sym != rbrack){
			error();
		}
		read_symbol();
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(sent_list).\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	if(sym == ident){
		temp_symbol = now_symbol -> next_symbol;
		temp_sym = temp_symbol->symbol_type;
		if(temp_sym == split){
			judge = is_func_usage(sym_pl, func);
			if(judge != MATCH){
				error();
			}
			if(sym != split){
				error();
			}
			read_symbol();
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(no_args_func_usage).\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		if(temp_sym == lparent){
			judge = is_func_usage(sym_pl, func);
			if(judge != MATCH){
				error();
			}
			if(sym != split){
				error();
			}
			read_symbol();
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(with_args_func_usage).\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		if(temp_sym == assignsy || temp_sym == lbrace){
			judge = is_assign(sym_pl, func);
			if(judge != MATCH){
				error();
			}
			if(sym != split){
				error();
			}
			read_symbol();
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a sentence(assign).\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		else{
			error();
		}
	}
	else{
		error();
	}
}

/*参数表*/

int is_args_list(int pl, FUNC* func){
	//int judge;
	int start_line, end_line;
	int type;
	string name;
	VAR* store_var;
	VAR** temp_var;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;
	while(1){
		if(sym != intsy && sym != charsy){
			error();
		}
		type = sym;
		read_symbol();
		if(sym != ident){
			error();
		}
		name = now_symbol->symbol_name;
		//插入表项
		store_var = imp_var(type, name, 0, 0);	//这个0是变量值的信息
		i = LIST_SEARCH_VARS(func->vars, store_var->hash, func->vars_num, temp_var);
		if (i == FIND) {
			error(args_redefined);
		}
		LIST_INSERT_VARS(func->vars, store_var->hash, func->vars_num, i);
		//插入结束
		//reserved.h中有封装版本

		read_symbol();
		if(sym != comma){	//没有逗号，就结束吧
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is an args_list.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;	
		}
		read_symbol();
	}	
	return 0;
}

/*表达式*/

int is_statement(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;

	if(sym == addsy || sym == minussy){
		read_symbol();
	}
	judge = is_part(sym_pl, func);
	if(judge != MATCH){
		error();
	}
	while(1){
		if(sym != addsy && sym != minussy){	//结束了，就一个项
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a statement.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		read_symbol();
		judge = is_part(sym_pl, func);
		if(judge != MATCH){
			error();
		}
	}
}

/*项*/

int is_part(int pl, FUNC* func){					//If it is "项"
	int judge = 0;
	int start_line, end_line;
	start_line = now_symbol->line;
	sym = now_symbol->symbol_type;

	judge = is_factor(sym_pl, func);
	if(judge != MATCH){
		error();
	}
	while(1){	//多个因子相乘
		if(sym != multsy && sym != divsy){	//end
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a part.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		read_symbol();
		judge = is_factor(sym_pl, func);
		if(judge != MATCH){
			error();
		}
	}
}

/*因子*/

int is_factor(int pl, FUNC* func) {				//If it is a factor（因子）
	int judge;
	int start_line, end_line;
	int num;
	string name;
	FUNC* temp_func;
	sym = now_symbol->symbol_type;
	start_line = now_symbol->line;

	if(sym == ident){	//标识符，又返回函数定义
		strcpy(name, now_symbol->symbol_name);
		read_symbol();
		if(sym == lparent){
			
			read_symbol();
			judge = is_value_args_list(sym_pl, func);
			if (LIST_SEARCH_FUNCS(name, &temp_func) != FIND) {
				error(args_undefined);
			}
			else if (temp_func->type == voidsy) {
				error(not_ret_func);
			}
			else if (temp_func->args_num != args_list_num) {	//undefined global vars
				error(args_not_equal);
			}
			if(judge != MATCH){
				error();
			}
			if(sym != rparent){
				error();
			}
			read_symbol();
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a factor.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		else if(sym == lbrace){
			read_symbol();
			judge = is_statement(sym_pl, func);	//根据一个全局变量可以判断越界问题（实际上不是在这里能判断的）
			if(judge != MATCH){
				error();
			}
			if(sym != rbrace){
				error();
			}
			read_symbol();
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a factor.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
		else{
			end_line = last_symbol->line;
			fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a factor.\n", start_line, end_line, pl, sym_pl - 1);
			return MATCH;
		}
	}
	else if(sym == lparent){
		read_symbol();
		judge = is_statement(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		if(sym != rparent){
			error();
		}
		read_symbol();
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a factor.\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	else if(sym == addsy || sym == minussy || sym == uintsy){
		judge = is_integer(sym_pl, &num);
		if(judge != MATCH){
			error();
		}
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a factor.\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	else if(sym == alphasy){
		read_symbol();
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a factor.\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	else{
		error();
	}
}

/*条件*/

int is_condition(int pl, FUNC* func){
	int judge;
	int start_line, end_line;
	sym = now_symbol->symbol_type;
	start_line = now_symbol -> line;

	judge = is_statement(sym_pl, func);
	if(judge != MATCH){
		error();
	}
	if(sym == leqsy || sym == lstsy || sym == bgtsy || sym == beqsy || sym == eqsy || sym == neqsy){
		read_symbol();
		judge = is_statement(sym_pl, func);
		if(judge != MATCH){
			error();
		}
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a condition.\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	else{
		end_line = last_symbol->line;
		fprintf(fp_gram,"Line %d ~ %d:\t%d ~ %d:\tThis is a condition.\n", start_line, end_line, pl, sym_pl - 1);
		return MATCH;
	}
	return 0;
}


int grammar_judgement() {		//main function of grammar's analysis. Return the grammar's type
	now_symbol = s_list.first;
	sym_pl = 1;
	fp_gram = fopen("grammar_result.txt", "w");
	return is_program(sym_pl);
}