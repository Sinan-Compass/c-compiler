#include "Lexical.h"
#include "lexical.h"

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
				character[ch] == '!') {
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
				|| character[ch] == '*') && char_now == "") {
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
				for (int i = 0; i < TABLE.SYNBL.arr.size(); i++) {
					if (char_now == TABLE.SYNBL.arr[i]->name) {
						synbl_part* p = TABLE.SYNBL.arr[i];
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
					TABLE.SYNBL.add(char_now, NULL, null, NULL);
					int i = TABLE.SYNBL.arr.size();
					synbl_part* p = TABLE.SYNBL.arr[i - 1];
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
				constant_part* p = TABLE.C2.arr[i-1];
				token a("CT2", p);
				tokens.push_back(a);
				state = 1;
				char_now.clear();
				continue;
			}
		}
	}

	if (error == 1) {
		cout << "ERROR" << endl;
		return false;
	}
	else {
		cout << "Lexical OK!" << endl;
		/*cout << "Token :";
		for (int i = 0; i < tokens.size(); i++) {
			cout << "(" << tokens[i].first << "," << tokens[i].second->getInfo() << ")";
		}
		cout << endl;

		cout << "K :";
		for (int i = 0; i < TABLE.K.arr.size(); i++) {
			TABLE.K.arr[i]->print();
		}
		cout << endl;

		cout << "I :";
		for (int i = 0; i < TABLE.SYNBL.arr.size(); i++) {
			TABLE.SYNBL.arr[i]->print();
		}
		cout << endl;

		cout << "C :";
		for (int i = 0; i < TABLE.C.arr.size(); i++) {
			TABLE.C.arr[i]->print();
		}
		cout << endl;

		cout << "P :";
		for (int i = 0; i < TABLE.P.arr.size(); i++) {
			TABLE.P.arr[i]->print();
		}
		cout << endl;*/
		return true;
	}
}