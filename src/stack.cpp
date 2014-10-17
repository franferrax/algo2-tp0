/* 
 * File:   Stack.cpp
 * Author: juanzaragoza
 * 
 * Created on 17 de octubre de 2014, 0:25
 */

#include <iostream>

template <class T>
stack::~stack() {
    
   while(_last) pop();
   
}

template <class T>
void stack::push(const T& v) {
    
   node *new_node;
   new_node = new node(v, _last);
   
   _last = new_node; //asigno el nuevo nodo a la pila
}

template <class T>
T stack::pop() {

   node *auxNode;
   T v;
   
   if(!_last) return 0; //pila vacia
   
   auxNode = _last; //primer elemento de la pila
   
   _last = auxNode->_next; //asignamos a la pila toda la pila sin el último nodo
   
   v = auxNode->_value; //guardamos el valor del primero elemento de la pila
   
   delete auxNode; //borramos el nodo
   
   return v;
}


template <class T>
bool stack::isEmpty() const {

    if(_last != NULL) return true;
    else return false;

}

template <class T>
T& stack::topElement() {
    
    return _last->_value;
    
}