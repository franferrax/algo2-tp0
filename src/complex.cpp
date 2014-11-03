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
    return std::sqrt(square(this->_x) + square(this->_y));
}

double complex::getPha()const {
    return std::atan2(this->_y, this->_x);
}


/*|////////////| Operadores, solo los utilizados, faltan varios |\\\\\\\\\\\\|*/

const complex complex::operator+(const double f)const {
    return complex ( this->_x + f, this->_y );
}

void complex::operator+=(const complex &c) {
    this->_x += c._x;
    this->_y += c._y;
}

void complex::operator-=(const complex &c) {
    this->_x -= c._x;
    this->_y -= c._y;
}


/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////| Operadores, operaciones binarias |\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
const complex complex::operator_add(const complex &w, const complex &z){
    return complex ( w._x + z._x, w._y + z._y );
}

const complex complex::operator_subt(const complex &w, const complex &z){
    return complex ( w._x - z._x, w._y - z._y );
}

// (x1+jy1)*(x2+jy2) = x1*x2 - y1*y2  + j( x1*y2 + y1*x2 )
const complex complex::operator_mult(const complex &w, const complex &z){
    return complex ( w._x*z._x - w._y*z._y, w._x*z._y + w._y*z._x );
}

// (x1+jy1)/(x2+jy2) = ( x1*x2 + y1*y2 + j( y1*x2 - x1*y2 ) ) / ( x1^2 + y2^2 )
const complex complex::operator_div(const complex &w, const complex &z){
    double aux = square(z._x) + square(z._y);

    return complex ( (w._x * z._x + w._y * z._y) / aux,
                     (w._y * z._x - w._x * z._y) / aux );
}

//w^z = exp(z * log(w))
const complex complex::operator_pow(const complex &w, const complex &z) {
    return complex::exp( complex::operator_mult(z, complex::log(w)) );
}


/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////| Funciones, operaciones unarias |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/

//exp(z) = exp(x+jy) = exp(x) * exp(jy) = exp(x) * (cos(y) + j sin(y))
const complex complex::exp(const complex &z) {
    return complex ( std::exp(z._x) * std::cos(z._y),
                     std::exp(z._x) * std::sin(z._y) );
}

//ln(z) = ln|z| + j Phase(z)
const complex complex::log(const complex &z) {
    return complex ( std::log(z.getArg()), z.getPha() );
}


// sin(z) = sin(x+jy) = sin(x) * cosh(y) + j cos(x) * sinh(y)
const complex complex::sin(const complex &z) {
    return complex ( std::sin(z._x) * std::cosh(z._y),
                     std::cos(z._x) * std::sinh(z._y) );
}

// cos(z) = cos(x+jy) = cos(x) * cosh(y) - j sin(x) * sinh(y)
const complex complex::cos(const complex &z) {
    return complex ( std::cos(z._x) * std::cosh(z._y),
                    -std::sin(z._x) * std::sinh(z._y) );
}

// Adaptaciones a devolución compleja de algunos métodos

const complex complex::real_as_complex(const complex &z)
{
    return complex(z.getReal(), 0);
}

const complex complex::imag_as_complex(const complex &z)
{
    return complex(z.getImag(), 0);
}

const complex complex::abs_as_complex(const complex &z)
{
    return complex(z.getArg(), 0);
}

const complex complex::phase_as_complex(const complex &z)
{
    return complex(z.getPha(), 0);
}
