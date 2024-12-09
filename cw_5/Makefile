#
# make: help, all, clean, tar
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
NAME1 = zad5
PROD = producent
KONS = konsument
PLIK_WE = input.txt
PLIK_WY = output.txt
POTOK = FIFO
SCIEZKA = ./
#----------------------
EXEC1 = $(NAME1)
OBJS1 = $(NAME1).o


EXECP  = $(PROD)
OBJSP  = $(PROD).o

EXECK  = $(KONS)
OBJSK  = $(KONS).o
#----------------------
#############################
COFLAGS = -Wall -O
LDFLAGS = -Wall -O
CO = gcc
LD = $(CO)
#############################
%.o: %.c
	$(CO) $(COFLAGS) -c $<
#############################
.PHONY: help
help:
	@echo "make all - kompiluje wszystkie program"
	@echo "make runA - uruchamia program z podpunkt a"
	@echo "make runB - uruchamia program z podpunkt b"
	@echo "make clean - usuwa niepotrzebne pliki"
	@echo "make tar - kompresuje pliki"
#############################
.PHONY: all
all: $(EXEC1) $(EXECP) $(EXECK)
#############################
$(EXEC1): $(OBJS1) $(LIBS1)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXECP): $(OBJSP) $(LIBSP)
	$(LD) -o $@ $(LFLAGS) $^
#############################
$(EXECK): $(OBJSK) $(LIBSK)
	$(LD) -o $@ $(LFLAGS) $^
#############################
.PHONY: runA runB
runA: $(EXEC1) $(EXECP) $(EXECK)
	clear
	./$(EXEC1) $(EXECP) $(EXECK) $(PLIK_WE) $(PLIK_WY) $(POTOK)
	@echo "\n===========================\n"
	@diff -s $(PLIK_WE) $(PLIK_WY)

runB: $(EXECP) $(EXECK)
	clear
	open -a XQuartz
	mkfifo -m 0644 $(POTOK)
	xterm -title $(PROD) -e ./$(EXECP) $(PLIK_WE) $(POTOK) & xterm -title $(KONS) -e ./$(EXECK) $(PLIK_WY) $(POTOK)
	rm $(POTOK)
	@echo "\n=============================\n"
	@diff -s $(PLIK_WE) $(PLIK_WY)
#############################
.PHONY: clean tar
# Porzadki
EXECS = $(EXEC1) $(PROD) $(KONS)
clean:
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
