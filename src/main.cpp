#define _CRT_SECURE_NO_WARNINGS
#pragma
#include <filesystem>
#include "semantic.h"
#include "TuXing.h"
#include "toAssembly.h"

int main() {
	std::filesystem::create_directories("outputs");

	Table table;
	Lexical_analysis lex;
	lex.input(table);
	Parser parser(lex.tokens, table);
	ASTNode* root = parser.Syntax_analyzer();
	ASTVisualizer visualizer;
	visualizer.visualize(root, "outputs/ast7.dot");

	semantic sem(root);
	auto& quats = sem.quats;

	freopen("outputs/quadruple.txt", "w", stdout);
	for (int i = 0; i < quats.size(); i++) {
		cout << i << ":\t" << "(\t" << quats[i][0] 
			<< "\t" << quats[i][1] << "\t" << quats[i][2] 
			<< "\t" << quats[i][3] << "\t)" << endl;
	}
	cout << "size of quats is : " << quats.size() << endl;

	
	freopen("outputs/assmbly.asm", "w", stdout);
	toAssembly to(quats, table);
	to.generateAssembly();
	for (auto& sentence : to.assembly_code) {
		cout << sentence << endl;
	}
	return 0;
}