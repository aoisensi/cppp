#include <iostream>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

namespace qi = boost::spirit::qi;
namespace spirit = boost::spirit;

struct expr {
	template <typename T1, typename T2 = void>
	struct result { typedef void type; };

	expr(char op) : op(op) {}

	void operator()(spirit::utree& expr, spirit::utree const& rhs) const
	{
		spirit::utree lhs;
		lhs.swap(expr);
		expr.push_back(spirit::utf8_symbol_range_type(&op, &op+1));
		expr.push_back(lhs);
		expr.push_back(rhs);
	}

	char const op;
};

boost::phoenix::function<expr> const plus = expr('+');
boost::phoenix::function<expr> const minus = expr('-');
boost::phoenix::function<expr> const times = expr('*');
boost::phoenix::function<expr> const divide = expr('/');

struct negate_expr
{
	template <typename T1, typename T2 = void>
	struct result { typedef void type; };

	void operator()(spirit::utree& expr, spirit::utree const& rhs) const
	{
		char const op = '-';
		expr.clear();
		expr.push_back(spirit::utf8_symbol_range_type(&op, &op+1));
		expr.push_back(rhs);
	}
};

boost::phoenix::function<negate_expr> neg;

template <typename Iterator>
struct Formula : public qi::grammar<Iterator, spirit::utree()> {
	Formula() : Formula::base_type(expression) {

		expression = 
			term					[qi::_val = qi::_1]
			>> *(	('+' >> term	[plus(qi::_val, qi::_1)])
				|	('-' >> term	[minus(qi::_val, qi::_1)])
				)
			;

		term =
			factor					[qi::_val = qi::_1]
			>> *(	('*' >> factor	[times(qi::_val, qi::_1)])
				|	('/' >> factor	[divide(qi::_val, qi::_1)])
				)
			;

		factor =
			qi::int_				[qi::_val = qi::_1]
			|	'(' >> expression	[qi::_val = qi::_1] >> ')'
			|	('-' >> factor		[neg(qi::_val, qi::_1)])
			|	('+' >> factor		[qi::_val = qi::_1])
			;
	}

	qi::rule<Iterator, spirit::utree()> expression, term, factor;
};

int main() {
	Formula<std::string::iterator> formula;
	std::string str;
	while(std::cin >> str) {
		std::string::iterator itr = str.begin();
		spirit::utree ut;
		bool success = qi::parse(itr, str.end(), formula, ut);
		if(success)
			std::cout << ut << std::endl;
		else
			std::cout << "NG" << std::endl;
	}
}
