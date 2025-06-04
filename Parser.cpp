#include "Parser.h"

void Parser::next(){
	cur_t_index++;
	if (cur_t_index < t.size()) {
		cur_t = t[cur_t_index];
	}
}

// <程序> -> { <函数定义> }
ASTNode* Parser::programProduction(){
	ProgramNode* program = new ProgramNode();
	while (1) {
		if (cur_t == "int" || cur_t == "float" || cur_t == "void") {
			//即cur_t是否是 <函数定义> 的first集合终结符
			ASTNode* function = functionProduction();
			if (function == nullptr)return nullptr;
			program->childs.push_back(function);
		}
		else {
			break;
		}
	}
	return program;
}

// <函数定义> -> <类型说明符> '标识符' '('[<参数列表>] ')' < 块 >
ASTNode* Parser::functionProduction(){
	ASTNode* type = typeProduction();
	if (type == nullptr)return nullptr;

	if (cur_t != "标识符") {
		return nullptr; //错误
	}
	NameNode* name = new NameNode(cur_t);
	next();

	if (cur_t != "(") {
		return nullptr; //错误
	}
	next();

	ASTNode* list = nullptr;
	if (cur_t != ")") {
		list = listProduction();
		if (cur_t != ")" || list == nullptr) {
			return nullptr; // 错误
		}
		next();
	}
	else {
		next();
	}
	ASTNode* block = blockProduction();
	if (block == nullptr)return nullptr;
	return new FunctionNode(type, name, list, block);
}

// <类型说明符> -> "int" | "float" | "void"
ASTNode* Parser::typeProduction(){
	TypeNode* type = nullptr;
	if (cur_t == "int" || cur_t == "float" || cur_t == "void") {
		type = new TypeNode(cur_t);
		next();
	}
	return type;
}

// <参数列表> -> <参数> { ',' <参数> }
ASTNode* Parser::listProduction(){
	ListNode* list = nullptr;
	ASTNode* pararmeter = pararmeterProduction();
	if (pararmeter == nullptr)return nullptr;
	list = new ListNode(pararmeter);
	while(1) {
		if (cur_t == ",") {
			next();
		}
		else {
			break;
		}
		pararmeterProduction();
		if (pararmeter == nullptr)return nullptr;
		list->childs.push_back(pararmeter);
	}
	return list;
}

// <块> -> '{' { <变量声明> } { <语句> } '}'
ASTNode* Parser::blockProduction() {
	BlockNode* block = new BlockNode();
	if (cur_t != "{") {
		return nullptr; //错误
	}
	next();

	while (1) {
		if (cur_t == "int" || cur_t == "float" || cur_t == "void" ) {
			//即cur_t是否是 <变量声明> 的first集合终结符
			ASTNode* declare  = declareProduction();
			if (declare == nullptr) return nullptr;
			block->childs.push_back(declare);
		}
		else {
			break;
		}
	}

	while (1) {
		if (cur_t == "if" || cur_t == "while" || cur_t == "return" || cur_t == "{" || cur_t == "("
			|| cur_t == "标识符" || cur_t == "整数常量" || cur_t == "浮点数常量") {
			//即cur_t是否是 <语句> 的first集合终结符
			ASTNode* statement = statementProducation();
			if (statement == nullptr) return nullptr;
			block->childs.push_back(statement);
		}
		else {
			break;
		}
	}

	if (cur_t != "}") {
		return nullptr; //错误
	}
	next();
	return block;
}

// <参数> -> <类型说明符> <标识符>
ASTNode* Parser::pararmeterProduction(){
	ASTNode* type = typeProduction();
	if (type == nullptr)return nullptr;
	if (cur_t != "标识符") {
		return nullptr; //错误
	}
	NameNode* name = new NameNode(cur_t);
	next();
	return new PararmeterNode(type, name);
}

// <变量声明> -> <类型说明符> '标识符' ';'
ASTNode* Parser::declareProduction(){
	ASTNode* type = typeProduction();
	if (type == nullptr)return nullptr;
	if (cur_t != "标识符") {
		return nullptr; //错误
	}
	NameNode* name = new NameNode(cur_t);
	next();

	if (cur_t != ";") {
		return nullptr; //错误
	}
	next();
	return new DeclareNode(type, name);
}

