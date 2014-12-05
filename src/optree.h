#ifndef OPTREE_H
#define OPTREE_H

#include <iostream>
#include <cstdlib>
#define _USE_MATH_DEFINES // Constantes matemáticas al cargar <cmath>
#include <cmath>
#include "complex.h"
#include "stack.h"
#include "parser.h"

using namespace std;


// Tipo de nodo en el árbol de operaciones
typedef enum
{
    NODE_UNARY_OP,
    NODE_BINARY_OP,
    NODE_STATIC_COMPLEX,
    NODE_DYNAMIC_COMPLEX,
    NODE_PIXEL_COMPLEX,
    NODE_UNDEFINED
} node_type;



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*|||||||||||||||||||||||||||||||| OptreeNode ||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
class optree_node {

    node_type     _t;      // Tipo de nodo
    complex const *_c;     // Puntero al complejo para evaluar
    operator_t    _bin_op; // Puntero a la operación binaria
    function_t    _un_op;  // Puntero a la operación unaria
    optree_node   *_left;  // Subárbol izquierdo (o hijo en operaciones unarias)
    optree_node   *_right; // Subárbol derecho
    optree_node   *_up;    // Padre


    ///////////////////////// Utilidades internas //////////////////////////

    // Simplificar el sub-árbol eliminando las expresiones independientes de z
    // NOTA: devuelve true si el subárbol depende de z
    bool simplify();


public:
    // 1) Constructor por defecto
    optree_node();

    // 2) Constructor a partir de token
    optree_node(const token &, optree_node *);

    // 3) Destructor
    ~optree_node();

    // 4) Operar, para realizar la operación
    const complex operate(complex *) const;

    friend class optree;

private:
    // TODO: constructor por copia
    optree_node(const optree_node &);
};




/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*|||||||||||||||||||||||||||||||||| Optree ||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
class optree {

    optree_node *_root;       // Raíz del árbol
    complex     *_asoc_pixel; // Puntero al complejo asociado al pixel

public:
    // 1) Constructor por defecto
    optree() { _root = NULL; _asoc_pixel = NULL; }

    // 2) Constructor desde pila de tokens con RPN + complejo de pixel
    optree(stack<token> &, complex &);

    // 3) Destructor
    ~optree() { delete this->_root; }

    // 4) Operar, para realizar la operación
    const complex operate() const;

private:
    // TODO: constructor por copia
    optree(const optree &);
};


#endif // OPTREE_H
