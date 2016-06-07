#include <iostream>
#include <regex>
#include <map>
#include <set>
#include <vector>
#include <string>

#include <mysql++.h>
#include <header.hpp>

using namespace std;
using namespace mysqlpp;

namespace {
	const char *DBHOST = "localhost";
	const char *USER = "root";
	const char *PASSWORD = "greatclw";
	const char *DATABASE = "OrderDB";
	const char *MYQUERY = "(productNo, productName) @Product @OrderDetail [quantity > 3]";
	//const char *QUERY = "select Product.productNo, Product.productName from Product, OrderDetail where Product.productNo = OrderDetail.productNo and OrderDetail.quantity > 3;";
}
 
int main(int argc, const char *argv[]) {
	const string url{argc >= 2 ? argv[1]: DBHOST};
	const string user{argc >= 3 ? argv[2]: USER};
	const string password{argc >= 4 ? argv[3]: PASSWORD};
	const string database{argc >= 5 ? argv[4]: DATABASE};

    Connection conn(false);
    conn.connect(database.c_str(), url.c_str(), user.c_str(), password.c_str());
    std::map<std::string, std::string> index;
    std::map<std::string, std::set<std::string>> dbcolumn;
    auto tables = QueryTable(conn, database);
    for (auto &tablename: tables) {
    	auto columns = QueryColumn(conn, tablename);
    	for (auto &colname: columns) index[colname] = tablename;
    	dbcolumn[tablename] = std::move(columns);
    }
    cout << "ORIGIN:" << endl << MYQUERY << endl;

    std::string SQL = 
        "Select" + BuildSelect(Argument(MYQUERY), index) 
        + " From" + BuildFrom(Dbname(MYQUERY))
        + " Where" + BuildDBconn(Dbname(MYQUERY), dbcolumn)
        + " and" + BuildWhere(Condition(MYQUERY), index)
        + ";";
    cout << "SQL:" << endl << SQL << endl;
    
    auto res = QuerySQL(conn, SQL, 15);
    cout << "RESULT:" << endl; for (auto &s: res) cout << s << endl;
    return 0;
}