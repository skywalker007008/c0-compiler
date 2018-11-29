#pragma once
#include<stdio.h>
#include<stdlib.h>
#include "types.h"

#define MAX_LINE_CHARS 50
#define MAX_LINES 200
#define NOFOUND	-1	

//¶¨Òå±£Áô×Ö

#define KEY_WORDS_NUM 12

/*GLOBAL VARS*/

/*vars_for_read*/

extern int char_pl;		//temperary place of char
extern int line;			//No. of line
extern int line_len;		//line's length
extern char char_read;			//char_read
extern char* line_char;		//store line's word
extern int end;



/*vars_for_analysis*/

extern char *id_addr;				//id first addr
extern int id_length;			//id's length
extern int int_num;			//integer's value
extern int judge;				//if it is key_word
extern char Alpha;

/*vars_for_type_list*/

//static struct symbol_list symbols;
//struct symbol_str symbol;

extern FILE* fpin;
extern FILE* fpout;

int readline();
char read();
void readfile();
int is_keys(char* word, int length);
int next_symbol();
void word_analysis();
