#include "toAssembly.h"

using namespace std;

string toAssembly::getNewLable() {
	return "Lable" + to_string(lable_ptr++);
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
	ASSUME CS : CSEG, DS : DSEG, SS : SSEG\n";

string assembly_tail = "\
CSEG ENDS; 代码段结束\n\
END MAIN; 程序结束\n\
";

string printFun = "\
PrintSignedInt PROC\n\
PUSH AX\n\
PUSH BX\n\
PUSH CX\n\
PUSH DX\n\
\n\
MOV AX, BX\n\
CMP AX, 0\n\
JGE POSITIVE\n\
\n\
; 输出负号\n\
MOV DL, '-'\n\
MOV AH, 02h\n\
INT 21h\n\
\n\
NEG AX; AX = -AX\n\
\n\
POSITIVE :\n\
XOR CX, CX; 计数器清零\n\
MOV BX, 10; 除数\n\
\n\
STORE_DIGITS :\n\
XOR DX, DX\n\
DIV BX; AX ÷ 10, 商AX, 余数DX\n\
PUSH DX; 保存余数\n\
INC CX\n\
CMP AX, 0\n\
JNZ STORE_DIGITS\n\
\n\
PRINT_LOOP :\n\
POP DX\n\
ADD DL, '0'\n\
MOV AH, 02h\n\
INT 21h\n\
LOOP PRINT_LOOP\n\
\n\
POP DX\n\
POP CX\n\
POP BX\n\
POP AX\n\
RET\n\
PrintSignedInt ENDP";

void toAssembly::generateAssembly() {

	assembly_code.push_back(assembly_header);
	while (cur_ptr <= end) {
		generateOneBlock();
	}

	assembly_code.push_back(printFun);
	assembly_code.push_back(assembly_tail);
}


//void toAssembly::generateAssembly() {
//	while (cur_ptr <= end) {
//		generateOneBlock();
//	}
//}
int point = 0;

void toAssembly::generateOneBlock() {
	quat& q = quats[cur_ptr];
	if (q[0] == "+" || q[0] == "-" || q[0] == "*" || q[0] == "/" || q[0] == "=") {
		generateArithmetic();
	}
	else if (q[0] == ">" || q[0] == "<" || q[0] == ">=" || q[0] == "<=" || q[0] == "==" || q[0] == "!=" || q[0] == "&&" || q[0] == "||" || q[0] == "!") {
		generateLogic();
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

	//for (; point < assembly_code.size(); point++) {
	//	cout << assembly_code[point] << endl;
	//}
}
/*
*遵循无后效性原则，每一句源代码语句之后（最好每个四元式语句都释放），释放所有寄存器
*/
void toAssembly::generateArithmetic() {
	quat& q = quats[cur_ptr];
	string addr_q[4];
	string name;
	int number = 0;

	addr_q[0] = q[0];
	for (int i = 1; i <= 3; i++) {
		if (q[0] == "=" && i == 2) {
			continue;
		}

		name = q[i];
		if (isdigit(name[0])) {		//如果碰到数字字面量，直接赋值
			addr_q[i] = q[i];
		}
		else if (name[0] == 't') {
			name.erase(name.begin());
			number = stoi(name);
			number *= 2;
			addr_q[i] = format("WORD PTR [T+{}]", number);
			/*assembly_code.push_back(format("CMP [T+{}],0", number));*/
		}
		else {
			number = table.getoff(name);
			addr_q[i] = format("[BP-{}]", number + 2);
			/*assembly_code.push_back(format("CMP [BP-{}],0", number + 2));*/
		}
	}
	

	if (addr_q[0] == "+") {
		// 生成加法指令
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("ADD AX, {}", addr_q[2]));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3])); // 将结果存储到目标变量
	}
	else if (addr_q[0] == "-") {
		// 生成减法指令
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("SUB AX, {}", addr_q[2]));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3])); // 将结果存储到目标变量
	}
	else if (addr_q[0] == "*") {
		// 生成乘法指令
		// 
		// 4字节版本
		//assembly_code.push_back(format("MOV AX, {} ", addr_q[1]));	//把第一个操作数加载到AX寄存器
		//assembly_code.push_back(format("IMUL {} ", addr_q[2]));		// 乘以第二个操作数

		//assembly_code.push_back(format("LEA BX, {} ", addr_q[3]));	//获取乘积的地址
		//assembly_code.push_back(format("MOV WORD PTR [BX], AX ")); //把AX给低两位
		//assembly_code.push_back(format("ADD BX, 2 ", addr_q[3]));	//地址+2
		//assembly_code.push_back(format("MOV WORD PTR [BX], DX ")); //把DX给高两位


		//2字节版本
		assembly_code.push_back(format("MOV AL, {} ", addr_q[1]));	//把第一个操作数加载到AX寄存器

		string add;
		for (int i = 0; i < addr_q[2].size(); i++) {
			if (addr_q[2][i] == '[') {
				while (addr_q[2][i] != ']') {
					add += addr_q[2][i];
					i++;
				}
				add += addr_q[2][i];
			}
		}
		assembly_code.push_back(format("IMUL BYTE PTR {} ", add/*addr_q[2]*/));		// 乘以第二个操作数

		assembly_code.push_back(format("MOV {}, AX ", addr_q[3])); //把AX给低两位
	}
	else if (addr_q[0] == "/") {
		// 生成除法指令
		//把被除数的高两位低两位分别给AX和DX寄存器
		// 
		// 4字节版本
		//assembly_code.push_back(format("LEA BX, {} ", addr_q[1]));
		//assembly_code.push_back(format("MOV AX, WORD PTR [BX] "));
		//assembly_code.push_back(format("ADD BX, 2 ", addr_q[1]));
		//assembly_code.push_back(format("MOV DX, WORD PTR [BX] "));

		//assembly_code.push_back(format("IDIV {} ", addr_q[2]));
		//assembly_code.push_back(format("MOV {}, AX", addr_q[3]));

		//2字节版本
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); //把AX给低两位

		string add;
		for (int i = 0; i < addr_q[2].size(); i++) {
			if (addr_q[2][i] == '[') {
				while (addr_q[2][i] != ']') {
					add += addr_q[2][i];
					i++;
				}
				add += addr_q[2][i];
			}
		}

		assembly_code.push_back(format("IDIV BYTE PTR {}",  add /*addr_q[2]*/));	//把第一个操作数加载到AX寄存器
		assembly_code.push_back(format("XOR AH, 0", addr_q[3]));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));		// 乘以第二个操作数

		
	}
	else if (addr_q[0] == "=") {
		// 生成赋值指令
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("MOV {}, AX", addr_q[3])); // 将结果存储到目标变量
	}

	cur_ptr++;
}
void toAssembly::generateLogic() {
	quat& q = quats[cur_ptr];
	string addr_q[4];
	string name;
	int number = 0;

	addr_q[0] = q[0];
	for (int i = 1; i <= 3; i++) {
		if (q[0] == "=" && i == 2) {
			continue;
		}

		name = q[i];
		if (isdigit(name[0])) {		//如果碰到数字字面量，直接赋值
			addr_q[i] = q[i];
		}
		else if (name[0] == 't') {
			name.erase(name.begin());
			number = stoi(name);
			number *= 2;
			addr_q[i] = format("WORD PTR [T+{}]", number);
			/*assembly_code.push_back(format("CMP [T+{}],0", number));*/
		}
		else {
			number = table.getoff(name);
			addr_q[i] = format("[BP-{}]", number + 2);
			/*assembly_code.push_back(format("CMP [BP-{}],0", number + 2));*/
		}
	}

	if (addr_q[0] == ">") {
		// 生成大于比较指令
		string lable_right = getNewLable();
		string lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", addr_q[2]));
		assembly_code.push_back(format("JG {}", lable_right)); // 跳转到标签addr_q[3]如果AX > addr_q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));

	}
	else if (addr_q[0] == ">=") {
		// 生成大于等于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", addr_q[2]));
		assembly_code.push_back(format("JGE {}", lable_right)); // 跳转到标签addr_q[3]如果AX > addr_q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));

	}
	else if (addr_q[0] == "<") {
		// 生成大于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", addr_q[2]));
		assembly_code.push_back(format("JL {}", lable_right)); // 跳转到标签addr_q[3]如果AX > addr_q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));
	}
	else if (addr_q[0] == ">=") {
		// 生成大于等于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", addr_q[2]));
		assembly_code.push_back(format("JLE {}", lable_right)); // 跳转到标签addr_q[3]如果AX > addr_q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));

	}
	else if (addr_q[0] == "==") {
		// 生成大于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", addr_q[2]));
		assembly_code.push_back(format("JE {}", lable_right)); // 跳转到标签addr_q[3]如果AX > addr_q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));
	}
	else if (addr_q[0] == "!=") {
		// 生成大于比较指令
		string lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, {}", addr_q[2]));
		assembly_code.push_back(format("JNE {}", lable_right)); // 跳转到标签addr_q[3]如果AX > addr_q[2]

		//错误阶段
		assembly_code.push_back(format("MOV AX, 0"));
		assembly_code.push_back(format("JMP {}", lable_end));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));

		//结束阶段
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));
	}
	else if (addr_q[0] == "&&") {
		// 生成与逻辑指令
		string lable_one_right = getNewLable(), lable_two_right = getNewLable(),
			lable_wrong = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV AX, {}", addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, 0"));
		assembly_code.push_back(format("JNE {}", lable_one_right)); // 如果AX == 0，跳转到结束标签
		assembly_code.push_back(format("JMP {}", lable_wrong));

		//第一个判断正确
		assembly_code.push_back(format("{}:", lable_one_right));
		assembly_code.push_back(format("MOV AX, {}",addr_q[2])); // 继续检查第二个操作数
		assembly_code.push_back(format("CMP AX, 0"));
		assembly_code.push_back(format("JNE {}", lable_two_right)); // 如果AX == 0，跳转到结束标签
		assembly_code.push_back(format("JMP {}", lable_wrong));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_two_right));
		assembly_code.push_back(format("MOV AX, 1"));
		assembly_code.push_back(format("JMP {}", lable_end)); // 将结果存储到目标变量

		//错误阶段
		assembly_code.push_back(format("{}:", lable_wrong));
		assembly_code.push_back(format("MOV AX, 0"));

		//结束阶段	
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));
	}
	else if (addr_q[0] == "||") {
		// 生成与逻辑指令
		string lable_second = getNewLable(),
			lable_wrong = getNewLable(), lable_right = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV AX, {}",addr_q[1])); // 假设使用AX寄存器
		assembly_code.push_back(format("CMP AX, 0"));
		assembly_code.push_back(format("JNE {}", lable_right)); // 如果AX == 0，跳转到结束标签
		assembly_code.push_back(format("JMP {}", lable_second));

		//第一个判断正确
		assembly_code.push_back(format("{}:", lable_second));
		assembly_code.push_back(format("MOV AX, {}",addr_q[2])); // 继续检查第二个操作数
		assembly_code.push_back(format("CMP AX, 0"));
		assembly_code.push_back(format("JNE {}", lable_right)); // 如果AX == 0，跳转到结束标签
		assembly_code.push_back(format("JMP {}", lable_wrong));

		//正确阶段
		assembly_code.push_back(format("{}:", lable_right));
		assembly_code.push_back(format("MOV AX, 1"));
		assembly_code.push_back(format("JMP {}", lable_end)); // 将结果存储到目标变量

		//错误阶段
		assembly_code.push_back(format("{}:", lable_wrong));
		assembly_code.push_back(format("MOV AX, 0"));

		//结束阶段	
		assembly_code.push_back(format("{}:", lable_end));
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));
	}
	else if (addr_q[0] == "!") {
		string lable_1 = getNewLable(), lable_0 = getNewLable(), lable_end = getNewLable();

		//判断阶段
		assembly_code.push_back(format("MOV AX, {}",addr_q[1])); // 假设使用AX寄存器
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
		assembly_code.push_back(format("MOV {}, AX", addr_q[3]));
	}
	cur_ptr++;
}

