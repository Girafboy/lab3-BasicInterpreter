/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

/* Implementation of the REMSTATE class */

REMSTATE::REMSTATE()
{
	/* Empty */
}

void REMSTATE::execute(EvalState & state)
{
	/* Empty */
}

/*
* Implementation notes: the LETState subclass
* ------------------------------------------------
* The LETState subclass assigned value to a single instance variable 
* and it will check the name of variable to make sure them are not
* conflict to the reserved name.
*/

LETState::LETState(std::string var, Expression * exp)
{
	const string reserved = "REM LET PRINT INPUT END GOTO IF THEN RUN LIST CLEAR QUIT HELP";
	if (reserved.find(var) != string::npos) {
		cout << "SYNTAX ERROR" << endl;
		error("variable conflict to the reserved name");
	}
	this->exp = exp;
	this->var = var;
}

LETState::~LETState()
{
	delete exp;
}

void LETState::execute(EvalState & state)
{
	state.setValue(var, exp->eval(state));
}

/* Implementation of the PRINTState class */

PRINTState::PRINTState(Expression * exp)
{
	this->exp = exp;
}

PRINTState::~PRINTState()
{
	delete exp;
}

void PRINTState::execute(EvalState & state)
{
	cout << exp->eval(state) << endl;
}

/*
* Implementation notes: the INPUTState subclass
* ------------------------------------------------
* The INPUTState subclass ask user to input a value to variable 
* which must be an integer, if not, repeat output prompt " ? ". 
*/

INPUTState::INPUTState(std::string var)
{
	this->var = var;
}

void INPUTState::execute(EvalState & state)
{
	string value;
	const string digit = "0123456789-";
	while (true) {
		cout << " ? ";
		getline(cin, value);
		bool flag = 1;
		for (auto it = value.begin(); it != value.end(); it++)
			if (digit.find(*it) == string::npos) {
				cout << "INVALID NUMBER" << endl;
				flag = 0;
				break;
			}
		if (flag)	break;
	}
	state.setValue(var, stoi(value));
}

/* Implementation of the ENDState class */

ENDState::ENDState()
{
}

void ENDState::execute(EvalState & state)
{
	error("end");
}

/* Implementation of the GOTOState class */

GOTOState::GOTOState(int lineNumber)
{
	this->lineNumber = lineNumber;
}

void GOTOState::execute(EvalState & state)
{
	state.setValue("GOTO", lineNumber);
}

/*
* Implementation notes: the IFTHENState subclass
* ------------------------------------------------
* The IFTHENState subclass declares a representation of IF_THEN conditional
* control. It will tell whether condition is true or not and which line to go.
*/

IFTHENState::IFTHENState(Expression * lhs, char cmp, Expression * rhs, int lineNumber)
{
	this->lhs = lhs;
	this->cmp = cmp;
	this->rhs = rhs;
	this->lineNumber = lineNumber;
}

IFTHENState::~IFTHENState()
{
	delete lhs, rhs;
}

void IFTHENState::execute(EvalState & state)
{
	if ((cmp == '='&&lhs->eval(state) == rhs->eval(state))
		|| (cmp == '>'&&lhs->eval(state) > rhs->eval(state))
		|| (cmp == '<'&&lhs->eval(state) < rhs->eval(state)))
		state.setValue("THEN", lineNumber);
}

