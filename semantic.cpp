#include "semantic.h"
#include <algorithm>

quat::quat(const string& op, const string& arg1, const string& arg2, const string& result) {
    elements.push_back(op);
    elements.push_back(arg1);
    elements.push_back(arg2);
    elements.push_back(result);
}
quat::quat(vector<string> arr) {
    elements = vector<string>(arr);
}

string& quat::operator[](size_t index) {
    return elements[index];
}
//const string& quat::operator[](size_t index) const {
//    return elements[index];
//}

vector<quat> semantic::getQuats(){
    return quats;
}

semantic::semantic() :tempVarCount(0), root(new ASTNode) {}
semantic::~semantic() {}

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

void semantic::analyzeFunction(FunctionNode* func) {
    string type = analyzeType(dynamic_cast<TypeNode*>(func->childs[0]));
    string name = analyzeName(dynamic_cast<NameNode*>(func->childs[1]));
    quats.push_back({ "df", "fun", type, name });

    analyzeList(dynamic_cast<ListNode*>(func->childs[2]));
    analyzeBlock(dynamic_cast<BlockNode*>(func->childs[3]));

    quats.push_back({ "de", "_", "_", "_" }); // Function end
}

void semantic::analyzeBlock(BlockNode* block) {
    for (ASTNode* child : block->childs) {
        if (DeclareNode* declare = dynamic_cast<DeclareNode*>(child)) {
            analyzeDeclare(declare);
        }
        else if (StatementNode* stmt = dynamic_cast<StatementNode*>(child)) {
            analyzeStatement(stmt);
        }
    }
}

void semantic::analyzeStatement(StatementNode* stmt) {
    if (stmt->childs[0]->t.first == "block") {
        analyzeBlock(dynamic_cast<BlockNode*>(stmt->childs[0]));
    }
    else if (stmt->childs[0]->t.first == "if") {
        analyzeIf(dynamic_cast<IfNode*>(stmt->childs[0]));
    }
    else if (stmt->childs[0]->t.first == "while") {
        analyzeWhile(dynamic_cast<WhileNode*>(stmt->childs[0]));
    }
    else if (stmt->childs[0]->t.first == "return") {
        analyzeReturn(dynamic_cast<ReturnNode*>(stmt->childs[0]));
    }
    else if (stmt->childs[0]->t.first == "express") {
        analyzeExpress(dynamic_cast<ExpressNode*>(stmt->childs[0]));
    }
}

string semantic::analyzeExpress(ExpressNode* express) {
    return analyzeExpression(dynamic_cast<ExpressionNode*>(express->childs[0]));
}

string semantic::analyzeExpression(ExpressionNode* expr) {
    string tempVal;
    if (expr->childs.size() > 0)
        for (auto child : expr->childs) {
            tempVal = analyzeAssign(dynamic_cast<AssignNode*>(child));
            return tempVal;
        }
    //string tempVal = analyzeAssign(dynamic_cast<AssignNode*>(expr->childs[0])); // 存放表达式的计算结果
    else {
        return "NULL";
    }
}

string semantic::analyzeAssign(AssignNode* assign) {
    /*    if(assign->childs.size() <= 1 || dynamic_cast<OpNode*>(assign->childs[1])->t.second->getInfo() == "||")*/
    if (assign->childs[0]->t.first == "or")
    {
        return analyzeOr(dynamic_cast<OrNode*>(assign->childs[0]));
    }
    else {
        string dst = analyzeName(dynamic_cast<NameNode*>(assign->childs[0]));
        /*string op = analyzeOp(dynamic_cast<OpNode *>(assign->childs[1]));*/
        string src = analyzeAssign(dynamic_cast<AssignNode*>(assign->childs[1]));
        quats.push_back({ "=", src, "_", dst });
        return dst;
    }

}

void semantic::analyzeList(ListNode* list) {
    if (list != nullptr) {
        for (ASTNode* child : list->childs) {
            if (PararmeterNode* param = dynamic_cast<PararmeterNode*>(child)) {
                analyzeParam(param);
            }
        }
    }

    quats.push_back({ "pe", "_", "_", "_" }); // List end
}

string semantic::analyzeParam(PararmeterNode* param) {
    string type = analyzeType(dynamic_cast<TypeNode*>(param->childs[0]));
    string name = analyzeName(dynamic_cast<NameNode*>(param->childs[1]));
    quats.push_back({ "para", "_", type, name });
    return name;
}


string semantic::analyzeDeclare(DeclareNode* declare) {
    string type = analyzeType(dynamic_cast<TypeNode*>(declare->childs[0]));
    string name = analyzeName(dynamic_cast<NameNode*>(declare->childs[1]));
    quats.push_back({ "df", "idt", type, name });
    return name;
}

