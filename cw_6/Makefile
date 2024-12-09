#
# make: help, all, clean, tar
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
NAME1 = zad6
NAME2 = inkrementator
LIB1 = semafory
ILOSC_PROCESOW = 3
ILOSC_SEKCJI = 2
PLIK_WE_WY = numer.txt
SEMAFOR = /moj_semafor
SYNCHRONIZUJ = 1
#----------------------
EXEC1 = $(NAME1).x
OBJS1 = $(NAME1).o

EXEC2  = $(NAME2).x
OBJS2  = $(NAME2).o

LIBS1 = $(LIB1).c
#----------------------
#############################
CFLAGS = -pthread -Wall -std=c99 -pedantic -O
LFLAGS = -pthread -Wall -std=c99 -pedantic -O
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
	@echo "make run6 - uruchamia program"
	@echo "make clean - usuwa niepotrzebne pliki"
	@echo "make tar - kompresuje pliki"
#############################
.PHONY: all
all: $(EXEC1) $(EXEC2)
#############################
$(EXEC1): $(OBJS1) $(LIBS1)
	$(LD) -o $@ $(LFLAGS) $^
#############################
$(EXEC2): $(OBJS2) $(LIBS1)
	$(LD) -o $@ $(LFLAGS) $^
#############################
.PHONY: run6
run6: $(EXEC1) $(EXEC2)
	clear
	./$(EXEC1) $(EXEC2) $(ILOSC_PROCESOW) $(ILOSC_SEKCJI) $(PLIK_WE_WY) $(SEMAFOR) $(SYNCHRONIZUJ)
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
