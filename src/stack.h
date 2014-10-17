/* 
 * File:   Stack.h
 * Author: juanzaragoza
 *
 * Created on 17 de octubre de 2014, 0:25
 */

#ifndef STACK_H
#define	STACK_H

#include <iostream>
#include "node.h"
#include "stack.h"

template <class T>
class stack {
    
    public:
    
        stack() : _last(NULL) {};
        ~stack();

        bool isEmpty() const;
        void push(const T& );
        T pop();
        T& topElement();

    private:        
        node _last;    

};

#endif	/* STACK_H */

