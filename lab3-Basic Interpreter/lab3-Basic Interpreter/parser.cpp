/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp.h"
#include "parser.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner & scanner) {
   Expression *exp = readE(scanner);
   if (scanner.hasMoreTokens()) {
      error("parseExp: Found extra token: " + scanner.nextToken());
   }
   return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner & scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int newPrec = precedence(token);
      if (newPrec <= prec) break;
      Expression *rhs = readE(scanner, newPrec);
      exp = new CompoundExp(token, exp, rhs);
   }
   scanner.saveToken(token);
   return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner) {
   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   if (type == WORD) return new IdentifierExp(token);
   if (type == NUMBER) return new ConstantExp(stringToInteger(token));
   if (token != "(") error("Illegal term in expression");
   Expression *exp = readE(scanner);
   if (scanner.nextToken() != ")") {
      error("Unbalanced parentheses in expression");
   }
   return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
   if (token == "=") return 1;
   if (token == "+" || token == "-") return 2;
   if (token == "*" || token == "/") return 3;
   return 0;
}

/*
 * Implementation notes: parseState
 * --------------------------------
 * This code just reads an statement and parse it as different type.
 * If no statement be parsed, return NULL.
 */

Statement * parseState(TokenScanner & scanner)
{
	string test = scanner.nextToken();
	if (test == "REM" )
		return new REMSTATE();
	if (test == "LET") {
		string var = scanner.nextToken();
		if (scanner.nextToken() != "=")
			error("need = after variable");
		Expression *exp = parseExp(scanner);
		return new LETState(var, exp);
	}
	if (test == "PRINT")
		return new PRINTState(parseExp(scanner));
	if (test == "INPUT")
		return new INPUTState(scanner.nextToken());
	if (test == "END")
		return new ENDState();
	if (test == "GOTO")
		return new GOTOState(stoi(scanner.nextToken()));
	if (test == "IF") {
		Expression *exp1 = readE(scanner, 1);
		char cmp = scanner.nextToken()[0];
		Expression *exp2 = readE(scanner);
		if (scanner.nextToken() != "THEN")
			error("IF_THEN statement is illegal");
		return new IFTHENState(exp1,cmp,exp2,stoi(scanner.nextToken()));
	}
	scanner.saveToken(test);
	return NULL;
}

