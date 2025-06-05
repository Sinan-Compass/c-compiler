#include "semantic.h"

string semantic::getNextTempVar()
{
    return "t" + to_string(this->tempVarCount++);
}

void semantic::analyzeProgram() {
    for (ASTNode* child : root->childs) {
        if (FunctionNode* func = dynamic_cast<FunctionNode*>(child)) {
            analyzeFunction(func);
        }
    }
}

void semantic::analyzeFunction(FunctionNode *func) {
    quat q;
    string type = analyzeType(dynamic_cast<TypeNode*>(func->childs[0]));
    string name = analyzeName(dynamic_cast<NameNode*>(func->childs[1]));
    q[0] = "df";
    q[1] = "fun";
    q[2] = type;
    q[3] = name;
    quats.push_back(q);

    analyzeList(dynamic_cast<ListNode*>(func->childs[2]));
    analyzeBlock(dynamic_cast<BlockNode*>(func->childs[3]));
}

void semantic::analyzeBlock(BlockNode *block) {
    for (ASTNode* child : block->childs) {
        if (DeclareNode* declare = dynamic_cast<DeclareNode*>(child)) {
            analyzeDeclare(declare);
        } else if (StatementNode* stmt = dynamic_cast<StatementNode*>(child)) {
            analyzeStatement(stmt);
        }
    }
}

void semantic::analyzeStatement(StatementNode *stmt) {
    if (ExpressNode* express = dynamic_cast<ExpressNode*>(stmt->childs[0])) {
        analyzeExpress(express);
    } else if (BlockNode* block = dynamic_cast<BlockNode*>(stmt->childs[1])) {
        analyzeBlock(block);
    } else if (IfNode* ifNode = dynamic_cast<IfNode*>(stmt->childs[2])) {
        analyzeIf(ifNode);
    } else if (WhileNode* whileNode = dynamic_cast<WhileNode*>(stmt->childs[3])) {
        analyzeWhile(whileNode);
    } else if (ReturnNode* returnNode = dynamic_cast<ReturnNode*>(stmt->childs[4])) {
        analyzeReturn(returnNode);
    }
}

void semantic::analyzeExpress(ExpressNode *express) {
    analyzeExpression(dynamic_cast<ExpressionNode*>(express->childs[0]));
}

string semantic::analyzeExpression(ExpressionNode *expr) {
    string tempVal = analyzeAssign(dynamic_cast<AssignNode *>(expr->childs[0])); // 存放表达式的计算结果
    return tempVal;
}

string semantic::analyzeAssign(AssignNode *assign) {
    if(assign->childs.size() <= 1){
        return analyzeOr(dynamic_cast<OrNode *>(assign->childs[0]));
    }
    else{
        analyzeName(dynamic_cast<NameNode *>(assign->childs[0]));
        analyzeOp(dynamic_cast<opNode *>(assign->childs[1]));
        analyzeExpression(dynamic_cast<ExpressionNode *>(assign->childs[2]));
    }

}

void semantic::analyzeList(ListNode *list) {
    for (ASTNode* child : list->childs) {
        if (PararmeterNode* param = dynamic_cast<PararmeterNode*>(child)) {
            analyzeParam(param);
        }
    }
}

void semantic::analyzeParam(PararmeterNode *param) {
    string type = analyzeType(dynamic_cast<TypeNode*>(param->childs[0]));
    string name = analyzeName(dynamic_cast<NameNode*>(param->childs[1]));
    quats.push_back({"para", "_", type, name});
}

void semantic::analyzeDeclare(DeclareNode *declare) {
    string type = analyzeType(dynamic_cast<TypeNode*>(declare->childs[0]));
    string name = analyzeName(dynamic_cast<NameNode*>(declare->childs[1]));
    quats.push_back({"df", "idt", type, name});  
}

void semantic::analyzeIf(IfNode *ifNode) {
    analyzeExpression(dynamic_cast<ExpressionNode*>(ifNode->childs[0]));
    analyzeStatement(dynamic_cast<StatementNode*>(ifNode->childs[1]));
    if (StatementNode* elseStmt = dynamic_cast<StatementNode*>(ifNode->childs[2])) {
        analyzeStatement(elseStmt);
    }
}

void semantic::analyzeWhile(WhileNode *whileNode) {
    analyzeExpression(dynamic_cast<ExpressionNode*>(whileNode->childs[0]));
    analyzeStatement(dynamic_cast<StatementNode*>(whileNode->childs[1]));
}

void semantic::analyzeReturn(ReturnNode *returnNode) {
    if (ExpressionNode* expr = dynamic_cast<ExpressionNode*>(returnNode->childs[0])) {
        analyzeExpression(expr);
    }
}

void semantic::analyzeOr(OrNode *orNode) {
    if (orNode->childs.size() <= 1)
    {
        return analyzeAnd(dynamic_cast<AndNode *>(orNode->childs[0]));
    }
    else
    {
        analyzeName(dynamic_cast<NameNode *>(orNode->childs[0]));
        analyzeOp(dynamic_cast<opNode *>(orNode->childs[1]));
        analyzeExpression(dynamic_cast<ExpressionNode *>(orNode->childs[2]));
    }

}

