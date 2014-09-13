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

using namespace std;

void validateCommandLineArguments(int , char** , string &, string &, string &);

complejo getComplexFromIndex(int , int , int , int );

#endif	/* UTILS_H */

