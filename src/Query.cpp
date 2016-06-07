#include <iostream>
#include <sstream>
#include <string>

#include "boost/format.hpp"
#include <mysql++.h>

using namespace std;
using namespace mysqlpp;

namespace {
    const char *QUERY_COLUMNS = "select COLUMN_NAME from information_schema.COLUMNS\
        where table_name = '%s';";
    const char *QUERY_DB = "SELECT TABLE_NAME \
        FROM INFORMATION_SCHEMA.TABLES\
        WHERE TABLE_TYPE = 'BASE TABLE' AND TABLE_SCHEMA='%s';";
}

std::set<std::string> QuerySQL(Connection &conn, const string &Query_str, int width = 0) {
    std::set<std::string> v;
    Query query{conn.query(Query_str)}; 
    StoreQueryResult res{query.store()};
    if (res) 
        for (mysqlpp::Row &row: res) {
            stringstream res; res << std::setw(width) << std::left;
            for (auto &element: row) {
                string tmp; element.to_string(tmp); res << tmp;
            }
            v.insert(std::move(res.str()));
        }
    return std::move(v);
}

std::set<std::string> QueryColumn
    (mysqlpp::Connection &conn, const string &tablename) {
    string queryStr = str(boost::format(QUERY_COLUMNS) % tablename);
    return std::move(QuerySQL(conn, queryStr));
}

std::set<std::string> QueryTable
    (mysqlpp::Connection &conn, const string &dbname) {
    string queryStr = str(boost::format(QUERY_DB) % dbname);
    return std::move(QuerySQL(conn, queryStr));
}
