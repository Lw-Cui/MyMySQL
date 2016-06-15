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

	Lexer(const std::string &exp):expression{exp} {GetNextTok();}
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

inline std::ostream& operator<<(std::ostream &out, ExprAST &exp) {
	exp.print(out); return out;
}

class NumberExprAST: public ExprAST {
public:
	NumberExprAST(double v):Val{v} {}
	virtual void print(std::ostream &out) override { out << Val;}
private:
	double Val;
};

class NameExprAST: public ExprAST {
public:
	NameExprAST(const std::string &n):Vname{n}{}
	virtual void print(std::ostream &out) override {out << Vname;}
private:
	std::string Vname;
};

class BinaryExprAST: public ExprAST {
public:
	BinaryExprAST(const std::string &op,
		std::shared_ptr<ExprAST> left, std::shared_ptr<ExprAST> right)
	:Op{op}, LHS{std::move(left)}, RHS{std::move(right)} {}
	virtual void print(std::ostream &out) override {
		if (Op == "&") out << "(" << *LHS << " and " << *RHS << ")";
		else if (Op == "|") out << "(" << *LHS << " or " << *RHS << ")";
	}
private:
	std::string Op;
	std::shared_ptr<ExprAST> LHS, RHS;
};

class GroupExprAST: public ExprAST {
public:
	GroupExprAST(std::vector<std::shared_ptr<ExprAST>> v):ptr{std::move(v)}{}
	virtual void print(std::ostream &out) override {
		for (auto &p: ptr)  out << *p;}
protected:
	std::vector<std::shared_ptr<ExprAST>> ptr;
};

class ConditionAST: public ExprAST {
public:
	ConditionAST(std::shared_ptr<ExprAST> v):ptr{std::move(v)}{}
	virtual void print(std::ostream &out) override {
		out << " WHERE " << *ptr << "\n";
	}
private:
	std::shared_ptr<ExprAST> ptr;
};

class ExistsAST: public GroupExprAST {
public:
	ExistsAST(std::vector<std::shared_ptr<ExprAST>> v):GroupExprAST{std::move(v)}{}
	virtual void print(std::ostream &out) override {
		out << "EXISTS ("; GroupExprAST::print(out); out << " )";}
};

class DataBaseAST: public GroupExprAST {
public:
	DataBaseAST(std::vector<std::shared_ptr<ExprAST>> v):GroupExprAST{std::move(v)}{}
	virtual void print(std::ostream &out) override {
		out << " FROM ";
		for (size_t i = 0; i < ptr.size(); i++)
			if (i) out << ", " << *ptr[i]; else out << *ptr[i];
		out << "\n";
	}
};

class ColumnAST: public GroupExprAST {
public:
	ColumnAST(std::vector<std::shared_ptr<ExprAST>> v):GroupExprAST{std::move(v)}{}
	virtual void print(std::ostream &out) override {
		out << " SELECT ";
		for (size_t i = 0; i < ptr.size(); i++)
			if (i) out << ", " << *ptr[i]; else out << *ptr[i];
		out << "\n";
	}
};

class SimpleConditionAST: public GroupExprAST {
public:
	SimpleConditionAST(std::vector<std::shared_ptr<ExprAST>> v):GroupExprAST{std::move(v)}{}
	virtual void print(std::ostream &out) override {
		for(auto &p: ptr) out << " " << *p << "";
	}
};

