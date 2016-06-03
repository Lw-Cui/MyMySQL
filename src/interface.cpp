#include <iostream>
#include <regex>
#include <map>
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
	const char *QUERY = "select Product.productNo, Product.productName from Product, OrderDetail where Product.productNo = OrderDetail.productNo and OrderDetail.quantity > 3;";
}
 
int main(int argc, const char *argv[]) {
	const string url{argc >= 2 ? argv[1]: DBHOST};
	const string user{argc >= 3 ? argv[2]: USER};
	const string password{argc >= 4 ? argv[3]: PASSWORD};
	const string database{argc >= 5 ? argv[4]: DATABASE};

    Connection conn(false);
    conn.connect(database.c_str(), url.c_str(), user.c_str(), password.c_str());

    //auto v = Regex(MYQUERY, "\\(.*\\)");
    //for (auto &ite: v) cout << ite << endl;
	//string query_str; getline(cin, query_str);
    //QuerySQL(conn, query_str);
    return 0;
}