#pragma once

#define c2i(ch) (ch) - '0'
//#define i2c(i) "i"
#define lower(ch) (ch - 'A' +'a')
#define isDigit(ch) (ch >= '0' && ch <= '9')
#define isHigher(ch) (ch >= 'A' && ch <= 'Z')
#define isLower(ch) (ch >= 'a' && ch <= 'z')

/*int c2i(char ch){
	return (ch - '0');
}
char i2c(int i){
	return i;
}

char lower(char ch){
	return ch - 'A' + 'a';
}
int isDigit(char ch){
	return (ch >= '0' && ch <= '9');
}
int isHigher(char ch){
	return (ch >= 'A' && ch <= 'Z');
}
int isLower(char ch){
	return (ch >= 'a' && ch <= 'z');
}
*/

