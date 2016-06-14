#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <Parser.hpp>
using namespace std;
namespace {
	//const char *MYQUERY = "(productNo, productName) @Product @OrderDetail [quantity == 6] | [quantity > 3] & [quantity < 5]";
	const char *MYQUERY = "(productNo, productName) @Product @OrderDetail \
		[(productNo) @Product [name = x] & [kind = Y]]\
		| [quantity > 3] & [quantity < 5]";

	//const char *TEST = "(a,b) @c @e [(x) @y [z]]| [y]";
}

inline std::shared_ptr<ExprAST> LogError(const std::string& error) {
	std::cerr << error << std::endl;
	return nullptr;
}

inline std::shared_ptr<ExprAST> ParseNameExpr(Lexer &lex) {
	auto v = std::make_shared<NameExprAST>(lex.GetNameStr());
	lex.GetNextTok();
	return std::move(v);
}

inline std::shared_ptr<ExprAST> ParseNumberExpr(Lexer &lex) {
	auto v = std::make_shared<NumberExprAST>(lex.GetNumVal());
	lex.GetNextTok();
	return std::move(v);
}

inline std::shared_ptr<ExprAST> ParseIdentifierExpr(Lexer &lex) {
	auto v = std::make_shared<NameExprAST>(lex.GetNameStr());
	lex.GetNextTok();
	return std::move(v);
}

inline std::shared_ptr<ExprAST> ParseDatabaseExpr(Lexer &lex) {
	std::vector<std::shared_ptr<ExprAST>> v;
	while (lex.GetTok() == Lexer::Token::TokDb) {
		lex.GetNextTok(); v.push_back(ParseNameExpr(lex));
	}
	return std::make_shared<ExpressionAST>(v);
}

inline std::shared_ptr<ExprAST> ParseColumnExpr(Lexer &lex) {
	std::vector<std::shared_ptr<ExprAST>> v;
	lex.GetNextTok(); 
	while ((lex.GetTok() == Lexer::Token::TokIdentifier && lex.GetNameStr() != ")")
		|| lex.GetTok() == Lexer::Token::TokName)
		if (lex.GetTok() == Lexer::Token::TokName)
			v.push_back(ParseNameExpr(lex));
		else
			lex.GetNextTok();
	lex.GetNextTok(); return std::make_shared<ExpressionAST>(v);
}

inline std::shared_ptr<ExprAST> ParseSimpleCondExpr(Lexer &lex) {
	std::vector<std::shared_ptr<ExprAST>> v;
	while (lex.GetNameStr() != "]")
		switch (lex.GetTok()) {
		case Lexer::Token::TokName:
			v.push_back(ParseNameExpr(lex)); break;
		case Lexer::Token::TokNumber:
			v.push_back(ParseNumberExpr(lex)); break;
		case Lexer::Token::TokIdentifier:
			v.push_back(ParseIdentifierExpr(lex)); break;
		default: break;
		}
	lex.GetNextTok();
	return std::make_shared<ExpressionAST>(v);
}

std::shared_ptr<ExprAST> ParseConditionExpr(Lexer &lex);

inline std::shared_ptr<ExprAST> ParseSQL(Lexer &lex) {
	std::vector<std::shared_ptr<ExprAST>> v;
	while (true)
		switch (lex.GetTok()) {
		case Lexer::Token::TokDb:
			v.push_back(ParseDatabaseExpr(lex)); break;
		case Lexer::Token::TokCond:
			v.push_back(ParseConditionExpr(lex)); break;
		case Lexer::Token::TokArgs:
			v.push_back(ParseColumnExpr(lex)); break;
		default:
			lex.GetNextTok(); return make_shared<ExpressionAST>(v);
		}
}

inline std::shared_ptr<ExprAST> ParseSingleCondExpr(Lexer &lex) {
	std::shared_ptr<ExprAST> ptr;
	switch (lex.GetNextTok()) {
	case Lexer::Token::TokNumber:
	case Lexer::Token::TokIdentifier:
	case Lexer::Token::TokName:
		ptr = ParseSimpleCondExpr(lex); break;
	case Lexer::Token::TokEOF:
		ptr = nullptr; break;
	default:
		ptr = ParseSQL(lex); break;
	}
	return std::move(ptr);
}

inline std::shared_ptr<ExprAST> ParseBinOpRHS
	(Lexer &lex, int ExprPrec, std::shared_ptr<ExprAST> LHS) {
	std::string op; std::shared_ptr<ExprAST> RHS;
	while (true) {
		auto tokPrec = lex.GetTokPrecedence();
		op = lex.GetNameStr();
		if (tokPrec < ExprPrec) return LHS; // parse stop.
		if (lex.GetNextTok() == Lexer::Token::TokEOF) return nullptr; //syntax error.
		RHS = ParseSingleCondExpr(lex);
		auto nextPrec = lex.GetTokPrecedence();
		if (nextPrec > tokPrec)
			RHS = ParseBinOpRHS(lex, ExprPrec + 1, RHS);
		LHS = make_shared<BinaryExprAST>(op, LHS, RHS);
	}
}

inline std::shared_ptr<ExprAST> ParseConditionExpr(Lexer &lex) {
	return ParseBinOpRHS(lex, 0, ParseSingleCondExpr(lex));
}

int main() {
	Lexer lex{MYQUERY};
	cout << "Origin:" << endl << MYQUERY << endl;

	/*
	Lexer::Token tok;
	cout << "Lexer:" << endl;
	while ((tok = lex.GetNextTok()) != Lexer::Token::TokEOF)
		if (tok == Lexer::Token::TokNumber) cout << lex.GetNumVal() << endl;
		else cout << lex.GetNameStr() << endl;
	*/
	 
	cout << "\nParse:" << endl;
	auto ptr = ParseSQL(lex);
	ptr->print(cout);
	cout << endl;
	
	return 0;
}