#include<iostream>
#include<math.h>
#include "complejo.h"

using namespace std;

complejo::complejo () : x(0.0) , y(0.0) {}  

complejo::complejo (const complejo & c) : x(c.x) , y (c.y) {} 

complejo::complejo (double a, double b): x(a) , y(b) {}

complejo::~complejo()  {
    cout<<"Se elimina una instancia de complejo con parte real "<<x<<
          " y parte imaginaria "<<y<<endl;
}

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

//e^z = e^(x+iy) = e^x * e^(iy) = e^x * (cos(y) + i sin(y))
complejo complejo::expo(const complejo &c){
    return complejo(exp(c.x)*cos(c.y),exp(c.x)*sin(c.y));
}

//z
complejo complejo::expo(const complejo &c){
    return complejo(c.x,c.y);
}
