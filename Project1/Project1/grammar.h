#pragma once
#include "word.h"
#include "symbol.h"
#include "types.h"

/*Error Deal*/

#define error() do{return -1;}while(0)

/*Define of ret value*/

#define MATCH 1
#define BUGS -1

/*Global vars*/

extern struct symbol_str *now_symbol, *last_symbol;	//record of this symbol and the last symbol
extern int sym_pl;		//the place in the symbol_list
extern int sym;	//current symbol just read

/*Flag on whether it should stop*/
extern int gram_end;

/*File Point to make outprint*/
extern FILE *fp_gram;

void read_symbol();
int is_program(int pl);
int is_const_option(int pl);
int is_const_define(int pl);
int is_head_declare(int pl);
int is_var_option(int pl);
int is_var_define(int pl);
int is_ret_func(int pl);
int is_void_func(int pl);
int is_main(int pl);
int is_integer(int pl);
int is_com_sentence(int pl);
int is_assign(int pl);
int is_if_sentence(int pl);
int is_func_usage(int pl);
int is_value_args_list(int pl);
int is_sent_list(int pl);
int is_scanf(int pl);
int is_printf(int pl);
int is_return(int pl);
int is_circle_sentence(int pl);
int is_sentence(int pl);
int is_args_list(int pl);
int is_statement(int pl);
int is_part(int pl);
int is_factor(int pl);
int is_condition(int pl);
int grammar_judgement();