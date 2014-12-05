#include "optree.h"


/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*|||||||||||||||||||||||||||||||| OptreeNode ||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////////| Constructor por defecto |\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
optree_node::optree_node()
{
    this->_t = NODE_UNDEFINED;
    this->_c       = NULL;
    this->_un_op   = NULL;
    this->_bin_op  = NULL;
    this->_left    = NULL;
    this->_right   = NULL;
    this->_up      = NULL;
}



/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////| Constructor a partir de token |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
optree_node::optree_node(const token &tok, optree_node *father = NULL)
{
    /*
        Los tipos de token son:

            *       value: tienen un double y aquí pasan a la parte real de un
                           complejo dinámico. Tipo: NODE_DYNAMIC_COMPLEX.

            * parenthesis: ya no hay de este tipo luego de pasar a RPN, se
                           imprimirá un error.

            *    operator: serán asociados a operaciones binarias.
                           Tipo: NODE_BINARY_OP.

            *    function: serán asociados a operaciones unarias.
                           Tipo: NODE_UNARY_OP.

            *     special:
                        -        z: será asociado al complejo de pixel.
                                    Tipo: NODE_PIXEL_COMPLEX.

                        - j, e, pi: serán asociados a complejos miembros
                                    estáticos del array special_complex_.
                                    Tipo: NODE_STATIC_COMPLEX.
    */

    if (tok.isParenthesis())
    {
        cerr << "Internal Error: the token can't be a parenthesis, "
             << "RPN convert is required before to make the optree."
             << endl;
        exit(2);
    }

    // Operandos
    if (tok.isSpecial() || tok.isValue())
    {
        this->_un_op  = NULL;
        this->_bin_op = NULL;

        // Special Token
        if (tok.isSpecial())
        {
            // Si es PIXEL_TOKEN (es decir, z)
            if (tok.getAsString() == PIXEL_TOKEN)
            {
                this->_t = NODE_PIXEL_COMPLEX;
                this->_c = NULL;
            }
            else // Si no es z, se trata de un complejo estático
            {
                this->_t = NODE_STATIC_COMPLEX;
                this->_c = &special_complex_[
                           find_in_list(special_tokens_, tok.getAsString()) ];
            }

        }
        // Value Token
        else
        {
           this->_t = NODE_DYNAMIC_COMPLEX;
           this->_c = new complex(tok.getAsDouble(), 0);
        }
    }

    // Operaciones
    if (tok.isOperator() || tok.isFunction())
    {
        this->_c = NULL;

        // Operator Token -> Operación binaria
        if (tok.isOperator())
        {
            this->_t = NODE_BINARY_OP;
            this->_un_op = NULL;
            this->_bin_op = operator_pointers_[
                    find_in_list(operator_tokens_, tok.getAsString()) ];
        }

        // Function Token -> Operación unaria
        else
        {
            this->_t = NODE_UNARY_OP;
            this->_bin_op = NULL;
            this->_un_op = function_pointers_[
                    find_in_list(function_tokens_, tok.getAsString()) ];
        }
    }


    this->_left  = NULL;
    this->_right = NULL;
    this->_up = father;
}



