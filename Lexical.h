#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

enum Kind_synbl {
	null, f, c, t, d, v, vn, vf
};

enum Kind_typel {
	i, r, cc, b, a, dd
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
			add("!="), add("<="), add("!"), add("|"), add("&");
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

class typel_part : public Part {
	Kind_typel tval;
	Part* point;
};

class typel {                       //类型表
	vector<typel_part*> arr;
};

class ainfl_part : public Part {
	int length;
	typel_part* type;
	int clen;
};

class ainfl {                       //数组表
	vector<ainfl_part*> arr;
};

class rinfl_part : public Part {
	string name;
	int off;
	typel_part* type;
};

class rinfl {                       //结构表
	vector<rinfl_part*> arr;
};

class vall {                        //活动记录  
private:
	int a;
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
		cout << name << " | " << index << endl;
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

class Table {
public:
	keyword K;
	delimeter P;
	constant1 C1;
	constant2 C2;
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
	bool table_defvar(token& funName, token& paraName, token& t);
	bool table_checkfunc(token& funName);
	bool table_checkvar(token& varName);
};

typedef pair<string, Part*> token;  //token结构
typedef vector<token> tokens;

class Lexical_analysis {
public:
	vector<token> tokens;
public:
	Lexical_analysis() {}                               //初始化
	bool token_generate(string& character, Table& T);   //产生token
	bool input(Table& T) {
		ifstream file("test.txt");
		stringstream buffer;
		buffer << file.rdbuf();
		file.close();

		string character = buffer.str();

		return token_generate(character, T);
	}
};