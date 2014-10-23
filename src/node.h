/* 
 * File:   node.h
 * Author: juanzaragoza
 *
 * Created on 17 de octubre de 2014, 1:10
 */

#ifndef NODE_H
#define	NODE_H

#include "stack.h"
#include "queue.h"

template <class T>
class node {

    public:
        node(T , node<T> * );

    private:
        T _value;
        node<T> *_next;

    friend class stack<T>;
    friend class queue<T>;
};

template <class T>
node<T>::node(T v, node<T> *nxt = NULL) {
    _value = v;
    _next = nxt;
}

#endif	/* NODE_H */

