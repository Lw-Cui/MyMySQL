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
	inline double GetNumVal() {return numVal;}
	inline std::string GetNameStr() {return nameStr;}
	inline Token GetTok() {return currentTok;}
	inline int GetTokPrecedence() {
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
			expression.putback(beg); nameStr.clear(); 
			while ((beg = expression.get()) && isalnum(beg))
				nameStr.push_back(beg);
			expression.putback(beg); currentTok = TokName;
		} else if (isdigit(beg)) {
			expression.putback(beg);
			expression >> numVal; currentTok = TokNumber;
		} else if (isspace(beg)) {
			return GetNextTok();
		} else {
			expression.putback(beg); nameStr.clear(); 
			while ((beg = expression.get()) && !isalnum(beg) && !isspace(beg) && beg != EOF)
				nameStr.push_back(beg);
			expression.putback(beg); 
			if (nameStr == "(") currentTok = TokArgs;
			else if (nameStr == "@") currentTok = TokDb;
			else if (nameStr == "]") currentTok = TokCond;
			else currentTok = TokIdentifier;
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
	virtual void print(std::ostream &out) override {out << Val << " ";}
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
		std::cout << "("; LHS->print(out);
		out << " " << Op << " "; RHS->print(out); std::cout << ")";}
private:
	std::string Op;
	std::shared_ptr<ExprAST> LHS, RHS;
};

class ExpressionAST: public ExprAST {
public:
	ExpressionAST(std::vector<std::shared_ptr<ExprAST>> v):ptr{std::move(v)}{}
	virtual void print(std::ostream &out) override {
		std::cout << "( "; for (auto &p: ptr)  p->print(out);
		std::cout << ")"; }
private:
	std::vector<std::shared_ptr<ExprAST>> ptr;
};

/*
class QueryExprAST: public ExprAST {
public:
	QueryExprAST(
		std::shared_ptr<ColumnExprAST> col,
		std::shared_ptr<DataBaseExprAST>d, 
		std::shared_ptr<ConditionExprAST> con)
	:Col{col}, Db{d}, Con{con}{}
private:
	std::shared_ptr<ColumnExprAST> Col;
	std::shared_ptr<DataBaseExprAST> Db;
	std::shared_ptr<ConditionExprAST> Con;
};
*/