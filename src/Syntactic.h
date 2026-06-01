#pragma once
#include "ast.h"
using namespace std;

class Parser {
	tokens& t;
	token cur_t;
	int cur_t_index;

	Table& table;
	string cur_func;

public:
	void next();
	ASTNode* programProduction();	// <程序> -> { <函数定义> }
	ASTNode* functionProduction();  // <函数定义> -> <类型说明符> '标识符' '(' [ <参数列表> ] ')' <块>
	ASTNode* typeProduction();		// <类型说明符> -> "int" | "float" | "void"
	ASTNode* listProduction();		// <参数列表> -> <参数> { ',' <参数> }
	ASTNode* blockProduction();		// <块> -> '{' { <变量声明> } { <语句> } '}'
	ASTNode* pararmeterProduction();// <参数> -> <类型说明符> <标识符>
	ASTNode* declareProduction();	// <变量声明> -> <类型说明符> '标识符' ';'   (*每行声明一个变量*)
									// <变量声明> -> <类型说明符> '标识符''[' <整数常量> ']' ';'
	ASTNode* statementProducation();// <语句> -> <表达式语句> | <块> | <if语句> | <while语句> | <return语句>
	ASTNode* expressProduction();   // <表达式语句> -> <表达式> ';'
	ASTNode* ifProduction();		// <if语句> -> "if" '(' <表达式> ')' <语句> [ "else" <语句> ]
	ASTNode* whileProduction();		// <while语句> -> "while" '(' <表达式> ')'  <语句>
	ASTNode* returnProduction();	// <return语句> -> "return" [ <表达式> ] ';'
	ASTNode* expressionProduction();// <表达式> -> <赋值表达式>
	ASTNode* assignProduction();	// <赋值表达式> -> <逻辑或表达式> | < 标识符> '=' < 赋值表达式 > (*赋值是右结合*)
	ASTNode* orProduction();		// <逻辑或表达式> -> <逻辑与表达式>{ "||" < 逻辑与表达式 > }
	ASTNode* andProduction();		// <逻辑与表达式> -> <相等表达式> { "&&" <相等表达式> }
	ASTNode* equalProduction();		// <相等表达式> -> <关系表达式> { ("==" | "!=") <关系表达式> }
	ASTNode* compareProduction();	// <关系表达式> -> <加法表达式> { ('<' | '>' | "<=" | ">=") <加法表达式> }
	ASTNode* addProduction();		// <加法表达式> -> <乘法表达式> { ('+' | '-') <乘法表达式> }
	ASTNode* mulProduction();		// <乘法表达式> -> <基本表达式> { ('*' | '/') <基本表达式> }
	ASTNode* basicProdution();		// <基本表达式> -> '标识符' | '整数常量' | '浮点数常量' | '(' < 表达式 > ')' | <函数调用>  
									// <基本表达式> -> arrNode
	ASTNode* arrProduction();		// arrNode -> <数组名> <表达式>
	ASTNode* useFuncProduction();   // <函数调用> -> <标识符> '(' [ <实参列表> ] ')'
	ASTNode* factParProduction();   // <实参列表> -> <表达式> { ',' <表达式> }

public:
	Parser(tokens& t, Table& table) :t(t),table(table) {
		cur_t_index = 0;
		if (t.size() != 0) {
			cur_t = t[cur_t_index];
		}
	}
	ASTNode* Syntax_analyzer() {
		ASTNode* program = programProduction();
		if (cur_t_index == t.size()) {
			return program; //正确
		}
		else {
			cout << "WANNING!" << endl;
			return nullptr; //错误
		}
	}
};