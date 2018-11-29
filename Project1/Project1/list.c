#include <stdio.h>
#include "list.h"

//list.c�а����������������ɷ��ű�ĺ���

FUNC func_list;
int func_num;
//LIST�йغ���

//(1)�ڱ�����������
//������б����ҵ��˸ñ�������ô�ͷ��ؽ��ñ�����VAR�洢��*ret_var�У�������FIND
//���û�ҵ�������һ������(temp_pl)��ʾ���ܵĲ���λ��
int LIST_SEARCH_VARS(VAR* vars_list, double hash, int var_num, VAR** ret_var) {
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
int LIST_SEARCH_CONSTS(CONST* consts_list, double hash, int const_num, CONST** ret_const) {
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
}

//(2)���뵽��������
//�������ɹ��򷵻�INSERT�����򷵻�EXIST��ʾ�ñ����Ѿ�����
int LIST_INSERT_CONSTS(CONST* consts_list, CONST insert_const, int const_num, int insert_pl) {
	int i;
	CONST* temp_const = NULL;
	for (i = const_num + 1; i > insert_pl; i--) {
		*(consts_list + i) = *(consts_list + i - 1);
	}
	*(consts_list + insert_pl) = insert_const;
	return INSERT;
}

//�йض�VAR,CONST,FUNC�ĸ���
//
//����һ��const����Ϣ
CONST* imp_const(int type, string name, int value){
	CONST* temp_const;
	temp_const = (CONST*)malloc(sizeof(CONST));
	temp_const->type = type;
	temp_const->name = name;
	temp_const->value = value;
	temp_const->hash = hash(name);
	return temp_const;
}
//����һ��var����Ϣ
VAR* imp_var(int type, string name, int value, int array_len){
	VAR* temp_var;
	temp_var = (VAR*)malloc(sizeof(VAR));
	temp_var->type = type;
	temp_var->name = name;
	temp_var->value = value;
	temp_var->hash = hash(name);
	temp_var->init = NO_INIT;
	temp_var->array_len = array_len;
	if(array_len == 0)
		array = NULL;
	else
		array = (ARRAY*)malloc(array_len * sizeof(int));	//size: ARRAT = int

	return temp_var;
}
//������Ϣ�ĵ�½
int imp_func(int type, string name, CONST* cons_list, VAR* vars_list, int cons_num, int vars_num){
	//����һ����û�б�������������
	int i = 0;
	FUNC* temp_func;
	for(i = 0;i < func_num;i++){
		if(strcmp(name, func_list[i].name) == 1)
			return REDEFINE;
	}
	temp_func = (FUNC*)malloc(sizeof(FUNC));
	temp_func -> name = name;
	temp_func -> cons = cons_list;
	temp_func -> vars = vars_list;
	temp_func -> cons_num = cons_num;
	temp_func -> vars_num = vars_num;
	temp_func -> type = type;
	func_list[func_num] = temp_func;
	return DONE;
}



