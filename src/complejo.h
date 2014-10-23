#ifndef COMPLEJO_H
#define COMPLEJO_H

#include<iostream>
using namespace std;

class complejo

{
private:

    double x, y;

public:

    complejo();

    complejo(double , double);

    complejo(const complejo &);

    ~complejo();

    double GetReal()const;

    double GetImag()const;

    void SetReal(double xx);

    void SetImag(double yy);

    double GetArg()const;

    double GetPha()const;

    friend ostream& operator<<(ostream&, const complejo &);

    friend istream& operator>>(istream&, complejo &);

    complejo& operator=(const complejo &);

    const complejo operator+(const complejo &)const;

    const complejo operator+(const double)const;

    const complejo operator-(const complejo &)const;

    const complejo operator*(const complejo &)const;

    const complejo operator/(const complejo &)const;

    static const complejo Exp(const complejo &);

    static const complejo Log(const complejo &);

    static const complejo Pow(const complejo &, const complejo &);
};

#endif
