#include <iostream>
#include <string>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct Formula : public qi::grammar<Iterator, qi::space_type> {
	Formula() : Formula::base_type(expr) {
		factor = qi::int_ | '(' >> expression >> ')';
		expression = factor >> *('+' >> factor);
		expr = expression >> qi::eoi;
	}

	qi::rule<Iterator, qi::space_type> factor;
	qi::rule<Iterator, qi::space_type> expression;
	qi::rule<Iterator, qi::space_type> expr;
};

int main(int argc, char *argv[]) {
	Formula<std::string::iterator> formula;
	for(int i=1; i<argc;i++) {
		std::string str = argv[i];
		std::cout << str << std::endl;
		std::string::iterator itrb = str.begin();
		std::string::iterator itre = str.end();
		if( qi::phrase_parse(itrb, itre, formula, qi::space))
			std::cout << "OK" << std::endl;
		else
			std::cout << "NG" << std::endl;
	}
}
