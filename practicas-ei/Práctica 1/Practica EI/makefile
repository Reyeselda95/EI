.PHONY= clean

CC=g++ 
OPTIONS= -g -std=gnu++0x
DEBUG= #-D DEBUG
LIBDIR=lib
INCLUDEDIR=include
_OBJ= tokenizadorClase.o tokenizador.o
OBJ = $(patsubst %,$(LIBDIR)/%,$(_OBJ))
MAIN=main.cpp

all: practica1
practica1: src/$(MAIN) $(OBJ)
	$(CC) $(OPTIONS) $(DEBUG) -I$(INCLUDEDIR) src/$(MAIN) $(OBJ) -o practica1
$(LIBDIR)/%.o : $(LIBDIR)/%.cpp $(INCLUDEDIR)/%.h
	$(CC) $(OPTIONS) $(DEBUG) -c -I$(INCLUDEDIR) -o $@ $<
clean:
	rm -f $(OBJ)
