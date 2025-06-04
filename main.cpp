#include "Parser.h"
#include "tuXIng.h"

int main() {
	//ÊäÈë
	vector<token> test = { 
		"int", "abs", "(", "int", "a", ")", "{", "if" ,"(", "a" ,
		">", "0",")","{", "return", "a", ";", "}", "else", 
		"{" , "return", "(", "0", "-" ,"a",")",";","}","}"
	};

	//Óï·¨·ÖÎöÆ÷
	Parser parser(test);
	ASTNode* root = parser.Syntax_analyzer();
	ASTVisualizer visualizer;
	visualizer.visualize(root, "ast.dot");
	return 0;
}