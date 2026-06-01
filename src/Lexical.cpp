#include "Lexical.h"

bool Lexical_analysis::token_generate(string& character, Table& TABLE) {
	string char_now;
	int error = 0;
	int ch;
	int state = 1;
	int find = 0;

	for (ch = 0; ch <= character.size();) {
		if (state == 1) {
			if (character[ch] >= 'a' && character[ch] <= 'z' || character[ch] >= 'A' && character[ch] <= 'Z' || character[ch] == '_') {
				state = 2;
				continue;
			}
			else if (character[ch] >= '0' && character[ch] <= '9') {
				state = 3;
				continue;
			}
			else if (character[ch] == '-' || character[ch] == '/' || character[ch] == '(' || character[ch] == ')' || character[ch] == '=' ||
				character[ch] == '<' || character[ch] == '+' || character[ch] == '*' || character[ch] == '>' || character[ch] == ',' ||
				character[ch] == ';' || character[ch] == '{' || character[ch] == '}' || character[ch] == '|' || character[ch] == '&' ||
				character[ch] == '!' || character[ch] == '[' || character[ch] == ']') {
				state = 4;
				continue;
			}
			else if (character[ch] == '\'') {
				char_now += character[ch];
				ch++;
				state = 5;
				continue;
			}
			else if (character[ch] == '"') {
				char_now += character[ch];
				ch++;
				state = 6;
				continue;
			}
			else {
				ch++;
				continue;
			}
		}
		else if (state == 2) {
			if (character[ch] >= 'a' && character[ch] <= 'z' || character[ch] >= 'A' && character[ch] <= 'Z'
				|| character[ch] == '_' || character[ch] >= '0' && character[ch] <= '9') {
				char_now += character[ch];
				ch++;
				continue;
			}
			else {
				state = 12;
				continue;
			}
		}
		else if (state == 3) {
			if (character[ch] >= '0' && character[ch] <= '9') {
				char_now += character[ch];
				ch++;
				continue;
			}
			else if (character[ch] == '.') {
				char_now += character[ch];
				ch++;
				state = 8;
				continue;
			}
			else {
				state = 13;
				continue;
			}
		}
		else if (state == 4) {
			if ((character[ch] == '{' || character[ch] == '}' || character[ch] == '(' || character[ch] == ')' ||
				character[ch] == ';' || character[ch] == '-' || character[ch] == '/' || character[ch] == ','
				|| character[ch] == '*' || character[ch] == '[' || character[ch] == ']') && char_now == "") {
				char_now += character[ch];
				ch++;
				state = 14;
				continue;
			}
			else if (character[ch] == '=' || character[ch] == '<' || character[ch] == '>' || character[ch] == '+'
				|| character[ch] == '|' || character[ch] == '&' || character[ch] == '!') {
				char_now += character[ch];
				for (int i = 0; i < TABLE.P.arr.size(); i++) {
					if (char_now == TABLE.P.arr[i]->name) {
						find = 1;
						break;
					}
				}
				if (find == 1) {
					ch++;
					find = 0;
				}
				else {
					char_now.pop_back();
					state = 14;
				}
			}
			else {
				state = 14;
				continue;
			}
		}
		else if (state == 5) {
			char_now += character[ch];
			ch++;
			if (character[ch] == '\'') {
				char_now += character[ch];
				ch++;
				state = 17;
			}
			else {
				error = 1;
				break;
			}
		}
		else if (state == 6) {
			if (character[ch] == '"') {
				char_now += character[ch];
				ch++;
				state = 18;
			}
			else {
				char_now += character[ch];
				ch++;
				continue;
			}
		}
		else if (state == 7) {
			if (character[ch] >= '0' && character[ch] <= '9' || character[ch] >= 'a' && character[ch] <= 'f') {
				char_now += character[ch];
				ch++;
				continue;
			}
			else {
				state = 15;
				continue;
			}
		}
		else if (state == 8) {
			if (character[ch] >= '0' && character[ch] <= '9') {
				char_now += character[ch];
				ch++;
				state = 9;
				continue;
			}
			else {
				error = 1;
				break;
			}
		}
		else if (state == 9) {
			if (character[ch] >= '0' && character[ch] <= '9') {
				char_now += character[ch];
				ch++;
				continue;
			}
			else {
				state = 16;
				continue;
			}
		}
		else if (state == 10) {
			if (character[ch] == '+' || character[ch] == '-') {
				char_now += character[ch];
				ch++;
				if (character[ch] >= '0' && character[ch] <= '9') {
					char_now += character[ch];
					ch++;
					state = 11;
					continue;
				}
				else {
					error = 1;
					break;
				}
				continue;
			}
			else if (character[ch] >= '0' || character[ch] <= '9') {
				char_now += character[ch];
				ch++;
				state = 11;
				continue;
			}
			else {
				error = 1;
				break;
			}
		}
		else if (state == 11) {
			if (character[ch] >= '0' && character[ch] <= '9') {
				char_now += character[ch];
				ch++;
				continue;
			}
			else {
				state = 16;
				continue;
			}
		}
		else if (state == 12) {
			for (int i = 0; i < TABLE.K.arr.size(); i++) {
				if (char_now == TABLE.K.arr[i]->name) {
					keyword_part* p = TABLE.K.arr[i];
					token a("KT", p);
					tokens.push_back(a);
					find = 1;
					break;
				}
			}
			if (find == 1) {
				state = 1;
				find = 0;
				char_now.clear();
				continue;
			}
			else {
				for (int i = 0; i < TABLE.I.arr.size(); i++) {
					if (char_now == TABLE.I.arr[i]->name) {
						identif_part* p = TABLE.I.arr[i];
						token a("IT", p);
						tokens.push_back(a);
						find = 1;
						break;
					}
				}
				if (find == 1) {
					state = 1;
					find = 0;
					char_now.clear();
					continue;
				}
				else {
					TABLE.I.add(char_now);
					int i = TABLE.I.arr.size();
					identif_part* p = TABLE.I.arr[i - 1];
					token a("IT", p);
					tokens.push_back(a);
					state = 1;
					char_now.clear();
					continue;
				}
			}
		}
		else if (state == 13) {
			/*int num = stoi(char_now);
			double result = static_cast<double>(num);*/
			for (int i = 0; i < TABLE.C1.arr.size(); i++) {
				if (char_now == TABLE.C1.arr[i]->name) {
					constant_part* p = TABLE.C1.arr[i];
					token a("CT1", p);
					tokens.push_back(a);
					find = 1;
					break;
				}
			}
			if (find == 1) {
				state = 1;
				find = 0;
				char_now.clear();
				continue;
			}
			else {
				TABLE.C1.add(char_now);
				int i = TABLE.C1.arr.size();
				constant_part* p = TABLE.C1.arr[i - 1];
				token a("CT1", p);
				tokens.push_back(a);
				state = 1;
				char_now.clear();
				continue;
			}
		}
		else if (state == 14) {
			for (int i = 0; i < TABLE.P.arr.size(); i++) {
				if (char_now == TABLE.P.arr[i]->name) {
					delimeter_part* p = TABLE.P.arr[i];
					token a("PT", p);
					tokens.push_back(a);
					break;
				}
			}
			state = 1;
			char_now.clear();
			continue;
		}
		else if (state == 15) {
			char_now.erase(0, 2);
			stringstream ss;
			ss << hex << char_now;
			int dec;
			ss >> dec;
			char_now = to_string(dec);
			state = 13;
			continue;
		}
		else if (state == 16) {
			/*double num = std::stod(char_now);*/
			for (int i = 0; i < TABLE.C2.arr.size(); i++) {
				if (char_now == TABLE.C2.arr[i]->name) {
					constant_part* p = TABLE.C2.arr[i];
					token a("CT2", p);
					tokens.push_back(a);
					find = 1;
					break;
				}
			}
			if (find == 1) {
				state = 1;
				find = 0;
				char_now.clear();
				continue;
			}
			else {
				TABLE.C2.add(char_now);
				int i = TABLE.C2.arr.size();
				constant_part* p = TABLE.C2.arr[i - 1];
				token a("CT2", p);
				tokens.push_back(a);
				state = 1;
				char_now.clear();
				continue;
			}
		}
	}


	if (error == 1) {
		return false;
	}
	else
		return true;
}

