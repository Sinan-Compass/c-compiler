#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <graphics.h>
#include <conio.h>
#include <windows.h>

using namespace std;

enum Kind_synbl {
	null, f, c, t, d, v, vn, vf
};

enum Kind_typel {
	i, r, vo, cc, b, a, dd
};
//
//enum Kind_token {
//	KT,            //关键字
//	IT,            //标识符
//	CT,            //常数
//	PT,            //界符
//	NT
//};

class Part {		//表体结构
public:
	virtual string getInfo() { return "NULL"; };
	virtual void print() {};
	virtual ~Part() {}
};

class keyword_part : public Part {
public:
	string name;
	int index;
public:
	void print() {
		cout << name << " | " << index << endl;
	}
	keyword_part(string& name, int index) :name(name), index(index) {}
	string getInfo() { return name; }
};

class keyword {                       //关键字表
public:
	vector<keyword_part*> arr;
	int n = 0;
public:
	keyword() {
		add("int"); add("void"); add("break"); add("float"); add("while"); add("do"); add("struct");
		add("const"); add("case"); add("for"); add("return"); add("if"); add("default"); add("else");
	}
	void add(string name) {
		keyword_part* p = new keyword_part(name, n);
		n++;
		arr.push_back(p);
	}
	void clear() {
		for (int i = arr.size(); i >= 0; i--) {
			delete arr[i];
		}
		arr.clear();
	}
};

class delimeter_part : public Part {
public:
	string name;
	int index;
public:
	delimeter_part(string& name, int index) :name(name), index(index) {}
	void print() {
		cout << name << " | " << index << endl;
	}
	string  getInfo() { return name; }
};

class delimeter {                     //界符表
public:
	vector<delimeter_part*> arr;
	int n = 0;
public:
	delimeter() {
		add("-"), add("/"), add("("), add(")"), add("=="), add(">="), add("<"), add("+"), add("*"),
			add(">"), add("="), add(","), add(";"), add("++"), add("{"), add("}"), add("||"), add("&&"),
			add("!="), add("<="), add("!"), add("|"), add("&"), add("["), add("]");
	}

	void add(string name) {
		delimeter_part* p = new delimeter_part(name, n);
		n++;
		arr.push_back(p);
	}

	void clear() {
		for (int i = arr.size(); i >= 0; i--) {
			delete arr[i];
		}
		arr.clear();
	}
};

class constant_part : public Part {
public:
	//double count;
	string name;
	int index;
public:
	constant_part(string& name, int index) :name(name), index(index) {}
	void print() {
		cout << name << " | " << index << endl;
	};
	string getInfo() { return name; }
};

class constant1 {                      //常数表（整数）
public:
	vector<constant_part*> arr;
	int n = 0;
public:
	void add(string& name) {
		constant_part* p = new constant_part(name, n);
		n++;
		arr.push_back(p);
	}

	void clear() {
		for (int i = arr.size(); i >= 0; i--) {
			delete arr[i];
		}
		arr.clear();
	}
};

class constant2 {                      //常数表(浮点数)
public:
	vector<constant_part*> arr;
	int n = 0;
public:
	void add(string& name) {
		constant_part* p = new constant_part(name, n);
		n++;
		arr.push_back(p);
	}

	void clear() {
		for (int i = arr.size(); i >= 0; i--) {
			delete arr[i];
		}
		arr.clear();
	}
};

class identif_part : public Part {
public:
	string name;
	int index;
public:
	identif_part(string& name, int index) :name(name), index(index) {}
	void print() {
		cout << name << " | " << index << endl;
	};
	string getInfo() { return name; }
};

class identif {                      //标识符初表
public:
	vector<identif_part*> arr;
	int n = 0;
public:
	void add(string& name) {
		identif_part* p = new identif_part(name, n);
		n++;
		arr.push_back(p);
	}

	void clear() {
		for (int i = arr.size(); i >= 0; i--) {
			delete arr[i];
		}
		arr.clear();
	}
};

class typel_part : public Part {
public:
	Kind_typel tval;
	Part* point;
	int index;

public:
	typel_part(Kind_typel tval, Part* point, int index) :tval(tval), point(point), index(index) {}

};

class typel {                       //类型表
public:
	vector<typel_part*> arr;
	int n = 0;

public:
	typel() {
		add(i, NULL), add(r, NULL), add(vo, NULL), add(cc, NULL), add(b, NULL);
	}

	Part* add(Kind_typel kind, Part* part) {
		typel_part* p = new typel_part(kind, part, n);
		n++;
		arr.push_back(p);
		return p;
	}
};

class ainfl_part : public Part {
public:
	int length;
	typel_part* type;
	int clen;
	int index;

public:
	ainfl_part(int& length, typel_part* type, int clen, int index) :length(length), type(type), clen(clen), index(index) {}
};

