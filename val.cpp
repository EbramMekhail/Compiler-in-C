
#include "val.h"

Value::Value() : T(VERR), Itemp(0), Rtemp(0.0) {}
Value::Value(bool vb) : T(VBOOL), Btemp(vb), Itemp(0), Rtemp(0.0) {}
Value::Value(int vi) : T(VINT), Itemp(vi) {}
Value::Value(float vr) : T(VREAL), Itemp(0), Rtemp(vr) {}
Value::Value(string vs) : T(VCHAR), Itemp(0), Rtemp(0.0), Stemp(vs) {}

ValType Value::GetType() const { return T; }
bool Value::IsErr() const { return T == VERR; }
bool Value::IsInt() const { return T == VINT; }
bool Value::IsChar() const { return T == VCHAR; }
bool Value::IsReal() const { return T == VREAL; }
bool Value::IsBool() const { return T == VBOOL; }

int Value::GetInt() const
{
	if (IsInt())
		return Itemp;
	throw "RUNTIME ERROR: Value not an integer";
}

string Value::GetChar() const
{
	if (IsChar())
		return Stemp;
	throw "RUNTIME ERROR: Value not a string";
}

float Value::GetReal() const
{
	if (IsReal())
		return Rtemp;
	throw "RUNTIME ERROR: Value not an integer";
}

bool Value::GetBool() const
{
	if (IsBool())
		return Btemp;
	throw "RUNTIME ERROR: Value not a boolean";
}

void Value::SetType(ValType type)
{
	T = type;
}

void Value::SetInt(int val)
{
	Itemp = val;
}

void Value::SetReal(float val)
{
	Rtemp = val;
}

void Value::SetChar(string val)
{
	Stemp = val;
}

void Value::SetBool(bool val)
{
	Btemp = val;
}

// add this with op
Value Value::operator+(const Value &op) const
{
	if (GetType() == op.GetType())
	{
		if (IsInt())
			return Value(Itemp + op.GetInt());
		if (IsReal())
			return Value(Rtemp + op.GetReal());
	}
	else if (IsInt() && op.IsReal())
	{

		return Value((float)GetInt() + op.GetReal());
	}
	else if (IsReal() && op.IsInt())
	{
		return Value(GetReal() + (float)op.GetInt());
	}

	return Value();
}

// subtract op from this
Value Value::operator-(const Value &op) const
{
	if (GetType() == op.GetType())
	{
		if (IsInt())
			return Value(Itemp - op.GetInt());
		if (IsReal())
			return Value(Rtemp - op.GetReal());
	}
	else if (IsInt() && op.IsReal())
	{

		return Value((float)GetInt() - op.GetReal());
	}
	else if (IsReal() && op.IsInt())
	{
		return Value(GetReal() - (float)op.GetInt());
	}

	return Value();
}

// multiply this with op
Value Value::operator*(const Value &op) const
{
	if (GetType() == op.GetType())
	{
		if (IsInt())
			return Value(Itemp * op.GetInt());
		if (IsReal())
			return Value(Rtemp * op.GetReal());
	}
	else if (IsInt() && op.IsReal())
	{

		return Value((float)GetInt() * op.GetReal());
	}
	else if (IsReal() && op.IsInt())
	{
		return Value(GetReal() * (float)op.GetInt());
	}

	return Value();
}

// divide this by op
Value Value::operator/(const Value &op) const
{
	if (GetType() == op.GetType())
	{
		if (IsInt())
			return Value(Itemp / op.GetInt());
		if (IsReal())
			return Value(Rtemp / op.GetReal());
	}
	else if (IsInt() && op.IsReal())
	{

		return Value((float)GetInt() / op.GetReal());
	}
	else if (IsReal() && op.IsInt())
	{
		return Value(GetReal() / ((float)op.GetInt()));
	}

	return Value();
}

Value Value::operator==(const Value &op) const
{
	if (GetType() == op.GetType())
	{
		if (IsInt())
			return Value((bool)(Itemp == op.GetInt()));
		if (IsReal())
			return Value((bool)(Rtemp == op.GetReal()));
		if (IsChar())
			return Value((bool)(Stemp < op.GetChar()));
	}
	else if (IsInt() && op.IsReal())
	{

		return Value((bool)(((float)GetInt()) == op.GetReal()));
	}
	else if (IsReal() && op.IsInt())
	{
		return Value((bool)(GetReal() == ((float)op.GetInt())));
	}

	return Value();
}

Value Value::operator<(const Value &op) const
{
	if (GetType() == op.GetType())
	{
		if (IsInt())
			return Value(Itemp < op.GetInt());
		if (IsReal())
			return Value(Rtemp < op.GetReal());
		if (IsChar())
			return Value(Stemp < op.GetChar());
	}
	else if (IsInt() && op.IsReal())
	{

		return Value(((float)GetInt()) < op.GetReal());
	}
	else if (IsReal() && op.IsInt())
	{
		return Value(GetReal() < ((float)op.GetInt()));
	}

	return Value();
}