#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <sstream>

#include <Parser.hpp>
#include <mysql++.h>
#include "boost/format.hpp"

using namespace std;
using namespace mysqlpp;

namespace {
	const char *MYQUERY = "(productNo, productName) @Product @OrderDetail \n\
		[(productNo) @Product [productPrice >= 10] & [productName != 'DRAM']]\n\
		| [quantity >= 3] & [quantity < 5]";
	
	const char *DBHOST = "localhost";
	const char *USER = "root";
	const char *PASSWORD = "greatclw";
	const char *DATABASE = "OrderDB";
	struct DbInfo {
	    std::map<std::string, std::string> getDBname;
	    std::map<std::string, std::set<std::string>> dbcolumn;
	};
}

inline std::shared_ptr<ExprAST> LogError(const std::string& error) {
	std::cerr << error << std::endl;
	return nullptr;
}

inline std::shared_ptr<ExprAST> ParseNameExpr(Lexer &lex, DbInfo &info) {
	std::shared_ptr<NameExprAST> v; auto str = lex.GetNameStr();
	if (info.getDBname.count(str))
		v = std::make_shared<NameExprAST>(info.getDBname[str] + "." + str);
	else
		v = std::make_shared<NameExprAST>(lex.GetNameStr());
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

std::string BuildDBconn(const std::vector<std::string> &db, 
	const std::map<std::string, std::set<std::string>> &dbcolumn) {
	std::string connect;
	for (size_t i = 0; i + 1 < db.size(); i++) {
		vector<std::string> commonPart;
		std::set_intersection (
			std::begin(dbcolumn.at(db[i])), std::end(dbcolumn.at(db[i])),
			std::begin(dbcolumn.at(db[i + 1])), std::end(dbcolumn.at(db[i + 1])),
			std::back_inserter(commonPart));
		if (i != 0) connect += ",";
		for (auto &s: commonPart)
			connect += str(boost::format(" %s.%s = %s.%s") % db[i] % s % db[i + 1] % s);
	}
	return std::move(connect);
}

inline std::shared_ptr<ExprAST> ParseDatabaseExpr(Lexer &lex, DbInfo &info) {
	std::vector<std::shared_ptr<ExprAST>> db;
	std::vector<std::string> v;
	while (lex.GetTok() == Lexer::Token::TokDb) {
		lex.GetNextTok(); v.push_back(lex.GetNameStr());
		db.push_back(ParseNameExpr(lex, info));
	}
	return std::make_shared<DataBaseAST>(db, BuildDBconn(v, info.dbcolumn));
}

inline std::shared_ptr<ExprAST> ParseColumnExpr(Lexer &lex, DbInfo &info) {
	std::vector<std::shared_ptr<ExprAST>> v;
	lex.GetNextTok(); 
	while ((lex.GetTok() == Lexer::Token::TokIdentifier && lex.GetNameStr() != ")")
		|| lex.GetTok() == Lexer::Token::TokName)
		if (lex.GetTok() == Lexer::Token::TokName)
			v.push_back(ParseNameExpr(lex, info));
		else
			lex.GetNextTok();
	lex.GetNextTok(); return std::make_shared<ColumnAST>(v);
}

inline std::shared_ptr<ExprAST> ParseSimpleCondExpr(Lexer &lex, DbInfo &info) {
	std::vector<std::shared_ptr<ExprAST>> v;
	while (lex.GetNameStr() != "]")
		switch (lex.GetTok()) {
		case Lexer::Token::TokName:
			v.push_back(ParseNameExpr(lex, info)); break;
		case Lexer::Token::TokNumber:
			v.push_back(ParseNumberExpr(lex)); break;
		case Lexer::Token::TokIdentifier:
			v.push_back(ParseIdentifierExpr(lex)); break;
		default: break;
		}
	lex.GetNextTok();
	return std::make_shared<SimpleConditionAST>(v);
}

std::shared_ptr<ExprAST> ParseConditionExpr(Lexer &lex, DbInfo &info);

inline std::shared_ptr<ExprAST> ParseSQL
	(Lexer &lex, DbInfo &info, bool isNest = false) {
	std::vector<std::shared_ptr<ExprAST>> v;
	while (true)
		switch (lex.GetTok()) {
		case Lexer::Token::TokDb:
			v.push_back(ParseDatabaseExpr(lex, info)); break;
		case Lexer::Token::TokCond:
			v.push_back(ParseConditionExpr(lex, info)); break;
		case Lexer::Token::TokArgs:
			v.push_back(ParseColumnExpr(lex, info)); break;
		default:
			lex.GetNextTok(); 
			if (!isNest) return make_shared<GroupExprAST>(v);
			else return make_shared<ExistsAST>(v);
		}
}

inline std::shared_ptr<ExprAST> ParseSingleCondExpr(Lexer &lex, DbInfo &info) {
	std::shared_ptr<ExprAST> ptr;
	switch (lex.GetNextTok()) {
	case Lexer::Token::TokNumber:
	case Lexer::Token::TokIdentifier:
	case Lexer::Token::TokName:
		ptr = ParseSimpleCondExpr(lex, info); break;
	case Lexer::Token::TokEOF:
		ptr = nullptr; break;
	default:
		ptr = ParseSQL(lex, info, 1); break;
	}
	return std::move(ptr);
}

inline std::shared_ptr<ExprAST> ParseBinOpRHS
	(Lexer &lex, int ExprPrec, std::shared_ptr<ExprAST> LHS, DbInfo &info) {
	std::string op; std::shared_ptr<ExprAST> RHS;
	while (true) {
		auto tokPrec = lex.GetTokPrecedence();
		op = lex.GetNameStr();
		if (tokPrec < ExprPrec) return LHS; // parse stop.
		if (lex.GetNextTok() == Lexer::Token::TokEOF) return nullptr; //syntax error.
		RHS = ParseSingleCondExpr(lex, info);
		auto nextPrec = lex.GetTokPrecedence();
		if (nextPrec > tokPrec)
			RHS = ParseBinOpRHS(lex, ExprPrec + 1, RHS, info);
		LHS = make_shared<BinaryExprAST>(op, LHS, RHS);
	}
}

inline std::shared_ptr<ExprAST> ParseConditionExpr(Lexer &lex, DbInfo &info) {
	return make_shared<ConditionAST>(
		ParseBinOpRHS(lex, 0, ParseSingleCondExpr(lex, info), info));
}

int main(int argc, const char *argv[]) {
	const string url{argc >= 2 ? argv[1]: DBHOST};
	const string user{argc >= 3 ? argv[2]: USER};
	const string password{argc >= 4 ? argv[3]: PASSWORD};
	const string database{argc >= 5 ? argv[4]: DATABASE};

    Connection conn(false);
    conn.connect(database.c_str(), url.c_str(), user.c_str(), password.c_str());
    std::map<std::string, std::string> getDBname;
    std::map<std::string, std::set<std::string>> dbcolumn;
    auto tables = QueryTable(conn, database);
    for (auto &tablename: tables) {
    	auto columns = QueryColumn(conn, tablename);
    	for (auto &colname: columns) getDBname[colname] = tablename;
    	dbcolumn[tablename] = std::move(columns);
    }
    DbInfo info;
    info.getDBname = std::move(getDBname);
    info.dbcolumn = std::move(dbcolumn);

	Lexer lex{MYQUERY};
	cout << "Origin:" << endl << MYQUERY << endl;

	/*
	Lexer::Token tok;
	cout << "Lexer:" << endl;
	while ((tok = lex.GetNextTok()) != Lexer::Token::TokEOF)
		if (tok == Lexer::Token::TokNumber) cout << lex.GetNumVal() << endl;
		else cout << lex.GetNameStr() << endl;
	*/
	 
	auto ptr = ParseSQL(lex, info);
	cout << "\nParse:\n" << *ptr << endl;
	return 0;
}