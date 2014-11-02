#ifndef COMPLEJO_H
#define COMPLEJO_H

#include<iostream>
using namespace std;

class complex

{
private:

    double _x, _y;

public:

    complex();
    complex(double , double);
    complex(const complex &);

    ~complex();

    double getReal()const;
    double getImag()const;

    void setReal(double xx);
    void setImag(double yy);

    double getArg()const;
    double getPha()const;

    friend ostream& operator<<(ostream&, const complex &);
    friend istream& operator>>(istream&, complex &);

    complex& operator=(const complex &);
    const complex operator+(const complex &)const;
    const complex operator+(const double)const;
    void operator+=(const complex &);
    const complex operator-(const complex &)const;
    void operator-=(const complex &);
    const complex operator*(const complex &)const;
    const complex operator/(const complex &)const;

    static const complex exp(const complex &);
    static const complex log(const complex &);
    static const complex pow(const complex &, const complex &);
    static const complex sin(const complex &);
    static const complex cos(const complex &);
};

#endif
