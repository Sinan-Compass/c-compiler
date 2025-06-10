#pragma once 

#include <string>
#include <vector>
#include <format>
#include "semantic.h"

using namespace std;

class toAssembly {
public:
	vector<quat> quats;
	vector<string> assembly_code; // To store generated assembly code
	int cur_ptr;
	int end;
	int lable_ptr;
public:
	string getNewLable();

	toAssembly(vector<quat>& quats) : quats(quats), cur_ptr(0), end(quats.size() - 1),lable_ptr(0){}
	void generateAssembly();
	void generateOneBlock();
	void generateArithmetic();
	void generateLogic();
	void generateTurn();
	void generateIf();
	void generateWhile();
	void generateReturn();
	void generateDefFunction();
	void generateDefIdt();
	void generateUseFunction();
};