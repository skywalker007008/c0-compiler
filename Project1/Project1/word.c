
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "types.h"
#include "define.h"
//#include "id.h"
#include "word.h"
#include "symbol.h"

/*GLOBAL VARS*/

/*vars_for_read*/

int char_pl;		//temperary place of char
int line;			//No. of line
int line_len;		//line's length
char char_read;			//char_read
char* line_char;		//store line's word
int end;



/*vars_for_analysis*/

char *id_addr;				//id first addr
int id_length;			//id's length
int int_num;			//integer's value
int judge;				//if it is key_word
char Alpha;

/*vars_for_type_list*/

//static struct symbol_list symbols;
//struct symbol_str symbol;

FILE* fpin;
FILE* fpout;

struct symbol_list s_list;

//ordered by relationship
char key[KEY_WORDS_NUM][8] = { "for","if","else","while","scanf","printf","return","int","char","const","void", "main" };
int key_id[KEY_WORDS_NUM] = { forsy, ifsy, elsesy, whilesy, scansy, printsy, returnsy, intsy, charsy, consy, voidsy, mainsy };
char symkind[MAX_SYM_NUM + 1][10] = { "SPACE","IDENT","ADD","MINUS","MULT","DIV","INT","UINT","CHAR","CONST","STRING","LEQ","LST","EQUAL","NOT_EQUAL","BEQ","BGT","ASSIGN","Alpha",\
										"LPARENT","RPARENT","LBRACE","RBRACE","LBRACK","RBRACK","COMMA","SPLIT","FOR","IF","ELSE","WHILE","SCANF","PRINTF","RETURN","VOID","MAIN" };

//if ordered by alpabet
//static char key[KEY_WORDS_NUM][8] = {"for","if","else","while","scanf","printf","return","int","char"};
//static int key_id[KEY_WORDS_NUM] = {forsy, ifsy, elsesy, whilesy, scanfsy, printsy, returnsy, intsy, charsy};
//because each'ss firstword isn't same, so it is not necessary.



/*Read Function*/

int readline() {
	line_char = fgets(line_char, 100, fpin);
	line++;
	if (line_char == NULL) {
		return 0;
	}
	line_len = strlen(line_char);
	return 1;
}
char read(){
	if(char_pl <= line_len){	//having char
		return line_char[char_pl++];	//return the char
	}

	end = readline() ? 0 : 1;
	fprintf(fpout, "This is Line %d.\n", line);
	char_pl = 0;
	return '\0';

}



void readfile(){
	fpin = fopen("word_test.txt", "r");
	fpout = fopen("word_result.txt", "w");
}

void error(){
	printf("Line: %d Something Error Here!\n",line - 1);
}
/*Judge If Keys*/

int is_keys(char* word, int length){
	//word is the first add, length is the length
	int i = 0;
	
	//Using strcmp
	
/*	char* temp_word;
	temp_word = (char*)malloc((length + 1) * sizeof(char));
	strcpy(word, temp_word, length);
	temp_word[length] = '\0';	//read words
	for(i = 0;i < KEY_WORDS;i++){
		if(strcmp(word, key[i], length) == 1)	//judge if same
			return key_id[i];
	}

	return -1;
*/

	//Because each key's first alp is different, so just compare first and then compare other is ok.
	int t = 0;
	while(i < KEY_WORDS_NUM){
		if(word[0] == key[i][0]){	//first alp is same
			if(length == strlen(key[i])){	//length is same
				while(t < length){
					if (word[t] == key[i][t]) {	//each char is same
						t++;	//compare next one
					}
					else
						return -1;
				}
				return key_id[i];	//bingo id
			}
		}
		i++;
	}
	return -1;

}

/*Deal with Words*/

