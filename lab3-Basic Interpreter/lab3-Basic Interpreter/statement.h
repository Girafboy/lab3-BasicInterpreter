/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"

enum StatementType{REM,LET,PRINT,INPUT,END,GOTO,IFTHEN};
/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */


 /*
 * Class: REMSTATE
 * ------------------
 * This subclass represents a REM statement.
 */

class REMSTATE : public Statement {
public:

/*
 * Constructor: REMSTATE
 * Usage: REMSTATE *stmt = new REMSTATE();
 * ------------------------------------------------
 * The constructor initializes a new empty statement.
 */

	REMSTATE();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a REM statement.It does nothing.
 */

	virtual void execute(EvalState & state);
private:
};

/*
* Class: LETState
* ------------------
* This subclass represents a LET statement.
*/

class LETState : public Statement {
public:

/*
 * Constructor: LETState
 * Usage: LETState *stmt = new LETState(var,exp);
 * ------------------------------------------------
 * The constructor initializes a new LET statement using a varible
 * and an expression.
 */

	LETState(std::string var,Expression* exp);

/*
 * Prototypes for the virtual method
 * ----------------------------------
 * The method have the same prototypes as those in the Expression
 * base class and don't require additional documentation.
 */

	virtual ~LETState();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a LET statement.It update the state to realize
 * to assign value to the variable.
 */

	virtual void execute(EvalState & state);

private:
	string var;
	Expression* exp;
};

/*
* Class: PRINTState
* ------------------
* This subclass represents a PRINT statement.
*/

class PRINTState : public Statement {
public:

/*
 * Constructor: REMPRINTStateSTATE
 * Usage: PRINTState *stmt = new PRINTState(exp);
 * ------------------------------------------------
 * The constructor initializes a new PRINT statement and output
 * the value of expression to the console.
 */

	PRINTState(Expression* exp);

/*
 * Prototypes for the virtual method
 * ----------------------------------
 * The method have the same prototypes as those in the Expression
 * base class and don't require additional documentation.
 */

	virtual ~PRINTState();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a PRINT statement.It print the value of 
 * expression to the console.
 */

	virtual void execute(EvalState & state);

private:
	Expression* exp;
};

/*
* Class: INPUTState
* ------------------
* This subclass represents a INPUT statement.
*/

class INPUTState : public Statement {
public:

/*
 * Constructor: INPUTState
 * Usage: INPUTState *stmt = new INPUTState(var);
 * ------------------------------------------------
 * The constructor initializes a new INPUT statement and ask user
 * to give the value of varible with prompt " ? ".
 */

	INPUTState(std::string var);

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a INPUT statement.It ask user to input a value
 * to variable which must be an integer, if not, repeat output 
 * prompt " ? ".
 */

	virtual void execute(EvalState & state);

private:
	string var;
};

/*
* Class: ENDState
* ------------------
* This subclass represents a END statement.
*/

class ENDState : public Statement {
public:

/*
 * Constructor: ENDState
 * Usage: ENDState *stmt = new ENDState();
 * ------------------------------------------------
 * The constructor initializes a new END statement.
 */

	ENDState();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a END statement.It halt the program.
 */

	virtual void execute(EvalState & state);
private:
};

/*
* Class: GOTOState
* ------------------
* This subclass represents a GOTO statement.
*/

class GOTOState :public Statement {
public:

/*
 * Constructor: GOTOState
 * Usage: GOTOState *stmt = new GOTOState(lineNumber);
 * ------------------------------------------------
 * The constructor initializes a new GOTO statement with the goal lineNumber.
 */

	GOTOState(int lineNumber);

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a GOTO statement.It skip the program to the goal line.
 */

	virtual void execute(EvalState & state);

private:
	int lineNumber;
};

/*
* Class: IFTHENState
* ------------------
* This subclass represents a IF_THEN statement.
*/

class IFTHENState :public Statement {
public:

/*
 * Constructor: IFTHENState
 * Usage: IFTHENState *stmt = new IFTHENState(lhs,cmp,rhs,lineNumber);
 * ------------------------------------------------
 * The constructor initializes a new IF_THEN statement with condition
 * of lhs cmp rhs and goal lineNumber give the result by create a variable
 * named "THEN" in Evalstate.
 */

	IFTHENState(Expression *lhs,char cmp, Expression *rhs, int lineNumber);

/*
 * Prototypes for the virtual method
 * ----------------------------------
 * The method have the same prototypes as those in the Expression
 * base class and don't require additional documentation.
 */

	virtual ~IFTHENState();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a IF_THEN statement.It judge the condition whether
 * true or false and give the result by create a variable named "THEN"
 * in Evalstate.
 */

	virtual void execute(EvalState & state);

private:
	Expression * lhs, *rhs;
	char cmp;
	int lineNumber;
};

#endif
