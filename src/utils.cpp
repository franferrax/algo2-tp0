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


//
///*|/////////////////////////////////|   9)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
///*|/////////////////| Conversión a notación polaca inversa |\\\\\\\\\\\\\\\\\|*/
///*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
//void convertToRPN1(queue<string> &result,const string &expr){
//
//    string caux, var;
//    double aux;
//    stack<string> container;
//    bool flagExpectingOperator = false;
//    bool flagExpectingNumber = false;
//    bool flagExpectingFunction = false;
//
//    for (size_t i = 0; i < expr.length(); ++i) {
//
//        caux = expr.at(i);
//        stringstream arg_stream_number(caux);
//
//        if (caux==" "){ //vacio. No hago nada
//
//            continue;
//
//        //Si el token es un operador, o1, entonces:
//        } else if (isOperator(caux)){
//
//            if (!flagExpectingOperator){
//                //devolver algo mas interesante
//                cout<<"Error de parametro"<<endl;
//                exit(1);
//            }
//
//            /*
//            mientras que haya un operador, o2, en el tope de la pila (esto
//            excluye el paréntesis abierto), y
//                * o1 es asociativo izquierdo y su precedencia es menor que (una
//                precedencia más baja) o igual a la de o2, ó
//                * o1 es asociativo derecho y su precedencia es menor que (una
//                precedencia más baja) que la de o2,
//            retire (pop) de la pila el o2, y póngalo en la cola de salida.
//            */
//            while (!container.isEmpty() && isOperator(container.topElement()) &&
//                   precedenceOf(container.topElement()) >= precedenceOf(caux)) {
//                var = container.topElement();
//                result.enqueue(var);
//                container.pop();
//            }
//
//            //ponga (push) o1 en el tope de la pila.
//            container.push(caux);
//
//            //Ya no estamos esperando un operador; que acabamos de encontrar uno
//            flagExpectingOperator = false;
//            flagExpectingNumber = false;
//            flagExpectingFunction = false;
//
//        //Si el token es un paréntesis abierto, entonces póngalo en la pila.
//        } else if (caux == "(" && !flagExpectingFunction) {
//            //si esperaba un operador
//            if (flagExpectingOperator){
//                //devolver algo mas interesante
//                cout<<"Esperabamos un operador, no un parentesis ("<<endl;
//                exit(1);
//            }
//            container.push(caux);
//
//            flagExpectingNumber = false;
//            flagExpectingFunction = false;
//        //Si el token es un paréntesis derecho
//        } else if (caux == ")" && !flagExpectingFunction) {
//            //esto debe aparecer despues de un numero/función, no de un operador
//            if (!flagExpectingOperator){
//                //devolver algo mas interesante
//                cout<<"Esperabamos un valor, no un parentesis )"<<endl;
//                exit(1);
//            }
//
//            /*Hasta que el token en el tope de la pila sea un paréntesis
//            abierto, retire (pop) a los operadores de la pila y colóquelos
//            en la cola de salida.*/
//            while (!container.isEmpty() && container.topElement() != "(") {
//                var = container.topElement();
//                result.enqueue(var);
//                container.pop();
//            }
//
//            //Si la pila se termina sin encontrar un paréntesis abierto,
//            //entonces hay paréntesis sin pareja.
//            if (container.isEmpty()){
//                //devolver algo mas interesante
//                cout<<"Falta un parentesis"<<endl;
//                exit(1);
//            }
//
//            //Retire (pop) el paréntesis abierto de la pila,
//            //pero no lo ponga en la cola de salida.
//            container.pop();
//
//            /* Ahora esperamos un operador */
//            flagExpectingOperator = true;
//            flagExpectingNumber = false;
//            flagExpectingFunction = false;
//
//        //encuentre un numero
//        } else if((arg_stream_number >> aux) || caux == "i") {
//            /* If we're expecting an operator, we're very disappointed. */
//            if (flagExpectingOperator && !flagExpectingNumber){
//                //devolver algo mas interesante
//                cout<<"Se esperaba un operador, se encontró "<<caux<<endl;
//                exit(1);
//            }
//
//            if(flagExpectingNumber){
//                result.frontElement() += caux;
//            } else{
//                //Si el token es un número, se agrega a la cola de salida
//                result.enqueue(caux);
//            }
//
//            flagExpectingOperator = true;
//            flagExpectingNumber = true;
//            flagExpectingFunction = false;
//
//        } else if(caux == "z" && !flagExpectingFunction){ //encontré una z
//
//            if (flagExpectingOperator){
//                //devolver algo mas interesante
//                cout<<"Esperabamos un operador, no una z"<<endl;
//                exit(1);
//            }
//
//            result.enqueue(caux);
//
//            flagExpectingOperator = true;
//            flagExpectingNumber = false;
//            flagExpectingFunction = false;
//
//        //si espero una parte de la función o es una función
//        } else if(flagExpectingFunction || isPartOfFunction(caux) ){
//
//            if(flagExpectingFunction){
//                result.frontElement() = result.frontElement() + caux;
//            } else if(isPartOfFunction(caux)){
//                result.enqueue(caux);
//            }
//
//            //si es una función, espero un operador
//            if(isFunction(result.frontElement())){
//                flagExpectingOperator = true;
//                flagExpectingNumber = false;
//                flagExpectingFunction = false;
//            //sino espero una funcion
//            } else{
//                flagExpectingOperator = false;
//                flagExpectingNumber = false;
//                flagExpectingFunction = true;
//            }
//
//        } else{
//
//            cout<<"Carácter no reconocido: "<<caux<<endl;
//            exit(1);
//
//        }
//    }
//
//    //ya se parseó todo el string. Esperamos un operador
//    //ya que lo ultimo fue un valor
//    if (!flagExpectingOperator){
//        cout<<"Se esperaba un valor"<<endl;
//        exit(1);
//    }
//
//    /*
//    Cuando no hay más tokens para leer:
//        Mientras todavía haya tokens de operadores en la pila:
//            Si el token del operador en el tope de la pila es un paréntesis,
//            entonces hay paréntesis sin la pareja correspondiente.
//            retire (pop) al operador y póngalo en la cola de salida.
//    */
//    while (!container.isEmpty()) {
//        if (container.topElement() == "("){
//            cout<<"Parentesis desbalanceados"<<endl;
//            exit(1);
//        }
//        var = container.topElement();
//        result.enqueue(var);
//        container.pop();
//    }
//
//    //return result;
//
//}

