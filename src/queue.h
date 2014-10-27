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
        node<T> *_first, *_last;

};


template <class T>
queue<T>::~queue() {

   while(_first) dequeue();

}

template <class T>
void queue<T>::enqueue(const T& v){

    node<T> *newNode;

    newNode = new node<T>(v); //creamos un nuevo auxNode

    if(_last) _last->_next = newNode;// Si la cola no estaba vacía, añadimos el nuevo a continuación de ultimo

    _last = newNode; // Ahora, el último elemento de la cola es el nuevo auxNode

    if(!_first) _first = newNode; // Si la cola estaba vacía, ahora primero apunta al nuevo auxNode

}

template <class T>
T queue<T>::dequeue(){

    node<T> *auxNode;
    T v;

    auxNode = _first;
    if(!auxNode) return 0;

    _first = auxNode->_next; //asignamos al primero el segundo nodo

    v = auxNode->_value;

    delete auxNode;

    if(!_first) _last = NULL;//si la cola quedó vacía, ultimo debe ser NULL también

    return v;

}

template <class T>
bool queue<T>::isEmpty() const{

    return _first == NULL; //estar vacía es tener _first en NULL

}

template <class T>
T& queue<T>::frontElement(){

    return _first->_value;

}

#endif	/* QUEUE_H */
