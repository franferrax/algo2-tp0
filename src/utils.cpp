/*
 * File:   utils.cpp
 * Author: juanzaragoza
 *
 * Created on 13 de septiembre de 2014, 14:54
 */
#include "utils.h"
#include "stack.h"
#include "queue.h"
#include <string>



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

/*|/////////////////////////////////|   8)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////| Conversion a notacion polaca \\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
std::string convertToRPN(const std::string &expr){
    
    char caux;
    std::string result;
    stack<char> container;
    bool flagExpectingOperator = false;
    
    for (size_t i = 0; i < expr.length(); ++i) {
        
        caux = expr.at(i);
        
        if(caux==' '){ //vacio. No hago nada
            
            continue;
            
        } else if(isOperator(caux)){//Si el token es un operador, o1, entonces:
            
            if (!flagExpectingOperator){
                //devolver algo mas interesante
                cout<<"Error de parametro"<<endl;
                exit(1);
            }

            /*
            mientras que haya un operador, o2, en el tope de la pila (esto excluye el paréntesis abierto), y
                o1 es asociativo izquierdo y su precedencia es menor que (una precedencia más baja) o igual a la de o2, ó
                o1 es asociativo derecho y su precedencia es menor que (una precedencia más baja) que la de o2,
            retire (pop) de la pila el o2, y póngalo en la cola de salida;  
             */
            while (!container.isEmpty() && isOperator(container.topElement()) &&
                   precedenceOf(container.topElement()) >= precedenceOf(caux)) {
                result.append(1u,container.topElement());
                container.pop();
            }

            //ponga (push) o1 en el tope de la pila.
            container.push(caux);

            //Ya no estamos esperando un operador; que acabamos de encontrar uno 
            flagExpectingOperator = false;
            
        } else if (caux == '(') { //Si el token es un paréntesis abierto, entonces póngalo en la pila.
            //si esperaba un operador
            if (flagExpectingOperator){
                //devolver algo mas interesante
                cout<<"Esperabamos un operador, no un parentesis ("<<endl;
                exit(1);
            }
            container.push(caux);
        } else if (caux == ')') { //Si el token es un paréntesis derecho
            //esto debe aparecer despues de un numero/funcion, no de un operador
            if (!flagExpectingOperator){
                //devolver algo mas interesante
                cout<<"Esperabamos un valor, no un parentesis )"<<endl;
                exit(1);
            }

            /*Hasta que el token en el tope de la pila sea un paréntesis abierto, retire (pop) a los 
            operadores de la pila y colóquelos en la cola de salida.*/
            while (!container.isEmpty() && container.topElement() != '(') {
                result.append(1u,container.topElement());
                container.pop();
            }
            
            //Si la pila se termina sin encontrar un paréntesis abierto, entonces hay paréntesis sin pareja.
            if (container.isEmpty()){
                //devolver algo mas interesante
                cout<<"Falta un parentesis"<<endl;
                exit(1);
            }

            //Retire (pop) el paréntesis abierto de la pila, pero no lo ponga en la cola de salida.
            container.pop();

            /* Ahora esperamos un operador */
            flagExpectingOperator = true;
            
        } else { //encuentre un numero
            /* If we're expecting an operator, we're very disappointed. */
            if (flagExpectingOperator){
                //devolver algo mas interesante
                cout<<"Se esperaba un operador, se encontró "<<caux<<endl;
                exit(1);
            }

            //Si el token es un número, entonces agregúelo a la cola de salida
            result.append(1u,caux);
            flagExpectingOperator = true;
            
        }
    }
    
    //ya se parseo todo el string. Esperamos un operador ya que lo ultimo fue un valor
    if (!flagExpectingOperator){
        cout<<"Se esperaba un valor"<<endl;
        exit(1);
    }
    
    /*  
    Cuando no hay más tokens para leer:
        Mientras todavía haya tokens de operadores en la pila:
            Si el token del operador en el tope de la pila es un paréntesis, entonces hay paréntesis sin la pareja correspondiente.
            retire (pop) al operador y póngalo en la cola de salida.
    */
    while (!container.isEmpty()) {
        if (container.topElement() == '('){
            cout<<"Parentesis desbalanceados"<<endl;
            exit(1);
        }
        result.append(1u,container.topElement());
        container.pop();
    }
    
    return result;

}

bool isOperator(const char& token) {
    return token == '+' || token == '-' || token == '*' || token == '/' || token == '%';
}

int precedenceOf(const char& token) {
    if (token == '+' || token == '-') return 0;
    if (token == '*' || token == '/' || token == '%') return 1;
    return -1;
}