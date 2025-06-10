#define _CRT_SECURE_NO_WARNINGS
#pragma
#include "semantic.h"
#include "TuXing.h"
#include "toAssembly.h"

int main() {
	//建立各种表
	Table table;
	//词法分析器
	Lexical_analysis lex;
	lex.input(table);

	//token检验
	//for (auto token : lex.tokens)
	//{
	//	cout << "<" << token.second->getInfo() << ", " << token.first << ">" << endl;
	//}

	//获取语法树
	Parser parser(lex.tokens, table);
	ASTNode* root = parser.Syntax_analyzer();
	//语法树可视化
	ASTVisualizer visualizer;
	visualizer.visualize(root, "ast3.dot");

	//语义分析
	semantic sem(root);
	//生成四元式
	vector<quat> quats = sem.getQuats();

	auto& quats = sem.quats;

	//for (int i = 0; i < quats.size(); i++) {
	//	cout << i << ":\t" << "(\t" << quats[i][0] 
	//		<< "\t" << quats[i][1] << "\t" << quats[i][2] 
	//		<< "\t" << quats[i][3] << "\t)" << endl;
	//}
	//cout << "size of quats is : " << quats.size() << endl;
	freopen("assmbly.asm", "w", stdout);
	toAssembly to(quats, table);
	to.generateAssembly();
	for (auto sentence : to.assembly_code) {
		cout << sentence << endl;
	}
	return 0;
}