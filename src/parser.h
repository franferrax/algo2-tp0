#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#define _USE_MATH_DEFINES // Constantes matemáticas al cargar <cmath>
#include <cmath>
#include <string>
#include "queue.h"
#include "stack.h"
#include "complex.h"

using namespace std;


#define NOT_FOUND  -1

// Macros de función para cómoda detección de casos
#define is_number_start(s) ( isdigit((s)[0]) || (s) == "." )
#define is_parenthesis(s)  ( (s) == "(" || (s) == ")" )
#define is_operator(s)     ( find_in_list(operator_tokens_, s) != NOT_FOUND )
#define is_special(s)      ( find_in_list(special_tokens_,  s) != NOT_FOUND )
#define is_function(s)     ( find_in_list(function_tokens_, s) != NOT_FOUND )

// Chequeo de +/- como operador unario: si es el primero o viene luego de '('
#define check_for_unary_op(q) ((q).isEmpty() || \
                               (q).lastAdded().isOpenParenthesis())

// Punteros a función para las operaciones
typedef const complex (*operator_t)(const complex &, const complex &);
typedef const complex (*function_t)(const complex &);



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||| Token ||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

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

    // 5) Precedencia, si es que el token es un operador o función
    int precedence() const;

    // 6) Impresión en flujo/archivo/stdin
    friend ostream & operator<<(ostream &, const token &);
};



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||| Variables especiales para parseo y evaluación de la expresión ||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/


/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////| Funciones a interpretar, operaciones unarias |\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/

// Cadenas asociadas, el orden importa
const string function_tokens_[] =
{
    "exp",
    "ln",
    "sin",
    "cos",
    "re",
    "im",
    "abs",
    "phase",
// No olvidar centinela de cadena vacía
    ""
};

// Punteros a funciones asociados, el orden importa
const function_t function_pointers_[] =
{
    complex::exp,
    complex::log,
    complex::sin,
    complex::cos,
    complex::real_as_complex,
    complex::imag_as_complex,
    complex::abs_as_complex,
    complex::phase_as_complex
};

/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////| Operadores a interpretar, operaciones binarias |\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/

// Cadenas asociadas, el orden importa
const string operator_tokens_[] =
{
    "+",
    "-",
    "*",
    "/",
    "^",
// No olvidar centinela de cadena vacía
    ""
};

// Punteros a funciones asociados, el orden importa
const operator_t operator_pointers_[] =
{
    complex::operator_add,
    complex::operator_subt,
    complex::operator_mult,
    complex::operator_div,
    complex::operator_pow
};


/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////| Tokens especiales a interpretar |\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/

#define PIXEL_TOKEN "z" // Token simbólico que representa el pixel a transformar
// Cadenas asociadas, el orden importa, PIXEL_TOKEN debe ir último
const string special_tokens_[] =
{
    "j",
    "e",
    "pi",
    PIXEL_TOKEN,
// No olvidar centinela de cadena vacía
    ""
};

// Complejos asociados a tokens especiales, excepto z, el orden importa
const complex special_complex_[] =
{
    complex(0, 1),       // j
    complex(M_E, 0),     // e
    complex(M_PI, 0)     // pi
};



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*|||||||||||||||||||||||||||||||| Utilidades ||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

// 1) Función para buscar en una lista con centinela de cadena vacía
int find_in_list(const string [], const string &);

// 2) Función para parsear la expresión de entrada partiéndola en tokens
void parse_expression_in_tokens(const string &, queue<token> &);

// 3) Conversión a notación polaca inversa
void convert_to_RPN(stack<token> &, queue<token> &);
void error_handler_unexpected_token(const token &);
void error_handler_mismatched_parentheses();


#endif /* PARSER_H */
