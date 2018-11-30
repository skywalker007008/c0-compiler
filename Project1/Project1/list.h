#pragma once
#include <stdio.h>
#include "types.h"
#include "fourstate.h"

//���ͷ�ļ����������йط��ű�����������ṹ

//�б�Ľṹ������

#define IS_VAR 10
#define IS_CONST 20

#define INIT 1
#define NOT_INIT -1

#define INSERT 1
#define EXIST 0
#define FIND -1

//ȫ�ֽṹ��

//(1)��������
typedef struct {	
	char* name;	//����������
	int type;	//����ֵ������
	CODE4* first_code4;	//��һ��4Ԫʽ
	CODE4* last_code4;	//���һ��4Ԫʽ
	CONST* cons;//������Ҫ�����г���
	VAR* vars;	//������Ҫ�����б���
	int args_num;//������Ҫ�Ĳ�������
	int cons_num;	//�����ĳ�������
	int vars_num;	//�����ı�������
	
}FUNC;

//(2)��������
typedef struct {
	int type;		//��������
	char* name;		//��������
	double hash;	//�������ƶ�Ӧ��hashֵ
	int value;		//����ֵ
}CONST;

//(3)��������
//ʵ���Ͻṹ�����������һ��
typedef struct {
	int type;		//��������
	char* name;		//��������
	double hash;	//�������ƶ�Ӧ��hashֵ
	int value;		//����ֵ
	int init;		//�����Ƿ񱻸�ֵ
	int array_len;	//���鳤�ȣ�������������򳤶�Ĭ��Ϊ0
	ARRAY* array;	//�ñ�����������Ӧ�������
}VAR;

//(4)���鲿��
typedef struct {
	int value;		//�õ�ַ��Ӧ������ֵ���ַ�����Ϊ���ʹ洢��
	//Reserved
}ARRAY;

//(5)��������
typedef struct {
	int type;
	string name;
	int value;	//������ֵ���������
}ARGS;

int LIST_SEARCH_CONSTS(CONST* consts_list, double hash, int const_num, CONST** ret_const);
int LIST_INSERT_CONSTS(CONST* consts_list, CONST insert_const, int const_num, int insert_pl);
int LIST_SEARCH_VARS(VAR* vars_list, double hash, int var_num, VAR** ret_var);
int LIST_INSERT_VARS(VAR* vars_list, VAR insert_var, int var_num, int insert_pl);
CONST* imp_const(int type, string name, int value);
VAR* imp_var(int type, string name, int value, int array_len);
int imp_func(int type, string name, CONST* cons_list, VAR* vars_list, int cons_num, int vars_num);