/*
*       quat(E)
    q1(if res(E)_ _ )
        quat(S1)
    q2(el  _  _  _  )
        quat(S2)
    q3(ie  _  _  _  )
*/
void semantic::analyzeIf(IfNode* ifNode) {
    //1.calc condition
    string condition = analyzeExpression(dynamic_cast<ExpressionNode*>(ifNode->childs[1]));

    //2.add (if _ _ _)
    quats.push_back({ "if", condition, "_", "_" });

    //3. if condition is true
    analyzeStatement(dynamic_cast<StatementNode*>(ifNode->childs[2]));

    //4.add (el _ _ _)
    if (ifNode->childs[3] != nullptr)
        quats.push_back({ "el", "_", "_", "_" });

    //5.else do
    if (StatementNode* elseStmt = dynamic_cast<StatementNode*>(ifNode->childs[4])) {
        analyzeStatement(elseStmt);
    }

    //6.add (ie _ _ _)
    quats.push_back({ "ie", "_", "_", "_" });
}

void semantic::analyzeWhile(WhileNode* whileNode) {
    //1.add (wh _ _ _)
    quats.push_back({ "wh", "_", "_", "_" });

    //2.calc condition
    string condition = analyzeExpression(dynamic_cast<ExpressionNode*>(whileNode->childs[1]));

    //3.add (do res(E)_ _ )
    quats.push_back({ "do", condition, "_", "_" });

    //4.while do
    analyzeStatement(dynamic_cast<StatementNode*>(whileNode->childs[2]));

    //5.add (we _ _ _)
    quats.push_back({ "we", "_", "_", "_" });
}

void semantic::analyzeReturn(ReturnNode* returnNode) {
    if (ExpressionNode* expr = dynamic_cast<ExpressionNode*>(returnNode->childs[1])) {
        string rst = analyzeExpression(expr);
        quats.push_back({ "rt", "_", "_", rst });
    }
}

string semantic::analyzeOr(OrNode* orNode) {
    if (orNode->childs.size() <= 1)
    {
        return analyzeAnd(dynamic_cast<AndNode*>(orNode->childs[0]));
    }
    else
    {
        string and1 = analyzeAnd(dynamic_cast<AndNode*>(orNode->childs[0]));
        string op = analyzeOp(dynamic_cast<OpNode*>(orNode->childs[1]));
        string and2 = analyzeAnd(dynamic_cast<AndNode*>(orNode->childs[2]));
        string tempVal = getNextTempVar();
        quats.push_back({ op, and1, and2, tempVal });
        return tempVal;
    }

}

string semantic::analyzeAnd(AndNode* andNode) {
    if (andNode->childs.size() <= 1)
    {
        return analyzeEqual(dynamic_cast<EqualNode*>(andNode->childs[0]));
    }
    else
    {
        string eq1 = analyzeEqual(dynamic_cast<EqualNode*>(andNode->childs[0]));
        string op = analyzeOp(dynamic_cast<OpNode*>(andNode->childs[1]));
        string eq2 = analyzeEqual(dynamic_cast<EqualNode*>(andNode->childs[2]));
        string tempVal = getNextTempVar();
        quats.push_back({ op, eq1, eq2, tempVal });
        return tempVal;
    }
}

string semantic::analyzeEqual(EqualNode* equalNode) {
    if (equalNode->childs.size() <= 1)
    {
        return analyzeCompare(dynamic_cast<CompareNode*>(equalNode->childs[0]));
    }
    else
    {
        string cmp1 = analyzeCompare(dynamic_cast<CompareNode*>(equalNode->childs[0]));
        string op = analyzeOp(dynamic_cast<OpNode*>(equalNode->childs[1]));
        string cmp2 = analyzeCompare(dynamic_cast<CompareNode*>(equalNode->childs[2]));
        string tempVal = getNextTempVar();
        quats.push_back({ op, cmp1, cmp2, tempVal });
        return tempVal;
    }
}

string semantic::analyzeCompare(CompareNode* compareNode) {
    if (compareNode->childs.size() <= 1)
    {
        return analyzeAdd(dynamic_cast<AddNode*>(compareNode->childs[0]));
    }
    else
    {
        string add1 = analyzeAdd(dynamic_cast<AddNode*>(compareNode->childs[0]));
        string op = analyzeOp(dynamic_cast<OpNode*>(compareNode->childs[1]));
        string add2 = analyzeAdd(dynamic_cast<AddNode*>(compareNode->childs[2]));
        string tempVal = getNextTempVar();
        quats.push_back({ op, add1, add2, tempVal });
        return tempVal;
    }
}

