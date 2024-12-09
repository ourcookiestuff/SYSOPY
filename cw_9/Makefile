#
# make: help, all, clean, tar
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
NAME1 = zad9
WATKI = 4
SEKCJE = 2
LIB1 = watki
#----------------------
EXEC1 = $(NAME1).x
OBJS1 = $(NAME1).o

HEADS1 = $(LIB1).h
LIBS1 = $(LIB1).c
#----------------------
#############################
CFLAGS = -Wall -std=c99 -pedantic -O
LFLAGS = -Wall -std=c99 -pedantic -O
LDLIBS = -lpthread
CO = gcc
LD = $(CO)
AR = ar
ARFLAGS = rsv
#############################
%.o: %.c
	$(CO) $(CFLAGS) -c $<
#############################
.PHONY: help
help:
	@echo "make all - kompiluje wszystkie program"
	@echo "make run9 - uruchamia program"
	@echo "make clean - usuwa niepotrzebne pliki"
	@echo "make tar - kompresuje pliki"
#############################
.PHONY: all
all: $(EXEC1)
#############################
$(EXEC1): $(OBJS1) $(LIBS1)
	$(LD) -o $@ $(LFLAGS) $^ $(LDLIBS)
$(OBJS1): $(HEADS1)
#############################
.PHONY: run9
run9: $(EXEC1)
	clear
	./$(EXEC1) $(WATKI) $(SEKCJE)
#############################
.PHONY: clean tar
# Porzadki
clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
