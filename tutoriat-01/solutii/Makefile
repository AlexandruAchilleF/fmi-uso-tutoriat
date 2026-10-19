# USO - Tutoriat 01: Makefile pentru solutia completa
# FMI, Universitatea din Bucuresti

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
TARGET = exercitiu
SRCS = exercitiu.c
OBJS = $(SRCS:.c=.o)

.PHONY: all run clean test check

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET) date.in

test: all
	@echo "=========================================================="
	@echo "Test 1: Apel fara argumente (asteptam eroare si cod 1)"
	@echo "Comanda: ./$(TARGET)"
	@echo "=========================================================="
	-./$(TARGET)
	@echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 2: Apel cu fisier existent (asteptam succes si cod 0)"
	@echo "Comanda: ./$(TARGET) date.in"
	@echo "=========================================================="
	./$(TARGET) date.in
	@echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 3: Apel cu fisier inexistent (asteptam eroare si cod 1)"
	@echo "Comanda: ./$(TARGET) fisier_inexistent.txt"
	@echo "=========================================================="
	-./$(TARGET) fisier_inexistent.txt
	@echo "Cod de retur (echo \$$?): $$?"

clean:
	rm -f $(TARGET) $(OBJS) *.o
