#ifndef PGMIMAGE_H
#define PGMIMAGE_H

#include <iostream>
#include <limits>
using namespace std;

// Limitación en la profundidad del color para ahorrar memoria usando uchar
#define MIN_COLOR_DEPTH   1
#define MAX_COLOR_DEPTH 255
typedef unsigned char pixel_t;



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||| PGMimage |||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
class PGMimage
{
    static const string _magic_number; // Número mágico PGM
    size_t              _width;        // Ancho del lienzo (imagen)
    size_t              _height;       // Alto del lienzo (imagen)
    pixel_t             _color_depth;  // Profundidad del color (niveles)
    pixel_t             **_canvas;     // Lienzo, matriz de imagen


    ///////////////////////// Utilidades internas //////////////////////////

    // Ignorar comentarios estilo PGM en un stream
    static void _ignore_comments(istream &);

    // Pedir memoria para un lienzo de w x h
    static pixel_t** _new_canvas(size_t, size_t);

    // Limitar profundidad de color
    static void _validate_color_depth(pixel_t &);

    // Destruir el lienzo sobre el objeto actual
    static void _canvas_destroy(size_t, pixel_t **);


public:
    // 1) Constructor (con sus argumentos por defecto)
    PGMimage(size_t w=1, size_t h=1, pixel_t d=MAX_COLOR_DEPTH);

    // 2) Constructor por copia
    PGMimage(const PGMimage &);

    // 3) Destructor
    ~ PGMimage() { _canvas_destroy(this->_height, this->_canvas); };

    // 4) Indexación del lienzo (l-value y r-value: c[y][x])
    pixel_t* operator[](size_t) const;

    // 5-7) Obtención de ancho, alto, profundidad de color
    size_t  getWidth() const;
    size_t  getHeight() const;
    pixel_t getColorDepth() const;

    // 8) Cambio de profundidad de color
    void setColorDepth(pixel_t);

    // 9) Cambio de tamaño de imagen
    void resize(size_t, size_t);

    // 10) Impresión en flujo/archivo/stdin
    friend ostream & operator<<(ostream &, const PGMimage &);

    // 11) Carga desde flujo/archivo/stdin
    friend istream & operator>>(istream &, PGMimage &);
};


#endif // PGMIMAGE_H
