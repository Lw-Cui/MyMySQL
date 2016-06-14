#pragma once
#include <mysql++.h>
#include <Query.hpp>
#include "boost/format.hpp"
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