#include <iostream>
#include <fstream>
#include <string>
#include "PGMimage.h"
#include "complex.h"
#include "utils.h"
#include "cmdline.h"
#include "node.h"
#include "queue.h"
#include "stack.h"
#include "parser.h"
#include "optree.h"

using namespace std;

////////////////////////////// Variables globales //////////////////////////////
option_t options_[] =           // Opciones CLA
{
    {true,  "i", "input",    "-",       opt_input,    OPT_DEFAULT},
    {true,  "o", "output",   "-",       opt_output,   OPT_DEFAULT},
    {true,  "f", "function", "z",       opt_function, OPT_DEFAULT},
    {true,  "r", "region",   "2,2,0,0", opt_region,   OPT_DEFAULT},
    {false, "h", "help",     NULL,      opt_help,     OPT_DEFAULT},
    {0, },
};
istream      *iss_ = NULL;      // Puntero a stream de entrada
ostream      *oss_ = NULL;      // Puntero a stream de salida
fstream      ifs_;              // Archivo de entrada
fstream      ofs_;              // Archivo de salida
char         *prog_name_;       // Nombre del programa
double       map_w_;            // Ancho de la región de mapeo
double       map_h_;            // Alto de la región de mapeo
complex      map_c_;            // Centro de la región de mapeo
stack<token> rpn_expr_;         // Expresión convertida a RPN



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||| Main |||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
int main(int argc, char** argv)
{
    prog_name_ = argv[0];

    // Validación de argumentos
    cmdline cmdl(options_);
    cmdl.parse(argc, argv);


#ifdef DEBUG
    // Prueba del árbol de operaciones
    complex my_z(5, 5);

    optree arbolito(rpn_expr_, my_z);

    cout << "expression" << my_z << " = " << arbolito.operate() << endl;

    my_z.setReal(0);
    cout << "expression" << my_z << " = " << arbolito.operate() << endl;

    exit(1);
#endif


    // Lectura del archivo de entrada
    PGMimage in_image;
    if ( !(*iss_ >> in_image) )
    {
        cerr << "Invalid PGM formated input." << endl;
        exit(1);
    }
    // Si nada ha fallado, se puede cerrar el archivo
    if (ifs_) ifs_.close();

    // Creación de una nueva imagen con las mismas dimensiones
    size_t h = in_image.getHeight();
    size_t w = in_image.getWidth();

    PGMimage out_image(w, h, in_image.getColorDepth());

    // Variables para recorrer la imagen
    complex in_plane, out_plane;
    size_t i, j, row, col;

    // Árbol de la operación
    optree operation(rpn_expr_, out_plane);

    // Recorrido de la imagen y transformación
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            // Pixel en la imagen de salida <-> Punto en el plano de salida
            get_complex_from_index(out_plane, i, j, h, w);

            // Aplicación de la operación
            in_plane = operation.operate();

            // Punto en el plano de entrada <-> Pixel en la imagen de entrada
            row = get_row_from_complex(in_plane, h);
            col = get_col_from_complex(in_plane, w);

            // Si no se cayó fuera de la imagen, se copia
            if (row < h && col < w)
            {
                out_image[i][j] = in_image[row][col];
            }
        }
    }

    // Volcado en el archivo de salida
    *oss_ << out_image;
    if (ofs_) ofs_.close();

    return 0;
}
