/*
 * File:   utils.cpp
 * Author: juanzaragoza
 *
 * Created on 13 de septiembre de 2014, 14:54
 */
#include "utils.h"
#include "stack.h"
#include "queue.h"
#include "parser.h"
#include <string>



/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////////| CLA: Archivo de entrada |\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void opt_input(const string &arg)
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
void opt_output(const string &arg)
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
void opt_function(const string &arg)
{
    queue<token> tokenized_expr;

    parse_expression_in_tokens(arg, tokenized_expr);
    convertToRPN(rpn_expr, tokenized_expr);
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////| CLA: Región del plano complejo |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void opt_region(const string &arg)
{
    double aux;
    stringstream arg_stream(arg);
    bool errors = true;

    // Lectura de los parámetros, ignorando el separador (',' o cualquier char)
    if (arg_stream >> map_w)
    {
        arg_stream.ignore();

        if (arg_stream >> map_h)
        {
            arg_stream.ignore();

            if (arg_stream >> aux)
            {
                map_c.SetReal(aux);
                arg_stream.ignore();

                if (arg_stream >> aux)
                {
                    map_c.SetImag(aux);

                    // Si se llegó hasta aquí, se pudieron leer los
                    // 4 parámetros, si hay algo más se ignora
                    errors = false;
                }
            }
        }
    }

    // Error de lectura, región inválida
    if (errors)
    {
        cerr << "Invalid region description: "
             << arg
             << "."
             << endl;
        exit(1);
    }

    // Error por ancho o alto no "positivos distintos de cero"
    if (map_w <= 0 || map_h <=0)
    {
        cerr << map_w << "," << map_h
             << ": must be positive nonzero numbers."
             << endl;
        exit(1);
    }

}



/*|/////////////////////////////////|   5)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////////////| CLA: Ayuda |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void opt_help(const string &arg)
{
    cout << "Usage: "
         << prog_name
         << " [-i file] [-o file] [-r w,h,x0,y0] [-f expression(z)]"
         << endl;
    exit(0);
}



/*|/////////////////////////////////|   6)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////| Obtener complejo asociado a los índices |\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void getComplexFromIndex(complejo &z, size_t i, size_t j,
                         size_t h, size_t w)
{
    if ( h && w && i < h && j < w)
    {
        z.SetReal( map_w * ( ((double)j + 0.5) / (double)w  -  0.5 ) );
        z.SetImag( map_h * ( 0.5  -  ((double)i + 0.5) / (double)h ) );
        z += map_c;
    }
}



/*|/////////////////////////////////|   7)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////| Obtener la fila asociada al complejo ( [i][ ] ) |\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t getRowFromComplex(const complejo &z, size_t h)
{
    return h * ( 0.5 - (z.GetImag()-map_c.GetImag()) / map_h );
}



/*|/////////////////////////////////|   8)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////| Obtener la columna asociada al complejo ( [ ][j] ) \\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t getColFromComplex(const complejo &z, size_t w)
{
    return w * ( 0.5 + (z.GetReal()-map_c.GetReal()) / map_w );
}



/*|/////////////////////////////////|   9)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////| Cálculo y resultado de la operación en notación polaca inversa |\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
complejo calculateRPN(queue<string>* rpn){

    stack<string> *container = new stack<string>();
    string caux;
    double op1, op2, result=0;
    complejo c;

    while(!rpn->isEmpty()){
        caux = rpn->dequeue();

        if(is_operator(caux)){ //si es un operador

            //TODO: Lo operandos deben ser complejos
            //op1 = atof(container->pop().c_str());
            //op2 = atof(container->pop().c_str());
            stringstream arg_stream_op1(container->pop());
            stringstream arg_stream_op2(container->pop());

            if(arg_stream_op1 >> op1 && arg_stream_op2 >> op2){
                //TODO: aplicar la funcion caux a op1 y op2
            }else{
                cout<<"Se esperaban operandos en la pila"<<endl;
                exit(1);
            }

        } else{ //sino es un numero
            container->push(caux);
        }
    }

    c.SetReal(result);
    return c;

}
