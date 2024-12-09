#
# make: help, all, clean, tar
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
NAME1 = zad4
PLIK_WE = input.txt
PLIK_WY = output.txt
#----------------------
EXEC1 = $(NAME1)
OBJS1 = $(NAME1).o
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
	@echo "make run4 - uruchamia program"
	@echo "make clean - usuwa niepotrzebne pliki"
	@echo "make tar - kompresuje pliki"
#############################
.PHONY: all
all: $(EXEC1)
#############################
$(EXEC1): $(OBJS1)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
.PHONY: run4
run4: $(EXEC1)
	clear
	./$(EXEC1) $(PLIK_WE) $(PLIK_WY)
	@echo "\n===========================\n"
	@diff -s $(PLIK_WE) $(PLIK_WY)
#############################
.PHONY: clean tar
# Porzadki
EXECS = $(EXEC1)
clean:                                                     
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
