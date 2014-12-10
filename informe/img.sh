#!/bin/bash

# Directorio de imágenes
mkdir -p informe/img

# Archivo grid.pgm (ENUNCIADO)
[ -f informe/img/grid-id.pgm ] || bin/tp1 -i test/grid.pgm -o informe/img/grid-id.pgm -f "z"
[ -f informe/img/grid-exp.pgm ] || bin/tp1 -i test/grid.pgm -o informe/img/grid-exp.pgm -f "exp(z)"

# Archivo evolution.pgm (ENUNCIADO)
[ -f informe/img/evolution-id.pgm ] || bin/tp1 -i test/evolution.pgm -o informe/img/evolution-id.pgm -f "z"
[ -f informe/img/evolution-exp.pgm ] || bin/tp1 -i test/evolution.pgm -o informe/img/evolution-exp.pgm -f "exp(z)"
[ -f informe/img/evolution-sqr.pgm ] || bin/tp1 -i test/evolution.pgm -o informe/img/evolution-sqr.pgm -f "z^2"
[ -f informe/img/evolution-cube.pgm ] || bin/tp1 -i test/evolution.pgm -o informe/img/evolution-cube.pgm -f "z^3"

# Archivo clocks.pgm
[ -f informe/img/clocks-1.pgm ] || bin/tp1 -i test/clocks.pgm -r 6.4,4.8,0,0 -o informe/img/clocks-1.pgm -f "z"
[ -f informe/img/clocks-2.pgm ] || bin/tp1 -i test/clocks.pgm -r 6.4,4.8,0,0 -o informe/img/clocks-2.pgm -f "j*z"
[ -f informe/img/clocks-3.pgm ] || bin/tp1 -i test/clocks.pgm -r 6.4,4.8,0,0 -o informe/img/clocks-3.pgm -f "exp(z^3)"
[ -f informe/img/clocks-4.pgm ] || bin/tp1 -i test/clocks.pgm -r 6.4,4.8,0,0 -o informe/img/clocks-4.pgm -f "exp(z^j)"
[ -f informe/img/clocks-5.pgm ] || bin/tp1 -i test/clocks.pgm -r 6.4,4.8,0,0 -o informe/img/clocks-5.pgm -f "j^(-z)"

# Archivo pattern.pgm
[ -f informe/img/pattern-1.pgm ] || bin/tp1 -i test/pattern.pgm -r 5,3.75,0,0 -o informe/img/pattern-1.pgm -f "z"
[ -f informe/img/pattern-2.pgm ] || bin/tp1 -i test/pattern.pgm -r 5,3.75,0,0 -o informe/img/pattern-2.pgm -f "(z*1.7*e^(-j*pi/4))"
[ -f informe/img/pattern-3.pgm ] || bin/tp1 -i test/pattern.pgm -r 5,3.75,0,0 -o informe/img/pattern-3.pgm -f "(z*1.5)^0.85"

# Archivo de 9097x6500 px
[ -f informe/img/9097x6500-1.pgm ] || bin/tp1 -i test/9097x6500.pgm -r 9.097,6.5,0,0 -o informe/img/9097x6500-1.pgm -f "z"
[ -f informe/img/9097x6500-2.pgm ] || bin/tp1 -i test/9097x6500.pgm -r 9.097,6.5,0,0 -o informe/img/9097x6500-2.pgm -f "z^(j*sin(z))"

# Conversión de imágenes a PNG
cd informe/img
mkdir -p png
for file in *.pgm; do
    convert -resize '350' $file png/${file%pgm}png
done
