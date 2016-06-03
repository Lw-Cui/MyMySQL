#pragma once
#include <iostream>
#include <regex>
#include <map>
#include <vector>
#include <string>
#include <mysql++.h>
std::vector<std::string> QuerySQL(mysqlpp::Connection &conn, const std::string &Query_str);
std::vector<std::string> Argument(const std::string &origin);
std::vector<std::string> Dbname(const std::string &origin);
std::vector<std::string> Condition(const std::string &origin);
const std::string Build(const std::vector<std::string> &arg,
	 const std::vector<std::string> &db, const std::vector<std::string> &con);