#include <iostream>
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

std::set<std::string> QuerySQL(Connection &conn, const string &Query_str) {
    std::set<std::string> v;
    Query query{conn.query(Query_str)}; 
    StoreQueryResult res{query.store()};
    if (res) for (auto &r: res) {
        string tmp;r[0].to_string(tmp);
        v.insert(std::move(tmp));
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
