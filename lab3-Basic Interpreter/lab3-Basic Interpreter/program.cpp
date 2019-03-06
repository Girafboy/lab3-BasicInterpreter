/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {
   // Replace this stub with your own code
}

Program::~Program() {
   // Replace this stub with your own code
}

void Program::clear() {
	Sourceline.clear();
	ParsedStatement.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
	if (Sourceline.find(lineNumber)!=Sourceline.end())
		ParsedStatement.erase(lineNumber);
	Sourceline[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
	if (Sourceline.find(lineNumber)==Sourceline.end())
		return;
	Sourceline.erase(lineNumber);
	ParsedStatement.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
	if (Sourceline.find(lineNumber) != Sourceline.end())
		return Sourceline[lineNumber];
	return "";    
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
	if (Sourceline.find(lineNumber) == Sourceline.end())
		error("no line exist in program");
	ParsedStatement[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
	if (ParsedStatement.find(lineNumber)!=ParsedStatement.end())
		return ParsedStatement[lineNumber];
	return NULL;  
}

int Program::getFirstLineNumber() {
	if (Sourceline.empty())
		return -1;
	return Sourceline.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
	if ((--Sourceline.end())->first == lineNumber)
		return -1;
	return (++Sourceline.find(lineNumber))->first;
}
