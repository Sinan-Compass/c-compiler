#include "semantic.h"

void semantic::analyzeProgram() {
    for (ASTNode* child : root->childs) {
        if (FunctionNode* func = dynamic_cast<FunctionNode*>(child)) {
            analyzeFunction(func);
        }
    }
}

void semantic::analyzeFunction(FunctionNode *func) {
    analyzeType(dynamic_cast<TypeNode*>(func->childs[0]));
    analyzeName(dynamic_cast<NameNode*>(func->childs[1]));
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

void semantic::analyzeExpression(ExpressionNode *expr) {
    analyzeAssign(dynamic_cast<AssignNode*>(expr->childs[0]));
}

void semantic::analyzeAssign(AssignNode *assign) {
    analyzeName(dynamic_cast<NameNode*>(assign->childs[0]));
    analyzeOp(dynamic_cast<opNode*>(assign->childs[1]));
    analyzeExpression(dynamic_cast<ExpressionNode*>(assign->childs[2]));
}

void semantic::analyzeCall(UseFuncNode *call) {
    analyzeName(dynamic_cast<NameNode*>(call->childs[0]));
    analyzeFactPar(dynamic_cast<FactParNode*>(call->childs[1]));
}

void semantic::analyzeList(ListNode *list) {
    for (ASTNode* child : list->childs) {
        if (PararmeterNode* param = dynamic_cast<PararmeterNode*>(child)) {
            analyzeParam(param);
        }
    }
}

void semantic::analyzeParam(PararmeterNode *param) {
    analyzeType(dynamic_cast<TypeNode*>(param->childs[0]));
    analyzeName(dynamic_cast<NameNode*>(param->childs[1]));
}

void semantic::analyzeDeclare(DeclareNode *declare) {
    analyzeType(dynamic_cast<TypeNode*>(declare->childs[0]));
    analyzeName(dynamic_cast<NameNode*>(declare->childs[1]));
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
    analyzeAnd(dynamic_cast<AndNode*>(orNode->childs[0]));
}

void semantic::analyzeAnd(AndNode *andNode) {
    analyzeEqual(dynamic_cast<EqualNode*>(andNode->childs[0]));
}

void semantic::analyzeEqual(EqualNode *equalNode) {
    analyzeCompare(dynamic_cast<CompareNode*>(equalNode->childs[0]));
}

void semantic::analyzeCompare(CompareNode *compareNode) {
    analyzeAdd(dynamic_cast<AddNode*>(compareNode->childs[0]));
}

void semantic::analyzeAdd(AddNode *addNode) {
    analyzeMul(dynamic_cast<MulNode*>(addNode->childs[0]));
}

void semantic::analyzeMul(MulNode *mulNode) {
    for (ASTNode* child : mulNode->childs) {
        if (BasicNode* basic = dynamic_cast<BasicNode*>(child)) {
            analyzeBasic(basic);
        }
    }
}

void semantic::analyzeBasic(BasicNode *basic) {
    for (ASTNode* child : basic->childs) {
        if (NameNode* name = dynamic_cast<NameNode*>(child)) {
            analyzeName(name);
        } else if (IntNode* intNode = dynamic_cast<IntNode*>(child)) {
            analyzeInt(intNode);
        } else if (DoubleNode* doubleNode = dynamic_cast<DoubleNode*>(child)) {
            analyzeDouble(doubleNode);
        }
    }
}

void semantic::analyzeUseFun(UseFuncNode *useNode) {
    analyzeName(dynamic_cast<NameNode*>(useNode->childs[0]));
    analyzeFactPar(dynamic_cast<FactParNode*>(useNode->childs[1]));
}

void semantic::analyzeFactPar(FactParNode *factPar) {
    for (ASTNode* child : factPar->childs) {
        if (ExpressionNode* expr = dynamic_cast<ExpressionNode*>(child)) {
            analyzeExpression(expr);
        }
    }
}

void semantic::analyzeType(TypeNode *typeNode) {
    // Analyze type information
}

void semantic::analyzeName(NameNode *nameNode) {
    // Analyze name information
}

void semantic::analyzeInt(IntNode *intNode) {
    // Analyze integer information
}

void semantic::analyzeDouble(DoubleNode *doubleNode) { 
    // Analyze double information
}

void semantic::analyzeOp(opNode *op) {
    // Analyze operator information
}
