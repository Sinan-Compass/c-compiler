#include "toAssembly.h"

using namespace std;

string toAssembly::getNewLable() {
	return "Lable" + to_string(lable_ptr);
	lable_ptr++;
}

/*下面的分函数，约定：
* 1.进入函数时，第一句四元式已经读来，即cur_ptr指向要处理的第一句
* 2.离开函数时，cur_ptr指向下一句四元式，即cur_ptr指向要处理的下一句
*/

string assembly_header = "\
SSEG SEGMENT stack\n\
	DB 1000 DUP(0); 定义栈段的大小\n\
SSEG ENDS\n\
DSEG SEGMENT;\n\
	T DB 1000 DUP(0)\n\
DSEG ENDS;\n\
CSEG SEGMENT; 定义代码段\n\
	ASSUME CS : CSEG, DS : DSEG, SS : SSEG\n\
MAIN : ";

string assembly_tail = "\
    MOV AX, 4c00h\n\
INT 21h; 调用 DOS 中断，退出程序\n\
CSEG ENDS; 代码段结束\n\
END MAIN; 程序结束\n\
";

void toAssembly::generateAssembly() {
	assembly_code.push_back(assembly_header);
	while (cur_ptr <= end) {
		generateOneBlock();
	}
	assembly_code.push_back(assembly_tail);
}

