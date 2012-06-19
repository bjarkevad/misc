#ifndef EVAL_EXPRESSION_HPP
#define EVAL_EXPRESSION_HPP

#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <string>

/**
 * ExprException: Thrown when evaluation of an expression cannot proceed
 */
class ExprException
{
	public:
		ExprException(const std::string& what) : message(what) {};
		const std::string& what() const { return message; };
	private:
		std::string message;
};

/**
 * EndException: Thrown to indicate "end of expression"
 */
class EndException {};

/**
 * DivException: Thrown on attempted division by zero
 */
class DivException {};

/**
 * Expression: Parses and evaluates string expressions
 */
class Expression
{
	public:
		/**
		 * Constructs an expression by splitting into bracketed sub-expressions
		 */
		Expression(std::istream& in);
		
		/**
		 * evaluate: Evaluates expressions, performing variable substitutions
		 * and causing evaluation of subexpressions
		 */
		double evaluate(std::map<char, double>& substitutions);

		const std::string& getExpr() const
			{ return expr; }; // Return internal expression representation
	private:
		/**
		 * Read a token (number, variable or $ character indicating subexpression)
		 * Calls itself recursively to deal with unary '-' (negative numbers)
		 */
		double readToken(std::map<char, double>& substitutions, std::istream& in);

		void eatwhite(std::istream& in); // Remove whitespace from a stream

		std::string expr; // Internal representation of the (sub)expression

		std::list<Expression> subs; // Subexpression list
		std::list<Expression> local_subs; // Copy of subs used during evaluation
};

/**
 * NOTE: Subexpressions
 * Internally, subexpressions are represented with the $ symbol in the expr string.
 * Upon encountering a $ symbol, the evaluate() function (actually, readToken())
 * replaces it with the evaluated subexpression from the front of the local_subs
 * list. Each call to evaluate() replaces local_subs with a fresh copy from
 * subs.
 */

#endif

