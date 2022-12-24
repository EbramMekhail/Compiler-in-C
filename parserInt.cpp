/* Implementation of Recursive-Descent Parser
 * parse.cpp
*/

#include "parserInt.h"

using namespace std;

static int error_count = 0;

namespace Parser
{
	bool pushed_back = false;
	LexItem pushed_token;

	static LexItem GetNextToken(istream &in, int &line)
	{
		if (pushed_back)
		{
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem &t)
	{
		if (pushed_back)
		{
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

int ErrCount()
{
	return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream &in, int &line)
{
	bool dl = false, sl = false;
	LexItem tok = Parser::GetNextToken(in, line);
	//cout << "in Prog" << endl;

	if (tok.GetToken() == PROGRAM)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT)
		{
			string programName = tok.GetLexeme();
			dl = Decl(in, line);
			if (!dl)
			{
				ParseError(line, "Incorrect Declaration in Program");
				return false;
			}
			sl = Stmt(in, line);
			if (!sl)
			{
				ParseError(line, "Incorrect Statement in program");
				return false;
			}
			tok = Parser::GetNextToken(in, line);

			if (tok.GetToken() == END)
			{
				tok = Parser::GetNextToken(in, line);

				if (tok.GetToken() == PROGRAM)
				{
					tok = Parser::GetNextToken(in, line);

					if (tok.GetToken() == IDENT && tok.GetLexeme() == programName)
					{
						return true;
					}
					else
					{
						ParseError(line, "Missing Program Name");
						return false;
					}
				}
				else
				{
					ParseError(line, "Missing PROGRAM at the End");
					return false;
				}
			}
			else
			{
				ParseError(line, "Missing END of Program");
				return false;
			}
		}
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}

//Decl = Type : VarList
//Type = INTEGER | REAL | CHAR
bool Decl(istream &in, int &line)
{
	bool status = false;
	LexItem tok;
	//cout << "in Decl" << endl;
	LexItem t = Parser::GetNextToken(in, line);

	if (t == INTEGER || t == REAL || t == CHAR)
	{
		tok = t;
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == COLON)
		{
			status = IdList(in, line, t);
			//cout<< tok.GetLexeme() << " " << (status? 1: 0) << endl;
			if (status)
			{
				status = Decl(in, line);
				return status;
			}
		}
		else
		{
			ParseError(line, "Missing Colon");
			return false;
		}
	}

	Parser::PushBackToken(t);
	return true;
}

//Stmt is either a PrintStmt, ReadStmt, IfStmt, or an AssigStmt
//Stmt = AssigStmt | IfStmt | PrintStmt | ReadStmt
bool Stmt(istream &in, int &line)
{
	bool status;
	//cout << "in Stmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);

	switch (t.GetToken())
	{

	case PRINT:
		status = PrintStmt(in, line);
		//cout << "status: " << (status? true:false) <<endl;
		if (status)
			status = Stmt(in, line);
		break;

	case IF:
		status = IfStmt(in, line);
		if (status)
			status = Stmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if (status)
			status = Stmt(in, line);
		break;

	case READ:
		status = ReadStmt(in, line);
		//cout << "status: " << (status? true:false) <<endl;
		if (status)
			status = Stmt(in, line);
		break;

	default:
		Parser::PushBackToken(t);
		return true;
	}

	return status;
}

//PrintStmt:= print, ExpreList
bool PrintStmt(istream& in, int& line) { 
    //cout << "IN PRINTSTMT" << endl;
    LexItem t;
    ValQue = new queue<Value>;
    if( (t=Parser::GetNextToken(in,line)) != COMA){
        ParseError(line,"Missing a Comma");
        return false;
    }

    bool ex=ExprList(in,line);
    
    if(!ex){
        ParseError(line,"Missing expression after print");
        while( !(*ValQue).empty() ){
            ValQue->pop();
        }
        delete ValQue;
        return false;
    }

    //Evaluate:print out the list of expressions' values
    while( !(*ValQue).empty() ){
        Value nextVal=(*ValQue).front();
        cout << nextVal;ValQue->pop();
    }
    cout<<endl;
    return ex;
}// PrintStmt

//IfStmt:= if (Expr) then {Stmt} END IF
bool IfStmt(istream &in, int &line)
{
	bool ex = false;
	LexItem t;
	//cout << "in IfStmt" << endl;
	if ((t = Parser::GetNextToken(in, line)) != LPAREN)
	{

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	Value val;
	ex = LogicExpr(in, line, val);
	if (!ex)
	{
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}

	if ((t = Parser::GetNextToken(in, line)) != RPAREN)
	{

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	if ((t = Parser::GetNextToken(in, line)) != THEN)
	{

		ParseError(line, "Missing THEN");
		return false;
	}

	if(!val.GetBool()) 
	{
		LexItem tok = Parser::GetNextToken(in, line);
		while(tok != IF) {
			tok = Parser::GetNextToken(in, line);
		}
		return true;
	}

	bool st = Stmt(in, line);
	if (!st)
	{
		ParseError(line, "Missing statement for IF");
		return false;
	}

	//Evaluate: execute the if statement

	if ((t = Parser::GetNextToken(in, line)) != END)
	{

		ParseError(line, "Missing END of IF");
		return false;
	}
	if ((t = Parser::GetNextToken(in, line)) != IF)
	{

		ParseError(line, "Missing IF at End of IF statement");
		return false;
	}

	return true;
}


bool ReadStmt(istream &in, int &line)
{
	//bool ex = false ;
	LexItem t;
	//cout << "in ReadStmt" << endl;
	if ((t = Parser::GetNextToken(in, line)) != COMA)
	{

		ParseError(line, "Missing a Comma");
		return false;
	}

	bool ex = VarList(in, line);

	if (!ex)
	{
		ParseError(line, "Missing Variable after Read Statement");
		return false;
	}

	//Evaluate: print out the list of expressions values

	return ex;
}
//IdList:= IDENT {,IDENT}
bool IdList(istream& in, int& line, LexItem tok){

    LexItem ntok = Parser::GetNextToken(in, line);
    LexItem toka;
    if(ntok == IDENT) {
        if(defVar.find(ntok.GetLexeme()) == defVar.end()){
            
            defVar[ntok.GetLexeme()] = true;
            SymTable[ntok.GetLexeme()] = tok.GetToken();
        } else {
            ParseError(line, "Variable Redefinition");
            return false;
        }
        toka= Parser::GetNextToken(in, line);
        if(toka == COMA)
            return IdList(in, line, tok);
    } else {
        ParseError(line, "Missing Variable");
        return false;
    } 
    Parser::PushBackToken(toka);
    return true;
}

//VarList
bool VarList(istream &in, int &line)
{
	bool status = false;
	string identstr;
	//cout << "in VarList" << endl;
	LexItem tok;
	status = Var(in, line, tok);

	if (!status)
	{
		ParseError(line, "Missing Variable");
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok == COMA)
	{
		status = VarList(in, line);
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
} 

//Var:= ident
bool Var(istream &in, int &line, LexItem & tok)
{
	//called only from the AssignStmt function
	string identstr;
	//cout << "in Var" << endl;
	tok = Parser::GetNextToken(in, line);

	if (tok == IDENT)
	{
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}
		return true;
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}

//AssignStmt:= Var = Expr
bool AssignStmt(istream &in, int &line)
{
	//cout << "in AssignStmt" << endl;
	bool varstatus = false, status = false;
	LexItem t;
	
	
	varstatus = Var(in, line, t);
	LexItem dup = t;
	//cout << "varstatus:" << varstatus << endl;

	if (varstatus)
	{
		t = Parser::GetNextToken(in, line);
		//cout << t.GetLexeme() << endl;
		if (t == ASSOP)
		{
			Value val;
			status = Expr(in, line, val);
			if(SymTable[dup.GetLexeme()] == INTEGER && val.GetType() == VREAL) {
				float num = val.GetReal();
				val.SetType(VINT);
				val.SetInt((int)num);
			}

			TempsResults.insert({ dup.GetLexeme(), val});
			if (!status)
			{
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
		}
		else if (t.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else
		{
			ParseError(line, "Missing Assignment Operator =");
			return false;
		}
	}
	else
	{
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;
}

//ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line)
{
	bool status = false;
	//cout << "in ExprList" << endl;
	Value val;
	status = Expr(in, line, val);
	ValQue->push(val);

	if (!status)
	{
		ParseError(line, "Missing Expression");
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMA)
	{
		status = ExprList(in, line);
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Expr:= Term {(+|-) Term}
bool Expr(istream &in, int &line, Value &retVal)
{
	//cout << "in Expr" << endl;
	Value val1, val2;
	bool t1 = Term(in, line, val1);
	LexItem tok;

	if (!t1)
	{
		return false;
	}

	retVal = val1;

	tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	while (tok == PLUS || tok == MINUS)
	{
		t1 = Term(in, line, val2);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (retVal.GetType() == VCHAR || val2.GetType() == VCHAR)
		{
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands");
			return false;
		}
		else
		{
			if (tok == PLUS)
			{
				retVal = retVal + val2;
			}
			else if (tok == MINUS)
			{
				retVal = retVal - val2;
			}
		}
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

		//Evaluate: evaluate the expression for addition or subtraction
	}

	Parser::PushBackToken(tok);
	return true;
} //EXPR

//Term:= SFactor {(*|/) SFactor}
bool Term(istream &in, int &line, Value &retVal)
{
	//cout << "in Term << endl;
	Value val1, val2;
	bool t1 = SFactor(in, line, val1);
	LexItem tok;

	if (!t1)
	{
		return false;
	}

	retVal = val1;

	tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	while (tok == MULT || tok == DIV)
	{
		t1 = SFactor(in, line, val2);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (retVal.GetType() == VCHAR || val2.GetType() == VCHAR)
		{
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands");
			return false;
		}
		else
		{
			if (tok == MULT)
			{
				retVal = retVal * val2;
			}
			else if (tok == DIV)
			{
				if(val2.IsInt() && val2.GetInt() == 0) {
					ParseError (line, "Run-Time Error-Illegal Division by Zero");
					return false;
				}
				if(val2.IsReal() && val2.GetReal() == 0.0) {
					ParseError(line, "Run-Time Error-Illegal Division by Zero");
					return false;
				}

				retVal = retVal / val2;
			}
		}
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

		//Evaluate: evaluate the expression for addition or subtraction
	}

	Parser::PushBackToken(tok);
	return true;
} //TERM

//SFactor = Sign Factor | Factor
bool SFactor(istream &in, int &line, Value &retVal )
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 1;
	if (t == MINUS)
	{
		sign = -1;
	}
	else if (t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);

	status = Factor(in, line, sign, retVal);
	return status;
}
//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream &in, int &line, Value &retVal)
{
	//cout << "in Logic Expr" << endl;
	Value val1, val2;

	bool t1 = Expr(in, line, val1);
	LexItem tok;

	if (!t1)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (tok == LTHAN || tok == EQUAL)
	{
		t1 = Expr(in, line, val2);
		if (!t1)
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
		if(val1.IsChar()){
            ParseError(line,"Run-Time Error-Illegal Mixed Type operation");
            return false;
        }
        if(val2.IsChar()){
            ParseError(line,"Run-Time Error-Illegal Mixed Type operation");
            return false;    
        }

		Value gayVal;
		if(tok == LTHAN) {
			gayVal = val1 < val2;
			if(gayVal.GetBool()) {
				retVal.SetType(VBOOL);
				retVal.SetBool(true);
			} else {
				retVal.SetType(VBOOL);
				retVal.SetBool(false);
			}
		} 
		if(tok == EQUAL) {
			gayVal = val1 == val2;
			if(gayVal.GetBool()) {
				retVal.SetType(VBOOL);
				retVal.SetBool(true);
			} else {
				retVal.SetType(VBOOL);
				retVal.SetBool(false);
			}
		}

		return true;
	}
	Parser::PushBackToken(tok);
	return true;
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream &in, int &line, int sign, Value &retVal)
{
	//cout << "in Factor" << endl;
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == IDENT)
	{
		//check if the var is defined
		//int val;
		string lexeme = tok.GetLexeme();
		if ((defVar.find(lexeme) == defVar.end()) || TempsResults.find(lexeme) == TempsResults.end() )
		{
			ParseError(line, "Undefined Variable");
			return false;
		}
		if(SymTable[lexeme] == INTEGER || SymTable[lexeme] == REAL) 
			retVal = TempsResults[lexeme] * sign;
		else
			retVal = TempsResults[lexeme];
		return true;
	}
	else if (tok == ICONST)
	{
		retVal.SetType(VINT);
		retVal.SetInt(stoi(tok.GetLexeme()) * sign);
		return true;
	}
	else if (tok == SCONST)
	{
		retVal.SetType(VCHAR);
		retVal.SetChar(tok.GetLexeme());
		return true;
	}
	else if (tok == RCONST)
	{
		retVal.SetType(VREAL);
		retVal.SetReal(stof(tok.GetLexeme()) * sign);
		return true;
	}
	else if (tok == LPAREN)
	{
		bool ex = Expr(in, line, retVal);
		if (!ex)
		{
			ParseError(line, "Missing expression after (");
			return false;
		}
		if (Parser::GetNextToken(in, line) == RPAREN)
			return ex;

		ParseError(line, "Missing ) after expression");
		return false;
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	ParseError(line, "Unrecognized input");
	return 0;
}
