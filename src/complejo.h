#ifndef COMPLEJO_H
#define COMPLEJO_H

#include<iostream>
using namespace std;

class complejo

{
private:

    double x,y;

public:

    complejo();

    complejo(double , double );

    complejo(const complejo &);

    ~complejo();

    double GetReal()const;

    double GetImag()const;

    void SetReal(double xx);

    void SetImag(double yy);

    friend ostream& operator<<(ostream&, const complejo &);

    friend istream& operator>>(istream&,complejo &);

    complejo& operator=(const complejo &);

    const complejo operator+(const complejo &);

    const complejo operator+(const double);

    void identidadDesde(const complejo &); //f(z) = z

    void exponencialDesde(const complejo &); //f(z) = e^z

    void cuadradoDesde(const complejo &); //f(z) = z^2

    void cuboDesde(const complejo &); //f(z) = z^3

    void senoDesde(const complejo &); //f(z) = sin(z)
};

#endif
