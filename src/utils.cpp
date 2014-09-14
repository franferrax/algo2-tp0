/*
 * File:   utils.cpp
 * Author: juanzaragoza
 *
 * Created on 13 de septiembre de 2014, 14:54
 */

#include "utils.h"

const string transforms_opts[] =
{
    "z",
    "exp(z)",
    "z^2",
    "z^3",
    "sin(z)"
};

#define N_FUNCS 5

/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////| Validar argumentos en la línea de comandos |\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void validateCommandLineArguments(int argc, char** argv, string &input,
                                  string &output, size_t &function){

    string aux;
    string pgmExtension = ".pgm";

    function = 0; // Default

    if(argc <= 1){

        input = "cin";
        output = "cout";

    } else{

        for(int i=1; i < argc; i++){

            if(strcmp(argv[i],"-i")==0){

                if(i+1 < argc){

                    aux = argv[i+1];

                    if(aux.length() >= pgmExtension.length() &&
                       aux.compare(aux.length() - pgmExtension.length(),
                                   pgmExtension.length(), pgmExtension) == 0){
                        input = aux;
                    } else{
                        input = "cin";
                    }

                } else{
                    input = "cin";
                }

            } else if(strcmp(argv[i],"-o")==0){

                if(i+1 < argc){

                    aux = argv[i+1];

                    if(aux.length() >= pgmExtension.length() &&
                       aux.compare(aux.length() - pgmExtension.length(),
                                   pgmExtension.length(), pgmExtension) == 0){
                        output = aux;
                    } else{
                        output = "cout";
                    }

                } else{
                    output = "cout";
                }

            } else if(strcmp(argv[i],"-f")==0){

                if(i+1 < argc){

                    aux = argv[i+1];

                    for(size_t k=1; k < N_FUNCS; k++)
                    {
                        if(transforms_opts[k] == aux)
                            function = k;
                    }
                }
            }

        }

    }

    if(input == "") input = "cin";
    if(output == "") output = "cout";
}



/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////| Obtener complejo asociado a los índices |\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void setComplexFromIndex (complejo &z, size_t i, size_t j,
                          size_t h, size_t w)
{
    if ( h && w && i < h && j < w)
    {
        z.SetReal ( MAP_X * ( ((double)j + 0.5) / (double)w  -  0.5 ) );
        z.SetImag ( MAP_Y * ( 0.5  -  ((double)i + 0.5) / (double)h ) );
    }
}



/*|/////////////////////////////////|   3)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////| Obtener la fila asociada al complejo ( [i][ ] ) |\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t getRowFromComplex (const complejo &z, size_t h)
{
    return h * ( 0.5 - z.GetImag () / MAP_Y );
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////| Obtener la columna asociada al complejo ( [ ][j] ) \\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t getColFromComplex (const complejo &z, size_t w)
{
    return w * ( 0.5 + z.GetReal () / MAP_X );
}



/*|/////////////////////////////////|   5)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
