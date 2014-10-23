/*
 * File:   utils.h
 * Author: juanzaragoza
 *
 * Created on 13 de septiembre de 2014, 14:54
 */
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "complejo.h"
#include "cmdline.h"
using namespace std;


// Porción del plano complejo a utilizar
#define MAP_X 2  // Ancho del mapeo centrado en el origen
#define MAP_Y 2  // Alto del mapeo centrado en el origen


// Tipo para los punteros a función compleja
typedef void (complejo::*function_t)(const complejo&);


//////////////////////// Variables globales de main.cpp ////////////////////////
extern option_t options[];            // Opciones CLA
extern function_t complex_function;   // Puntero a función compleja
extern istream *iss;                  // Puntero a stream de entrada
extern ostream *oss;                  // Puntero a stream de salida
extern fstream ifs;                   // Archivo de entrada
extern fstream ofs;                   // Archivo de salida
extern char *prog_name;               // Nombre del programa


/////////////////// Configuraciones de la función a aplicar ////////////////////
static const string functions_opts[] = // Argumentos de la opción -f
{
    "z",
    "exp(z)",
    "z^2",
    "z^3",
    "sin(z)",
    "" // No olvidar centinela de cadena vacía
};

static const function_t functions_ptrs[] = // Punteros a funciones complejas
{
    &complejo::identidadDesde,
    &complejo::exponencialDesde,
    &complejo::cuadradoDesde,
    &complejo::cuboDesde,
    &complejo::senoDesde
};



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*|||||||||||||||||||||||||||||||| Utilidades ||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

// 1) CLA: Archivo de entrada
void opt_input(string const &);

// 2) CLA: Archivo de salida
void opt_output(string const &);

// 3) CLA: Función a aplicar
void opt_function(string const &);

// 4) CLA: Ayuda
void opt_help(string const &);

// 5) Obtener complejo asociado a los índices
void getComplexFromIndex(complejo &, size_t, size_t, size_t, size_t);

// 6) Obtener la fila asociada al complejo ( [i][ ] )
size_t getRowFromComplex(const complejo &, size_t);

// 7) Obtener la columna asociada al complejo ( [ ][j] )
size_t getColFromComplex(const complejo &, size_t);

std::string convertToRPN(const std::string &);
bool isOperator(const char& );
int precedenceOf(const char& );

#endif    /* UTILS_H */