// <语句> -> <表达式语句> | <块> | <if语句> | <while语句> | <return语句>
ASTNode* Parser::statementProducation(){
	if (cur_t == "return") {
		//即cur_t为 <return语句> 的first集合的终结符
		ASTNode* _return = returnProduction();
		if (_return == nullptr)return nullptr;
		return new StatementNode(_return);
	}
	else if (cur_t == "while") {
		//即cur_t为 <while语句> 的first集合的终结符
		ASTNode* _while = whileProduction();
		if (_while == nullptr)return nullptr;
		return new StatementNode(_while);
	}
	else if (cur_t == "if") {
		//即cur_t为 <if语句> 的first集合的终结符
		ASTNode* _if = ifProduction();
		if (_if == nullptr)return nullptr;
		return new StatementNode(_if);
	}
	else if (cur_t == "{") {
		//即cur_t为 <块> 的first集合的终结符
		ASTNode* block = blockProduction();
		if (block == nullptr)return nullptr;
		return new StatementNode(block);
	}
	else if (cur_t == "(" || cur_t == "标识符" || cur_t == "整数常量" || cur_t == "浮点数常量") {
		//即cur_t为 <表达式语句> 的first集合的终结符
		ASTNode* express = expressProduction();
		if (express == nullptr)return nullptr;
		return new StatementNode(express);
	}
	else {
		return nullptr; //错误
	}
}

// <表达式语句> -> <表达式> ';'
ASTNode* Parser::expressProduction(){
	ASTNode* expression = expressionProduction();
	if (cur_t != ";" || expression == nullptr) {
		return nullptr; // 错误
	}
	next();
	return new ExpressNode(expression);
}

// <if语句> -> "if" '(' <表达式> ')' <语句> [ "else"  <语句> ]
ASTNode* Parser::ifProduction(){
	if (cur_t != "if") {
		return nullptr; //错误
	}
	opNode* op1 = new opNode(cur_t);
	next();

	if (cur_t != "(") {
		return nullptr; //错误
	}
	next();

	ASTNode* condition = expressionProduction();
	if (condition == nullptr)return nullptr;

	if (cur_t != ")") {
		return nullptr; //错误
	}
	next();
	
	ASTNode* then = statementProducation();
	if (then == nullptr)return nullptr;

	ASTNode* els = nullptr;
	opNode* op2 = nullptr;
	if (cur_t == "else") {
		op2 = new opNode(cur_t);
		next();
		els = statementProducation();
		if (els == nullptr)return nullptr;
	}
	return new IfNode(op1, condition, then, op2, els);

}

// <while语句> -> "while" '(' <表达式> ')'  <语句>
ASTNode* Parser::whileProduction(){
	if (cur_t != "while") {
		return nullptr; //错误
	}
	opNode* op = new opNode(cur_t);
	next();
	
	if (cur_t != "(") {
		return nullptr; //错误
	}
	next();

	ASTNode* condition = expressionProduction();
	if (condition == nullptr)return nullptr;

	if (cur_t != ")") {
		return nullptr; //错误
	}
	next();
	
	ASTNode* then = statementProducation();
	if (then == nullptr)return nullptr;

	return new WhileNode(op, condition, then);
}

// <return语句> -> "return" [ <表达式> ] ';'
ASTNode* Parser::returnProduction(){
	if (cur_t != "return") {
		return nullptr; //错误
	}
	opNode* op = new opNode(cur_t);
	next();

	ASTNode* value = nullptr;
	if (cur_t == "(" || cur_t == "标识符" || cur_t == "整数常量" || cur_t == "浮点数常量") {
		//即cur_t为 <表达式> 的first集合的终结符
		value = expressionProduction();
		if (value == nullptr)return nullptr;
	}
	if (cur_t != ";") {
		return nullptr; //错误
	}
	next();
	return new ReturnNode(op,value);
}

// <表达式> -> <赋值表达式>
ASTNode* Parser::expressionProduction(){
	ASTNode* assign = assignProduction();
	if (assign == nullptr)return nullptr;
	return new ExpressionNode(assign);
}

// <赋值表达式> -> <逻辑或表达式> | < 标识符> '=' < 赋值表达式 > (*赋值是右结合*)
ASTNode* Parser::assignProduction(){
	AssignNode* assign = nullptr;
	if (cur_t == "标识符" && cur_t_index + 1 < t.size() && t[cur_t_index + 1] == "=") {
		//判断是  < 标识符> '=' < 赋值表达式 >
		NameNode* name = new NameNode(cur_t);
		next();
		if (cur_t != "=") {
			return nullptr;
		}
		next();

		ASTNode* ass = assignProduction();
		if (ass == nullptr)return nullptr;
		return new AssignNode(name, ass);
	}
	else {
		ASTNode* _or = orProduction();
		if (_or == nullptr)return nullptr;
		return new AssignNode(_or);
	}
}

// <逻辑或表达式> -> <逻辑与表达式>{ "||" < 逻辑与表达式 > }
ASTNode* Parser::orProduction(){
	ASTNode* _and = andProduction();
	if (_and == nullptr)return nullptr;
	OrNode* _or = new OrNode(_and);
	while (1) {
		if (cur_t == "||") {
			_or->childs.push_back(new opNode(cur_t));
			next();
			_and = andProduction();
			if (_and == nullptr)return nullptr;
			_or->childs.push_back(_and);
		}
		else {
			break;
		}
	}
	return _or;
}

