#include <header.hpp>
using namespace std;

const string Build(const std::vector<std::string> &arg,
	const std::vector<std::string> &db, const std::vector<std::string> &con) {
	return "Hello world";
}

const std::string BuildSelect(const std::vector<std::string> &arg,
	 const std::map<std::string, std::string> &index) {
	string select;
	for (int i = 0; i < arg.size(); i++) {
		if (i) select += str(boost::format(", %s.%s") % index.at(arg[i]) % arg[i]);
		else select += str(boost::format(" %s.%s") % index.at(arg[i]) % arg[i]);
	}
	return std::move(select);
}

const std::string BuildFrom(const std::vector<std::string> &db) {
	string from;
	for (int i = 0; i < db.size(); i++) {
		if (i) from += str(boost::format(", %s") % db[i]);
		else from += str(boost::format(" %s") % db[i]);
	}
	return std::move(from);
}

const std::string BuildWhere(const std::vector<std::string> &con, 
    const std::map<std::string, std::string> &index) {
	string where;
	for (int i = 0; i < con.size(); i++) {
		if (i) where += " and"; 
		if (index.count(con[i]))
			where += " " + str(boost::format(" %s.%s") % index.at(con[i]) % con[i]);
		else where += " " + con[i];
	}
	return std::move(where);
}

const std::string BuildDBconn(const std::vector<std::string> &db, 
	const std::map<std::string, std::set<std::string>> &dbcolmn) {
	string connect;
	for (int i = 0; i + 1 < db.size(); i++) {
		vector<std::string> commonPart;
		std::set_intersection (
			std::begin(dbcolmn.at(db[i])), std::end(dbcolmn.at(db[i])),
			std::begin(dbcolmn.at(db[i + 1])), std::end(dbcolmn.at(db[i + 1])),
			std::back_inserter(commonPart));
		if (i != 0) connect += ",";
		for (auto &s: commonPart)
			connect += str(boost::format(" %s.%s = %s.%s") % db[i] % s % db[i + 1] % s);
	}
	return std::move(connect);
}