bool Table::table_deffunc(token& funName, token& t) {

	bool find;
	int num;

	//在主表中定义
	for (num = 0; num < SYNBL.arr.size(); num++) {

		if (funName.second->getInfo() == SYNBL.arr[num]->name) {
			break;
		}
	}

	if (num == SYNBL.arr.size()) {         //尚未定义
		//添加新标识符
		string name = funName.second->getInfo();
		funName.second = SYNBL.add(name, NULL, null, NULL);

		//定义类型
		if (t.second->getInfo() == "int") {
			SYNBL.arr[num]->type = TYPEL.arr[i];
		}
		else if (t.second->getInfo() == "float") {
			SYNBL.arr[num]->type = TYPEL.arr[r];
		}
		else if (t.second->getInfo() == "void") {
			SYNBL.arr[num]->type = TYPEL.arr[vo];
		}

		//定义种类
		SYNBL.arr[num]->cat = f;

		//创造活动记录
		Part* p = VALL.add(name);

		//定义addr指向
		SYNBL.arr[num]->addr = p;

		find = true;
	}
	else {
		find = false;
	}

	return find;
}

bool Table::table_defpara(token& funcName, token& paraName, token& t) {


	bool find;
	int num;
	int num1;
	int num2;

	string name = funcName.second->getInfo();
	name = name + "_" + paraName.second->getInfo();

	//1.先检查funName是否已经被定义过，如果没找到，返回false
	for (num2 = 0; num2 < VALL.arr.size(); num2++) {

		if (funcName.second->getInfo() == VALL.arr[num2]->FuncName) {
			break;
		}
	}

	if (num2 == VALL.arr.size()) {
		find = false;
	}
	else {
		//2.检查这个function的vall里面有没有paraName,如果存在，返回false，证明重定义

		for (num1 = 0; num1 < VALL.arr[num2]->num; num1++) {

			if (name == VALL.arr[num2]->name[num1]) {
				break;
			}
		}

		if (num1 != VALL.arr[num2]->num) {
			find = false;
		}
		else {
			//3.在该函数的vall新加一行变量，名字为funcName_paraName

			if (t.second->getInfo() == "int") {
				VALL.arr[num2]->add(name, i,1);
			}
			else if (t.second->getInfo() == "float") {
				VALL.arr[num2]->add(name, r,1);
			}
			else if (t.second->getInfo() == "void") {
				VALL.arr[num2]->add(name, vo,1);
			}

			//4.再在主表里新加该变量，名字为funcName_paraName，指针指向刚刚在vall新加的一行,改token，
			num = SYNBL.arr.size();
			paraName.second = SYNBL.add(name, NULL, null, NULL);

			//定义类型
			if (t.second->getInfo() == "int") {
				SYNBL.arr[num]->type = TYPEL.arr[i];
			}
			else if (t.second->getInfo() == "float") {
				SYNBL.arr[num]->type = TYPEL.arr[r];
			}
			else if (t.second->getInfo() == "void") {
				SYNBL.arr[num]->type = TYPEL.arr[vo];
			}

			//定义种类
			SYNBL.arr[num]->cat = v;

			SYNBL.arr[num]->addr = VALL.arr[num2];

			find = true;
		}
	}

	return find;
}

