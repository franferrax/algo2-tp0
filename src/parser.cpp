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
// NOTA: si bien es capaz de devolver el valor en una string no utilizar
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

            // Si se logra, se agrega a la cola de output
            output.enqueue(token(aux_n));
        }

        // Si no, puede tratarse de un operador o un paréntesis
        else if ( is_operator(aux_s) || is_parenthesis(aux_s) )
        {
            // Caso especial, "-" como operador unario, se agrega un 0
            if ( aux_s == "-" &&
                 (output.isEmpty() || output.lastAdded().isOpenParenthesis()) )
                output.enqueue(token(0));

            expr.ignore();
            output.enqueue(token(aux_s));
        }

        // O si no, queda el grupo alfabético de output: funciones o especiales
        else
        {
            expr.ignore();

            // Se levantan todos los caracteres alfabéticos a la cadena
            while ( isalpha(expr.peek()) )
                aux_s += expr.get();

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