void toAssembly::generateTurn() {

}
void toAssembly::generateIf() {
	quat& q = quats[cur_ptr];
	string name = quats[cur_ptr][1];
	int number = 0;

	if (isdigit(name[0])) {		//如果碰到数字字面量，直接赋值
		assembly_code.push_back(format("MOV BX, {}", name));
		assembly_code.push_back(format("CMP BX, 0"));
	}
	else if (name[0] == 't') {
		name.erase(name.begin());
		number = stoi(name);
		number *= 2;
		assembly_code.push_back(format("CMP WORD PTR [T+{}],0", number));
	}
	else {
		number = table.getoff(name);
		assembly_code.push_back(format("CMP [BP-{}],0", number + 2));
	}

	string lable1 = getNewLable();
	string lable_far = getNewLable();
	string lable_ifright = getNewLable();

	//判断成功的话
	assembly_code.push_back(format("JNE {}", lable_ifright));	//真值为1

	//判断失败的话
	/*assembly_code.push_back(format("{} :", lable_far));*/
	assembly_code.push_back(format("JMP NEAR PTR  {}", lable1));	//else 的入口


	assembly_code.push_back(format("{}:", lable_ifright));
	cur_ptr++;
	while (quats[cur_ptr][0] != "el" && quats[cur_ptr][0] != "ie") {
		generateOneBlock();
	}
	string lable2 = getNewLable();
	if (quats[cur_ptr][0] == "el") {
		assembly_code.push_back(format("JMP NEAR PTR {}", lable2));	//跳到if结束
		assembly_code.push_back(format("{}:", lable1));	//进入else
		cur_ptr++;

		while (quats[cur_ptr][0] != "ie") {
			generateOneBlock();
		}

		//结束
		assembly_code.push_back(format("{}:", lable2));
		cur_ptr++;
	}
	else {
		assembly_code.push_back(format("{}:", lable1));	//进入else， 但是此处else为空
		assembly_code.push_back(format("{}:", lable2));
		cur_ptr++;
	}
}
void toAssembly::generateWhile() {
	int number = 0;

	string lable1 = getNewLable();
	assembly_code.push_back(format("{}:", lable1));
	cur_ptr++;

	while (quats[cur_ptr][0] != "do") {
		generateOneBlock();
	}

	quat& q = quats[cur_ptr];
	string name = quats[cur_ptr][1];
	name = quats[cur_ptr][1];

	if (isdigit(name[0])) {		//如果碰到数字字面量，直接赋值
		assembly_code.push_back(format("MOV BX, {}", name));
		assembly_code.push_back(format("CMP BX, 0"));
	}
	else if (name[0] == 't') {
		name.erase(name.begin());
		number = stoi(name);
		number *= 2;
		assembly_code.push_back(format("CMP WORD PTR [T+{}],0", number));
	}
	else {
		number = table.getoff(name);
		assembly_code.push_back(format("CMP [BP-{}],0", number + 2));
	}

	string lable2 = getNewLable();
	string lable_far = getNewLable();

	assembly_code.push_back(format("JE {}", lable_far));
	assembly_code.push_back(format("{}:", lable_far));
	assembly_code.push_back(format("JMP NEAR PTR  {}", lable2));

	cur_ptr++;

	while (quats[cur_ptr][0] != "we") {
		generateOneBlock();
	}

	assembly_code.push_back(format("JMP NEAR PTR  {}", lable1));
	assembly_code.push_back(format("{}:", lable2));
	cur_ptr++;
}
void toAssembly::generateReturn() {
	quat& q = quats[cur_ptr];
	string name = q[3];
	int number = 0;

	if (isdigit(name[0])) {		//如果碰到数字字面量，直接赋值
		assembly_code.push_back(format("MOV BX, {}", name));
	}
	else if (name[0] == 't') {
		name.erase(name.begin());
		number = stoi(name);
		number *= 2;
		assembly_code.push_back(format("MOV BX, WORD PTR [T+{}]", number));
	}
	else {
		number = table.getoff(name);
		assembly_code.push_back(format("MOV BX,[BP-{}]", number + 2));
	}

	name = cur_func + "RETURN";
	assembly_code.push_back(format("JMP {}", name));

	cur_ptr++;
}
void toAssembly::generateDefFunction() {
	cur_func = quats[cur_ptr][3];
	string returnType = quats[cur_ptr][2]; // 函数返回类型
	assembly_code.push_back(format("{} PROC FAR", quats[cur_ptr][3]));

	if (cur_func == "MAIN") {
		assembly_code.push_back("MOV AX, DSEG\nMOV DS, AX");
	}

	assembly_code.push_back("PUSH BP");
	assembly_code.push_back("MOV BP,SP");
	cur_ptr++;

	while (quats[cur_ptr][0] == "para") {
		int len = 0;
		assembly_code.push_back(format("SUB SP, {}", 2));
		cur_ptr++;
	}
	if (quats[cur_ptr][0] != "pe") {
		cout << "四元式没有pe!" << endl;
		return;
	}
	cur_ptr++;

	while (quats[cur_ptr][0] != "de") {
		generateOneBlock();
	}

	assembly_code.push_back(cur_func + "RETURN:");
	assembly_code.push_back("MOV SP, BP");
	assembly_code.push_back("POP BP");

	if(cur_func != "MAIN") 
		assembly_code.push_back("RETF"); // 如果是MAIN函数，使用RETF
	else

	if (cur_func == "MAIN") {
		assembly_code.push_back("CALL PrintSignedInt");
		assembly_code.push_back("MOV AX, 4c00h\nINT 21h; 调用 DOS 中断，退出程序\n");
	}
	assembly_code.push_back(format("{} ENDP", cur_func));
	cur_ptr++;
}
void toAssembly::generateDefIdt() {
	//while (quats[cur_ptr][0] == "df" && quats[cur_ptr][1] == "idt") {
		assembly_code.push_back(format("SUB SP, {}", 2));
		cur_ptr++;
	//}
}
void toAssembly::generateUseFunction() {
	string name = quats[cur_ptr][1];
	string value = quats[cur_ptr][3];
	cur_ptr++;

	int i = 0;
	while (quats[cur_ptr][0] != "fe") {

		if (quats[cur_ptr][0] == "para") {

			if (isdigit(quats[cur_ptr][3][0])) {		//如果碰到数字字面量，直接赋值
				assembly_code.push_back(format("MOV BX, SP"));
				assembly_code.push_back(format("MOV SS:WORD PTR [BX-{}], {}", 4 + 4 + i, quats[cur_ptr][3]));
			}
			else if (quats[cur_ptr][3][0] == 't') {
				quats[cur_ptr][3].erase(quats[cur_ptr][3].begin());
				int number = stoi(quats[cur_ptr][3]);
				number *= 2;
				assembly_code.push_back(format("MOV AX,[T+{}]", number));
				assembly_code.push_back(format("MOV BX, SP"));
				assembly_code.push_back(format("MOV SS:WORD PTR [BX-{}], AX", 4 + 4 + i));
			}
			else {
				assembly_code.push_back(format("MOV AX,[BP-{}]", table.getoff(quats[cur_ptr][3]) + 2));
				assembly_code.push_back(format("MOV BX, SP"));
				assembly_code.push_back(format("MOV SS:WORD PTR [BX-{}], AX", 4 + 4 + i));
			}
			i += 2;
			cur_ptr++;
		}
		else {
			generateOneBlock();
		}
	}

	if (quats[cur_ptr][0] != "fe") {
		cout << "没有fe!" << endl;
		return;
	}
	assembly_code.push_back(format("CALL FAR PTR {}", name));

	int number = 0;
	value.erase(value.begin());
	number = stoi(value);
	number *= 2;
	assembly_code.push_back(format("MOV WORD PTR [T+{}], BX", number));

	cur_ptr++;
}