// <逻辑与表达式> -> <相等表达式> { "&&" <相等表达式> }
ASTNode* Parser::andProduction(){
	ASTNode* equal = equalProduction();
	if (equal == nullptr)return nullptr;
	AndNode* And = new AndNode(equal);
	while (1) {
		if (cur_t == "&&") {
			And->childs.push_back(new opNode(cur_t));
			next();
			equal = equalProduction();
			if (equal == nullptr)return nullptr;
			And->childs.push_back(equal);
		}
		else {
			break;
		}
	}
	return And;
}

// <相等表达式> -> <关系表达式> { ("==" | "!=") <关系表达式> }
ASTNode* Parser::equalProduction(){
	ASTNode* compare = compareProduction();
	if (compare == nullptr)return nullptr;
	EqualNode* equal = new EqualNode(compare);
	while (1) {
		if (cur_t == "==" || cur_t == "!=") {
			equal->childs.push_back(new opNode(cur_t));
			next();
			compare = compareProduction();
			if (compare == nullptr)return nullptr;
			equal->childs.push_back(compare);
		}
		else {
			break;
		}
	}
	return equal;
}

// <关系表达式> -> <加法表达式> { ('<' | '>' | "<=" | ">=") <加法表达式> }
ASTNode* Parser::compareProduction(){
	ASTNode* add = addProduction();
	if (add == nullptr)return nullptr;
	CompareNode* compare = new CompareNode(add);
	while (1) {
		if (cur_t == "<"|| cur_t == ">" || cur_t == "<=" || cur_t == ">=") {
			compare->childs.push_back(new opNode(cur_t));
			next();
			add = addProduction();
			if (add == nullptr)return nullptr;
			compare->childs.push_back(add);
		}
		else {
			break;
		}
	}
	return compare;
}

// <加法表达式> -> <乘法表达式> { ('+' | '-') <乘法表达式> }
ASTNode* Parser::addProduction(){
	ASTNode* mul = mulProduction();
	if (mul == nullptr)return nullptr;
	AddNode* add = new AddNode(mul);
	while (1) {
		if (cur_t == "+"|| cur_t == "-" ) {
			add->childs.push_back(new opNode(cur_t));
			next();
			mul = mulProduction();
			if (mul == nullptr)return nullptr;
			add->childs.push_back(mul);
		}
		else {
			break;
		}
	}
	return add;
}

// <乘法表达式> -> <基本表达式> { ('*' | '/') <基本表达式> }
ASTNode* Parser::mulProduction(){
	ASTNode* basic = basicProdution();
	if (basic == nullptr)return nullptr;
	MulNode* mul = new MulNode(basic);
	while (1) {
		if (cur_t == "*" || cur_t == "/" ) {
			mul->childs.push_back(new opNode(cur_t));
			next();
			basic = basicProdution();
			if (basic == nullptr)return nullptr;
			mul->childs.push_back(basic);
		}
		else {
			break;
		}
	}
	return mul;
}

// <基本表达式> -> '标识符' | '整数常量' | '浮点数常量' | '(' < 表达式 > ')' | <函数调用>
ASTNode* Parser::basicProdution(){
	ASTNode* c = nullptr;
	if (cur_t == "标识符" && cur_t_index + 1 < t.size() && t[cur_t_index + 1] == "(") {
		//判断是  <函数调用>
		c = useFuncProduction();
	}
	else if (cur_t == "标识符") {
		c = new NameNode(cur_t);
		next();
	}
	else if (cur_t == "整数常量") {
		c = new IntNode(cur_t);
		next();
	}
	else if (cur_t == "浮点数常量") {
		c = new DoubleNode(cur_t);
		next();
	}
	else if (cur_t == "(") {
		next();
		c = expressionProduction();
		if (cur_t == ")") {
			next();
		}
		else {
			return nullptr; //错误
		}
	}
	else {
		return nullptr; //错误
	}
	if (c == nullptr)return nullptr;
	return new BasicNode(c);
}

// <函数调用> -> <标识符> '(' [ <实参列表> ] ')'
ASTNode* Parser::useFuncProduction(){
	if (cur_t != "标识符") {
		return nullptr; //错误
	}
	NameNode* name = new NameNode(cur_t);
	next();

	if (cur_t != "(") {
		return nullptr; //错误
	}
	next();
	
	ASTNode* factPar = nullptr;
	if (cur_t != ")") {
		factPar = factParProduction();
		if (cur_t != ")" || factPar == nullptr) {
			return nullptr; //错误
		}
		next();
	}
	else {
		next();
	}
	return new UseFuncNode(name, factPar);
}

ASTNode* Parser::factParProduction(){
	// <实参列表> -> <表达式> { ',' <表达式> }
	ASTNode* expression = expressionProduction();
	if (expression == nullptr)return nullptr;
	FactParNode* factPar = new FactParNode(expression);
	while (1) {
		if (cur_t == ",") {
			next();
		}
		else {
			break;
		}
		expression = expressionProduction();
		if (expression == nullptr)return nullptr;
		factPar->childs.push_back(expression);
	}
	return factPar;
}
