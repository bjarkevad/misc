#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "Expression.hpp"

/* Returns true on END, false on QUIT */
bool read_subs(std::map<char, double>& substitutions);

int main()
{
	std::map<char, double> substitutions;
	std::cout << "Expression:\n";
	Expression e(std::cin);
	
	std::cout << "Enter variables in the form name=number.\n"
		<< "Use END to end variable definitions, or QUIT to end the program.\n"
		<< "Default value is 1 and variables retain their value across \n"
		<< "evaluations unless redefined.\n";

	while ( true )
	{
		try {
			if ( read_subs(substitutions) == false )
				return 0;
		}
		catch (EndException& e)
		{
			return 0;
		}

		try {
			std::cout << "Result: " << e.evaluate(substitutions) << "\n\n";
			std::cout << "Redefine variables, followed by END (or QUIT):\n";
		}
		catch ( DivException& e )
		{
			std::cerr << "Attempted to divide by zero.\n";
		}
		catch ( ExprException& e )
		{
			std::cerr << e.what() << "\n";
			return 1;
		}
	}
	return 0;
}

bool read_subs(std::map<char, double>& substitutions)
{
	std::string line;
	while(true)
	{
		getline(std::cin, line);
		if ( line == "QUIT" )
			return false;
		else if ( line == "END" )
			return true;
		else if ( line.length() >= 3 )
		{
			// Expect lines of the form a = 3
			std::istringstream in(line);
			char var, tmp;
			double d;
			in >> var;
			if (isalpha(var) && in.good() )
			{
				in >> tmp;
				if ( tmp == '=' && in.good() )
				{
					in >> d;
					substitutions[var] = d;
					continue;
				}
				else std::cerr << "Invalid assignment\n";
			}
		}
		else std::cerr << "Invalid assignment\n";
		if ( std::cin.eof() )
			throw EndException();
	}
	return true;
}
