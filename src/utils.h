/*
 * File:   utils.h
 * Author: juanzaragoza
 *
 * Created on 13 de septiembre de 2014, 14:54
 */

#ifndef UTILS_H
#define	UTILS_H

#include <string>
#include <cstring>
#include <iostream>
#include "complejo.h"

// Porción del plano complejo a utilizar, ancho
// y alto del mapeo centrado en el origen
#define MAP_X 2
#define MAP_Y 2

// Tipo para los punteros a funcion
typedef void (complejo :: *transform_t)(const complejo&);

using namespace std;

// 1) Validar argumentos en la línea de comandos
void validateCommandLineArguments(int , char** , string &, string &, size_t &);

// 2) Obtener complejo asociado a los índices
void setComplexFromIndex (complejo &, size_t, size_t, size_t, size_t);

// 3) Obtener la fila asociada al complejo ( [i][ ] )
size_t getRowFromComplex (const complejo &, size_t);

// 4) Obtener la columna asociada al complejo ( [ ][j] )
size_t getColFromComplex (const complejo &, size_t);

#endif	/* UTILS_H */
