#include<iostream>
#include<cmath>
#include "complex.h"

using namespace std;

complex::complex () : _x(0.0) , _y(0.0) {}

complex::complex (const complex & c) : _x(c._x) , _y(c._y) {}

complex::complex (double a, double b): _x(a) , _y(b) {}

complex::~complex() {}

ostream& operator<<(ostream &os, const complex &c){
    os<<"("<<c._x<<","<<c._y<<")"<<endl;
    return os;
}

istream& operator>>(istream &is, complex &c){
    bool good=false;
    double r=0,i=0;
    char ch=0;

    if(is>>ch && ch=='('){
        if(is>>r && is>>ch && ch==',' && is>>i && is>>ch && ch==')'){
            good=true;
        } else{
            good=false;
        }
    } else if(is.good()){
        is.putback(ch);
        if(is>>r)
            good=true;
        else
            good=false;
    }

    if(good){
        c._x=r;
        c._y=i;
    } else{
        is.clear(ios::badbit);
    }

    return is;
}

complex& complex::operator=(const complex & b){
    this->_x = b._x;
    this->_y = b._y;
    return *this;
}

double complex::getReal()const {
    return this->_x;
}

double complex::getImag()const {
    return this->_y;
}

void complex::setReal(double xx){
    this->_x = xx;
}

void complex::setImag(double yy){
    this->_y = yy;
}

double complex::getArg()const {
    return std::sqrt(this->_x * this->_x + this->_y * this->_y);
}

double complex::getPha()const {
    return std::atan2(this->_y, this->_x);
}


/* ////////////| Operadores, solo los utilizados, faltan algunos |/////////// */

const complex complex::operator+(const complex &c)const {
    return complex (this->_x + c._x, this->_y + c._y);
}

const complex complex::operator+(const double f)const {
    return complex (this->_x + f, this->_y);
}

void complex::operator+=(const complex &c) {
    this->_x += c._x;
    this->_y += c._y;
}

const complex complex::operator-(const complex &c)const {
    return complex (this->_x - c._x, this->_y - c._y);
}

void complex::operator-=(const complex &c) {
    this->_x -= c._x;
    this->_y -= c._y;
}

// (a+bi)(c+di) = ac-bd + (ad+bc)i
const complex complex::operator*(const complex &c)const {
    return complex ( this->_x * c._x - this->_y * c._y,
                      this->_x * c._y + this->_y * c._x );
}

// (a+bi)/(c+di) = ( ac+bd + (bc-ad)i ) / ( b^2 + d^2 )
const complex complex::operator/(const complex &c)const {
    double aux = c._x*c._x + c._y*c._y;

    return complex ( (this->_x * c._x + this->_y * c._y) / aux,
                      (this->_y * c._x - this->_x * c._y) / aux );
}


/*|/////////| Miembros estáticos, funciones exp, log, pow, sin, cos |\\\\\\\\|*/

//exp(z) = exp(x+iy) = exp(x) * exp(iy) = exp(x) * (cos(y) + i sin(y))
const complex complex::exp(const complex &z) {
    return complex ( std::exp(z._x) * std::cos(z._y),
                      std::exp(z._x) * std::sin(z._y) );
}

//ln(z) = ln|z| + i Phase(z)
const complex complex::log(const complex &z) {
    return complex (std::log(z.getArg()), z.getPha());
}

//w^z = exp(z * log(w))
const complex complex::pow(const complex &w, const complex &z) {
    return complex::exp(z * complex::log(w));
}

// sin(z) = sin(x+iy) = sin(x)*cosh(y) + i cos(x)*sinh(y)
const complex complex::sin(const complex &z) {
    return complex ( std::sin(z._x) * std::cosh(z._y),
                      std::cos(z._x) * std::sinh(z._y) );
}

// cos(z) = cos(x+iy) = cos(x)*cosh(y) - i sin(x)*sinh(y)
const complex complex::cos(const complex &z) {
    return complex ( std::cos(z._x) * std::cosh(z._y),
                     -std::sin(z._x) * std::sinh(z._y) );
}
