#ifndef OPTREE_H
#define OPTREE_H

// Constantes matemáticas (requeridas: M_PI, M_E)
#define _USE_MATH_DEFINES

#include <iostream>
#include <cstdlib>
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
/*|||||||||||||||||||||||||||||||||| Optree ||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

class optree {

    node_type             _t;  // Tipo de nodo
    complex const        *_z;  // Puntero al complejo para evaluar

    operator_t       _bin_op;  // Puntero a la operación binaria
    function_t        _un_op;  // Puntero a la operación unaria

    optree      *_left;  // Subárbol izquierdo (o hijo en operaciones unarias)
    optree      *_right; // Subárbol derecho
    optree      *_up;    // Padre

public:
    // 1) Constructor por defecto
    optree();

    // 2) Constructor a partir de token
    optree(const token &, optree *);

    // 3) Destructor
    ~optree();

    // 4) Operar, para realizar la operación
    const complex operate();

    // 5) Cargar desde una RPN en una pila
    static optree * load_from_stack_RPN(stack<token> &);

    // Puntero al complejo asociado al pixel
    static complex *asociated_pixel;
};


#endif // OPTREE_H
