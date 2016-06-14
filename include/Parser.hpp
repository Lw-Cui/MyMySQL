#pragma once
#include <iostream>

class Lexer {
public:
	enum Token {
		TokEOF = -1,
		TokName = -2,
		TokNumber = -3,
		TokIdentifier = -4,
		TokArgs = -5,
		TokCond = -6,
		TokDb = -7,
	};

	Lexer(const std::string &exp):expression{exp}, currentTok{TokEOF} {GetNextTok();}
	double GetNumVal() {return numVal;}
	std::string GetNameStr() {return nameStr;}
	Token GetTok() {return currentTok;}
	int GetTokPrecedence() {
		if (currentTok == TokIdentifier) {
			if (GetNameStr() == "&") return 2;
			else if (GetNameStr() == "|") return 1;
		}
		return -1;
	}
	Token GetNextTok() {
		int beg{expression.get()}; 
		if (beg == EOF) {
			currentTok = TokEOF;
		} else if (isalpha(beg)) {
			expression.unget(); nameStr.clear(); 
			while ((beg = expression.get()) && isalnum(beg))
				nameStr.push_back(beg);
			expression.unget(); currentTok = TokName;
		} else if (isdigit(beg)) {
			expression.unget();
			expression >> numVal; currentTok = TokNumber;
		} else if (isspace(beg)) {
			return GetNextTok();
		} else {
			switch (beg) {
			case '(':currentTok = TokArgs; break;
			case '@':currentTok = TokDb; break;
			case '[':currentTok = TokCond; break;
			default: currentTok = TokIdentifier; break;
			}
			nameStr.clear(); nameStr.push_back(beg);
			if (expression.get() == '=') nameStr.push_back('=');
			else expression.unget();
		}
		return currentTok;
	}

private:
	std::istringstream expression;
	std::string nameStr;
	double numVal;
	Token currentTok;
};

class ExprAST {
public:
	virtual ~ExprAST(){}
	virtual void print(std::ostream &out) = 0;
};

class NumberExprAST: public ExprAST {
public:
	NumberExprAST(double v):Val{v} {}
	virtual void print(std::ostream &out) override { out << Val << " ";}
private:
	double Val;
};

class NameExprAST: public ExprAST {
public:
	NameExprAST(const std::string &n):Vname{n}{}
	virtual void print(std::ostream &out) override {out << Vname << " ";}
private:
	std::string Vname;
};

class BinaryExprAST: public ExprAST {
public:
	BinaryExprAST(const std::string &op,
		std::shared_ptr<ExprAST> left, std::shared_ptr<ExprAST> right)
	:Op{op}, LHS{std::move(left)}, RHS{std::move(right)} {}
	virtual void print(std::ostream &out) override {
		std::cout << "\n\t("; LHS->print(out);
		out << " " << Op << " "; RHS->print(out); std::cout << ")";}
private:
	std::string Op;
	std::shared_ptr<ExprAST> LHS, RHS;
};

class ExpressionAST: public ExprAST {
public:
	ExpressionAST(std::vector<std::shared_ptr<ExprAST>> v):ptr{std::move(v)}{}
	virtual void print(std::ostream &out) override {
		std::cout << "["; for (auto &p: ptr)  p->print(out); std::cout << "]"; }
private:
	std::vector<std::shared_ptr<ExprAST>> ptr;
};