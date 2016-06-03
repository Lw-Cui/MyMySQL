#pragma once
#include <iostream>
#include <string>
#include <mysql++.h>
int QuerySQL(mysqlpp::Connection &conn, const std::string& Query_str);