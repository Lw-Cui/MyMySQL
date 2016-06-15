#pragma once
#include <iostream>
#include <set>
#include <regex>
#include <map>
#include <vector>
#include <string>
#include <map>
#include <mysql++.h>
std::set<std::string> QuerySQL(mysqlpp::Connection &conn, 
	const std::string &Query_str, int width);
std::set<std::string> QueryColumn (mysqlpp::Connection &conn, const std::string &name);
std::set<std::string> QueryTable (mysqlpp::Connection &conn, const std::string &dbname);
std::vector<std::string> Argument(const std::string &origin);
std::vector<std::string> Dbname(const std::string &origin);
std::vector<std::string> Condition(const std::string &origin);