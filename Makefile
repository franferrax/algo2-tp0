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
SRC_PATH = src
# Directorio de archivos binarios:
BIN_PATH = bin
# Directorio de instalación:
INSTALL_PATH = /usr/bin



# //////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ #
# ||||||||||||||||||||||||| Objetivos y dependencias ||||||||||||||||||||||||| #
# \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////////// #

    # -------------------------- Códigos objeto -------------------------- #
OBJECTS = $(BIN_PATH)/GrayscaleCanvasPGM.o \
          $(BIN_PATH)/complejo.o           \
	  $(BIN_PATH)/utils.o		    \
          $(BIN_PATH)/main.o


    # ---------------------- Reglas de construcción ---------------------- #
$(BIN_PATH)/$(OUT): $(OBJECTS) $(BIN_PATH)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(BIN_PATH)/$(OUT)

$(BIN_PATH)/GrayscaleCanvasPGM.o: $(SRC_PATH)/GrayscaleCanvasPGM.cpp \
                                  $(SRC_PATH)/GrayscaleCanvasPGM.h   \
                                  $(BIN_PATH)

$(BIN_PATH)/complejo.o:           $(SRC_PATH)/complejo.cpp           \
                                  $(SRC_PATH)/complejo.h             \
                                  $(BIN_PATH)

$(BIN_PATH)/utils.o:		    $(SRC_PATH)/utils.cpp           \
                                  $(SRC_PATH)/utils.h             \
                                  $(BIN_PATH)

$(BIN_PATH)/main.o:               $(SRC_PATH)/main.cpp               \
                                  $(SRC_PATH)/GrayscaleCanvasPGM.h   \
                                  $(SRC_PATH)/complejo.h             \
				  $(SRC_PATH)/utils.h             \
                                  $(BIN_PATH)

$(BIN_PATH)/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN_PATH):
	mkdir $(BIN_PATH)



# //////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ #
# ||||||||||||||||||||||||||||| Utilidades extras |||||||||||||||||||||||||||| #
# \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////////// #

    # ---------------------- Limpiar códigos objeto ---------------------- #
objclean:
	rm -f $(BIN_PATH)/*.o

    # -------------------------- Limpiar (todo) -------------------------- #
clean:
	rm -rf $(BIN_PATH)

    # ------------- Construir y eliminar archivos temporales ------------- #
deltemps: $(BIN_PATH)/$(OUT) objclean

    # ------------------------------ Instalar ---------------------------- #
install: $(BIN_PATH)/$(OUT)
	@ cp $(BIN_PATH)/$(OUT) "$(INSTALL_PATH)"
	@ echo "'$(OUT)' --> Instalado en: $(INSTALL_PATH)"


    # --------------------- Todo (instalar y limpiar) -------------------- #
all: install clean


    # ---------------------------- Desinstalar --------------------------- #
remove:
	rm -f "$(INSTALL_PATH)/$(OUT)"


    # ------------------ Purgar (desinstalar y limpiar) ------------------ #
purge: remove clean