/*|/////////////////////////////////|   3)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////////////| Destructor |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
optree_node::~optree_node()
{
    if (this->_t == NODE_DYNAMIC_COMPLEX) delete this->_c;
    if (this->_left != NULL)              delete this->_left;
    if (this->_right != NULL)             delete this->_right;
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////| Operar, para realizar la operación |\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
const complex optree_node::operate(complex *z) const
{
    // z es un puntero que apunta al complejo asociado al pixel para operar

    // Caso base y de corte, operandos.
    if ( this->_t == NODE_STATIC_COMPLEX || this->_t == NODE_DYNAMIC_COMPLEX )
        return *(this->_c);

    if ( this->_t == NODE_PIXEL_COMPLEX )
        return *z;

    // Operación binaria (operadores)
    if ( this->_t == NODE_BINARY_OP )
        return this->_bin_op(this->_left->operate(z), this->_right->operate(z));

    // Operación unaria (funciones)
    if  (this->_t == NODE_UNARY_OP )
        return this->_un_op(this->_left->operate(z));

    // Si se llega hasta aquí, el nodo estaba sin definir, error
    cerr << "Internal Error: there are some node, not setted in the optree."
         << endl;
    exit(2);
}



/*|/////////////////////////////////|   *)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////| Utilidades internas |\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/

// Simplificar el sub-árbol eliminando las expresiones independientes de z
// NOTA: devuelve true si el subárbol depende de z
bool optree_node::simplify()
{
    bool pixel_dependent;

    // Caso base: hoja, entonces depende de él, si es z u otra cosa
    if (this->_left == NULL && this->_right == NULL)
        return this->_t == NODE_PIXEL_COMPLEX;

    // Si tiene único hijo (está a la izquierda, op unaria), depende de éste
    if (this->_right == NULL)
        pixel_dependent = this->_left->simplify();

    // Si tiene ambos hijos (op binaria), con que uno dependa de z, suficiente
    else
        pixel_dependent = this->_left->simplify() || this->_right->simplify();

    // Si no es pixel dependiente, como no es hoja (caso base), se simplifica
    if (!pixel_dependent)
    {
        /*
           El resultado no depende de z, por eso no hay problema con pasar NULL
           a operate(). Además, como el nodo actual no es hoja, es una
           operación, por lo tanto _c está libre (en NULL).
        */
        this->_c = new complex(this->operate(NULL));

        // Se cambia el tipo del nodo subárbol, ahora convertido en hoja
        this->_t = NODE_DYNAMIC_COMPLEX;
        this->_un_op  = NULL;
        this->_bin_op = NULL;

        // Se destruye el hijo izquierdo (tiene que existir)
        delete this->_left;
        this->_left = NULL;
        // Si tiene, se destruye el hijo derecho
        if (this->_right != NULL)
        {
            delete this->_right;
            this->_right = NULL;
        }
    }

    return pixel_dependent;
}




/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*|||||||||||||||||||||||||||||||||| Optree ||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
#define is_operation(t) ((t) == NODE_UNARY_OP || (t) == NODE_BINARY_OP)

/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////| Constructor desde pila de tokens con RPN + complejo de pixel |\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
optree::optree(stack<token> &rpn, complex &z)
{
    optree_node *current;

    // Primer token a la raíz
    this->_root = new optree_node(rpn.pop());

    // current siempre será una operación, por la construcción del algoritmo,
    // si no lo fuera, la pila ya está vacía.
    current = this->_root;
    while (!rpn.isEmpty())
    {
        // Si actual es unario
        if (current->_t == NODE_UNARY_OP)
        {
            // Único hijo libre, se usa _left
            if (current->_left == NULL)
            {
                // Token al hijo
                current->_left = new optree_node(rpn.pop(), current);

                // Si el token era una operación, bajar
                if (is_operation(current->_left->_t))
                    current = current->_left;
            }
            // Hijo ocupado, subir
            else
                current = current->_up;
        }
        // Si actual es binario
        else
        {
            // Derecha libre
            if (current->_right == NULL)
            {
                // Token a la derecha
                current->_right = new optree_node(rpn.pop(), current);

                // Si el token era una operación, bajar por derecha
                if (is_operation(current->_right->_t))
                    current = current->_right;
            }
            // Derecha ocupada
            else
            {
                // Izquierda libre
                if (current->_left == NULL)
                {
                    // Token a la izquierda
                    current->_left = new optree_node(rpn.pop(), current);

                    // Si el token era una operación, bajar por izquierda
                    if (is_operation(current->_left->_t))
                        current = current->_left;
                }
                // Si ambas ramas están ocupadas, subir
                else
                    current = current->_up;
            }
        }
    }

    // Asociación del complejo para iterar los pixel
    this->_asoc_pixel = &z;

    // Simplificación del árbol, eliminando expresiones 'estáticas'
    this->_root->simplify();
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////| Operar, para realizar la operación |\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
const complex optree::operate() const
{
    // Validaciónes
    if ( this->_asoc_pixel == NULL || this->_root == NULL ) return complex();

    return _root->operate(this->_asoc_pixel);
}
