#
# make: help, all, clean, run, tar
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
NAME1 = zad1_b
NAME2 = zad2
#----------------------
EXEC1 = $(NAME1)
OBJS1 = $(NAME1).o
#----------------------
EXEC2 = $(NAME2)
OBJS2 = $(NAME2).o
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
	@echo "make run2 - uruchamia program z ćwiczenia 2"
	@echo "make clean - usuwa niepotrzebne pliki"
	@echo "make tar - kompresuje pliki"
#############################
.PHONY: all
all: $(EXEC1) $(EXEC2)
#############################
$(EXEC1): $(OBJS1)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXEC2): $(OBJS2)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
.PHONY: run2
run2: $(EXEC2)
	chmod a+x zad1_b.c 
	./$(EXEC2)
#############################
.PHONY: clean tar
# Porzadki
EXECS = $(EXEC1) $(EXEC2)
clean:                                                     
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