class ainfl {                       //数组表
public:
	vector<ainfl_part*> arr;
	int n = 0;

public:
	Part* add(int& length, typel_part* type, int clen) {
		ainfl_part* a = new ainfl_part(length, type, clen, n);
		n++;
		arr.push_back(a);
		return a;
	}
};

class rinfl_part : public Part {
	string name;
	int off;
	typel_part* type;
};

class rinfl {                       //结构表
	vector<rinfl_part*> arr;
};

class vall_part : public Part {
public:
	string FuncName;
	int num = 0;
	vector<string> name;
	vector<Kind_typel> kind;
	vector<int> off;

public:
	string getInfo() { return FuncName; }

public:
	vall_part(string& name) :FuncName(name) {
		off.push_back(0);
	}

	void add(string n, Kind_typel ki, int number) {
		num++;

		name.push_back(n);
		kind.push_back(ki);

		if (ki == i) {
			off.push_back(off[num - 1] + 2 * number);
		}
		else if (ki == r) {
			off.push_back(off[num - 2] + 4 * number);
		}
		else if (ki == vo) {
			off.push_back(off[num - 2]);
		}

	}

	void print() {
		cout << FuncName << " | " << num << endl;
		for (int i = 0; i < name.size(); i++) {
			cout << name[i] << " | " << kind[i] << " | " << off[i] << endl;
		}
	}
};

class vall {                        //活动记录  
public:
	vector<vall_part*> arr;

public:
	Part* add(string name) {
		vall_part* p = new vall_part(name);
		arr.push_back(p);
		return p;
	}
};

class parameter_part : public Part {
	string name;
	typel_part* type;
	Kind_synbl cat;
	vall* addr;
};

class parameter {                    //形参表
	vector<parameter_part*> arr;
};

class pfinfl_part : public Part {
	int level;
	int off;
	int count;
	parameter_part* param;
	int entry;
};

class pfinfl {                       //函数表
	vector<rinfl_part*> arr;
};

class lenl_part : public Part {
	double count;
};

class lenl {                         //长度表
	vector<rinfl_part*> arr;
};

class synbl_part : public Part {
public:
	string name;
	typel_part* type;
	Kind_synbl cat;
	Part* addr;
	int index;
public:
	synbl_part(string name, typel_part* t, Kind_synbl cat, Part* addr, int index)
		:name(name), type(t), cat(cat), addr(addr), index(index) {}
	void print() {
		cout << index << " | " << name << " | " << type->index << " | " << cat << "|" << addr->getInfo() << endl;
	}
	synbl_part getInfo(int i) { return synbl_part(name, type, cat, addr, index); }
	string getInfo() { return name; }
};

class synbl {                           //符号表主表
public:
	vector<synbl_part*> arr;
	int n = 0;
public:
	Part* add(string& name, typel_part* t, Kind_synbl cat, Part* addr) {
		synbl_part* a = new synbl_part(name, t, cat, addr, n);
		n++;
		arr.push_back(a);
		return a;
	}


};

typedef pair<string, Part*> token;  //token结构

typedef vector<token> tokens;

class Table {
public:
	keyword K;
	delimeter P;
	constant1 C1;
	constant2 C2;
	identif I;
	synbl SYNBL;
	typel TYPEL;
	ainfl AINFL;
	rinfl RINFL;
	pfinfl PRINFL;
	parameter PRA;
	lenl LEN;
	vall VALL;

public:
	Table() {}

	bool table_deffunc(token& funName, token& t);

	bool table_defpara(token& funName, token& paraName, token& t);

	bool table_defvar(string& funName, token& paraName, token& t);

	bool table_defarr(string& funName, token& arrName, token& t, string& num);

	bool table_checkfunc(token& funName);

	bool table_checkvar(string& funName, token& varName);

	bool table_checkarr(string& funName, token& varName);

	int getoff(string& name);
};

class Lexical_analysis {
public:
	vector<token> tokens;
public:
	Lexical_analysis() {}                               //初始化

	bool token_generate(string& character, Table& T);   //产生token

	bool input(Table& T) {
		ifstream file("jcArr.txt");
		stringstream buffer;
		buffer << file.rdbuf();
		file.close();

		string character = buffer.str();

		return token_generate(character, T);
	}
};

class Interface {
private:
	vector<string> userInput;  // 存储多行文字
	int currentLine = 0;       // 当前行号
	int scrollOffset = 0;      // 滚动偏移量
	const int MAX_LINE_LENGTH = 100;  // 每行最大字符数
	const int VISIBLE_LINES = 15;     // 可见行数
	string savedContent;       // 存储保存的内容

public:

	// 绘制输入框和已输入的文字
	void drawInputBox();

	// 处理鼠标点击
	bool handleMouseClick();

	// 处理键盘输入
	void handleKeyboardInput();

	// 编译
	void saveContent();

	// 词法分析函数
	void lexicalAnalysis();

	// 语法分析函数
	void syntaxAnalysis();

	// 四元式生成函数
	void generateQuadruples();

	// 目标代码生成函数
	void generateTargetCode();
};