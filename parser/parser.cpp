#include <string>
#include <iostream>
#include <sstream>
using namespace std;
namespace {
	const char *MYQUERY = "(productNo, productName) @Product @OrderDetail [quantity > 3]";
}

class Lexer {
public:
	enum Token {
		TokEOF = -1,
		TokName = -2,
		TokNumber = -3,
		TokIdentifier = -4,
	};

	Lexer(const std::string &exp):expression{exp} {}
	inline double GetNumVal() {return numVal;}
	inline std::string GetNameStr() {return nameStr;}
	Token GetTok() {
		int beg{expression.get()}; 
		if (beg == EOF) {
			return TokEOF;
		} else if (isalpha(beg)) {
			expression.putback(beg); nameStr.clear(); 
			while ((beg = expression.get()) && isalnum(beg))
				nameStr.push_back(beg);
			expression.putback(beg); return TokName;
		} else if (isdigit(beg)) {
			expression.putback(beg);
			expression >> numVal; return TokNumber;
		} else if (isspace(beg)) {
			return GetTok();
		} else {
			expression.putback(beg); nameStr.clear(); 
			while ((beg = expression.get()) && !isalnum(beg) && !isspace(beg) && beg != EOF)
				nameStr.push_back(beg);
			expression.putback(beg); return TokIdentifier;
		}
	}

private:
	std::istringstream expression;
	std::string nameStr;
	double numVal;
};

int main() {
	Lexer lex{MYQUERY};
	Lexer::Token tok;
	while ((tok = lex.GetTok()) != Lexer::Token::TokEOF)
		if (tok == Lexer::Token::TokNumber) cout << lex.GetNumVal() << endl;
		else cout << lex.GetNameStr() << endl;
	return 0;
}