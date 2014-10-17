/* 
 * File:   queue.h
 * Author: juanzaragoza
 *
 * Created on 17 de octubre de 2014, 1:14
 */

#ifndef QUEUE_H
#define	QUEUE_H

#include <iostream>
#include "node.h"

template <class T>
class queue {
    public:
        queue() : _first(NULL), _last(NULL) {}
        ~queue();

        void enqueue(const T&);
        T dequeue();
        bool isEmpty() const;
        T& frontElement();

    private:
        node *_first, *_last;

};

#endif	/* QUEUE_H */

