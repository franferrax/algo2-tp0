#ifndef GRAYSCALECANVASPGM_H
#define GRAYSCALECANVASPGM_H

#include <iostream>
using namespace std;

// Limitación en la profundidad del color para ahorrar memoria usando uchar
#define MIN_COLOR_DEPTH   1
#define MAX_COLOR_DEPTH 255
typedef unsigned char pixel_t;

// Largo máximo de los comentarios
#define MAX_COMMENT_LENGTH 65536


/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*|//////////////////////////| GrayscaleCanvasPGM |\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
class GrayscaleCanvasPGM
{
    static const string MagicNumber; // Número mágico PGM
    size_t              Width;       // Ancho del lienzo (imagen)
    size_t              Height;      // Alto del lienzo (imagen)
    pixel_t             ColorDepth;  // Profundidad del color (niveles)
    pixel_t             **canvas;    // Lienzo, matriz de imagen


public:
    // 1) Constructor
    GrayscaleCanvasPGM (size_t, size_t, pixel_t);

    // 2) Constructor por defecto
    GrayscaleCanvasPGM () : GrayscaleCanvasPGM (1, 1, MAX_COLOR_DEPTH) {}

    // 3) Constructor por copia
    GrayscaleCanvasPGM (const GrayscaleCanvasPGM &);

    // 4) Destructor
    ~ GrayscaleCanvasPGM ();

    // 5) Indexación del lienzo (l-value y r-value: c[y][x])
    pixel_t* & operator[] (size_t) const;

    // 6-8) Obtención de ancho, alto, profundidad de color
    size_t  getWidth ();
    size_t  getHeight ();
    pixel_t getColorDepth ();

    // 9) Cambio de profundidad de color
    void setColorDepth (pixel_t);

    // 10) Cambio de tamaño de imagen 
    void resize (size_t, size_t);

    // 11) Impresión en flujo/archivo/stdin
    friend ostream & operator<< (ostream &, const GrayscaleCanvasPGM &);

    // 12) Carga desde flujo/archivo/stdin
    friend istream & operator>> (istream &, GrayscaleCanvasPGM &);
};


#endif // GRAYSCALECANVASPGM_H
