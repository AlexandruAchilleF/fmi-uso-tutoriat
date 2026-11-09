# USO - Tutoriat 03: Makefile pentru scheletul de laborator
# FMI, Universitatea din Bucuresti

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
TARGET = exercitiu
SRCS = exercitiu.c
OBJS = $(SRCS:.c=.o)

.PHONY: all run clean test script valgrind

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET) 9 10 8

test: all
	@echo "=========================================================="
	@echo "Test 1: Fara argumente (asteptam mesajul de utilizare si cod 2)"
	@echo "Comanda: ./$(TARGET)"
	@echo "=========================================================="
	-./$(TARGET); echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 2: Student promovat (asteptam 9.00 si cod 0)"
	@echo "Comanda: ./$(TARGET) 9 10 8"
	@echo "=========================================================="
	./$(TARGET) 9 10 8; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 3: Student restant (asteptam 4.00 si cod 1)"
	@echo "Comanda: ./$(TARGET) 4 5 3"
	@echo "=========================================================="
	-./$(TARGET) 4 5 3; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 4: Media cu zecimale (asteptam 9.67 si cod 0)"
	@echo "Comanda: ./$(TARGET) 10 9 10"
	@echo "=========================================================="
	./$(TARGET) 10 9 10; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 5: Nota invalida (asteptam eroare pe stderr si cod 2)"
	@echo "Comanda: ./$(TARGET) 7 abc"
	@echo "=========================================================="
	-./$(TARGET) 7 abc; echo "Cod de retur (echo \$$?): $$?"

script: all
	bash note.sh note.txt

# Bonus: verificam ca programul nu pierde memorie si nu lasa fisiere deschise
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(TARGET) 9 10 8

clean:
	rm -f $(TARGET) $(OBJS) *.o *.out
