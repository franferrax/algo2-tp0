/* 
 * File:   node.cpp
 * Author: juanzaragoza
 * 
 * Created on 17 de octubre de 2014, 1:10
 */

#include "node.h"
#include <iostream>

template <class T>
node<T>::node(T v, node<T> *nxt = NULL) {
    _value = v;
    _next = nxt;
}

