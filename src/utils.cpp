#include "utils.h"


/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////////| CLA: Archivo de entrada |\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void opt_input(const string &arg)
{
    // Por defecto stdin, o bien archivo
    if (arg == "-")
    {
        iss_ = &cin;
    }
    else
    {
        ifs_.open(arg.c_str(), ios::in);
        iss_ = &ifs_;
    }

    // Comprobación de errores
    if ( !iss_->good() )
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
        oss_ = &cout;
    }
    else
    {
        ofs_.open(arg.c_str(), ios::out);
        oss_ = &ofs_;
    }

    // Comprobación de errores
    if ( !oss_->good() )
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
    convert_to_RPN(rpn_expr_, tokenized_expr);
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
    if (arg_stream >> map_w_)
    {
        arg_stream.ignore();

        if (arg_stream >> map_h_)
        {
            arg_stream.ignore();

            if (arg_stream >> aux)
            {
                map_c_.setReal(aux);
                arg_stream.ignore();

                if (arg_stream >> aux)
                {
                    map_c_.setImag(aux);

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
    if (map_w_ <= 0 || map_h_ <=0)
    {
        cerr << map_w_ << "," << map_h_
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
         << prog_name_
         << " [-i file] [-o file] [-r w,h,x0,y0] [-f expression(z)]"
         << endl;
    exit(0);
}



/*|/////////////////////////////////|   6)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////| Obtener complejo asociado a los índices |\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void get_complex_from_index(complex &z, size_t i, size_t j,
                         size_t h, size_t w)
{
    if ( h && w && i < h && j < w)
    {
        z.setReal( map_w_ * ( ((double)j + 0.5) / (double)w  -  0.5 ) );
        z.setImag( map_h_ * ( 0.5  -  ((double)i + 0.5) / (double)h ) );
        z += map_c_;
    }
}



/*|/////////////////////////////////|   7)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////| Obtener la fila asociada al complejo ( [i][ ] ) |\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t get_row_from_complex(const complex &z, size_t h)
{
    return h * ( 0.5 - (z.getImag()-map_c_.getImag()) / map_h_ );
}



/*|/////////////////////////////////|   8)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////| Obtener la columna asociada al complejo ( [ ][j] ) \\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t get_col_from_complex(const complex &z, size_t w)
{
    return w * ( 0.5 + (z.getReal()-map_c_.getReal()) / map_w_ );
}
