#pragma once
#include <stdio.h>

//有关语法检测部分

typedef enum {
	args_redefined = 1, 
	args_undefined,
	const_defined,
	var_no_init,
	const_assigned,
	char_assigned,
	args_less,
	args_more,
	types_mismatch,
	return_miss,
	return_no_args,
	func_undefined,
	func_redefined,
	return_type_mismatch,
	if_step_mismatch
}error_msg;


