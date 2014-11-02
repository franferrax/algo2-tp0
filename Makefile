# //////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ #
# |||||||||||||||||||||||||||||| Configuraciones ||||||||||||||||||||||||||||| #
# \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////////// #

# Compilador:
CC = g++
# Flags para linkeo:
LFLAGS = -pedantic -Wall
# Flags para compilación:
CFLAGS = -ansi -pedantic-errors -Wall -O3
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

    # -------------------------- Códigos objeto -------------------------- #
OBJECTS = $(addprefix $(BIN_DIR)/, \
    PGMimage.o     \
    complex.o      \
    utils.o        \
    cmdline.o      \
    parser.o       \
    main.o         \
)
FULL_OUT = $(BIN_DIR)/$(OUT)

    # ---------------------- Reglas de construcción ---------------------- #
$(FULL_OUT): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(FULL_OUT) $(DFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@ $(DFLAGS)

$(BIN_DIR):
	mkdir $(BIN_DIR)

    # -------------------------- Dependencias ---------------------------- #
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
) | $(BIN_DIR)

$(BIN_DIR)/cmdline.o: $(addprefix $(SRC_DIR)/, \
    cmdline.cpp    \
    cmdline.h      \
) | $(BIN_DIR)

$(BIN_DIR)/parser.o: $(addprefix $(SRC_DIR)/, \
    parser.cpp     \
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
) | $(BIN_DIR)



# //////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ #
# ||||||||||||||||||||||||||||| Utilidades extras |||||||||||||||||||||||||||| #
# \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////////// #
.PHONY: debug clean objclean deltemps install all remove purge

    # ----------- Debug (limpiar y compilar con flag de debug) ----------- #
debug:
	@ rm -rf $(BIN_DIR)/*.o
	@ make --no-print-directory DFLAGS="-g -DDEBUG"

    # -------------------------- Limpiar (todo) -------------------------- #
clean:
	rm -rf $(BIN_DIR)

    # ---------------------- Limpiar códigos objeto ---------------------- #
objclean:
	rm -f $(BIN_DIR)/*.o

    # ------------- Construir y eliminar archivos temporales ------------- #
deltemps: $(FULL_OUT) objclean

    # ------------------------------ Instalar ---------------------------- #
install: $(FULL_OUT)
	@ cp $(FULL_OUT) "$(INSTALL_DIR)"
	@ echo "'$(OUT)' --> Installed in: $(INSTALL_DIR)"

    # --------------------- Todo (instalar y limpiar) -------------------- #
all: install clean

    # ---------------------------- Desinstalar --------------------------- #
remove:
	rm -f "$(INSTALL_DIR)/$(OUT)"

    # ------------------ Purgar (desinstalar y limpiar) ------------------ #
purge: remove clean
