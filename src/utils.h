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
#include <sstream>
#include <cstdlib>
#include "complejo.h"
#include "cmdline.h"
#include "queue.h"
#include "parser.h"

using namespace std;

//////////////////////// Variables globales de main.cpp ////////////////////////
extern option_t options[];            // Opciones CLA
extern istream *iss;                  // Puntero a stream de entrada
extern ostream *oss;                  // Puntero a stream de salida
extern fstream ifs;                   // Archivo de entrada
extern fstream ofs;                   // Archivo de salida
extern char *prog_name;               // Nombre del programa
extern double map_w;                  // Ancho de la región de mapeo
extern double map_h;                  // Alto de la región de mapeo
extern complejo map_c;                // Centro de la región de mapeo
extern queue<token> rpn_expr;         // Expresión convertida a RPN



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*|||||||||||||||||||||||||||||||| Utilidades ||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

// 1) CLA: Archivo de entrada
void opt_input(const string &);

// 2) CLA: Archivo de salida
void opt_output(const string &);

// 3) CLA: Función a aplicar
void opt_function(const string &);

// 4) CLA: Región del plano complejo
void opt_region(const string &);

// 5) CLA: Ayuda
void opt_help(const string &);

// 6) Obtener complejo asociado a los índices
void getComplexFromIndex(complejo &, size_t, size_t, size_t, size_t);

// 7) Obtener la fila asociada al complejo ( [i][ ] )
size_t getRowFromComplex(const complejo &, size_t);

// 8) Obtener la columna asociada al complejo ( [ ][j] )
size_t getColFromComplex(const complejo &, size_t);

// 9) Conversión a notación polaca inversa
void convertToRPN(queue<token> &, queue<token> &);
int precedenceOf(const string &);
void errorHandlerUnexpectedToken(const token &);
void errorHandlerMismatchedParentheses();

// 10) Calculo y resultado de la operación en notación polaca inversa
complejo calculateRPN(queue<string> *);

#endif    /* UTILS_H */
