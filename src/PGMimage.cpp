#include "PGMimage.h"
const string PGMimage :: MagicNumber = "P2";



/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////| Constructor |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
PGMimage :: PGMimage (size_t w, size_t h, pixel_t d)
{
    Width  = w;
    Height = h;

    // Limitación en la profundidad de color
    if (d > MAX_COLOR_DEPTH) { d = MAX_COLOR_DEPTH; }
    if (d < MIN_COLOR_DEPTH) { d = MIN_COLOR_DEPTH; }
    ColorDepth = d;

    // Memoria para el lienzo
    canvas = PGMimage :: new_canvas (Width, Height);

    // Inicialización en 0
    for (size_t i = 0; i < h; i++)
        for (size_t j = 0; j < w; j++)
            canvas[i][j] = 0;
}



/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////////| Constructor por copia |\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
PGMimage :: PGMimage (const PGMimage &o)
{
    Width  = o.Width;
    Height = o.Height;
    ColorDepth = o.ColorDepth;

    // Memoria para la copia
    canvas = PGMimage :: new_canvas (Width, Height);

    // Copia de los datos
    for (size_t i = 0; i < Height; i++)
        for (size_t j = 0; j < Width; j++)
            canvas[i][j] = o.canvas[i][j];
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////| Indexación del lienzo (l-value y r-value: c[y][x]) |\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
pixel_t* PGMimage :: operator[] (size_t y) const
{
    if (y >= Height) { return canvas[Height-1]; }
    return canvas[y];
}



/*|/////////////////////////////////|  5-7) |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////| Obtención de ancho, alto, profundidad de color |\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t  PGMimage :: getWidth () const      { return Width;      }
size_t  PGMimage :: getHeight () const     { return Height;     }
pixel_t PGMimage :: getColorDepth () const { return ColorDepth; }



/*|/////////////////////////////////|   8)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////| Cambio de profundidad de color |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void PGMimage :: setColorDepth (pixel_t d)
{
    if (d > MAX_COLOR_DEPTH) { d = MAX_COLOR_DEPTH; }
    if (d < MIN_COLOR_DEPTH) { d = MIN_COLOR_DEPTH; }

    // Cálculo de factor de amplificación y actualización de la profundidad
    float amp = (float) d / (float) ColorDepth;
    ColorDepth = d;

    // Cálculo de los datos con la nueva profundidad
    for (size_t i = 0; i < Height; i++)
        for (size_t j = 0; j < Width; j++)
            canvas[i][j] *= amp;
}



/*|/////////////////////////////////|   9)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////| Cambio de tamaño de imagen |\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void PGMimage :: resize (size_t w, size_t h)
{
    size_t w_max, h_max;
    pixel_t **aux;

    // Memoria para el nuevo lienzo
    aux = PGMimage :: new_canvas (w, h);

    // Copiado de los datos, con posible pérdida por recorte
    w_max = min (w, Width);
    h_max = min (h, Height);

    for (size_t i = 0; i < h_max; i++)
        for (size_t j = 0; j < w_max; j++)
            aux[i][j] = canvas[i][j];

    // Liberación de la memoria antigua
    canvasDestroy ();

    // Actualización de los valores
    Width = w;
    Height = h;
    canvas = aux;
}
// NOTA: no se realiza un escalado de la imagen, solo se modifica el lienzo,
// resultando en posibles recortes o agregado de pixels con eventual basura.



/*|/////////////////////////////////|  10)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////| Impresión en flujo/archivo/stdin |\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
ostream & operator<< (ostream &os, const PGMimage &c)
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
            os << ' ' << (size_t) c.canvas[i][j];

        os << endl;
    }

    return os;
}



/*|/////////////////////////////////|  11)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////| Carga desde flujo/archivo/stdin |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
istream & operator>> (istream &is, PGMimage &c)
{
    size_t w = 0, h = 0, d = 0, px = 0;
    bool errors = true;

    // Lectura del encabezado
    int mnL = c.MagicNumber.length () + 1;
    char *mn = new char[mnL];
    is.get (mn, mnL);
    if ( mn == c.MagicNumber )
    {
        PGMimage :: ignore_comments (is);
        if (is >> w && is >> h)
        {
            PGMimage :: ignore_comments (is);
            if (is >> d) { errors = false; }
        }
    }
    delete mn;

    // Lectura de los datos de pixel
    if (!errors)
    {
        // Limitar profundidad de color
        if (d > MAX_COLOR_DEPTH) { d = MAX_COLOR_DEPTH; }
        if (d < MIN_COLOR_DEPTH) { d = MIN_COLOR_DEPTH; }

        // Vaciar c
        c.canvasDestroy ();

        // Memoria para el lienzo
        c.canvas = PGMimage :: new_canvas (w, h);

        // Actualización de valores
        c.Width = w;
        c.Height = h;
        c.ColorDepth = d;

        // Carga de datos
        for (size_t i = 0; i < h; i++)
        {
            for (size_t j = 0; j < w; j++)
            {
                PGMimage :: ignore_comments (is);
                if (is >> px) { c.canvas[i][j] = px; }
                else { errors = true; break; }
            }
            if (errors) { break; }
        }
    }

    // En caso de falla se indica en el stream
    if (errors) { is.clear (ios :: badbit); }

    return is;
}



/*|/////////////////////////////////|   *)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////| Utilidades internas |\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void PGMimage :: ignore_comments (istream &s)
{
    char ch;
    while (s >> ch)
    {
        if (ch == '#')
        {
            s.ignore (numeric_limits<streamsize> :: max (), '\n');
        }
        else
        {
            s.putback (ch);
            break;
        }
    }
}


pixel_t** PGMimage :: new_canvas (size_t w, size_t h)
{
    pixel_t **canvas = new pixel_t* [h];

    for (size_t i = 0; i < h; i++)
        canvas[i] = new pixel_t[w];

    return canvas;
}


void PGMimage :: canvasDestroy ()
{
    for (size_t i = 0; i < Height; i++)
        delete [] canvas[i];

    delete [] canvas;
}
