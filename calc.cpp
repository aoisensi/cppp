#include <iostream>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_utree.hpp>

namespace qi = boost::spirit::qi;
namespace spirit = boost::spirit;

template <typename Iterator>
struct Formula : public qi::grammar<Iterator, spirit::utree()> {
	Formula() : Formula::base_type(expr) {
		block = '(' >> expression >> ')';
		factor = qi::int_ | block;
		expression = factor >> *('+' >> factor);
		expr = expression >> qi::eoi;
	}

	qi::rule<Iterator, spirit::utree::list_type()> block;
	qi::rule<Iterator, spirit::utree()> factor;
	qi::rule<Iterator, spirit::utree()> expression;
	qi::rule<Iterator, spirit::utree()> expr;
};

int main(int argc, char* argv[]) {
	Formula<std::string::iterator> formula;
	for(int i=1; i<argc;i++) {
		std::string str = argv[i];
		std::cout << str << std::endl;
		std::string::iterator itr = str.begin();
		spirit::utree ut;
		bool success = qi::parse(itr, str.end(), formula, ut);
		if(success)
			std::cout << ut << std::endl;
		else
			std::cout << "NG" << std::endl;
	}
}
