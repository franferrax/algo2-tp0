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
    x = b.x;
    y= b.y;
    return *this;
}

double complejo::GetReal()const {
    return x;
}

double complejo::GetImag()const {
    return y;
}

void complejo::SetReal(double xx){
    x=xx;
}

void complejo::SetImag(double yy){
    y=yy;
}

const complejo complejo::operator+(const complejo &r){
    return complejo (x+r.x , y+r.y);
}

const complejo complejo::operator+(const double f){
    return complejo (x+f,y);
}

//z
void complejo::identidadDesde(const complejo &c){
    x = c.x;
    y = c.y;
}

//e^z = e^(x+iy) = e^x * e^(iy) = e^x * (cos(y) + i sin(y))
void complejo::exponencialDesde(const complejo &c){
    x = exp(c.x)*cos(c.y);
    y = exp(c.x)*sin(c.y);
}

// z^2 = (x+iy)^2 = x^2 - y^2 + i 2*x*y
void complejo::cuadradoDesde(const complejo &c){
    x = c.x*c.x - c.y*c.y;
    y = 2*c.x*c.y;
}

// z^3 = (x+iy)^3 = x^3 - 3*x*y^2 - i (y^3 - 3*x^2*y)
void complejo::cuboDesde(const complejo &c){
    x = c.x*c.x*c.x - 3*c.x*c.y*c.y;
    y = 3*c.x*c.x*c.y - c.y*c.y*c.y;
}

// sin(z) = sin(x+iy) = sin(x)*cosh(y) + i cos(x)sinh(y)
void complejo::senoDesde(const complejo &c){
    x = sin(c.x)*cosh(c.y);
    y = cos(c.x)*sinh(c.y);
}
