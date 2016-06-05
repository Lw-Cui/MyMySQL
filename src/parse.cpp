#include <vector>
#include <string>

#include <header.hpp>

using namespace std;
using namespace mysqlpp;

namespace {
	const char *argument = "\\(.*\\)";
	const char *dbname = "@[\\w]*";
	const char *condition =  "\\[.*\\]";
	const char *singleWord = "\\w+";
	std::vector<string> Regex(const string &origin, const string &pattern,
		const string &unit = singleWord) {
		regex range{pattern}, word{unit};
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
}

/*
    auto x = Argument("(a,b)[(ddfa(df, s))ads] (c)(d, e)[dsf()](f, g)");
    for (auto &str: x) cout << str << endl;
*/
std::vector<std::string> Argument(const std::string &origin) {
	std::vector<std::string> v; int lastpos = -1;
	for (size_t index = 0; index < origin.size(); index++)
		if (origin[index] == '[') {
			lastpos++;
			auto tmp = Regex(origin.substr(lastpos, index - lastpos), argument);
			copy(std::begin(tmp), std::end(tmp), std::back_inserter(v));
		} else if (origin[index] == ']') lastpos = index;

	lastpos++;
	auto tmp = Regex(origin.substr(lastpos, origin.size() - lastpos), argument);
	copy(std::begin(tmp), std::end(tmp), std::back_inserter(v));
	return std::move(v);
}

std::vector<std::string> Dbname(const std::string &origin) {
	return std::move(Regex(origin, dbname));
}

std::vector<std::string> Condition(const std::string &origin) {
	std::vector<std::string> v{Regex(origin, condition, "\\S+")};
	for (auto &s:v)
		if (s.front() == '[') s = s.substr(1, s.size() - 1);
		else if (s.back() == ']') s.pop_back();
	return std::move(v);
}

