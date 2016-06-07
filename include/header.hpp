#pragma once
#include <iostream>
#include <set>
#include <regex>
#include <map>
#include <vector>
#include <string>
#include <map>
#include <mysql++.h>
#include "boost/format.hpp"
std::set<std::string> QuerySQL(mysqlpp::Connection &conn, 
	const std::string &Query_str, int width);
std::set<std::string> QueryColumn (mysqlpp::Connection &conn, const std::string &name);
std::set<std::string> QueryTable (mysqlpp::Connection &conn, const std::string &dbname);
std::vector<std::string> Argument(const std::string &origin);
std::vector<std::string> Dbname(const std::string &origin);
std::vector<std::string> Condition(const std::string &origin);
const std::string Build(const std::vector<std::string> &arg,
	 const std::vector<std::string> &db, const std::vector<std::string> &con);
const std::string BuildSelect(const std::vector<std::string> &arg,
	 const std::map<std::string, std::string> &index);
const std::string BuildFrom(const std::vector<std::string> &db);
const std::string BuildWhere(const std::vector<std::string> &con, 
    const std::map<std::string, std::string> &index);
const std::string BuildDBconn(const std::vector<std::string> &con, 
	const std::map<std::string, std::set<std::string>> &dbcolmn);
