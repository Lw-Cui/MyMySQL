#include <iostream>
#include <string>
#include <mysql++.h>
#include <header.hpp>
using namespace std;
using namespace mysqlpp;

const char *DBHOST = "localhost";
const char *USER = "root";
const char *PASSWORD = "greatclw";
const char *DATABASE = "OrderDB";
const char *QUERY = "select Product.productNo, Product.productName, SUM(OrderDetail.quantity)\
    from Product, OrderDetail\
    where Product.productNo = OrderDetail.productNo\
    group by Product.productNo, Product.productName\
    having sum(OrderDetail.quantity) > 4;";
 
int main(int argc, const char *argv[]) {
	const string url{argc >= 2 ? argv[1]: DBHOST};
	const string user{argc >= 3 ? argv[2]: USER};
	const string password{argc >= 4 ? argv[3]: PASSWORD};
	const string database{argc >= 5 ? argv[4]: DATABASE};

    Connection conn(false);
    conn.connect(database.c_str(), url.c_str(), user.c_str(), password.c_str());
    QuerySQL(conn, QUERY);
    return 0;
}