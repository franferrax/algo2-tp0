#include "GrayscaleCanvasPGM.h"
const string GrayscaleCanvasPGM :: MagicNumber = "P2";

/*|/////////////////////////////////|   *)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////| Utilidades internas |\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void _ignoreComments (istream &s)
{
    char ch;
    while (s >> ch)
    {
        if (ch == '#')
        {
            s.ignore(MAX_COMMENT_LENGTH, '\n');
        }
        else
        {
            s.putback(ch);
            break;
        }
    }
}



/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////| Constructor |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
GrayscaleCanvasPGM :: GrayscaleCanvasPGM (size_t w, size_t h, pixel_t d)
{
    size_t i, j;

    Width  = w;
    Height = h;

    // Limitación en la profundidad de color
    if (d > MAX_COLOR_DEPTH) d = MAX_COLOR_DEPTH;
    if (d < MIN_COLOR_DEPTH) d = MIN_COLOR_DEPTH; 
    ColorDepth = d;

    // Memoria para el lienzo
    canvas = new pixel_t* [h];
    for (i = 0; i < h; i++)
        canvas[i] = new pixel_t[w];

    // Inicialización en 0
    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++)
            canvas[i][j] = 0;
}



/*|/////////////////////////////////|   3)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////////| Constructor por copia |\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
GrayscaleCanvasPGM :: GrayscaleCanvasPGM (const GrayscaleCanvasPGM &o)
{
    size_t i, j;

    Width  = o.Width;
    Height = o.Height;
    ColorDepth = o.ColorDepth;

    // Memoria para la copia
    canvas = new pixel_t*[Height];
    for (i = 0; i < Height; i++)
        canvas[i] = new pixel_t[Width];

    // Copia de los datos
    for (i = 0; i < Height; i++)
        for (j = 0; j < Width; j++)
            canvas[i][j] = o.canvas[i][j];
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////////////| Destructor |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
GrayscaleCanvasPGM :: ~ GrayscaleCanvasPGM ()
{
    for (size_t i = 0; i < Height; i++)
        delete [] canvas[i];

    delete [] canvas;
}



/*|/////////////////////////////////|   5)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////| Indexación del lienzo (l-value y r-value: c[y][x]) |\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
pixel_t* & GrayscaleCanvasPGM :: operator[] (size_t y) const
{
    if (y >= Height)
        return canvas[Height-1];
    return canvas[y];
}



/*|/////////////////////////////////|  6-8) |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////| Obtención de ancho, alto, profundidad de color |\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t  GrayscaleCanvasPGM :: getWidth ()      { return Width;      }
size_t  GrayscaleCanvasPGM :: getHeight ()     { return Height;     }
pixel_t GrayscaleCanvasPGM :: getColorDepth () { return ColorDepth; }



/*|/////////////////////////////////|   9)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////| Cambio de profundidad de color |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void GrayscaleCanvasPGM :: setColorDepth (pixel_t d)
{
    if (d > MAX_COLOR_DEPTH) d = MAX_COLOR_DEPTH;
    if (d < MIN_COLOR_DEPTH) d = MIN_COLOR_DEPTH; 
   
    // Cálculo de factor de amplificación y actualización de la profundidad
    float amp = (float) d / (float) ColorDepth;
    ColorDepth = d;

    // Cálculo de los datos con la nueva profundidad
    for (size_t i = 0; i < Height; i++)
        for (size_t j = 0; j < Width; j++)
            canvas[i][j] *= amp;
}



/*|/////////////////////////////////|  10)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////| Cambio de tamaño de imagen |\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void GrayscaleCanvasPGM :: resize (size_t w, size_t h)
{
    size_t i, j, w_max, h_max;
    pixel_t **aux;

    // Memoria para el nuevo lienzo
    aux = new pixel_t* [h];
    for (i = 0; i < h; i++)
        aux[i] = new pixel_t[w];

    // Copiado de los datos, con posible pérdida por recorte
    w_max = min(w, Width);
    h_max = min(h, Height);

    for (i = 0; i < h_max; i++)
        for (j = 0; j < w_max; j++)
            aux[i][j] = canvas[i][j];

    // Liberación de la memoria antigua
    for (i = 0; i < Height; i++)
        delete [] canvas[i];
    delete [] canvas;

    // Actualización de los valores
    Height = h;
    Width = w;
    canvas = aux;
}
// NOTA: no se realiza un escalado de la imagen, solo se modifica el lienzo,
// resultando en posibles recortes o agregado de pixels negros.



/*|/////////////////////////////////|  11)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////| Impresión en flujo/archivo/stdin |\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
ostream & operator<< (ostream &os, const GrayscaleCanvasPGM &c)
{
    // Encabezado del archivo
    os << c.MagicNumber << endl;
    os << c.Width << ' ' << c.Height << endl;
    os << (size_t) c.ColorDepth << endl;

    // Datos de pixels
    for (size_t i = 0; i < c.Height; i++)
    {
        os << (size_t) c.canvas[i][0];
        for (size_t j = 1; j < c.Width; j++)
        {
            os << ' ' << (size_t) c.canvas[i][j];
        }
        os << endl;
    }

    return os;
}



/*|/////////////////////////////////|  12)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////| Carga desde flujo/archivo/stdin |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
istream & operator>> (istream &is, GrayscaleCanvasPGM &c)
{
    string mn;
    size_t w = 0, h = 0, d = 0, px = 0;
    bool errors = true;

    // Lectura del encabezado
    if (is >> mn && mn == c.MagicNumber)
    {
        _ignoreComments(is); 
        if (is >> w && is >> h)
        {
            _ignoreComments(is);
            if (is >> d)
            {
                _ignoreComments(is);
                errors = false;
            }
        }
    }

    if(!errors)
    {
        size_t i, j;

        // Limitar profundidad de color
        if (d > MAX_COLOR_DEPTH) d = MAX_COLOR_DEPTH;
        if (d < MIN_COLOR_DEPTH) d = MIN_COLOR_DEPTH; 

        // Vaciar c
        for (i = 0; i < c.Height; i++)
            delete [] c.canvas[i];
        delete [] c.canvas;

        // Memoria para el lienzo
        c.canvas = new pixel_t* [h];
        for (i = 0; i < h; i++)
            c.canvas[i] = new pixel_t[w];

        // Actualización de valores
        c.Width = w;
        c.Height = h;
        c.ColorDepth = d;

        // Carga de datos
        for (i = 0; i < h; i++)
        {
            for (j = 0; j < w; j++)
            {
                if (is >> px) c.canvas[i][j] = px;
                else { errors = true; break; }
            }
            if (errors) break;
        }
    }

    // En caso de falla se indica en el stream
    if (errors)
        is.clear (ios::badbit);

    return is;
}