bool Table::table_defvar(string& funcName, token& paraName, token& t) {
	bool find;
	int num;
	int num1;
	int num2;

	string name = funcName + "_" + paraName.second->getInfo();

	//1.先检查funName是否已经被定义过，如果没找到，返回false
	for (num2 = 0; num2 < VALL.arr.size(); num2++) {

		if (funcName == VALL.arr[num2]->FuncName) {
			break;
		}
	}

	if (num2 == VALL.arr.size()) {
		find = false;
	}
	else {
		//2.检查这个function的vall里面有没有paraName,如果存在，返回false，证明重定义

		for (num1 = 0; num1 < VALL.arr[num2]->num; num1++) {

			if (name == VALL.arr[num2]->name[num1]) {
				break;
			}
		}

		if (num1 != VALL.arr[num2]->num) {
			find = false;
		}
		else {
			//3.在该函数的vall新加一行变量，名字为funcName_paraName

			if (t.second->getInfo() == "int") {
				VALL.arr[num2]->add(name, i,1);
			}
			else if (t.second->getInfo() == "float") {
				VALL.arr[num2]->add(name, r,1);
			}
			else if (t.second->getInfo() == "void") {
				VALL.arr[num2]->add(name, vo,1);
			}

			//4.再在主表里新加该变量，名字为funcName_paraName，指针指向刚刚在vall新加的一行,改token，
			num = SYNBL.arr.size();
			paraName.second = SYNBL.add(name, NULL, null, NULL);

			//定义类型
			if (t.second->getInfo() == "int") {
				SYNBL.arr[num]->type = TYPEL.arr[i];
			}
			else if (t.second->getInfo() == "float") {
				SYNBL.arr[num]->type = TYPEL.arr[r];
			}
			else if (t.second->getInfo() == "void") {
				SYNBL.arr[num]->type = TYPEL.arr[vo];
			}

			//定义种类
			SYNBL.arr[num]->cat = v;

			SYNBL.arr[num]->addr = VALL.arr[num2];

			find = true;
		}
	}

	return find;
}

