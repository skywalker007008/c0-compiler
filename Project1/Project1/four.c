/*四元式-mips设计文档*/

不需要进行标准四元式定义的部分：
	常量定义
	变量定义
	函数定义
	main定义

(1)
const char a = 'c';
----->
const a = 'c'

(2)
int a;
----->

(3)表达式类型：
//使用符号栈的方式进行处理
//实际上可以划分为项，因子之间的关系
//这里看做一个程序块，活跃变量分析时可以将其进行简化
a = b * c + (c + d) * e;
----------------------->
t0 = c + d;		--->add t0 c d
t1 = t0 * e;	--->mult t0 e
				--->mflo t1
t0 = b * c;		--->mult b c
				--->mflo t0
a = t0 + t1;	--->add a t0 t1

(4)数组类型：
//采用offset
a[9] = c[9];
------------>
t0 &= c;		--->t0是c的起始地址
t1 =* t0 + 9;	--->lw $t1 9($t0)	//可能还是分开像T2一样的好

t0 &= a;		--->t0是a的起始地址
t2 = t0 + 9;	--->addi $t2 $t0 9
t2 *= t1;		--->sw $t1 0(&t2)

(5)函数调用：
//函数调用时，需要将本函数内的变量值进行保存到对应栈空间

a = foo(b,c)
---------->
a0 = b;			--->add a0 b 0
a1 = c;			--->add a1 c 0
//省略一堆的保存现场指令
jumpto foo();	--->jal foo
//省略一堆的恢复现场的指令
a = v0;			--->add a0 v0 0

(6)函数定义：
//需要弄一个标签：
int foo(int a, int b){

}
--------------------->
print "foo : \n"
a = a0;
b = a1;
//下面是一堆表达式的指令

(7)while循环：
//比较恶心…………
//得先设立两个标签：func_loop_num_start, func_loop_num_end
//分别保存在label1和label2里面
while(a + b > 0){
	t = t + 1;
}
-------------->
label1:
t0 = a + b;
t1 = t0 > 0
jumpifn t1 label2;
t = t + 1 		--->表达式部分
jump label1;

label2:
(8)for循环：
//这个更恶心……
//TNND居然还要do一下……
//跟while类似，设立两个标签：func_loop_num_start, func_loop_num_end
//分别保存在label1, label2里面
for(i = 1 + 9; i < 10; i = i + 1){
	print(i);
}

--------->

i = 1 + 9;
label1:
print i;
i = i + 1;
t0 = i - 10;
t1 = t0 < 0
jumpif t1 label1
label2:






