# USO - Tutoriat 02: Makefile pentru solutia completa
# FMI, Universitatea din Bucuresti

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
TARGET = exercitiu
SRCS = exercitiu.c
OBJS = $(SRCS:.c=.o)

.PHONY: all run clean test pipeline valgrind

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET) -n -i linux date.in

test: all
	@echo "=========================================================="
	@echo "Test 1: Fara argumente (asteptam mesajul de utilizare si cod 2)"
	@echo "Comanda: ./$(TARGET)"
	@echo "=========================================================="
	-./$(TARGET); echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 2: Cautare cu fisier (asteptam 2 linii si cod 0)"
	@echo "Comanda: ./$(TARGET) -n Linux date.in"
	@echo "=========================================================="
	./$(TARGET) -n Linux date.in; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 3: Citire din pipe, fara diferenta de majuscule (asteptam 4)"
	@echo "Comanda: cat date.in | ./$(TARGET) -c -i linux"
	@echo "=========================================================="
	cat date.in | ./$(TARGET) -c -i linux; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 4: Redirectare de intrare si stderr ascuns (asteptam 8)"
	@echo "Comanda: ./$(TARGET) -v -i linux < date.in 2> /dev/null | wc -l"
	@echo "=========================================================="
	./$(TARGET) -v -i linux < date.in 2> /dev/null | wc -l
	@echo ""
	@echo "=========================================================="
	@echo "Test 5: Tipar inexistent (asteptam 0 linii si cod 1)"
	@echo "Comanda: ./$(TARGET) windows date.in"
	@echo "=========================================================="
	-./$(TARGET) windows date.in; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 6: Fisier inexistent (asteptam eroare si cod 2)"
	@echo "Comanda: ./$(TARGET) linux lipsa.txt"
	@echo "=========================================================="
	-./$(TARGET) linux lipsa.txt; echo "Cod de retur (echo \$$?): $$?"

pipeline: all
	bash pipeline.sh

# Bonus: verificam ca programul nu pierde memorie si nu lasa fisiere deschise
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(TARGET) -i linux date.in

clean:
	rm -f $(TARGET) $(OBJS) *.o *.out
