#pragma once
#include "types.h"
#include <stdlib.h>

#define ID_MAX_LENGTH 20

int line;
struct symbol_list{
	struct symbol_str* first;
	struct symbol_str* bot;
	int symbol_num;
};

struct symbol_str{
	int line;	//which line it appears
	int symbol_type;
	//type symbol_type;
	char* symbol_name;
	int symbol_num;
	struct symbol_str* next_symbol;
};

extern struct symbol_list s_list;
void store(int symbol_type, char* symbol_name, int symbol_num);


