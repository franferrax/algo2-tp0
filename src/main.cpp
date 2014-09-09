#include <iostream>
#include <fstream>
#include "GrayscaleCanvasPGM.h"
#include "complejo.h"


int main() // Main de pruebas, ejecutar con: bin/tp0
{
    // Archivo de entrada
    ifstream infile;
    infile.open("test/pre.pgm");

    GrayscaleCanvasPGM image;

    if ( !(infile >> image) )
        return 1;

    infile.close();

    // Oscurecimiento, probando la sobrecarga de [][]
    size_t h = image.getHeight();
    size_t w = image.getWidth();

    for (size_t i = 0; i < h; i++)
        for (size_t j = 0; j < w; j++)
            image[i][j] *= 0.5;

    // Cambio de profundidad
    image.setColorDepth(4);

    // Archivo de salida
    ofstream outfile;
    outfile.open("test/post.pgm");

    outfile << image;

    outfile.close();
    
    complejo z(5, -2);
    cout << complejo::expo(z);

    return 0;
}
