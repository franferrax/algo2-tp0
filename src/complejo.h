#ifndef COMPLEJO_H
#define COMPLEJO_H

#include<iostream>
using namespace std;

class complejo

{
private:

    double _x, _y;

public:

    complejo();
    complejo(double , double);
    complejo(const complejo &);

    ~complejo();

    double getReal()const;
    double getImag()const;

    void setReal(double xx);
    void setImag(double yy);

    double getArg()const;
    double getPha()const;

    friend ostream& operator<<(ostream&, const complejo &);
    friend istream& operator>>(istream&, complejo &);

    complejo& operator=(const complejo &);
    const complejo operator+(const complejo &)const;
    const complejo operator+(const double)const;
    void operator+=(const complejo &);
    const complejo operator-(const complejo &)const;
    void operator-=(const complejo &);
    const complejo operator*(const complejo &)const;
    const complejo operator/(const complejo &)const;

    static const complejo exp(const complejo &);
    static const complejo log(const complejo &);
    static const complejo pow(const complejo &, const complejo &);
    static const complejo sin(const complejo &);
    static const complejo cos(const complejo &);
};

#endif
