#define _CRT_SECURE_NO_WARNINGS
#include "tuXIng.h"
#include "semantic.h"
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

	//语法分析
	Parser parser(lex.tokens);
	//获取语法树
	ASTNode* root = parser.Syntax_analyzer();
	//语法树可视化
	ASTVisualizer visualizer;
	visualizer.visualize(root, "ast2.dot");


	//语义分析
	semantic sem(root);
	//生成四元式
	vector<quat> quats = sem.getQuats();

	//四元式输出
	/*freopen("quadruple.txt", "w", stdout);*/
	//for (int i = 0; i < quats.size(); i++) {
	//	/*string q = to_string(i) + ":\t" + quats[i][0] + "\t" + quats[i][1] + "\t" + quats[i][2] + "\t" + quats[i][3] + "\n";*/
	//	cout << i << ":\t" << "(\t" << quats[i][0] << "\t" << quats[i][1] << "\t" << quats[i][2] << "\t" << quats[i][3] << "\t)" << endl;
	//}
	//cout << "size of quats is : " << quats.size() << endl;

	toAssembly	toasm(quats, table);


	return 0;
}