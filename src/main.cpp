#include <iostream>
#include <fstream>
#include "cmdline.h"
#include "PGMimage.h"
#include "complejo.h"
#include "utils.h"


const transform_t tranforms[] =
{
    & complejo :: identidadDesde,
    & complejo :: exponencialDesde,
    & complejo :: cuadradoDesde,
    & complejo :: cuboDesde,
    & complejo :: senoDesde
};


int main (int argc, char** argv)
{
    string in_file_name = "";
    string out_file_name = "";
    size_t function_index = 0;

    // Validación de argumentos
    validateCommandLineArguments (argc, argv, in_file_name, out_file_name,
                                  function_index);

    //////////////////////////////////////////////////////////////
    // DEBUG
    cerr << "El archivo de entrada es " << in_file_name  << endl;
    cerr << "El archivo de salida es "  << out_file_name << endl;
    cerr << "La funcion de entrada es " << function_index << endl;
    //////////////////////////////////////////////////////////////

    // Apertura del archivo de entrada o cin
    istream *in_stream = &cin;
    ifstream in_file;
    if ( in_file_name != "cin" )
    {
        in_file.open (in_file_name.c_str ());
        if (!in_file)
        {
            cerr << "No se pudo abrir el archivo de entrada, ¿existe?" << endl;
            return 1;
        }
        in_stream = &in_file;
    }

    // Apertura del archivo de salida o cout
    ostream *out_stream = &cout;
    ofstream out_file;
    if ( out_file_name != "cout" )
    {
        out_file.open (out_file_name.c_str ());
        if (!out_file)
        {
            cerr << "No se pudo abrir el archivo de salida, ¿tiene permisos?"
                 << endl;
            return 1;
        }
        out_stream = &out_file;
    }

    // Lectura del archivo de entrada
    PGMimage in_image;
    if ( !(*in_stream >> in_image) )
    {
        cerr << "Imagen PGM con formato inválido" << endl;
        return 1;
    }
    in_file.close ();

    // Creación de una nueva imagen con las mismas dimensiones
    size_t h = in_image.getHeight ();
    size_t w = in_image.getWidth ();

    PGMimage out_image (w, h, in_image.getColorDepth ());

    // Recorrido de la imagen y transformación
    complejo in_plane, out_plane;
    size_t i, j, row, col;

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            // Pixel en la imagen de salida <-> Punto en el plano de salida
            getComplexFromIndex (out_plane, i, j, h, w);

            // Aplicación de la función, previamente seleccionada y apuntada
            (in_plane.*tranforms[function_index]) (out_plane);

            // Punto en el plano de entrada <-> Pixel en la imagen de entrada
            row = getRowFromComplex (in_plane, h);
            col = getColFromComplex (in_plane, w);

            // Si no se cayó fuera de la imagen, se copia
            if (row < h && col < w)
            {
                out_image[i][j] = in_image[row][col];
            }
        }
    }

    // Volcado en el archivo de salida
    *out_stream << out_image;
    out_file.close ();

    return 0;
}
