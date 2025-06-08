#define _CRT_SECURE_NO_WARNINGS
#include "tuXIng.h"
#include "semantic.h"

int main() {
	Table table;
	Lexical_analysis lex;
	lex.input(table);

	//for (auto token : lex.tokens)
	//{
	//	cout << "<" << token.second->getInfo() << ", " << token.first << ">" << endl;
	//}
	Parser parser(lex.tokens);
	ASTNode* root = parser.Syntax_analyzer();
	ASTVisualizer visualizer;
	visualizer.visualize(root, "ast2.dot");


	//ÓïÒå·ÖÎöÆ÷
	semantic sem(root);

	auto quats = sem.getQuats();

	/*freopen("quadruple.txt", "w", stdout);*/
	for (int i = 0; i < quats.size(); i++) {
		/*string q = to_string(i) + ":\t" + quats[i][0] + "\t" + quats[i][1] + "\t" + quats[i][2] + "\t" + quats[i][3] + "\n";*/
		cout << i << ":\t" << "(\t" << quats[i][0] << "\t" << quats[i][1] << "\t" << quats[i][2] << "\t" << quats[i][3] << "\t)" << endl;
	}
	cout << "size of quats is : " << quats.size() << endl;

	return 0;
}