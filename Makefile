# //////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ #
# |||||||||||||||||||||||||||||| Configuraciones ||||||||||||||||||||||||||||| #
# \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////////// #

# Compilador:
CC = g++
# Flags para linkeo:
LFLAGS = -pedantic -Wall
# Flags para compilación:
CFLAGS = -ansi -pedantic-errors -Wall -O3
# Flags para debugging:
DFLAGS = -g -DDEBUG
# Centinela de debugging:
DEBUG_CENTINEL = .last_debug
DEBUG_CENTINEL_CONTENT = "This file indicates that the last build was made with\
the \'debug\' option."
# Nombre de salida del proyecto:
OUT = tp1
# Directorio de archivos fuente:
SRC_DIR = src
# Directorio de archivos binarios:
BIN_DIR = bin
# Directorio de instalación:
INSTALL_DIR = /usr/bin



# //////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ #
# ||||||||||||||||||||||||| Objetivos y dependencias ||||||||||||||||||||||||| #
# \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////////// #


# |----------------------------| Códigos objeto |----------------------------| #

OBJECTS = $(addprefix $(BIN_DIR)/, \
    PGMimage.o     \
    complex.o      \
    utils.o        \
    cmdline.o      \
    parser.o       \
    optree.o       \
    main.o         \
)
FULL_OUT = $(BIN_DIR)/$(OUT)


# |------------------------| Reglas de construcción |------------------------| #

# Objetivo de fantasía por defecto, para manejar la opción debug
.PHONY: $(OUT)
ifeq (,$(wildcard $(BIN_DIR)/$(DEBUG_CENTINEL)))
# Si no existe el centinela de debug, se procede normalmente
$(OUT): $(FULL_OUT)
else
# Si existe, es necesario limpiar (con lo que también se eliminará el mismo)
$(OUT): clean $(FULL_OUT)
endif

# Construcción del ejecutable de salida
$(FULL_OUT): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(FULL_OUT)

# Construcción de los archivos objeto
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

# Creación del directorio de binarios
$(BIN_DIR):
	mkdir $(BIN_DIR)


# |-----------------------------| Dependencias |-----------------------------| #

$(BIN_DIR)/PGMimage.o: $(addprefix $(SRC_DIR)/, \
    PGMimage.cpp   \
    PGMimage.h     \
) | $(BIN_DIR)

$(BIN_DIR)/complex.o: $(addprefix $(SRC_DIR)/, \
    complex.cpp    \
    complex.h      \
) | $(BIN_DIR)

$(BIN_DIR)/utils.o: $(addprefix $(SRC_DIR)/, \
    utils.cpp      \
    utils.h        \
    complex.h      \
    cmdline.h      \
    node.h         \
    stack.h        \
    queue.h        \
    parser.h       \
) | $(BIN_DIR)

$(BIN_DIR)/cmdline.o: $(addprefix $(SRC_DIR)/, \
    cmdline.cpp    \
    cmdline.h      \
) | $(BIN_DIR)

$(BIN_DIR)/parser.o: $(addprefix $(SRC_DIR)/, \
    parser.cpp     \
    parser.h       \
    node.h         \
    queue.h        \
    stack.h        \
    complex.h      \
) | $(BIN_DIR)

$(BIN_DIR)/optree.o: $(addprefix $(SRC_DIR)/, \
    optree.cpp     \
    optree.h       \
    complex.h      \
    node.h         \
    stack.h        \
    parser.h       \
) | $(BIN_DIR)

$(BIN_DIR)/main.o: $(addprefix $(SRC_DIR)/, \
    main.cpp       \
    PGMimage.h     \
    complex.h      \
    utils.h        \
    cmdline.h      \
    node.h         \
    queue.h        \
    stack.h        \
    parser.h       \
    optree.h       \
) | $(BIN_DIR)



# //////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ #
# ||||||||||||||||||||||||||||| Utilidades extras |||||||||||||||||||||||||||| #
# \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////////// #


# |------------------| Debug (compilar con flags de debug) |-----------------| #

.PHONY: debug
debug: CFLAGS += $(DFLAGS)
debug: LFLAGS += $(DFLAGS)
ifeq (,$(wildcard $(BIN_DIR)/$(DEBUG_CENTINEL)))
# Si no existe el centinela de debug, hay que limpiar y crearlo
debug: clean $(FULL_OUT)
	@ echo "$(DEBUG_CENTINEL_CONTENT)" > $(BIN_DIR)/$(DEBUG_CENTINEL)
else
# Si existe, solo actualizar si es necesario
debug: $(FULL_OUT)
endif


# |----------------------------| Limpiar (todo) |----------------------------| #

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)


# |------------------------| Limpiar códigos objeto |------------------------| #

.PHONY: objclean
objclean:
	rm -f $(BIN_DIR)/*.o


# |---------------| Construir y eliminar archivos temporales |---------------| #

.PHONY: deltemps
deltemps: $(FULL_OUT) objclean


# |-------------------------------| Instalar |-------------------------------| #

.PHONY: install
install: $(FULL_OUT)
	cp $(FULL_OUT) "$(INSTALL_DIR)"


# |------------------------------| Desinstalar |-----------------------------| #

.PHONY: uninstall
uninstall:
	rm -f "$(INSTALL_DIR)/$(OUT)"
