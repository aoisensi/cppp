#include <iostream>
#include <string>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

int main(int argc, char *argv[]) {
	if(argc == 1) return 1;
	std::string s = argv[1];
	std::cout << s << std::endl;
	std::string::iterator first = s.begin(), last = s.end();
	bool success = qi::parse(
		first,
		last,
		qi::int_
	);
	if (success) { std::cout << "OK" << std::endl; }
	return 0;
}
