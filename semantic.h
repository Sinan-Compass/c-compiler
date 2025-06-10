#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include "Syntactic.h"

using namespace std;
// quat is short for quadruple, which is a 4-tuple used in intermediate code representation.

class quat {
public:
    vector<string> elements;
public:
    quat() :elements(4) {};
    quat(const string& op, const string& arg1, const string& arg2, const string& result);
    quat(vector<string> arr);
    string& operator[](size_t index);
    const string& operator[](size_t index) const;
};

class semantic{
public:
    vector<quat> quats;
    long long tempVarCount = 0;
    ASTNode* root;
public:
    semantic();
    semantic(ASTNode* root) :root(root), tempVarCount(0) {
        analyzeProgram();
    };
    ~semantic();

    vector<quat> getQuats();

    void analyzeProgram();
    void analyzeFunction(FunctionNode* func);
    void analyzeList(ListNode* list);
    string analyzeParam(PararmeterNode* param);

    void analyzeBlock(BlockNode* block);
    string analyzeDeclare(DeclareNode* declare);

    void analyzeStatement(StatementNode* stmt);
    string analyzeExpression(ExpressionNode* expr);
    string analyzeExpress(ExpressNode* express);
    void analyzeIf(IfNode* ifNode);
    void analyzeWhile(WhileNode* whileNode);
    void analyzeReturn(ReturnNode* returnNode);

    string analyzeAssign(AssignNode* assign);
    string analyzeOr(OrNode* orNode);
    string analyzeAnd(AndNode* andNode);
    string analyzeEqual(EqualNode* equalNode);
    string analyzeCompare(CompareNode* compareNode);
    string analyzeAdd(AddNode* addNode);
    string analyzeMul(MulNode* mulNode);

    string analyzeBasic(BasicNode* basic);
    string analyzeUseFun(UseFuncNode* useNode);
    void analyzeFactPar(FactParNode* factPar);

    string analyzeType(TypeNode* typeNode);
    string analyzeName(NameNode* nameNode);
    string analyzeInt(IntNode* intNode);
    string analyzeDouble(DoubleNode* doubleNode);
    string analyzeOp(OpNode* op);

    string getNextTempVar();
};