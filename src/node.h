/* 
 * File:   node.h
 * Author: juanzaragoza
 *
 * Created on 17 de octubre de 2014, 1:10
 */

#ifndef NODE_H
#define	NODE_H

template <class T>
class node {

    public:
        node(T , node * );

    private:
        T _value;
        node *_next;

    friend class stack;
    friend class queue;
};

#endif	/* NODE_H */

