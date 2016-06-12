#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <parser.hpp>
using namespace std;
namespace {
	const char *MYQUERY = "(productNo, productName) @Product @OrderDetail\
		 [quantity == 6] | [quantity < 5] & [quantity > 3]";
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
	lex.GetNextTok();
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

inline std::shared_ptr<ExprAST> ParseSingleCondExpr(Lexer &lex) {
	std::shared_ptr<ExprAST> ptr;
	switch (lex.GetTok()) {
	case Lexer::Token::TokNumber:
	case Lexer::Token::TokIdentifier:
	case Lexer::Token::TokName:
		ptr = ParseSimpleCondExpr(lex); break;
	case Lexer::Token::TokEOF:
		ptr = nullptr; break;
	default:
		;//ptr = ParseSQL(lex); lex.GetNextTok(); break;
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
		//if (lex.GetNextTok() == Lexer::Token::TokEOF) return nullptr; syntax error.
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

inline std::shared_ptr<ExprAST> ParseSQL(Lexer &lex) {
	switch (lex.GetTok()) {
	case Lexer::Token::TokDb:
		return ParseDatabaseExpr(lex);
	case Lexer::Token::TokCond:
		return ParseColumnExpr(lex);
	case Lexer::Token::TokArgs:
		return ParseConditionExpr(lex);
	default:
		return LogError("unknown token when expecting an expression");
	}
}

int main() {
	Lexer lex{MYQUERY};
	/*
	Lexer::Token tok;
	cout << "Lexer:" << endl;
	while ((tok = lex.GetNextTok()) != Lexer::Token::TokEOF)
		if (tok == Lexer::Token::TokNumber) cout << lex.GetNumVal() << endl;
		else cout << lex.GetNameStr() << endl;
	*/
	cout << "Column:" << endl;
	auto ptr = ParseColumnExpr(lex);
	ptr->print(cout);
	cout << endl << "Database:" << endl;
	ptr = ParseDatabaseExpr(lex);
	ptr->print(cout);
	cout << endl << "Condition:" << endl;
	ptr = ParseConditionExpr(lex);
	ptr->print(cout);
	return 0;
}