bool Table::table_checkfunc(token& funName) {

	int num;
	bool find;

	//1.在主表中检查funName在不在，不在返回false
	for (num = 0; num < SYNBL.arr.size(); num++) {

		if (funName.second->getInfo() == SYNBL.arr[num]->name) {
			break;
		}
	}

	if (num == SYNBL.arr.size()) {         //尚未定义
		find = false;
	}
	else {
		//2.改指针，使其指向主表
		funName.second = SYNBL.arr[num];

		find = true;
	}

	return find;
}

bool Table::table_checkvar(string& funName, token& varName) {

	int num;
	bool find;

	string name = funName;
	name = name + "_" + varName.second->getInfo();

	//1.在主表中检查funName_varName在不在，不在返回false
	for (num = 0; num < SYNBL.arr.size(); num++) {

		if (name == SYNBL.arr[num]->name) {
			break;
		}
	}

	if (num == SYNBL.arr.size()) {         //尚未定义
		find = false;
	}
	else {
		//2.改指针，使其指向主表
		varName.second = SYNBL.arr[num];

		find = true;
	}

	return find;
}

bool Table::table_defarr(string& funcName, token& arrName, token& t, string& n) {

	bool find;
	int num;
	int num1;
	int num2;

	string name = funcName;
	name = name + "_" + arrName.second->getInfo();

	int number = stoi(n);

	//1.先检查funName是否已经被定义过，如果没找到，返回false
	for (num2 = 0; num2 < VALL.arr.size(); num2++) {

		if (funcName == VALL.arr[num2]->FuncName) {
			break;
		}
	}

	if (num2 == VALL.arr.size()) {
		find = false;
	}
	else {
		//2.检查这个function的vall里面有没有arrName,如果存在，返回false，证明重定义

		for (num1 = 0; num1 < VALL.arr[num2]->num; num1++) {

			if (name == VALL.arr[num2]->name[num1]) {
				break;
			}
		}

		if (num1 != VALL.arr[num2]->num) {
			find = false;
		}
		else {
			//3.在该函数的vall新加一行变量，名字为funcName_paraName

			if (t.second->getInfo() == "int") {
				VALL.arr[num2]->add(name, i, number);
			}
			else if (t.second->getInfo() == "float") {
				VALL.arr[num2]->add(name, r, number);
			}
			else if (t.second->getInfo() == "void") {
				VALL.arr[num2]->add(name, vo, number);
			}

			//4.再在主表里新加该变量，名字为funcName_paraName，指针指向刚刚在vall新加的一行,改token，
			num = SYNBL.arr.size();
			arrName.second = SYNBL.add(name, NULL, null, NULL);

			//定义种类
			SYNBL.arr[num]->cat = v;

			SYNBL.arr[num]->addr = VALL.arr[num2];

			//对数组表进行操作
			Part* p = NULL;

			if (t.second->getInfo() == "int") {
				p = AINFL.add(number, TYPEL.arr[i], 2);
			}
			else if (t.second->getInfo() == "float") {
				p = AINFL.add(number, TYPEL.arr[i], 4);
			}
			else if (t.second->getInfo() == "void") {
				p = AINFL.add(number, TYPEL.arr[i], 0);
			}

			SYNBL.arr[num]->type = (typel_part*)TYPEL.add(a, p);

			find = true;
		}
	}

	return find;
}

int Table::getoff(string& name) {
	int num;
	int num2;

	for (num = 0; num < SYNBL.arr.size(); num++) {

		if (name == SYNBL.arr[num]->name) {
			break;
		}
	}

	string funcname = SYNBL.arr[num]->addr->getInfo();

	for (num = 0; num < VALL.arr.size(); num++) {

		if (funcname == VALL.arr[num]->FuncName) {
			break;
		}
	}

	for (num2 = 0; num2 < VALL.arr[num]->name.size(); num2++) {

		if (name == VALL.arr[num]->name[num2]) {
			break;
		}
	}

	return VALL.arr[num]->off[num2];
}

bool Table::table_checkarr(string& funName, token& varName) {

	int num;
	bool find;

	string name = funName;
	name = name + "_" + varName.second->getInfo();

	//1.在主表中检查funName_varName在不在，不在返回false
	for (num = 0; num < SYNBL.arr.size(); num++) {

		if (name == SYNBL.arr[num]->name) {
			break;
		}
	}

	if (num == SYNBL.arr.size()) {         //尚未定义
		find = false;
	}
	else {
		//2.改指针，使其指向主表
		varName.second = SYNBL.arr[num];

		find = true;
	}

	return find;
}