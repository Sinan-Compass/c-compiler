#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;
typedef string token;
typedef vector<token> tokens;

class ASTNode {
public:
	std::vector<ASTNode*> childs;
	token t;
public:
	ASTNode() :childs(0),t("NULL") {}
	virtual ~ASTNode() {
		for (ASTNode* child : childs) {
			delete child;
		}
	}
};

// <程序> -> { <函数定义> }
class ProgramNode : public ASTNode {
public:
	ProgramNode() { t = token("program"); }
};

// <函数定义> -> <类型说明符> '标识符' '('[<参数列表>] ')' < 块 >
class FunctionNode : public ASTNode {
public:
	FunctionNode(ASTNode* type, ASTNode* name, ASTNode* list, ASTNode* block) {
		childs = { type,name,list,block };
		t = token("functoin");
	}
};

// <类型说明符> -> "int" | "float" | "void"
class TypeNode : public ASTNode {
public:
	TypeNode(token& type) {t = type;}
};

// "标识符"
class NameNode : public ASTNode {
public:
	NameNode(token& name){t = name;}
};

// "整数常量"
class IntNode : public ASTNode {
public:
	IntNode(token& num){t = num;}
};

// "浮点常量"
class DoubleNode : public ASTNode {
public:
	DoubleNode(token& num){	t = num;}
};

// <参数列表> -> <参数> { ',' <参数> }
class ListNode : public ASTNode {
public:
	ListNode(ASTNode* par) { 
		childs = { par }; 
		t = token("list");
	}
};

// <块> -> '{' { <变量声明> } { <语句> } '}'
class BlockNode : public ASTNode {
public:
	BlockNode() { t = token("block"); }
};

// <参数> -> <类型说明符> <标识符>
class PararmeterNode : public ASTNode {
public:
	PararmeterNode(ASTNode* type, ASTNode* name){
		childs = { type,name };
		t = token("pararmeter");
	}
};

// <变量声明> -> <类型说明符> '标识符' ';'
class DeclareNode : public ASTNode {	
public:
	DeclareNode(ASTNode* type, ASTNode* name){
		childs = { type,name };
		t = token("declare");
	}
};

// <语句> -> <表达式语句> | <块> | <if语句> | <while语句> | <return语句>
class StatementNode : public ASTNode {
public:
	StatementNode(ASTNode* child) {
		childs = { child };
		t = token("statement");
	}
};

// <表达式语句> -> <表达式> ';'
class ExpressNode : public ASTNode {
public:
	ExpressNode(ASTNode* expression) { 
		childs = { expression };
		t = token("express");
	}
};

// <表达式> -> <赋值表达式>
class ExpressionNode : public ASTNode {
public:
	ExpressionNode(ASTNode* assign){
		childs = { assign }; 
		t = token("expression");
	}
};

// <if语句> -> "if" '(' <表达式> ')' <语句> [ "else"  <语句> ]
class IfNode : public ASTNode {
public:
	IfNode(ASTNode* op1 ,ASTNode* condition, ASTNode* then, ASTNode* op2 ,ASTNode* els){
		childs = { op1,condition ,then,op2,els };
		t = token("if");
	}
};

// <while语句> -> "while" '(' <表达式> ')'  <语句>
class WhileNode : public ASTNode {
public:
	WhileNode(ASTNode* op, ASTNode* condition, ASTNode* then) {
		childs = { op,condition ,then };
		t = token("while");
	}
};

// <return语句> -> "return" [ <表达式> ] ';'
class ReturnNode : public ASTNode {
public:
	ReturnNode(ASTNode* op, ASTNode* value) {
		childs = { op,value };
		t = token("return");
	}
};

// <赋值表达式> -> <逻辑或表达式> | <标识符> '=' < 赋值表达式 > (*赋值是右结合*)
class AssignNode : public ASTNode {
public:
	AssignNode(ASTNode* _or) {
		childs = { _or };
		t = token("assign");
	}
	AssignNode(ASTNode* name, ASTNode* assign) {
		childs = { name,assign };
		t = token(" = ");
	}
};

// <逻辑或表达式> -> <逻辑与表达式>{ "||" < 逻辑与表达式 > }
class OrNode : public ASTNode {
public:
	OrNode(ASTNode* _and) { 
		childs = { _and };
		t = token("or");
	}
};

// <逻辑与表达式> -> <相等表达式> { "&&" <相等表达式> }
class AndNode : public ASTNode {
public:
	AndNode(ASTNode* equal) { 
		childs = { equal }; 
		t = token("and");
	}
};

// <相等表达式> -> <关系表达式> { ("==" | "!=") <关系表达式> }
class EqualNode : public ASTNode {
public:
	EqualNode(ASTNode* compare) { 
		childs = { compare };
		t = token("equal");	}
};

// <关系表达式> -> <加法表达式> { ('<' | '>' | "<=" | ">=") <加法表达式> }
class CompareNode : public ASTNode {
public:
	CompareNode(ASTNode* add) { 
		childs = { add };
		t = token("compare");
	}
};

// <加法表达式> -> <乘法表达式> { ('+' | '-') <乘法表达式> }
class AddNode : public ASTNode {
public:
	AddNode(ASTNode* mul) { 
		childs = { mul };  
		t = token("add");
	}
};

// <乘法表达式> -> <基本表达式> { ('*' | '/') <基本表达式> }
class MulNode : public ASTNode {
public:
	MulNode(ASTNode* basic) { 
		childs = { basic }; 
		t = token("mul");
	}
};

// <基本表达式> -> '标识符' | '整数常量' | '浮点数常量' | '(' < 表达式 > ')' | <函数调用>
class BasicNode : public ASTNode {
public:
	BasicNode(ASTNode* c) {
		childs = { c }; 
		t = token("basic");
	}
};

// <函数调用> -> <标识符> '(' [ <实参列表> ] ')'
class UseFuncNode : public ASTNode {
public:
	UseFuncNode(ASTNode* name, ASTNode* factPar) {
		childs = { name,factPar };
		t = token("useFunc");
	}
};

// <实参列表> -> <表达式> { ',' <表达式> }
class FactParNode : public ASTNode {
public:
	FactParNode(ASTNode* expression) { 
		childs = { expression };
		t = token("factPar");
	}
};

//运算符
class opNode : public ASTNode {
public:
	opNode(token& op) { t = op; }
};