void semantic::analyzeAnd(AndNode *andNode) {
    if (andNode->childs.size() <= 1)
    {
        return analyzeEqual(dynamic_cast<EqualNode *>(andNode->childs[0]));
    }
    else
    {
        analyzeName(dynamic_cast<NameNode *>(andNode->childs[0]));
        analyzeOp(dynamic_cast<opNode *>(andNode->childs[1]));
        analyzeExpression(dynamic_cast<ExpressionNode *>(andNode->childs[2]));
    }
}

void semantic::analyzeEqual(EqualNode *equalNode) {
    if (equalNode->childs.size() <= 1)
    {
        return analyzeCompare(dynamic_cast<CompareNode *>(equalNode->childs[0]));
    }
    else
    {
        analyzeName(dynamic_cast<NameNode *>(equalNode->childs[0]));
        analyzeOp(dynamic_cast<opNode *>(equalNode->childs[1]));
        analyzeExpression(dynamic_cast<ExpressionNode *>(equalNode->childs[2]));
    }
}

void semantic::analyzeCompare(CompareNode *compareNode) {
    if (compareNode->childs.size() <= 1)
    {
        return analyzeAdd(dynamic_cast<AddNode *>(compareNode->childs[0]));
    }
    else
    {
        analyzeName(dynamic_cast<NameNode *>(compareNode->childs[0]));
        analyzeOp(dynamic_cast<opNode *>(compareNode->childs[1]));
        analyzeExpression(dynamic_cast<ExpressionNode *>(compareNode->childs[2]));
    }
}

void semantic::analyzeAdd(AddNode *addNode) {
    if (addNode->childs.size() <= 1)
    {
        return analyzeMul(dynamic_cast<MulNode *>(addNode->childs[0]));
    }
    else
    {
        analyzeName(dynamic_cast<NameNode *>(addNode->childs[0]));
        analyzeOp(dynamic_cast<opNode *>(addNode->childs[1]));
        analyzeExpression(dynamic_cast<ExpressionNode *>(addNode->childs[2]));
    }   
}

void semantic::analyzeMul(MulNode *mulNode) {
    if (mulNode->childs.size() <= 1)
    {
        return analyzeBasic(dynamic_cast<BasicNode *>(mulNode->childs[0]));
    }
    else
    {
        analyzeName(dynamic_cast<NameNode *>(mulNode->childs[0]));
        analyzeOp(dynamic_cast<opNode *>(mulNode->childs[1]));
        analyzeExpression(dynamic_cast<ExpressionNode *>(mulNode->childs[2]));
    }
}

//<基本表达式>    ::= <标识符>        (* 变量引用 *)
// | <整数常量>       (* 整数 *)
// | <浮点数常量>     (* 浮点数 *)
// | '(' <表达式> ')' (* 括号表达式 *)
// | <函数调用>       (* 函数调用 *)

void semantic::analyzeBasic(BasicNode *basic) {
    for (ASTNode* child : basic->childs) {
        if (NameNode* name = dynamic_cast<NameNode*>(child)) {
            analyzeName(name);
        } 
        else if (IntNode* intNode = dynamic_cast<IntNode*>(child)) {
            analyzeInt(intNode);
        } 
        else if (DoubleNode* doubleNode = dynamic_cast<DoubleNode*>(child)) {
            analyzeDouble(doubleNode);
        }
        else if (ExpressionNode *expr = dynamic_cast<ExpressionNode *>(child))
        {
            analyzeExpression(expr);
        }
        else if (UseFuncNode *funcCall = dynamic_cast<UseFuncNode *>(child))
        {
            analyzeUseFun(funcCall);
        }
    }
}

void semantic::analyzeUseFun(UseFuncNode *useNode) {
    quat q; //(fun _ _ add)
    q[0] = "fun";
    q[1] = q[2] = '_';
    q[3] = analyzeName(dynamic_cast<NameNode *>(useNode->childs[0]));
    analyzeFactPar(dynamic_cast<FactParNode*>(useNode->childs[1]));

    quats.push_back(q);
    return;
}

void semantic::analyzeFactPar(FactParNode *factPar) {
    quat q;
    string para;
    for (ASTNode *child : factPar->childs)
    {
        if (ExpressionNode* expr = dynamic_cast<ExpressionNode*>(child)) {
            para = analyzeExpression(expr);      //这里返回一个临时变量，临时变量存放的是表达式的计算结果
            q[0] = "para";
            q[1] = q[2] = '_';
            q[3] = para;
            quats.push_back(q);
        }
    }
    q[0] = "fe";
    q[1] = q[2] = q[3] = '_';
    quats.push_back(q);

    return;
}

string semantic::analyzeType(TypeNode *typeNode) {
    // Analyze type information
    return string(typeNode->t);
}

string semantic::analyzeName(NameNode *nameNode) {
    // Analyze name information
    return string(nameNode->t); 
}

//==============这里最后要修改为int===========
string semantic::analyzeInt(IntNode *intNode) {
    // Analyze integer information
    // return stoi(intNode->t);
    return string(intNode->t);
}

//==============这里最后要修改为double===========
string semantic::analyzeDouble(DoubleNode *doubleNode) { 
    // Analyze double information
    // return std::stod(doubleNode->t);    

    return string(doubleNode->t);
}

string semantic::analyzeOp(opNode *op) {
    // Analyze operator information
    return op->t;
}
