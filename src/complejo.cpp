#include<iostream>
#include<math.h>
#include "complejo.h"

using namespace std;

complejo::complejo () : x(0.0) , y(0.0) {}

complejo::complejo (const complejo & c) : x(c.x) , y (c.y) {}

complejo::complejo (double a, double b): x(a) , y(b) {}

complejo::~complejo() {}

ostream& operator<<(ostream &os, const complejo &c){
    os<<"("<<c.x<<","<<c.y<<")"<<endl;
    return os;
}

istream& operator>>(istream &is,complejo &c){
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
        c.x=r;
        c.y=i;
    } else{
        is.clear(ios::badbit);
    }

    return is;
}

complejo& complejo::operator=(const complejo & b){
    this->x = b.x;
    this->y = b.y;
    return *this;
}

double complejo::GetReal()const {
    return this->x;
}

double complejo::GetImag()const {
    return this->y;
}

void complejo::SetReal(double xx){
    this->x = xx;
}

void complejo::SetImag(double yy){
    this->y = yy;
}

double complejo::GetArg()const {
    return sqrt(this->x * this->x + this->y * this->y);
}

double complejo::GetPha()const {
    return atan2(this->y, this->x);
}


/* ////////////| Operadores, solo los utilizados, faltan algunos |/////////// */

const complejo complejo::operator+(const complejo &c)const {
    return complejo (this->x + c.x, this->y + c.y);
}

const complejo complejo::operator+(const double f)const {
    return complejo (this->x + f, this->y);
}

void complejo::operator+=(const complejo &c) {
    this->x += c.x;
    this->y += c.y;
}

const complejo complejo::operator-(const complejo &c)const {
    return complejo (this->x - c.x, this->y - c.y);
}

void complejo::operator-=(const complejo &c) {
    this->x -= c.x;
    this->y -= c.y;
}

// (a+bi)(c+di) = ac-bd + (ad+bc)i
const complejo complejo::operator*(const complejo &c)const {
    return complejo ( this->x * c.x - this->y * c.y,
                      this->x * c.y + this->y * c.x );
}

// (a+bi)/(c+di) = ( ac+bd + (bc-ad)i ) / ( b^2 + d^2 )
const complejo complejo::operator/(const complejo &c)const {
    double aux = c.x*c.x + c.y*c.y;

    return complejo ( (this->x * c.x + this->y * c.y) / aux,
                      (this->y * c.x - this->x * c.y) / aux );
}


/*|/////////| Miembros estáticos, funciones exp, log, pow, sin, cos |\\\\\\\\|*/

//exp(z) = exp(x+iy) = exp(x) * exp(iy) = exp(x) * (cos(y) + i sin(y))
const complejo complejo::Exp(const complejo &z) {
    return complejo (exp(z.x) * cos(z.y), exp(z.x) * sin(z.y));
}

//ln(z) = ln|z| + i Phase(z)
const complejo complejo::Log(const complejo &z) {
    return complejo (log(z.GetArg()), z.GetPha());
}

//w^z = exp(z * log(w))
const complejo complejo::Pow(const complejo &w, const complejo &z) {
    return complejo::Exp(z * complejo::Log(w));
}

// sin(z) = sin(x+iy) = sin(x)*cosh(y) + i cos(x)*sinh(y)
const complejo complejo::Sin(const complejo &z) {
    return complejo (sin(z.x) * cosh(z.y), cos(z.x) * sinh(z.y));
}

// cos(z) = cos(x+iy) = cos(x)*cosh(y) - i sin(x)*sinh(y)
const complejo complejo::Cos(const complejo &z) {
    return complejo (cos(z.x) * cosh(z.y), - sin(z.x) * sinh(z.y));
}