void toAssembly::generateOneBlock() {
	quat& q = quats[cur_ptr];
	if (q[0] == "+" || q[0] == "-" || q[0] == "*" || q[0] == "/" || q[0] == "=") {
		generateArithmetic();
	}
	else if (q[0] == ">" || q[0] == "<" || q[0] == ">=" || q[0] == "<=" || q[0] == "==" || q[0] == "!=" || q[0] == "&&" || q[0] == "||" || q[0] == "!") {
		generateTurn();
	}
	else if (q[0] == "if") {
		generateIf();
	}
	else if (q[0] == "wh") {
		generateWhile();
	}
	else if (q[0] == "rt") {
		generateReturn();
	}
	else if (q[0] == "df" && q[1] == "fun") {
		generateDefFunction();
	}
	else if (q[0] == "df" && q[1] == "idt") {
		generateDefIdt();
	}
	else if (q[0] == "fun") {
		generateUseFunction();
	}
}
/*
*遵循无后效性原则，每一句源代码语句之后（最好每个四元式语句都释放），释放所有寄存器
*/
void toAssembly::generateArithmetic() {
	quat& q = quats[cur_ptr];

	if (q[0] == "+") {
		// 生成加法指令
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("ADD AX, {}", q[2]));
		assembly_code.push_back(format("MOV {}, AX", q[3])); // 将结果存储到目标变量
	}
	else if (q[0] == "-") {
		// 生成减法指令
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("SUB AX, {}", q[2]));
		assembly_code.push_back(format("MOV {}, AX", q[3])); // 将结果存储到目标变量
	}
	else if (q[0] == "*") {
		// 生成乘法指令
		assembly_code.push_back(format("MOV AX, {} ", q[1]));	//把第一个操作数加载到AX寄存器
		assembly_code.push_back(format("IMUL {} ", q[2]));		// 乘以第二个操作数

		assembly_code.push_back(format("LEA BX, {} ", q[3]));	//获取乘积的地址
		assembly_code.push_back(format("MOV DW PTR [BX], AX ")); //把AX给低两位
		assembly_code.push_back(format("ADD BX, 2 ", q[3]));	//地址+2
		assembly_code.push_back(format("MOV DW PTR [BX], DX ")); //把DX给高两位
	}
	else if (q[0] == "/") {
		// 生成除法指令
		//把被除数的高两位低两位分别给AX和DX寄存器
		assembly_code.push_back(format("LEA BX, {} ", q[1]));
		assembly_code.push_back(format("MOV AX, DW PTR [BX] "));
		assembly_code.push_back(format("ADD BX, 2 ", q[1]));
		assembly_code.push_back(format("MOV DX, DW PTR [BX] ")); 

		assembly_code.push_back(format("IDIV {} ", q[2]));
		assembly_code.push_back(format("MOV {}, AX", q[3]));
	}
	else if (q[0] == "=") {
		// 生成赋值指令
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("MOV {}, AX", q[3])); // 将结果存储到目标变量
	}
}
void toAssembly::generateLogic() {
	quat & q = quats[cur_ptr];

	if (q[0] == ">") {
		// 生成大于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", q[2]));
		assembly_code.push_back(format("JG {}", lable_right)); // 跳转到标签q[3]如果AX > q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", q[3]));

	}
	else if (q[0] == ">=") {
		// 生成大于等于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", q[2]));
		assembly_code.push_back(format("JGE {}", lable_right)); // 跳转到标签q[3]如果AX > q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", q[3]));
	
	}
	else if (q[0] == "<") {
		// 生成大于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", q[2]));
		assembly_code.push_back(format("JL {}", lable_right)); // 跳转到标签q[3]如果AX > q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", q[3]));
	}
	else if (q[0] == ">=") {
		// 生成大于等于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", q[2]));
		assembly_code.push_back(format("JLE {}", lable_right)); // 跳转到标签q[3]如果AX > q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", q[3]));

	}
	else if (q[0] == "==") {
		// 生成大于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", q[2]));
		assembly_code.push_back(format("JE {}", lable_right)); // 跳转到标签q[3]如果AX > q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", q[3]));
	}
	else if (q[0] == "!=") {
		// 生成大于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", q[2]));
		assembly_code.push_back(format("JNE {}", lable_right)); // 跳转到标签q[3]如果AX > q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", q[3]));
	}
	else if (q[0] == "&&") {
		// 生成与逻辑指令
		string lable_one_right = getNewLable(), lable_two_right = getNewLable(),
			lable_wrong = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, 1"));
		assembly_code.push_back(format("JE {}", lable_one_right)); // 如果AX == 0，跳转到结束标签
		assembly_code.push_back(format("JMP {}", lable_wrong));
		
		//第一个判断正确
		assembly_code.push_back(format("{}:", lable_one_right));
		assembly_code.push_back(format("MOV {}, {}", "AX", q[2])); // 继续检查第二个操作数
		assembly_code.push_back(format("CMP AX, 1"));
		assembly_code.push_back(format("JE {}", lable_two_right)); // 如果AX == 0，跳转到结束标签
		assembly_code.push_back(format("JMP {}", lable_wrong));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_two_right));
		assembly_code.push_back(format("MOV AX, 1"));
		assembly_code.push_back(format("JMP", lable_end)); // 将结果存储到目标变量

		//错误阶段
		assembly_code.push_back(format("{}:", lable_wrong));
		assembly_code.push_back(format("MOV AX, 0"));

		//结束阶段	
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", q[3]));
	}
	else if (q[0] == "||") {
		// 生成与逻辑指令
		string lable_second = getNewLable(),
			lable_wrong = getNewLable(), lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, 1"));
		assembly_code.push_back(format("JE {}", lable_right)); // 如果AX == 0，跳转到结束标签
		assembly_code.push_back(format("JMP {}", lable_second));

		//第一个判断正确
		assembly_code.push_back(format("{}:", lable_second));
		assembly_code.push_back(format("MOV {}, {}", "AX", q[2])); // 继续检查第二个操作数
		assembly_code.push_back(format("CMP AX, 1"));
		assembly_code.push_back(format("JE {}", lable_right)); // 如果AX == 0，跳转到结束标签
		assembly_code.push_back(format("JMP {}", lable_wrong));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));
		assembly_code.push_back(format("JMP", lable_end)); // 将结果存储到目标变量

		//错误阶段
		assembly_code.push_back(format("{}:", lable_wrong));
		assembly_code.push_back(format("MOV AX, 0"));

		//结束阶段	
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", q[3]));
	}
	else if (q[0] == "!") {
		string lable_1 = getNewLable(), lable_0 = getNewLable(), lable_end = getNewLable();
		
		//判断阶段
		assembly_code.push_back(format("MOV {}, {}", "AX", q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, 0"));
		assembly_code.push_back(format("JNE {}", lable_1)); // 如果AX == 0，跳转到结束标签

		//如果真值为0
		assembly_code.push_back(format("MOV AX, 1"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//如果真值为1
		assembly_code.push_back(format("{}:", lable_1));
		assembly_code.push_back(format("MOV AX, 0")); 

		//结束阶段	
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", q[3]));
	}
}
void toAssembly::generateTurn() {

}
void toAssembly::generateIf() {

}
void toAssembly::generateWhile() {

}
void toAssembly::generateReturn() {

}
void toAssembly::generateDefFunction() {
	quat& q = quats[cur_ptr];
	string returnType =	q[2]; // 函数返回类型
	assembly_code.push_back(format("{} PROC FAR", q[3]));
	while (1) {

	}

}
void toAssembly::generateDefIdt() {

}
void toAssembly::generateUseFunction() {

}