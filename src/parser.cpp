#include "parser.h"

/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||| Token ||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/


/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////| Constructores |\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/

// De un token del tipo 'valor', por defecto (tipo 'valor' con valor 0)
token::token(const double &v)
{
    this->_v = v;
    this->_s = TOKEN_IS_VALUE;
}

// De un token que no es del tipo 'valor'
token::token(const string &s)
{
    this->_v = NAN;
    this->_s = s;
}

// Por copia
token::token(const token &t)
{
    this->_v = t._v;
    this->_s = t._s;
}



/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////////| Obtener como string |\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
string token::getAsString() const
{
    if (this->isValue())
    {
        ostringstream aux;
        aux << this->_v;
        return aux.str();
    }

    return this->_s;
}
// NOTA: si bien es capaz de devolver el valor en una string, no utilizar
// innecesariamente.



/*|/////////////////////////////////|   3)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////////| Obtener como double |\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
double token::getAsDouble() const
{
    return this->_v;
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////////////| Booleanos is... |\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/

// ¿Es del tipo 'valor'?
bool token::isValue() const
{
    return this->_s == TOKEN_IS_VALUE;
}

// ¿Es un paréntesis?
bool token::isParenthesis() const
{
    if (this->isValue()) return false;
    return is_parenthesis(this->_s);
}

// ¿Es un paréntesis abierto?
bool token::isOpenParenthesis() const
{
    if (this->isValue()) return false;
    return this->_s == "(";
}

// ¿Es un paréntesis cerrado?
bool token::isClosedParenthesis() const
{
    if (this->isValue()) return false;
    return this->_s == ")";
}

// ¿Es un operador?
bool token::isOperator() const
{
    if (this->isValue()) return false;
    return is_operator(this->_s);
}

// ¿Es especial?
bool token::isSpecial() const
{
    if (this->isValue()) return false;
    return is_special(this->_s);
}

// ¿Es una función?
bool token::isFunction() const
{
    if (this->isValue()) return false;
    return is_function(this->_s);
}



/*|/////////////////////////////////|   5)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////| Precedencia, si es que el token es un operador o función |\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
int token::precedence() const
{
    if (this->isOperator())
    {
        if (this->_s == "+" || this->_s == "-") return 0;
        if (this->_s == "*" || this->_s == "/") return 1;
        if (this->_s == "^") return 3;
    }

    if (this->isFunction()) return 2;

    return NOT_FOUND; // Si la precedencia no es aplicable, devuelve NOT_FOUND
}



/*|/////////////////////////////////|   6)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////| Impresión en flujo/archivo/stdin |\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
ostream & operator<<(ostream &os, const token &t)
{
    if (t.isValue()) os << t._v;
    else             os << t._s;

    return os;
}



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||| Otros ||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/


/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////| Función para buscar en una lista con centinela de cadena vacía |\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
int find_in_list(const string l[], const string &s)
{
    for (size_t i = 0; !l[i].empty(); i++)
        if (l[i] == s)
            return i; // Si lo encuentra devuelve su posición

    return NOT_FOUND; // Si no lo encuentra se devuelve NOT_FOUND
}



/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//| Función para parsear la expresión de entrada partiéndola en tokens |\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void parse_expression_in_tokens(const string &input, queue<token> &output)
{
    stringstream expr(input);
    string aux_s;
    double aux_n;


    while (expr)
    {
        // Se intenta cargar un carácter en aux_s
        aux_s = expr.peek();

        // Si es un espacio, se ignora y se vuelve a empezar
        if ( isblank(aux_s[0]) )
        {
            expr.ignore();
            continue;
        }

        // Si es el comienzo de un número, se intenta leerlo
        if ( is_number_start(aux_s) )
        {
            // Si no se logra, se sale con un error
            if ( !(expr >> aux_n) )
            {
                cerr << "Error: wrong input number in the expression."
                     << endl;
                exit(1);
            }

            // Si se logra, se agrega a la cola de salida
            output.enqueue(token(aux_n));
        }

        // Si no, puede tratarse de un operador o un paréntesis
        else if ( is_operator(aux_s) || is_parenthesis(aux_s) )
        {
            // Ya es algo válido, se elimina del stream
            expr.ignore();

            // Caso especial, "-" como operador unario, se agrega un 0 antes
            if ( aux_s == "-" && check_for_unary_op(output) )
                output.enqueue(token(0));

            // Caso especial, "+" como operador unario, se ignora
            if ( aux_s == "+" && check_for_unary_op(output) )
                continue;

            output.enqueue(token(aux_s));
        }

        // O si no, queda el grupo alfabético de output: funciones o especiales
        else
        {
            // Se levantan todos los caracteres alfabéticos a la cadena auxiliar
            expr.ignore(); // El primero ya está en la cadena, se ignora
            while ( isalpha(expr.peek()) ) aux_s += expr.get();

            // Si coincide con alguno de estos, se encola
            if ( is_function(aux_s) || is_special(aux_s) )
            {
                output.enqueue(token(aux_s));
            }

            // Si no es así, y tampoco se terminó la entrada, hay un error
            else if (expr)
            {
                cerr << "Error: malformed expression near of: "
                     << aux_s
                     << "."
                     << endl;
                exit(1);
            }
        }
    }
}



/*|/////////////////////////////////|   3)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////| Conversión a notación polaca inversa |\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void convert_to_RPN(stack<token> &result, queue<token> &tokens){

    token tok;
    stack<token> aux; // Pila auxiliar para la conversión

    bool flagExpectingOperator = false;
    bool flagExpectingNumber = false;
    bool flagExpectingFunction = true;

    while (!tokens.isEmpty()) {

        tok = tokens.dequeue();

        //Si el token es un operador, o1, entonces:
        if (tok.isOperator()){

            if (!flagExpectingOperator)
                error_handler_unexpected_token(tok);

            /*
            mientras que haya un operador, o2, en el tope de la pila (esto
            excluye el paréntesis abierto), y
                * o1 es asociativo izquierdo y su precedencia es menor que (una
                precedencia más baja) o igual a la de o2, ó
                * o1 es asociativo derecho y su precedencia es menor que (una
                precedencia más baja) que la de o2,
            retire (pop) de la pila el o2, y póngalo en la cola de salida.
            */
            while ( !aux.isEmpty() &&
                    (  aux.topElement().isOperator() ||
                       aux.topElement().isFunction()  )   &&
                     aux.topElement().precedence() >= tok.precedence() ) {

                result.push(aux.topElement());
                aux.pop();
            }

            //ponga (push) o1 en el tope de la pila.
            aux.push(tok);

            flagExpectingOperator = false;
            flagExpectingFunction = true;
            flagExpectingNumber = true;

        } else if (tok.isFunction()){

            if (!flagExpectingFunction)
                error_handler_unexpected_token(tok);

            while ( !aux.isEmpty() && aux.topElement().isOperator() &&
                    aux.topElement().precedence() >= tok.precedence() ) {

                result.push(aux.topElement());
                aux.pop();
            }

            //ponga (push) o1 en el tope de la pila.
            aux.push(tok);

            flagExpectingOperator = false;
            flagExpectingFunction = false;
            flagExpectingNumber = false;

        //Si el token es un paréntesis abierto, entonces póngalo en la pila.
        } else if (tok.isOpenParenthesis()) {

            //si esperaba un operador
            if (flagExpectingOperator)
                error_handler_unexpected_token(tok);

            aux.push(tok);

            flagExpectingNumber = false;

        //Si el token es un paréntesis derecho
        } else if (tok.isClosedParenthesis()) {

            //esto debe aparecer después de un numero/función, no de un operador
            if (!flagExpectingOperator)
                error_handler_unexpected_token(tok);

            /*Hasta que el token en el tope de la pila sea un paréntesis
            abierto, retire (pop) a los operadores de la pila y colóquelos
            en la cola de salida.*/
            while ( !aux.isEmpty() &&
                    !aux.topElement().isOpenParenthesis() ) {

                result.push(aux.topElement());
                aux.pop();
            }

            //Si la pila se termina sin encontrar un paréntesis abierto,
            //entonces hay paréntesis sin pareja.
            if (aux.isEmpty())
                error_handler_mismatched_parentheses();

            //Retire (pop) el paréntesis abierto de la pila,
            //pero no lo ponga en la cola de salida.
            aux.pop();

            /* Ahora esperamos un operador */
            flagExpectingOperator = true;
            flagExpectingFunction = true;
            flagExpectingNumber = false;

        //encuentre un numero
        } else if (tok.isValue() || tok.isSpecial()) {

            /* If we're expecting an operator, we're very disappointed. */
            if (flagExpectingOperator && !flagExpectingNumber)
                error_handler_unexpected_token(tok);

            //Si el token es un número, se agrega a la cola de salida
            result.push(tok);

            flagExpectingOperator = true;
            flagExpectingFunction = false;
            flagExpectingNumber = false;

        }
    }

    //ya se parsearon todos los tokens. Esperamos un operador
    //ya que lo ultimo fue un valor
    if (!flagExpectingOperator)
        error_handler_unexpected_token(tok);

    /*
    Cuando no hay más tokens para leer:
        Mientras todavía haya tokens de operadores en la pila:
            Si el token del operador en el tope de la pila es un paréntesis,
            entonces hay paréntesis sin la pareja correspondiente.
            retire (pop) al operador y póngalo en la cola de salida.
    */
    while (!aux.isEmpty()) {
        if (aux.topElement().isOpenParenthesis())
            error_handler_mismatched_parentheses();

        result.push(aux.topElement());
        aux.pop();
    }

}

// Extra: manejador de error en caso de token inesperado
void error_handler_unexpected_token(const token &t) {
    cerr << "Error: invalid syntax in the expression, near token: "
         << t
         << "."
         << endl;
    exit(1);
}

// Extra: manejador de error en caso de paréntesis desbalanceado
void error_handler_mismatched_parentheses() {
    cerr << "Error: mismatched parentheses in the expression."
         << endl;
    exit(1);
}
