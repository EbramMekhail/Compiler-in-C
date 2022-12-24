#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <queue>
#include <map>

using namespace std;

enum ValType
{
    VINT,
    VREAL,
    VCHAR,
    VBOOL,
    VERR
};

class Value
{
    ValType T;
    bool Btemp;
    int Itemp;
    float Rtemp;
    string Stemp;

public:
    Value();
    Value(bool vb);
    Value(int vi);
    Value(float vr);
    Value(string vs);

    ValType GetType() const;
    bool IsErr() const;
    bool IsInt() const;
    bool IsChar() const;
    bool IsReal() const;
    bool IsBool() const;

    int GetInt() const;

    string GetChar() const;

    float GetReal() const;

    bool GetBool() const;

    void SetType(ValType type);

    void SetInt(int val);

    void SetReal(float val);

    void SetChar(string val);

    void SetBool(bool val);

    // add op to this
    Value operator+(const Value &op) const;

    // subtract op from this
    Value operator-(const Value &op) const;

    // multiply this by op
    Value operator*(const Value &op) const;

    // divide this by opß
    Value operator/(const Value &op) const;

    Value operator==(const Value &op) const;

    Value operator<(const Value &op) const;

    friend ostream &operator<<(ostream &out, const Value &op)
    {
        if (op.IsInt())
            out << op.Itemp;
        else if (op.IsChar())
            out << op.Stemp;
        else if (op.IsReal())
            out << op.Rtemp;
        else
            out << "ERROR";
        return out;
    }
};

#endif
