#pragma once
#include <iostream>
#include <regex>
#include <map>
#include <vector>
#include <string>
#include <mysql++.h>
int QuerySQL(mysqlpp::Connection &conn, 
	const std::string& Query_str);
std::vector<std::string> 
	Regex(const std::string &origin, const std::string &pattern);