int next_symbol(){
	char ch;
	if (end == 1) {
		return 0;
	}
	ch = read();
	if(isDigit(ch)){		//read an integer
		int_num = c2i(ch);
		while(1){
			ch = read();
			if(isDigit(ch))
				int_num = 10 * int_num + c2i(ch);
			else{
				char_pl--;
				store(uintsy, 0, int_num);	//store(id_type, id's name, id's value)
				return uintsy;
			}
		}
	}
	else if(isHigher(ch) || isLower(ch) || ch == '_'){// read an char or string
		id_length = 1;
		if(isHigher(ch))
			*id_addr = lower(ch);
		else
			*id_addr = ch;
		while(1){
			ch = read();
			if(isHigher(ch))
				id_addr[id_length++] = lower(ch);
			else if(isLower(ch) || isDigit(ch) || ch == '_')
				id_addr[id_length++] = ch;
			else{
				id_addr[id_length] = '\0';
				char_pl--;
				judge = is_keys(id_addr, id_length);
				if(judge == NOFOUND){
					/*if(id_length == 1){
						store(charsy, id_addr, 0);
						return charsy;
					}*/
			
					store(ident, id_addr, 0);
		  			return ident;
					
				}
				else{
					store(judge, id_addr, 0);
					return judge;
				}
			}
		}
	}
	else if(ch == '<'){
		ch = read();
		if(ch == '='){
			store(leqsy, 0, 0);
			return leqsy;
		}
		else{
			char_pl--;
			store(lstsy, 0, 0);
			return lstsy;
		}
	}
	else if(ch == '>'){
		ch = read();
		if(ch == '='){
			store(beqsy, 0, 0);
			return beqsy;
		}
		else{
			char_pl--;
			store(bgtsy, 0, 0);
			return bgtsy;
		}
	}
	else if(ch == '!'){
		ch = read();
		if(ch == '='){
			store(neqsy, 0, 0);
			return neqsy;
		}
		else{
			error('!');	//error msg
			return ERROR;
		}
	}
	else if(ch == '='){
		ch = read();
		if(ch == '='){
			store(eqsy, 0, 0);
			return eqsy;
		}
		else{
			char_pl--;
			store(assignsy, 0, 0);
			return assignsy;
		}
	}
	else if(ch == '+'){
		store(addsy, 0, 0);
		return addsy;
	}
	else if(ch == '-'){
		store(minussy, 0, 0);
		return minussy;
	}
	else if(ch == '*'){
		store(multsy, 0, 0);
		return multsy;
	}
	else if(ch == '/'){
		store(divsy, 0, 0);
		return divsy;
	}
	else if(ch == 39){	// "'"
		ch = read();
		if(isDigit(ch)){
			Alpha = i2c(ch);
		}
		else if(isLower(ch) || ch == '+' || ch == '-'){
			Alpha = ch; 
		}
		else if(isHigher(ch)){
			Alpha = lower(ch);
		}
		else
			error();	//wrong ch
		ch = read();
		if(ch == 39){
			store(alphasy, &Alpha, 0);
			return alphasy;
		}
		else{
			error();
			return ERROR;
		}

	}
	else if(ch == '"'){
		id_length = 0;
		while(1){
			ch = read();
			if(ch == '"'){
				id_addr[id_length] = '\0';
				if(id_length == 0){
					error();
				}
				else{
					store(strsy, id_addr, 0);
					return strsy;
				}
			}
			else if(ch <= 126 && ch >= 32 && ch != 34)
				id_addr[id_length++] = ch;
		}
	}
	else if(ch == '('){
		store(lparent, 0, 0);
		return lparent;
	}
	else if(ch == ')'){
		store(rparent, 0, 0);
		return rparent;
	}
	else if(ch == '['){
		store(lbrace, 0, 0);
		return lbrace;
	}
	else if(ch == ']'){
		store(rbrace, 0, 0);
		return rbrace;
	}
	else if(ch == '{'){
		store(lbrack, 0, 0);
		return lbrack;
	}
	else if(ch == '}'){
		store(rbrack, 0, 0);
		return rbrack;
	}
	else if(ch == ','){
		store(comma, 0, 0);
		return comma;
	}
	else if(ch == ';'){
		store(split, 0, 0);
		return split;
	}
	else if(ch == '\n' || ch == '\0' || ch == ' ' || ch == '\t' )
		return next_symbol();
	else{
		error();
		return ERROR;
	}
}

void word_analysis(){
	int sym;
	char_pl = 0;
	line = 0;
	line_len = 0;
	readfile();
	readline();
	while(1){
		sym = next_symbol();
		if (end == 1)
			break;
		if (sym == ident || sym == strsy) {
			fprintf(fpout,"%s\t%s\n",symkind[sym],id_addr);
			//printf("%s\t%s\n", symkind[sym], id_addr);
		}
		else if (sym == uintsy) {
			fprintf(fpout,"%s\t%d\n",symkind[sym],int_num);
			//printf("%s\t%d\n", symkind[sym], int_num);
		}
		else if (sym == alphasy) {
			fprintf(fpout,"%s\t%c\n",symkind[sym],Alpha);
			//printf("%s\t%c\n", symkind[sym], Alpha);
		}
		else if (sym != ERROR && sym != SPACE) {
			fprintf(fpout,"%s\n",symkind[sym]);
			//printf("%s\n", symkind[sym]);
		}
		else
			fprintf(fpout,"There is error!\n");
			//printf("THERE IS ERROR\n");
		//if(end == 1)
			//break;
	}
	fprintf(fpout,"Word Analysis Fininshed!\n");
	//printf("Word Analysis Finished!\n");
}



void store(int symbol_type, char* symbol_name, int symbol_num) {
	struct symbol_str* temp_symbol;
	struct symbol_str* last_symbol;
	temp_symbol = (struct symbol_str*)malloc(sizeof(struct symbol_str));
	temp_symbol->symbol_type = symbol_type;
	temp_symbol->line = line;	//line_num:global var
	if (symbol_type == uintsy || symbol_type == charsy) {
		temp_symbol->symbol_num = symbol_num;
	}
	else if (symbol_type == strsy || symbol_type == ident) {
		temp_symbol->symbol_name = symbol_name;
	}
	else {
		//There should be some?
	}

	if (s_list.first == NULL) {
		s_list.first = temp_symbol;
		s_list.bot = temp_symbol;
	}
	//Add symbol into symbol_list
	last_symbol = s_list.bot;
	last_symbol->next_symbol = temp_symbol;
	s_list.bot = temp_symbol;
	s_list.symbol_num++;
	temp_symbol->next_symbol = NULL;


}
