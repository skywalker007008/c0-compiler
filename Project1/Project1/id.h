#include "types.h"

#define ID_MAX_LENGTH 20

typedef int type;

struct id_list{
	struct id_str* head;
	struct id_str* bot;
	int idnum;
};

struct id_str{
	type id_type;
	char* id_name;
	int id_num;
	struct var* next_id;
};



/*void store(int line_num, int id_type, char* id_addr, int int_num){
	
}*/

struct store_id{
	type id_type;		//存储的标识符的类型
	char* id_name;		//记录标识符名字的首地址
	int id_value;		//记录标识符的值，int与char存储的字节暂认为相同。
	int level;
}

struct AR_stack{
	struct AR* head;		//运行栈的顶部指针
	struct AR* bottom;	//运行栈的栈底指针
}

struct abp_list{
	struct abp* abp_first;
}
struct abp{
	struct AR* ar;
	struct abp* abp_next;
}
struct AR{
	int num;		//数据栈的编号
	int ret_addr;	//上一层数据栈的返回地址
	struct AR* prevabp;	//上一层数据栈的指针 
	struct abp_list abps;	//数据栈指针列表
}
