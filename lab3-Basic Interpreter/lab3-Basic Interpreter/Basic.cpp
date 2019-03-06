/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: Huquan Kang]
 * Section: [TODO: Yunchao Wang]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);
void run(Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   //cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);
   string test = scanner.nextToken();

   //command QUIT
   //--------------------------------------------------
   if (test == "QUIT")
	   exit(0);

   //command RUN
   //--------------------------------------------------
   if (test == "RUN") {
	   run(program, state);
	   return;
   }

   //command LIST
   //--------------------------------------------------
   if (test == "LIST") {
	   int begin = program.getFirstLineNumber();
	   while (begin != -1) {
		   cout << program.getSourceLine(begin) << endl;
		   begin = program.getNextLineNumber(begin);
	   }
	   return;
   }

   //command CLEAR
   //--------------------------------------------------
   if (test == "CLEAR") {
	   program.clear();
	   state.clear();
	   return;
   }

   //command HELP
   //--------------------------------------------------
   if (test == "HELP") {
	   cout << "REM \t This statement is used for comments." << endl
		   << "LET var = exp \t This statement is BASIC¡¯s assignment statement." << endl
		   << "PRINT exp \t This statement print the value of the expression on the console and then print a newline character." << endl
		   << "INPUT var \t This statement print a prompt consisting of the string ¡° ? ¡± and then to read in a value to be stored in the variable." << endl
		   << "END \t Marks the end of the program. Execution halts when this line is reached. Execution also stops if the program continues past the last numbered line." << endl
		   << "Besides, you can use command as follow:" << endl
		   << "GOTO n" << endl
		   << "IF exp cmp exp THEN n" << endl
		   << "RUN\nLIST\nCLEAR\nQUIT\nHELP" << endl
		   << "For example:" << endl
		   << "10 REM Program to simulate a countdown" << endl
		   << "20 LET T = 10" << endl
		   << "30 IF T < 0 THEN 70" << endl
		   << "40 PRINT T" << endl
		   << "50 LET T = T -1" << endl
		   << "60 GOTO 30" << endl
		   << "70 END" << endl;
	   return;
   }

   //Program with Line Number
   //--------------------------------------------------
   if (scanner.getTokenType(test) == NUMBER) {
	   if (!scanner.hasMoreTokens())
		   program.removeSourceLine(stoi(test));
	   else {
		   program.addSourceLine(stoi(test), line);
		   Statement *statement = parseState(scanner);
		   program.setParsedStatement(stoi(test), statement);
	   }
	   return;
   }

   //Executed directly
   //--------------------------------------------------
   scanner.saveToken(test);
   Statement *statement = parseState(scanner);
   statement->execute(state);
   delete statement;
}

/*
* Function: run
* Usage: run(program, state);
* -----------------------------------------
* Execute the run command, and it include the futher implementation
* of control statement of GOTO as well as IF_THEN
*/

void run(Program & program, EvalState & state) {
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	int begin = program.getFirstLineNumber();
	while (begin != -1) {
		Statement *stmt = program.getParsedStatement(begin);
		stmt->execute(state);
		begin = program.getNextLineNumber(begin);

		//command GOTO
		//--------------------------------------------------
		if (state.isDefined("GOTO")) {
			begin = state.getValue("GOTO");
			state.eraseValue("GOTO");
			if (program.getSourceLine(begin) == "") {
				cout << "LINE NUMBER ERROR" << endl;
				error("line number error");
			}
		}

		//command IF_THEN
		//---------------------------------------------------
		if (state.isDefined("THEN")) {
			begin = state.getValue("THEN");
			state.eraseValue("THEN");
			if (program.getSourceLine(begin) == "") {
				cout << "LINE NUMBER ERROR" << endl;
				error("line number error");
			}
		}
	}

}
