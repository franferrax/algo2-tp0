#include<iostream>
#include<cmath>
#include "complejo.h"

using namespace std;

complejo::complejo () : _x(0.0) , _y(0.0) {}

complejo::complejo (const complejo & c) : _x(c._x) , _y(c._y) {}

complejo::complejo (double a, double b): _x(a) , _y(b) {}

complejo::~complejo() {}

ostream& operator<<(ostream &os, const complejo &c){
    os<<"("<<c._x<<","<<c._y<<")"<<endl;
    return os;
}

istream& operator>>(istream &is, complejo &c){
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

complejo& complejo::operator=(const complejo & b){
    this->_x = b._x;
    this->_y = b._y;
    return *this;
}

double complejo::getReal()const {
    return this->_x;
}

double complejo::getImag()const {
    return this->_y;
}

void complejo::setReal(double xx){
    this->_x = xx;
}

void complejo::setImag(double yy){
    this->_y = yy;
}

double complejo::getArg()const {
    return std::sqrt(this->_x * this->_x + this->_y * this->_y);
}

double complejo::getPha()const {
    return std::atan2(this->_y, this->_x);
}


/* ////////////| Operadores, solo los utilizados, faltan algunos |/////////// */

const complejo complejo::operator+(const complejo &c)const {
    return complejo (this->_x + c._x, this->_y + c._y);
}

const complejo complejo::operator+(const double f)const {
    return complejo (this->_x + f, this->_y);
}

void complejo::operator+=(const complejo &c) {
    this->_x += c._x;
    this->_y += c._y;
}

const complejo complejo::operator-(const complejo &c)const {
    return complejo (this->_x - c._x, this->_y - c._y);
}

void complejo::operator-=(const complejo &c) {
    this->_x -= c._x;
    this->_y -= c._y;
}

// (a+bi)(c+di) = ac-bd + (ad+bc)i
const complejo complejo::operator*(const complejo &c)const {
    return complejo ( this->_x * c._x - this->_y * c._y,
                      this->_x * c._y + this->_y * c._x );
}

// (a+bi)/(c+di) = ( ac+bd + (bc-ad)i ) / ( b^2 + d^2 )
const complejo complejo::operator/(const complejo &c)const {
    double aux = c._x*c._x + c._y*c._y;

    return complejo ( (this->_x * c._x + this->_y * c._y) / aux,
                      (this->_y * c._x - this->_x * c._y) / aux );
}


/*|/////////| Miembros estáticos, funciones exp, log, pow, sin, cos |\\\\\\\\|*/

//exp(z) = exp(x+iy) = exp(x) * exp(iy) = exp(x) * (cos(y) + i sin(y))
const complejo complejo::exp(const complejo &z) {
    return complejo ( std::exp(z._x) * std::cos(z._y),
                      std::exp(z._x) * std::sin(z._y) );
}

//ln(z) = ln|z| + i Phase(z)
const complejo complejo::log(const complejo &z) {
    return complejo (std::log(z.getArg()), z.getPha());
}

//w^z = exp(z * log(w))
const complejo complejo::pow(const complejo &w, const complejo &z) {
    return complejo::exp(z * complejo::log(w));
}

// sin(z) = sin(x+iy) = sin(x)*cosh(y) + i cos(x)*sinh(y)
const complejo complejo::sin(const complejo &z) {
    return complejo ( std::sin(z._x) * std::cosh(z._y),
                      std::cos(z._x) * std::sinh(z._y) );
}

// cos(z) = cos(x+iy) = cos(x)*cosh(y) - i sin(x)*sinh(y)
const complejo complejo::cos(const complejo &z) {
    return complejo ( std::cos(z._x) * std::cosh(z._y),
                     -std::sin(z._x) * std::sinh(z._y) );
}
