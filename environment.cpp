#include <map>

class Environment {
private:
	std::map<string, Expression> map;
	Environment & parent;
public:
	Expression & operator[](const std::string symbol)
}

Expression & Environment::operator[](const std::string symbol) {
	if (map.find(symbol) != map.end()) {
		return map[symbol]
	} else {
}