string semantic::analyzeAdd(AddNode* addNode) {
    if (addNode->childs.size() <= 1)
    {
        return analyzeMul(dynamic_cast<MulNode*>(addNode->childs[0]));
    }
    else
    {
        string mul1 = analyzeMul(dynamic_cast<MulNode*>(addNode->childs[0]));
        string op = analyzeOp(dynamic_cast<OpNode*>(addNode->childs[1]));
        string mul2 = analyzeMul(dynamic_cast<MulNode*>(addNode->childs[2]));
        string tempVal = getNextTempVar();
        quats.push_back({ op, mul1, mul2, tempVal });
        return tempVal;
    }
}

string semantic::analyzeMul(MulNode* mulNode) {
    if (mulNode->childs.size() <= 1)
    {
        return analyzeBasic(dynamic_cast<BasicNode*>(mulNode->childs[0]));
    }
    else
    {
        string basic1 = analyzeBasic(dynamic_cast<BasicNode*>(mulNode->childs[0]));
        string op = analyzeOp(dynamic_cast<OpNode*>(mulNode->childs[1]));
        string basic2 = analyzeBasic(dynamic_cast<BasicNode*>(mulNode->childs[2]));
        string tempVal = getNextTempVar();
        quats.push_back({ op, basic1, basic2, tempVal });

        return tempVal;
    }
}

//<基本表达式>    ::= <标识符>        (* 变量引用 *)
// | <整数常量>       (* 整数 *)
// | <浮点数常量>     (* 浮点数 *)
// | '(' <表达式> ')' (* 括号表达式 *)
// | <函数调用>       (* 函数调用 *)
string semantic::analyzeBasic(BasicNode* basic) {
    string result;
    for (ASTNode* child : basic->childs) {
        if (NameNode* name = dynamic_cast<NameNode*>(child)) {
            result = analyzeName(name);
        }
        else if (IntNode* intNode = dynamic_cast<IntNode*>(child)) {
            result = analyzeInt(intNode);
        }
        else if (DoubleNode* doubleNode = dynamic_cast<DoubleNode*>(child)) {
            result = analyzeDouble(doubleNode);
        }
        else if (ExpressionNode* expr = dynamic_cast<ExpressionNode*>(child))
        {
            result = analyzeExpression(expr);
        }
        else if (UseFuncNode* funcCall = dynamic_cast<UseFuncNode*>(child))
        {
            result = analyzeUseFun(funcCall);
        }
    }
    return result;
}

string semantic::analyzeUseFun(UseFuncNode* useNode) {
    quat q; //(fun _ _ add)
    q[0] = "fun";
    q[1] = analyzeName(dynamic_cast<NameNode*>(useNode->childs[0]));
    q[2] = '_';
    q[3] = getNextTempVar();
    quats.push_back(q);

    analyzeFactPar(dynamic_cast<FactParNode*>(useNode->childs[1]));

    return q[3];
}

void semantic::analyzeFactPar(FactParNode* factPar) {
    quat q;
    string para;
    for (ASTNode* child : factPar->childs)
    {
        if (ExpressionNode* expr = dynamic_cast<ExpressionNode*>(child)) {
            para = analyzeExpression(expr);      //这里返回一个临时变量，临时变量存放的是表达式的计算结果
            q[0] = "para";
            q[1] = q[2] = '_';
            q[3] = para;
            quats.push_back(q);
        }
    }
    quats.push_back({ "fe", "_", "_", "_" });

    return;
}

string semantic::analyzeType(TypeNode* typeNode) {
    // Analyze type information
    keyword_part* ptr = dynamic_cast<keyword_part*>(typeNode->t.second);
    return ptr->getInfo();
}

string semantic::analyzeName(NameNode* nameNode) {
    // Analyze name information
    synbl_part* ptr = dynamic_cast<synbl_part*>(nameNode->t.second);
    return ptr->getInfo();
}

//==============这里最后要修改为int===========
string semantic::analyzeInt(IntNode* intNode) {
    // Analyze integer information
    // return stoi(intNode->t);
    auto ptr = dynamic_cast<constant_part*>(intNode->t.second);
    return ptr->getInfo();
}

//==============这里最后要修改为double===========
string semantic::analyzeDouble(DoubleNode* doubleNode) {
    // Analyze double information
    // return std::stod(doubleNode->t);    

    auto ptr = dynamic_cast<constant_part*>(doubleNode->t.second);
    return ptr->getInfo();
}

string semantic::analyzeOp(OpNode* op) {
    // Analyze operator information
    auto ptr = dynamic_cast<delimeter_part*>(op->t.second);
    return ptr->getInfo();
}
