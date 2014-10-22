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
OUT = tp0
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
OBJECTS = $(BIN_DIR)/PGMimage.o  \
          $(BIN_DIR)/complejo.o  \
          $(BIN_DIR)/utils.o     \
          $(BIN_DIR)/cmdline.o   \
	  $(BIN_DIR)/queue.o	  \
	  $(BIN_DIR)/node.o	  \
	  $(BIN_DIR)/stack.o	  \
          $(BIN_DIR)/main.o


    # ---------------------- Reglas de construcción ---------------------- #
$(BIN_DIR)/$(OUT): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(BIN_DIR)/$(OUT)

$(BIN_DIR)/PGMimage.o: $(SRC_DIR)/PGMimage.cpp \
                       $(SRC_DIR)/PGMimage.h   \
                     | $(BIN_DIR)

$(BIN_DIR)/complejo.o: $(SRC_DIR)/complejo.cpp \
                       $(SRC_DIR)/complejo.h   \
                     | $(BIN_DIR)

$(BIN_DIR)/utils.o:    $(SRC_DIR)/utils.cpp    \
                       $(SRC_DIR)/utils.h      \
                     | $(BIN_DIR)

$(BIN_DIR)/cmdline.o:  $(SRC_DIR)/cmdline.cpp  \
                       $(SRC_DIR)/cmdline.h    \
                     | $(BIN_DIR)

$(BIN_DIR)/queue.o:  $(SRC_DIR)/queue.cpp  \
                       $(SRC_DIR)/queue.h    \
                     | $(BIN_DIR)

$(BIN_DIR)/node.o:  $(SRC_DIR)/node.cpp  \
                       $(SRC_DIR)/node.h    \
                     | $(BIN_DIR)

$(BIN_DIR)/stack.o:  $(SRC_DIR)/stack.cpp  \
                       $(SRC_DIR)/stack.h    \
                     | $(BIN_DIR)

$(BIN_DIR)/main.o:    $(SRC_DIR)/main.cpp     \
                       $(SRC_DIR)/PGMimage.h   \
                       $(SRC_DIR)/complejo.h   \
                       $(SRC_DIR)/utils.h      \
                       $(SRC_DIR)/cmdline.h    \
		       $(SRC_DIR)/queue.h    \
		       $(SRC_DIR)/node.h    \
		       $(SRC_DIR)/stack.h    \
                     | $(BIN_DIR)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN_DIR):
	mkdir $(BIN_DIR)



# //////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ #
# ||||||||||||||||||||||||||||| Utilidades extras |||||||||||||||||||||||||||| #
# \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////////// #
.PHONY: clean objclean deltemps install all remove purge

    # -------------------------- Limpiar (todo) -------------------------- #
clean:
	rm -rf $(BIN_DIR)

    # ---------------------- Limpiar códigos objeto ---------------------- #
objclean:
	rm -f $(BIN_DIR)/*.o

    # ------------- Construir y eliminar archivos temporales ------------- #
deltemps: $(BIN_DIR)/$(OUT) objclean

    # ------------------------------ Instalar ---------------------------- #
install: $(BIN_DIR)/$(OUT)
	@ cp $(BIN_DIR)/$(OUT) "$(INSTALL_DIR)"
	@ echo "'$(OUT)' --> Installed in: $(INSTALL_DIR)"

    # --------------------- Todo (instalar y limpiar) -------------------- #
all: install clean

    # ---------------------------- Desinstalar --------------------------- #
remove:
	rm -f "$(INSTALL_DIR)/$(OUT)"

    # ------------------ Purgar (desinstalar y limpiar) ------------------ #
purge: remove clean
