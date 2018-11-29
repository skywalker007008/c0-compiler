
#include "grammar.h"
#include "types.h"
#include "define.h"
#include "symbol.h"
#include<stdio.h>

int main() {
	int i;
	id_addr = (char*)malloc(MAX_KEYWORD_LENGTH * sizeof(char));
	line_char = (char*)malloc(MAX_LINE_CHARS * sizeof(char));
	//	fpin = (FILE*)malloc(sizeof(FILE));
	//	fpout = (FILE*)malloc(sizeof(FILE));
	word_analysis();

	fclose(fpin);
	fclose(fpout);
	//fp_gram = fopen("grammar_result.txt", "w");
	i = grammar_judgement();
	fclose(fp_gram);
	//free(fpin);
	//free(fpout);
	return 0;
}