#include <vector>
#include <string>

#include <header.hpp>

using namespace std;
using namespace mysqlpp;

namespace {
	const char *argument = "\\(.*\\)";
	const char *dbname = "@[\\w]*";
	const char *condition =  "\\[.*\\]";
}

std::vector<string> Regex(const string &origin, const string &pattern) {
	regex range{pattern}, word{string{"\\w+"}};
	std::vector<string> res;

	sregex_iterator ite{std::begin(origin), std::end(origin), range}, end;
	for_each (ite, end, [&](auto &item){
		string tmp{item.str()};
		sregex_iterator ite{std::begin(tmp), std::end(tmp), word}, end;
		for_each(ite, end, [&](auto &item){
			res.push_back(item.str());
		});
	});
	return std::move(res);
}