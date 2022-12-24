/*
 * parse.h
 */

#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>

using namespace std;

#include "lex.h"
#include "val.h"


static map<string, bool> defVar;
static map<string, Token> SymTable;
static map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constance 
static queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects


extern bool Prog(istream& in, int& line);
extern bool Stmt(istream& in, int& line);
extern bool Decl(istream& in, int& line);
extern bool PrintStmt(istream& in, int& line);
extern bool IfStmt(istream& in, int& line);
extern bool ReadStmt(istream& in, int& line);
extern bool IdList(istream& in, int& line, LexItem type);
extern bool VarList(istream& in, int& line);
extern bool Var(istream& in, int& line, LexItem & tok);
extern bool AssignStmt(istream& in, int& line);
extern bool ExprList(istream& in, int& line);
extern bool LogicExpr(istream& in, int& line, Value & retVal);
extern bool Expr(istream& in, int& line, Value & retVal);
extern bool Term(istream& in, int& line, Value & retVal);
extern bool SFactor(istream& in, int& line, Value & retVal);
extern bool Factor(istream& in, int& line, int sign, Value & retVal);
extern int ErrCount();

#endif /* PARSE_H_ */
