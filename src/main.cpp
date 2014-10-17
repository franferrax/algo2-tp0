#include <iostream>
#include <fstream>
#include "cmdline.h"
#include "PGMimage.h"
#include "complejo.h"
#include "utils.h"


////////////////////////////// Variables globales //////////////////////////////
option_t options[] =
{
    {true,  "i", "input",    "-",  opt_input,    OPT_DEFAULT},
    {true,  "o", "output",   "-",  opt_output,   OPT_DEFAULT},
    {true,  "f", "function", "z",  opt_function, OPT_DEFAULT},
    {false, "h", "help",     NULL, opt_help,     OPT_DEFAULT},
    {0, },
};
function_t complex_function = NULL;
istream *iss = NULL;
ostream *oss = NULL;
fstream ifs;
fstream ofs;
char *prog_name;



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||| Main |||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
int main(int argc, char** argv)
{
    prog_name = argv[0];
    // Validación de argumentos
    cmdline cmdl(options);
    cmdl.parse(argc, argv);

    // Lectura del archivo de entrada
    PGMimage in_image;
    if ( !(*iss >> in_image) )
    {
        cerr << "Invalid PGM formated input." << endl;
        exit(1);
    }
    if (ifs) ifs.close();

    // Creación de una nueva imagen con las mismas dimensiones
    size_t h = in_image.getHeight();
    size_t w = in_image.getWidth();

    PGMimage out_image(w, h, in_image.getColorDepth());

    // Recorrido de la imagen y transformación
    complejo in_plane, out_plane;
    size_t i, j, row, col;

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            // Pixel en la imagen de salida <-> Punto en el plano de salida
            getComplexFromIndex(out_plane, i, j, h, w);

            // Aplicación de la función, previamente seleccionada y apuntada
            (in_plane.*complex_function)(out_plane);

            // Punto en el plano de entrada <-> Pixel en la imagen de entrada
            row = getRowFromComplex(in_plane, h);
            col = getColFromComplex(in_plane, w);

            // Si no se cayó fuera de la imagen, se copia
            if (row < h && col < w)
            {
                out_image[i][j] = in_image[row][col];
            }
        }
    }

    // Volcado en el archivo de salida
    *oss << out_image;
    if (ofs) ofs.close();

    return 0;
}
