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

//LIST�йغ���

//(1)�ڱ�����������
//������б����ҵ��˸ñ�������ô�ͷ��ؽ��ñ�����VAR�洢��*ret_var�У�������FIND
//���û�ҵ�������һ������(temp_pl)��ʾ���ܵĲ���λ��
/*int LIST_SEARCH_VARS(VAR* vars_list, double hash, int var_num, VAR** ret_var) {
	int find_pl;
	int min_pl = 1;
	int max_pl = var_num;
	int mid_pl = (min_pl + max_pl) / 2;
	VAR* temp_var;
	while (mid_pl != min_pl) {
		temp_var = (vars_list + mid_pl);
		if (hash < temp_var->hash) {
			max_pl = mid_pl;
		}
		else if (hash > temp_var->hash) {
			min_pl = mid_pl;
		}
		else if (hash == temp_var->hash) {
			*ret_var = temp_var;
			return FIND;
		}
		mid_pl = (min_pl + max_pl) / 2;
	}
	*ret_var = NULL;
	return max_pl;
}
*/

//(1)�ڱ������в���
//˼·�ܼ򵥣����Ե���������־�����
int LIST_SEARCH_VARS(VAR* vars_list, char* name, int var_num, VAR** ret_var){
	VAR* temp_var;
	int i;
	for(i = 0;i < var_num;i++){
		temp_var = vars_list + i;
		if(strcmp(temp_var->name, name) == 0){
			*ret_var = temp_var;
			return FIND;
		}
	}
	return NOT_FIND;
}
//(2)���뵽��������
//�������ɹ��򷵻�INSERT�����򷵻�EXIST��ʾ�ñ����Ѿ�����
int LIST_INSERT_VARS(VAR* vars_list, VAR insert_var, int var_num, int insert_pl) {
	int i;
	VAR* temp_var = NULL;
	for (i = var_num + 1; i > insert_pl; i--) {
		*(vars_list + i) = *(vars_list + i - 1);
	}
	*(vars_list + insert_pl) = insert_var;
	return INSERT;
}
//���ڲ�����ɾ������������ⲿ��û��Ҫд��
//�����Ĳ���ʮ������
//(1)�ڳ�����������
/*int LIST_SEARCH_CONSTS(CONST* consts_list, double hash, int const_num, CONST** ret_const) {
	int find_pl;
	int min_pl = 1;
	int max_pl = const_num;
	int mid_pl = (min_pl + max_pl) / 2;
	CONST* temp_const;
	while (mid_pl != min_pl) {
		temp_const = (consts_list + mid_pl);
		if (hash < temp_const->hash) {
			max_pl = mid_pl;
		}
		else if (hash > temp_const->hash) {
			min_pl = mid_pl;
		}
		else if (hash == temp_const->hash) {
			*ret_const = temp_const;
			return FIND;
		}
		mid_pl = (min_pl + max_pl) / 2;
	}
	*ret_const = NULL;
	return max_pl;
}*/

//(1)�ڳ���������
int LIST_SEARCH_CONSTS(CONST* consts_list, char* name, int const_num, CONST** ret_const){
	CONST* temp_const;
	int i;
	for(i = 0;i < const_num;i++){
		temp_const = consts_list + i;
		if(strcmp(temp_const->name, name) == 0){
			*ret_const = temp_const;
			return FIND;
		}
	}
	return NOT_FIND;
}

//(2)���볣������
//�������ɹ��򷵻�INSERT�����򷵻�EXIST��ʾ�ó����Ѿ�����
int LIST_INSERT_CONSTS(CONST* consts_list, CONST insert_const, int const_num, int insert_pl) {
	int i;
	CONST* temp_const = NULL;
	for (i = const_num + 1; i > insert_pl; i--) {
		*(consts_list + i) = *(consts_list + i - 1);
	}
	*(consts_list + insert_pl) = insert_const;
	return INSERT;
}
