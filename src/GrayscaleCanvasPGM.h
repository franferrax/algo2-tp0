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

    // Utilidades internas
    static void ignore_comments (istream &);       // Ignorar comentarios
    static pixel_t** new_canvas (size_t, size_t);  // Memoria para el lienzo
    void canvasDestroy (); // Destrucción del lienzo


public:
    // 1) Constructor (con sus argumentos por defecto)
    GrayscaleCanvasPGM (size_t w=1, size_t h=1, pixel_t d=MAX_COLOR_DEPTH);

    // 2) Constructor por copia
    GrayscaleCanvasPGM (const GrayscaleCanvasPGM &);

    // 3) Destructor
    ~ GrayscaleCanvasPGM () { canvasDestroy (); };

    // 4) Indexación del lienzo (l-value y r-value: c[y][x])
    pixel_t* operator[] (size_t) const;

    // 5-7) Obtención de ancho, alto, profundidad de color
    size_t  getWidth () const;
    size_t  getHeight () const;
    pixel_t getColorDepth () const;

    // 8) Cambio de profundidad de color
    void setColorDepth (pixel_t);

    // 9) Cambio de tamaño de imagen
    void resize (size_t, size_t);

    // 10) Impresión en flujo/archivo/stdin
    friend ostream & operator<< (ostream &, const GrayscaleCanvasPGM &);

    // 11) Carga desde flujo/archivo/stdin
    friend istream & operator>> (istream &, GrayscaleCanvasPGM &);
};


#endif // GRAYSCALECANVASPGM_H
