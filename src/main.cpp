#include <iostream>
#include <fstream>
#include "GrayscaleCanvasPGM.h"
#include "complejo.h"
#include "utils.h"

int main(int argc, char** argv) // Main de pruebas, ejecutar con: bin/tp0
{
    
    string in = "";
    string out = "";
    string function = "";
    
    cout << "/*** Probando linea de comandos ***/"<<endl;
    validateCommandLineArguments(argc,argv,in,out,function);
    
    cout<<"El archivo de entrada es "<< in<<endl;
    cout<<"El archivo de salida es "<< out<<endl;
    cout<<"La funcion de entrada es "<< function<<endl;
    
    cout << "/*** Probando GrayscaleCanvasPGM ***/"<<endl;
    // Archivo de entrada
    ifstream infile;
    infile.open("test/pre.pgm");

    GrayscaleCanvasPGM image;

    if ( !(infile >> image) )
    {
        cerr << "Imagen PGM con formato inválido" << endl;
        return 1;
    }

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
    
    cout << "/*** Probando complejos ***/"<<endl;
    complejo z(5, -2);
    cout << complejo::expo(z);

    return 0;
}
