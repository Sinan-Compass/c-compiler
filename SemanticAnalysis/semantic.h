#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include "ast.h"
#include "Parser.h"

using quat = string[4];

class semantic
{
public:
    vector<quat> quats;
    ASTNode *root;
public:
    semantic(/* args */);
    semantic(ASTNode *root):root(root){};
    ~semantic();

    vector<quat> getQuats();
    void analyzeProgram();
    void analyzeFunction(FunctionNode *func);
    void analyzeBlock(BlockNode *block);
    void analyzeStatement(StatementNode *stmt);
    void analyzeExpression(ExpressionNode *expr);
    void analyzeExpress(ExpressNode *express);
    void analyzeAssign(AssignNode *assign);
    void analyzeCall(UseFuncNode *call);
    void analyzeList(ListNode *list);
    void analyzeParam(PararmeterNode *param);
    void analyzeDeclare(DeclareNode *declare);
    void analyzeIf(IfNode *ifNode);
    void analyzeWhile(WhileNode *whileNode);
    void analyzeReturn(ReturnNode *returnNode);
    void analyzeOr(OrNode *orNode); 
    void analyzeAnd(AndNode *andNode);
    void analyzeEqual(EqualNode *equalNode);
    void analyzeCompare(CompareNode *compareNode);
    void analyzeAdd(AddNode *addNode);
    void analyzeMul(MulNode *mulNode);

    void analyzeBasic(BasicNode *basic);
    void analyzeUseFun(UseFuncNode *useNode);
    void analyzeFactPar(FactParNode *factPar);

    void analyzeType(TypeNode *typeNode);
    void analyzeName(NameNode *nameNode);
    void analyzeInt(IntNode *intNode);
    void analyzeDouble(DoubleNode *doubleNode);
    void analyzeOp(opNode *op);
};

semantic::semantic(/* args */){}
semantic::~semantic(){}

vector<quat> semantic::getQuats()
{

    return quats;
}
