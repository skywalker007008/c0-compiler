#pragma once
#include<stdio.h>
#include "define.h"
#define MAX_KEYWORD_LENGTH 8
#define ERROR -1
#define SPACE -2
//各个非终结符类型
typedef int type;

#define ident 1
#define addsy 2
#define minussy 3
#define multsy 4
#define divsy 5

#define leqsy 11
#define lstsy 12
#define eqsy 13
#define neqsy 14
#define beqsy 15
#define bgtsy 16 
#define assignsy 17

#define intsy 6
#define uintsy 7
#define charsy 8
#define consy 9
#define strsy 10

#define alphasy 18

#define lparent 19
#define rparent 20
#define lbrace 21
#define rbrace 22
#define lbrack 23
#define rbrack 24

#define comma 25
#define split 26

#define forsy 27
#define ifsy 28
#define elsesy 29
#define whilesy 30

#define scansy 31
#define printsy 32
#define returnsy 33
#define voidsy 34
#define mainsy 35

#define MAX_SYM_NUM 35



