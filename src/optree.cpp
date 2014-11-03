#include "optree.h"


// Puntero al complejo que itera los pixels
complex* optree::asociated_pixel = NULL;

/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////////| Constructor por defecto |\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
optree::optree()
{
    this->_t = NODE_UNDEFINED;
    this->_z       = NULL;
    this->_un_op   = NULL;
    this->_bin_op  = NULL;
    this->_left    = NULL;
    this->_right   = NULL;
    this->_up      = NULL;
}



/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////| Constructor a partir de token |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
optree::optree(const token &tok, optree *father = NULL)
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
                                    estáticos de la clase optree.
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
                this->_z = NULL;
            }
            else // Si no es z, se trata de un complejo estático
            {
                this->_t = NODE_STATIC_COMPLEX;
                this->_z = &special_complex_[
                           find_in_list(special_tokens_, tok.getAsString()) ];
            }

        }
        // Value Token
        else
        {
           this->_t = NODE_DYNAMIC_COMPLEX;
           this->_z = new complex(tok.getAsDouble(), 0);
        }
    }

    // Operaciones
    if (tok.isOperator() || tok.isFunction())
    {
        this->_z = NULL;

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
optree::~optree()
{
    if (this->_t == NODE_DYNAMIC_COMPLEX) delete this->_z;
    if (this->_left != NULL)              delete this->_left;
    if (this->_right != NULL)             delete this->_right;
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////| Operar, para realizar la operación |\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
const complex optree::operate()
{
    if ( this->asociated_pixel == NULL)
    {
        cerr << "Internal Error: z for pixel iteration not defined." << endl;
        exit(2);
    }

    // Caso base y de corte, operandos.
    if ( this->_t == NODE_STATIC_COMPLEX  || this->_t == NODE_DYNAMIC_COMPLEX )
        return *(this->_z);

    if ( this->_t == NODE_PIXEL_COMPLEX )
        return *(this->asociated_pixel);

    // Operación binaria (operadores)
    if ( this->_t == NODE_BINARY_OP )
        return this->_bin_op(this->_left->operate(), this->_right->operate());

    // Operación unaria (funciones)
    if  (this->_t == NODE_UNARY_OP )
        return this->_un_op(this->_left->operate());

    // Si se llega hasta aquí, el nodo estaba sin definir, error
    cerr << "Internal Error: there are some node, not setted in the optree."
         << endl;
    exit(2);
}



/*|/////////////////////////////////|   5)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////| Cargar desde una RPN en una pila |\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
#define is_operation(t) ((t) == NODE_UNARY_OP || (t) == NODE_BINARY_OP)

optree * optree::load_from_stack_RPN(stack<token> &rpn)
{
    optree *current, *result;

    // Primer token a la raíz
    result = new optree(rpn.pop());

    // current siempre será una operación, por la construcción del algoritmo,
    // si no lo fuera, la pila ya está vacía.
    current = result;
    while (!rpn.isEmpty())
    {
        // Si actual es unario
        if (current->_t == NODE_UNARY_OP)
        {
            // Único hijo libre, se usa _left
            if (current->_left == NULL)
            {
                // Token al hijo
                current->_left = new optree(rpn.pop(), current);

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
                current->_right = new optree(rpn.pop(), current);

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
                    current->_left = new optree(rpn.pop(), current);

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

    return result;
}
