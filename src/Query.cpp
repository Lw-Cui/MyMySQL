#include <iostream>
#include <string>
#include <mysql++.h>
using namespace std;
using namespace mysqlpp;

int QuerySQL(Connection &conn, const string& Query_str) {
    try {
        Query query{conn.query(Query_str)}; 
        StoreQueryResult res{query.store()};
        if (res) {
            cout << "We have:" << endl;
            for (auto it = res.begin(); it != res.end(); ++it) {
                Row row = *it;
                cout << '\t' << row[0] << '\t' << row[1] << endl;
	        }
	    }

    } catch (BadQuery er) { // handle any connection or
        // query errors that may come up
        cerr << "Error: " << er.what() << endl;
        return -1;
    } catch (const BadConversion& er) {
        // Handle bad conversions
        cerr << "Conversion error: " << er.what() << endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << endl;
        return -1;
    } catch (const Exception& er) {
        // Catch-all for any other MySQL++ exceptions
        cerr << "Error: " << er.what() << endl;
        return -1;
    }
    return 0;
}