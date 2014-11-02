#include "PGMimage.h"
const string PGMimage::_magic_number = "P2";
#define MAGIC_NUMBER_LENGTH 2



/*|/////////////////////////////////|   1)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////| Constructor |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
PGMimage::PGMimage(size_t w, size_t h, pixel_t d)
{
    this->_width  = w;
    this->_height = h;

    // Limitación en la profundidad de color
    PGMimage::_validate_color_depth(d);
    this->_color_depth = d;

    // Memoria para el lienzo
    this->_canvas = PGMimage::_new_canvas(this->_width, this->_height);

    // Inicialización en 0
    for (size_t i = 0; i < h; i++)
        for (size_t j = 0; j < w; j++)
            this->_canvas[i][j] = 0;
}



/*|/////////////////////////////////|   2)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////////| Constructor por copia |\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
PGMimage::PGMimage(const PGMimage &o)
{
    this->_width       = o._width;
    this->_height      = o._height;
    this->_color_depth = o._color_depth;

    // Memoria para la copia
    this->_canvas = PGMimage::_new_canvas(this->_width, this->_height);

    // Copia de los datos
    for (size_t i = 0; i < this->_height; i++)
        for (size_t j = 0; j < this->_width; j++)
            this->_canvas[i][j] = o._canvas[i][j];
}



/*|/////////////////////////////////|   4)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////| Indexación del lienzo (l-value y r-value: c[y][x]) |\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
pixel_t* PGMimage::operator[](size_t y) const
{
    if (y >= this->_height) // Tope, seguridad en altura
        return this->_canvas[this->_height-1];

    return this->_canvas[y];
}



/*|/////////////////////////////////|  5-7) |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////| Obtención de ancho, alto, profundidad de color |\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
size_t  PGMimage::getWidth() const      { return this->_width;       }
size_t  PGMimage::getHeight() const     { return this->_height;      }
pixel_t PGMimage::getColorDepth() const { return this->_color_depth; }



/*|/////////////////////////////////|   8)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|////////////////////| Cambio de profundidad de color |\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void PGMimage::setColorDepth(pixel_t d)
{
    PGMimage::_validate_color_depth(d);

    // Cálculo de factor de escala y actualización de la profundidad
    float scale = (float) d / (float) this->_color_depth;
    this->_color_depth = d;

    // Cálculo de los datos con la nueva profundidad
    for (size_t i = 0; i < this->_height; i++)
        for (size_t j = 0; j < this->_width; j++)
            this->_canvas[i][j] *= scale;
}



/*|/////////////////////////////////|   9)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|//////////////////////| Cambio de tamaño de imagen |\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
void PGMimage::resize(size_t w, size_t h)
{
    size_t w_max, h_max;
    pixel_t **auxcnv;

    // Memoria para el nuevo lienzo
    auxcnv = PGMimage::_new_canvas(w, h);

    // Copiado de los datos, con posible pérdida por recorte
    w_max = min(w, this->_width);
    h_max = min(h, this->_height);

    for (size_t i = 0; i < h_max; i++)
        for (size_t j = 0; j < w_max; j++)
            auxcnv[i][j] = this->_canvas[i][j];

    // Liberación de la memoria antigua
    PGMimage::_canvas_destroy(this->_height, this->_canvas);

    // Actualización de los valores
    this->_width  = w;
    this->_height = h;
    this->_canvas = auxcnv;
}
// NOTA: no se realiza un escalado de la imagen, solo se modifica el lienzo,
// resultando en posibles recortes o agregado de pixels con eventual basura.



/*|/////////////////////////////////|  10)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|///////////////////| Impresión en flujo/archivo/stdin |\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
ostream & operator<<(ostream &os, const PGMimage &c)
{
    // Encabezado del archivo
    os << c._magic_number << endl;
    os << c._width << ' ' << c._height << endl;
    os << (size_t) c._color_depth << endl;

    // Datos de pixels
    for (size_t i = 0; i < c._height; i++)
    {
        os << (size_t) c._canvas[i][0];
        for (size_t j = 1; j < c._width; j++)
            os << ' ' << (size_t) c._canvas[i][j];

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
    char mn[MAGIC_NUMBER_LENGTH + 1];
    double scale = 1;

    // Lectura del encabezado
    is.get(mn, MAGIC_NUMBER_LENGTH + 1);
    // Número mágico
    if ( mn == c._magic_number )
    {
        PGMimage::_ignore_comments(is);
        // Ancho y alto
        if (is >> w && is >> h)
        {
            PGMimage::_ignore_comments(is);
            // Profundidad de color
            if (is >> aux && aux >= MIN_COLOR_DEPTH)
            {
                errors = false;
                // Recorte en profundidad, de ser necesario
                if (aux > MAX_COLOR_DEPTH)
                {
                    cerr << "Warning: max color depth is "
                         << MAX_COLOR_DEPTH
                         << ", the image will be adapted."
                         << endl;
                    // Escala para adaptar la imagen
                    scale = (double) MAX_COLOR_DEPTH / (double) aux;
                    d = MAX_COLOR_DEPTH;
                }
                else d = aux;
            }
        }
    }

    // Lectura de los datos de pixel
    if (!errors)
    {
        // Memoria para el lienzo
        auxcnv = PGMimage::_new_canvas(w, h);

        // Carga de datos
        for (i = 0; i < h; i++)
        {
            for (j = 0; j < w; j++)
            {
                PGMimage::_ignore_comments(is);
                if (is >> aux) auxcnv[i][j] = scale * aux;
                else { errors = true; break; }
            }
            if (errors) break;
        }

        // Si no ha fallado la carga de valores
        if (!errors)
        {
            // Actualización del objeto
            PGMimage::_canvas_destroy(c._height, c._canvas);
            c._canvas = auxcnv;
            c._width = w;
            c._height = h;
            c._color_depth = d;
        }
        // En caso de falla, se deja el objeto intacto y se destruye auxcnv
        else PGMimage::_canvas_destroy(h, auxcnv);
    }

    if (errors) // Si hubo errores, se indica en el stream
        is.clear(ios::badbit);

    return is;
}



/*|/////////////////////////////////|   *)  |\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////| Utilidades internas |\\\\\\\\\\\\\\\\\\\\\\\\\\|*/
/*|/////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|*/

// Ignorar comentarios estilo PGM en un stream
void PGMimage::_ignore_comments(istream &s)
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
pixel_t** PGMimage::_new_canvas(size_t w, size_t h)
{
    pixel_t **cnv = new pixel_t* [h];

    for (size_t i = 0; i < h; i++)
        cnv[i] = new pixel_t[w];

    return cnv;
}

// Limitar profundidad de color
void PGMimage::_validate_color_depth(pixel_t &d)
{
    if (d > MAX_COLOR_DEPTH) d = MAX_COLOR_DEPTH;
    if (d < MIN_COLOR_DEPTH) d = MIN_COLOR_DEPTH;
}

// Destruir el lienzo sobre el objeto actual
void PGMimage::_canvas_destroy(size_t h, pixel_t **c)
{
    for (size_t i = 0; i < h; i++)
        delete [] c[i];

    delete [] c;
}
