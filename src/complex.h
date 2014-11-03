#ifndef COMPLEJO_H
#define COMPLEJO_H

#include<iostream>
using namespace std;


// Macro para elevar al cuadrado multiplicando por si mismo
#define square(x) ((x)*(x))

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
    const complex operator+(const double)const;
    void operator+=(const complex &);
    void operator-=(const complex &);


    // Operadores, operaciones binarias
    static const complex  operator_add(const complex &, const complex &);
    static const complex operator_subt(const complex &, const complex &);
    static const complex operator_mult(const complex &, const complex &);
    static const complex  operator_div(const complex &, const complex &);
    static const complex  operator_pow(const complex &, const complex &);

    // Funciones, operaciones unarias
    static const complex exp(const complex &);
    static const complex log(const complex &);
    static const complex sin(const complex &);
    static const complex cos(const complex &);

    static const complex  real_as_complex(const complex &);
    static const complex  imag_as_complex(const complex &);
    static const complex   abs_as_complex(const complex &);
    static const complex phase_as_complex(const complex &);
};


#endif
