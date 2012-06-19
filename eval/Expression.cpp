#include <cctype>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <sstream>

#include "Expression.hpp"

Expression::Expression(std::istream& in)
{
	char ch;
	while ( ! in.eof() )
	{
		in.get(ch);
		switch(ch)
		{
			case '(': // Start a new subexpression
				subs.push_back(Expression(in));
				expr += '$';
				break;
			case ')': // End the current expression
			case '\n': // End parsing
				return;
			default:
				expr += ch; // Build up expression representation
		}
	}
}

double Expression::evaluate(std::map<char, double>& substitutions)
{
	local_subs = subs; // Copy subexpressions

	// Parse the current expression left-to-right
	std::istringstream in(expr);
	char ch;
	double result = readToken(substitutions, in);
	try {
		do
		{
			// Expect an operator followed by a number, variable or subexpression
			eatwhite(in); // Skip whitespace
			in.get(ch);
			switch(ch)
			{
				case '+':
					result += readToken(substitutions, in);
					break;
				case '-':
					result -= readToken(substitutions, in);
					break;
				case '*':
					result *= readToken(substitutions, in);
					break;
				case '/':
				{
					double temp = readToken(substitutions, in);
					if ( temp ) // Guard against division by zero
						result /= temp;
					else throw DivException();
					break;
				}
				default:
					throw ExprException("Expected operator in expression: " + expr);
			}
		} while ( in.good() );
	}
	catch(EndException &e)
	{
		// Termination condition
	}
	return result;
}

double Expression::readToken(std::map<char, double>& substitutions, std::istream& in)
{
	char ch;
	eatwhite(in); // Skip whitespace

	in.get(ch);
	if ( isalpha(ch) )
		return substitutions[ch]; // Substitute a variable
	else if ( ch == '$' )
	{
		// Evaluate a subexpression
		Expression sub = local_subs.front();
		local_subs.pop_front();
		return sub.evaluate(substitutions);
	}
	else if ( isdigit(ch) || ch == '.' )
	{
		// Read a number
		in.putback(ch);
		double num;
		in >> num;
		return num;
	}
	else if ( ch == '-' )
	{
		// Negate a number, variable or subexpression
		return -1 * readToken(substitutions, in);
	}
	else if ( in.eof() )
		throw EndException(); // Signal end of expression
	else
		throw ExprException("Expected number or variable in expression " + expr);
}

void Expression::eatwhite(std::istream& in)
{
	char ch;
	do
		in.get(ch);
	while ( isspace(ch) );
	in.putback(ch); // Push the first non-whitespace character back onto input stream
}

