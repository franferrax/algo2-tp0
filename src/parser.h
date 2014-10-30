#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <limits>
#include "queue.h"

using namespace std;


/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||| Token ||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

#define NAN            (numeric_limits<double>::quiet_NaN())
#define TOKEN_IS_VALUE "0v0"

class token
{
    double _v; // Valor, vale NaN si no es del tipo 'valor'
    string _s; // Cadena del token, vale TOKEN_IS_VALUE si es del tipo 'valor'


public:
    // 1) Constructores
    token(const double &v = 0); // De un token del tipo 'valor', 0 por defecto
    token(const string &);      // De un token que no es del tipo 'valor'
    token(const token &);       // Por copia

    // 2) Obtener como string (devuelve número en string si es del tipo 'valor')
    string getAsString() const;

    // 3) Obtener como double (devuelve NAN, si no es del tipo 'valor')
    double getAsDouble() const;

    // 4) Booleanos is...
    bool isValue() const;             // ¿Es del tipo 'valor'?
    bool isParenthesis() const;       // ¿Es un paréntesis?
    bool isOpenParenthesis() const;   // ¿Es un paréntesis abierto?
    bool isClosedParenthesis() const; // ¿Es un paréntesis cerrado?
    bool isOperator() const;          // ¿Es un operador?
    bool isSpecial() const;           // ¿Es especial?
    bool isFunction() const;          // ¿Es una función?

    // 5) Impresión en flujo/archivo/stdin
    friend ostream & operator<<(ostream &, const token &);
};



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||| Otros ||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

#define NOT_FOUND  -1

// Macros de función para cómoda detección de casos
#define is_number_start(s) ( isdigit((s)[0]) || (s) == "." )
#define is_parenthesis(s)  ( (s) == "(" || (s) == ")" )
#define is_operator(s)     ( find_in_list(operator_tokens, s) != NOT_FOUND )
#define is_special(s)      ( find_in_list(special_tokens,  s) != NOT_FOUND )
#define is_function(s)     ( find_in_list(function_tokens, s) != NOT_FOUND )


// Funciones a interpretar
static const string function_tokens[] =
{
    "exp",
    "ln",
    "re",
    "im",
    "abs",
    "phase",
    "sin",
    "cos",
// No olvidar centinela de cadena vacía
    ""
};

// Operadores a interpretar
static const string operator_tokens[] =
{
    "+",
    "-",
    "*",
    "/",
    "^",
// No olvidar centinela de cadena vacía
    ""
};

// Tokens especiales a interpretar
static const string special_tokens[] =
{
    "z",
    "j",
    "e",
    "pi",
// No olvidar centinela de cadena vacía
    ""
};



// 1) Función para buscar en una lista con centinela de cadena vacía
int find_in_list(const string [], const string &);

// 2) Función para parsear la expresión de entrada partiéndola en tokens
void parse_expression_in_tokens(const string &, queue<token> &);

#endif /* PARSER_H */
