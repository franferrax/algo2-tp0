/*
 * File:   utils.cpp
 * Author: juanzaragoza
 *
 * Created on 13 de septiembre de 2014, 14:54
 */
#include "utils.h"



/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////////| CLA: Archivo de entrada |\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void opt_input(string const &arg)
{
    // Por defecto stdin, o bien archivo
    if (arg == "-")
    {
        iss = &cin;
    }
    else
    {
        ifs.open(arg.c_str(), ios::in);
        iss = &ifs;
    }

    // Comprobación de errores
    if ( !iss->good() )
    {
        cerr << "Cannot open "
             << arg
             << "."
             << endl;
        exit(1);
    }
}



/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////////| CLA: Archivo de salida |\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void opt_output(string const &arg)
{
    // Por defecto stdout, o bien archivo
    if (arg == "-")
    {
        oss = &cout;
    }
    else
    {
        ofs.open(arg.c_str(), ios::out);
        oss = &ofs;
    }

    // Comprobación de errores
    if ( !oss->good() )
    {
        cerr << "Cannot open "
             << arg
             << "."
             << endl;
        exit(1);
    }
}



/*|/////////////////////////////////|   3)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////////| CLA: Función a aplicar |\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void opt_function(string const &arg)
{
    for (size_t i=0; functions_opts[i]!=""; i++)
    {
        // Si se encuentra la función en la lista de las disponibles
        if(arg == functions_opts[i])
        {
            complex_function = functions_ptrs[i];
            return;
        }
    }

    // Si no se retornó antes, el parámetro es inválido
    cerr << "Invalid function parameter: "
         << arg
         << "."
         << endl;
    exit(1);
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////////////| CLA: Ayuda |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void opt_help(string const &arg)
{
    cout << "Usage: "
         << prog_name
         << " [-i file] [-o file] [-f function]"
         << endl
         << "Function list: "
         << functions_opts[0];

    for (size_t i=1; functions_opts[i]!=""; i++)
        cout << ", "
             << functions_opts[i];

    cout << "."
         << endl;
    exit(0);
}



/*|/////////////////////////////////|   5)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////| Obtener complejo asociado a los índices |\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void getComplexFromIndex(complejo &z, size_t i, size_t j,
                         size_t h, size_t w)
{
    if ( h && w && i < h && j < w)
    {
        z.SetReal( MAP_X * ( ((double)j + 0.5) / (double)w  -  0.5 ) );
        z.SetImag( MAP_Y * ( 0.5  -  ((double)i + 0.5) / (double)h ) );
    }
}



/*|/////////////////////////////////|   6)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////| Obtener la fila asociada al complejo ( [i][ ] ) |\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t getRowFromComplex(const complejo &z, size_t h)
{
    return h * ( 0.5 - z.GetImag() / MAP_Y );
}



/*|/////////////////////////////////|   7)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////| Obtener la columna asociada al complejo ( [ ][j] ) \\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t getColFromComplex(const complejo &z, size_t w)
{
    return w * ( 0.5 + z.GetReal() / MAP_X );
}
