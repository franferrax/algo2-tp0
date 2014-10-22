/* 
 * File:   queue.cpp
 * Author: juanzaragoza
 * 
 * Created on 17 de octubre de 2014, 1:14
 */

#include <iostream>
#include "queue.h"
#include "node.h"

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
    
    if(_first != NULL) return true;
    else return false;
    
}

template <class T>
T& queue<T>::frontElement(){
    
    return _first->_value;
    
}