#pragma once
//����֮ǰд�����˵Ķ�����������

typedef struct {
	list_t* first;
	int list_num;
}list;
//����Ľṹ������

typedef struct {
	list_t* prev;
	list_t* next;
}list_t;
//���Ҫ��list_t���������������λ��
//�������ܱ�֤����listָ�����ʱ������ָ����Ӧ�ṹ��

//������õ��������洢�ռ䣬����Ҫ����������������
//�෴�����������ڶ��ֲ��ң���������Ҳ������������

//�βνṹ��
typedef struct {
	int type;	//�β�����
	int isarray;	//�β��Ƿ����������ͣ�saved��ʵ�����ķ�������
	char* name;	//���βε�����
	arg* next_arg;	//��һ���βε�ַ
}arg;

//ȫ�ֽṹ��
//(1)��������
typedef struct {
	list_t point;	//����ָ�루saved��
	char* name;		//����������
	arg* args;	//������Ҫ�����б���
	int arg_num;	//�����ı�������
}FUNC;
//(2)��������
typedef struct {
	list_t point;
	int type;		//��������
	FUNC* func��		//���������ں�����ȫ�������ΪMAIN��
		char* name;		//��������
	int value;		//����ֵ
}CONST;
//(3)��������
//ʵ���Ͻṹ�����������һ��
typedef struct {
	list_t point;
	int type;		//��������
	FUNC* func��		//���������ں�����ȫ�������ΪMAIN��
		char* name;		//��������
	int value;		//����ֵ
}VARS;

//������غ���ȥ��/��/����ص��б���

void LIST_SEARCH(int type, void* store_str, int store_num) {

	if (type == VARS)
}


//�����ʱ����һ��Imp_const���,��װЧ����
int imp_const(int type, symbol_str* now_symbol, FUNC func) {
	CONST** temp_const;
	CONST* store_const;
	store_const = imp_const(intsy, now_symbol->symbol_name, now_symbol->symbol_num);
	i = LIST_SEARCH_CONSTS(func->cons, store_const->hash, func->cons_num, temp_const);
	if (i == FIND) {
		error(args_redefined);
		return ERROR;
	}
	LIST_INSERT_CONSTS(func->cons, store_const->hash, func->cons_num, i);
	return 1;
}