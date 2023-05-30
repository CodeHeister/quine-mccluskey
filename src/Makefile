IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -Wall -Werror -pedantic

ODIR=obj
LDIR =../lib

LIBS=

_DEPS=mccluskey.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=mccluskey.o test.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean: 
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
