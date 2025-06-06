//#include "Parser.h"
#include "tuXIng.h"
#include "semantic.h"

int main() {
	//输入
	vector<token> test = { 
		"int", "标识符", "(", "int", "标识符", ")", "{", "if" ,"(", "标识符" ,
		">", "整数常量",")","{", "return", "标识符", ";", "}", "else", 
		"{" , "return", "(", "整数常量", "-" ,"标识符",")",";","}","}"
	};

	//语法分析器
	Parser parser(test);
	ASTNode* root = parser.Syntax_analyzer();
	ASTVisualizer visualizer;
	visualizer.visualize(root, "ast.dot");

	//语义分析器
	semantic sem(root);

	auto quats = sem.getQuats();
	for(quat q : quats) {
		cout << q[0] << " " << q[1] << " " << q[2] << " " << q[3] << endl;
	}

	cout << "size of quats is : " << quats.size() << endl;

	//vector<quat> qt;
	//qt.push_back({ "begin", "_", "_", "_" });
	//
	//for (quat q : qt) {
	//	cout << q[0] << " " << q[1] << " " << q[2] << " " << q[3] << endl;
	//}

	return 0;
}