/*|/////////////////////////////////|   9)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////| Conversión a notación polaca inversa |\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void convertToRPN(queue<string> &result, queue<token> &tokens){

    string caux, var;
    //double aux;
    token caracter;
    
    stack<string> container;
    
    bool flagExpectingOperator = false;
    bool flagExpectingNumber = false;
    bool flagExpectingFunction = true;

    while(!tokens.isEmpty()) {

        caracter = tokens.dequeue();

        //Si el token es un operador, o1, entonces:
        if(caracter.isOperator()){
            
            if (!flagExpectingOperator){
                //devolver algo mas interesante
                cout<<"Error de parametro"<<endl;
                exit(1);
            }

            /*
            mientras que haya un operador, o2, en el tope de la pila (esto
            excluye el paréntesis abierto), y
                * o1 es asociativo izquierdo y su precedencia es menor que (una
                precedencia más baja) o igual a la de o2, ó
                * o1 es asociativo derecho y su precedencia es menor que (una
                precedencia más baja) que la de o2,
            retire (pop) de la pila el o2, y póngalo en la cola de salida.
            */
            while (!container.isEmpty() && isOperator(container.topElement()) &&
                   precedenceOf(container.topElement()) >= precedenceOf(caracter.getAsString())) {
                var = container.topElement();
                result.enqueue(var);
                container.pop();
            }

            //ponga (push) o1 en el tope de la pila.
            container.push(caracter.getAsString());
            
            flagExpectingOperator = false;
            flagExpectingFunction = true;
            flagExpectingNumber = true;
            


        } else if(caracter.isFunction()){
            
            if (!flagExpectingFunction){
                //devolver algo mas interesante
                cout<<"Error de parametro"<<endl;
                exit(1);
            }
            
            while (!container.isEmpty() && isOperator(container.topElement()) &&
                   precedenceOf(container.topElement()) >= precedenceOf(caracter.getAsString())) {
                var = container.topElement();
                result.enqueue(var);
                container.pop();
            }

            //ponga (push) o1 en el tope de la pila.
            container.push(caracter.getAsString());
            
            flagExpectingOperator = false;
            flagExpectingFunction = false;
            flagExpectingNumber = false;
            
        //Si el token es un paréntesis abierto, entonces póngalo en la pila.
        } else if (caracter.getAsString() == "(") {
            //si esperaba un operador
            if (flagExpectingOperator){
                //devolver algo mas interesante
                cout<<"Esperabamos un operador, no un parentesis ("<<endl;
                exit(1);
            }
            container.push(caracter.getAsString());

            flagExpectingNumber = false;

        //Si el token es un paréntesis derecho
        } else if (caracter.getAsString() == ")") {
            //esto debe aparecer despues de un numero/función, no de un operador
            if (!flagExpectingOperator){
                //devolver algo mas interesante
                cout<<"Esperabamos un valor, no un parentesis )"<<endl;
                exit(1);
            }

            /*Hasta que el token en el tope de la pila sea un paréntesis
            abierto, retire (pop) a los operadores de la pila y colóquelos
            en la cola de salida.*/
            while (!container.isEmpty() && container.topElement() != "(") {
                var = container.topElement();
                result.enqueue(var);
                container.pop();
            }

            //Si la pila se termina sin encontrar un paréntesis abierto,
            //entonces hay paréntesis sin pareja.
            if (container.isEmpty()){
                //devolver algo mas interesante
                cout<<"Falta un parentesis"<<endl;
                exit(1);
            }

            //Retire (pop) el paréntesis abierto de la pila,
            //pero no lo ponga en la cola de salida.
            container.pop();

            /* Ahora esperamos un operador */
            flagExpectingOperator = true;
            flagExpectingFunction = true;
            flagExpectingNumber = false;

        //encuentre un numero
        } else if(caracter.isValue() || caracter.getAsString() == "i") {
            /* If we're expecting an operator, we're very disappointed. */
            if (flagExpectingOperator && !flagExpectingNumber){
                //devolver algo mas interesante
                cout<<"Se esperaba un operador, se encontró "<<caux<<endl;
                exit(1);
            }

            //Si el token es un número, se agrega a la cola de salida
            result.enqueue(caracter.getAsString());

            flagExpectingOperator = true;
            flagExpectingFunction = true;
            flagExpectingNumber = true;

        } else if(caracter.getAsString() == "z" || caracter.getAsString() == "j"){ //encontré una z

            if (flagExpectingOperator){
                //devolver algo mas interesante
                cout<<"Esperabamos un operador, no una z"<<endl;
                exit(1);
            }

            result.enqueue(caracter.getAsString());

            flagExpectingOperator = true;
            flagExpectingFunction = true;
            flagExpectingNumber = false;

        } else{
            
            cout<<"Carácter no reconocido: "<<caracter.getAsString()<<endl;
            exit(1);

        }
    }

    //ya se parseó todo el string. Esperamos un operador
    //ya que lo ultimo fue un valor
    if (!flagExpectingOperator){
        cout<<"Se esperaba un valor"<<endl;
        exit(1);
    }

    /*
    Cuando no hay más tokens para leer:
        Mientras todavía haya tokens de operadores en la pila:
            Si el token del operador en el tope de la pila es un paréntesis,
            entonces hay paréntesis sin la pareja correspondiente.
            retire (pop) al operador y póngalo en la cola de salida.
    */
    while (!container.isEmpty()) {
        if (container.topElement() == "("){
            cout<<"Parentesis desbalanceados"<<endl;
            exit(1);
        }
        var = container.topElement();
        result.enqueue(var);
        container.pop();
    }

    //return result;

}

bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" ||
           token == "/" || token == "^" || isFunction(token);
}

/*bool isPartOfFunction(const string& token) {

    for (size_t i=0; !function_tokens[i].empty(); i++) {

        if (function_tokens[i].find(token) == 0) {

            return true;
        }
    }

    return false;
}*/

bool isFunction(const string& token) {

    for (size_t i=0; !function_tokens[i].empty(); i++){

        if (function_tokens[i].compare(token) == 0) {

            return true;
        }
    }

    return false;
}

int precedenceOf(const string& token) {
    if (token == "+" || token == "-") return 0;
    if (token == "*" || token == "/") return 1;
    if (token == "^") return 2;
    if(isFunction(token)) return 3;
    return -1;
}



/*|/////////////////////////////////|  10)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////| Cálculo y resultado de la operación en notación polaca inversa |\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
complejo calculateRPN(queue<string>* rpn){

    stack<string> *container = new stack<string>();
    string caux;
    double op1, op2, result=0;
    complejo c;

    while(!rpn->isEmpty()){
        caux = rpn->dequeue();

        if(isOperator(caux)){ //si es un operador

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
