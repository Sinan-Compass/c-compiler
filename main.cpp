#define _CRT_SECURE_NO_WARNINGS
#pragma
#include "semantic.h"
#include "TuXing.h"
#include "toAssembly.h"

int main() {
	Table table;
	Lexical_analysis lex;
	lex.input(table);
	Parser parser(lex.tokens, table);
	ASTNode* root = parser.Syntax_analyzer();
	//ASTVisualizer visualizer;
	//visualizer.visualize(root, "ast4.dot");

	semantic sem(root);
	auto& quats = sem.quats;



	for (int i = 0; i < quats.size(); i++) {
		cout << i << ":\t" << "(\t" << quats[i][0] 
			<< "\t" << quats[i][1] << "\t" << quats[i][2] 
			<< "\t" << quats[i][3] << "\t)" << endl;
	}
	cout << "size of quats is : " << quats.size() << endl;
	freopen("assmbly.asm", "w", stdout);
	toAssembly to(quats, table);
	to.generateAssembly();
	for (auto& sentence : to.assembly_code) {
		cout << sentence << endl;
	}
	return 0;
}