#include "PGMimage.h"
const string PGMimage::MagicNumber = "P2";



/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////| Constructor |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
PGMimage::PGMimage(size_t w, size_t h, pixel_t d)
{
    this->Width  = w;
    this->Height = h;

    // Limitación en la profundidad de color
    PGMimage::validate_color_depth(d);
    this->ColorDepth = d;

    // Memoria para el lienzo
    this->canvas = PGMimage::new_canvas(this->Width, this->Height);

    // Inicialización en 0
    for (size_t i = 0; i < h; i++)
        for (size_t j = 0; j < w; j++)
            this->canvas[i][j] = 0;
}



/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////////| Constructor por copia |\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
PGMimage::PGMimage(const PGMimage &o)
{
    this->Width  = o.Width;
    this->Height = o.Height;
    this->ColorDepth = o.ColorDepth;

    // Memoria para la copia
    this->canvas = PGMimage::new_canvas(this->Width, this->Height);

    // Copia de los datos
    for (size_t i = 0; i < this->Height; i++)
        for (size_t j = 0; j < this->Width; j++)
            this->canvas[i][j] = o.canvas[i][j];
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////| Indexación del lienzo (l-value y r-value: c[y][x]) |\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
pixel_t* PGMimage::operator[](size_t y) const
{
    if (y >= this->Height)
        return this->canvas[this->Height-1];

    return this->canvas[y];
}



/*|/////////////////////////////////|  5-7) |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////| Obtención de ancho, alto, profundidad de color |\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t  PGMimage::getWidth() const      { return this->Width;      }
size_t  PGMimage::getHeight() const     { return this->Height;     }
pixel_t PGMimage::getColorDepth() const { return this->ColorDepth; }



/*|/////////////////////////////////|   8)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////| Cambio de profundidad de color |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void PGMimage::setColorDepth(pixel_t d)
{
    PGMimage::validate_color_depth(d);

    // Cálculo de factor de amplificación y actualización de la profundidad
    float amp = (float) d / (float) this->ColorDepth;
    this->ColorDepth = d;

    // Cálculo de los datos con la nueva profundidad
    for (size_t i = 0; i < this->Height; i++)
        for (size_t j = 0; j < this->Width; j++)
            this->canvas[i][j] *= amp;
}



/*|/////////////////////////////////|   9)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////| Cambio de tamaño de imagen |\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void PGMimage::resize(size_t w, size_t h)
{
    size_t w_max, h_max;
    pixel_t **auxcnv;

    // Memoria para el nuevo lienzo
    auxcnv = PGMimage::new_canvas(w, h);

    // Copiado de los datos, con posible pérdida por recorte
    w_max = min(w, this->Width);
    h_max = min(h, this->Height);

    for (size_t i = 0; i < h_max; i++)
        for (size_t j = 0; j < w_max; j++)
            auxcnv[i][j] = this->canvas[i][j];

    // Liberación de la memoria antigua
    canvasDestroy();

    // Actualización de los valores
    this->Width = w;
    this->Height = h;
    this->canvas = auxcnv;
}
// NOTA: no se realiza un escalado de la imagen, solo se modifica el lienzo,
// resultando en posibles recortes o agregado de pixels con eventual basura.



/*|/////////////////////////////////|  10)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////| Impresión en flujo/archivo/stdin |\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
ostream & operator<<(ostream &os, const PGMimage &c)
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
istream & operator>>(istream &is, PGMimage &c)
{
    size_t w = 0, h = 0, aux = 0, i, j;
    pixel_t d = 0, **auxcnv;
    bool errors = true;

    // Lectura del encabezado
    int mnL = c.MagicNumber.length() + 1;
    char *mn = new char[mnL];
    is.get(mn, mnL);
    // Número mágico
    if ( mn == c.MagicNumber )
    {
        PGMimage::ignore_comments(is);
        // Ancho y alto
        if (is >> w && is >> h)
        {
            PGMimage::ignore_comments(is);
            // Profundidad de color
            if (is >> aux)
            {
                d = aux;
                errors = false;
            }
        }
    }
    delete mn;

    // Lectura de los datos de pixel
    if (!errors)
    {
        // Limitar profundidad de color
        PGMimage::validate_color_depth(d);

        // Memoria para el lienzo
        auxcnv = PGMimage::new_canvas(w, h);

        // Carga de datos
        for (i = 0; i < h; i++)
        {
            for (j = 0; j < w; j++)
            {
                PGMimage::ignore_comments(is);
                if (is >> aux) auxcnv[i][j] = aux;
                else { errors = true; break; }
            }
            if (errors) break;
        }

        // Si no ha fallado la carga de valores
        if (!errors)
        {
            // Actualización del objeto
            c.canvasDestroy();
            c.canvas = auxcnv;
            c.Width = w;
            c.Height = h;
            c.ColorDepth = d;
        }
        // En caso de falla, se deja el objeto intacto y se destruye auxcnv
        else
        {
            for (i = 0; i < h; i++)
                delete [] auxcnv[i];
            delete [] auxcnv;
        }
    }

    if (errors) // Si hubo errores, se indica en el stream
        is.clear(ios::badbit);

    return is;
}



/*|/////////////////////////////////|   *)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////| Utilidades internas |\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/

// Ignorar comentarios estilo PGM en un stream
void PGMimage::ignore_comments(istream &s)
{
    char ch;
    while (s >> ch)
    {
        if (ch == '#')
        {
            s.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            s.putback(ch);
            break;
        }
    }
}

// Pedir memoria para un lienzo de w x h
pixel_t** PGMimage::new_canvas(size_t w, size_t h)
{
    pixel_t **cnv = new pixel_t* [h];

    for (size_t i = 0; i < h; i++)
        cnv[i] = new pixel_t[w];

    return cnv;
}

// Limitar profundidad de color
void PGMimage::validate_color_depth(pixel_t &d)
{
    if (d > MAX_COLOR_DEPTH) d = MAX_COLOR_DEPTH;
    if (d < MIN_COLOR_DEPTH) d = MIN_COLOR_DEPTH;
}

// Destruir el lienzo sobre el objeto actual
void PGMimage::canvasDestroy()
{
    for (size_t i = 0; i < this->Height; i++)
        delete [] this->canvas[i];

    delete [] this->canvas;
}
