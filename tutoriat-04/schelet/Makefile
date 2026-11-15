# USO - Tutoriat 04: Makefile pentru scheletul de laborator
# FMI, Universitatea din Bucuresti
# Merge la fel pe Linux (gcc) si pe macOS (acolo "gcc" este de fapt clang).

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
TARGET = exercitiu
SRCS = exercitiu.c
OBJS = $(SRCS:.c=.o)

.PHONY: all run clean test script

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET) mesaj.txt copie.txt

test: all
	@echo "=========================================================="
	@echo "Test 1: Fara argumente (asteptam mesajul de utilizare si cod 1)"
	@echo "Comanda: ./$(TARGET)"
	@echo "=========================================================="
	-./$(TARGET); echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 2: Copiere reusita (asteptam 'Am copiat 204 octeti' si cod 0)"
	@echo "Comanda: ./$(TARGET) mesaj.txt copie.txt"
	@echo "=========================================================="
	./$(TARGET) mesaj.txt copie.txt; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 3: Copia e identica cu originalul (cmp nu afiseaza nimic)"
	@echo "Comanda: cmp mesaj.txt copie.txt"
	@echo "=========================================================="
	-cmp mesaj.txt copie.txt && echo "Fisierele sunt identice"
	@echo ""
	@echo "=========================================================="
	@echo "Test 4: Sursa nu exista (asteptam perror: No such file or directory, cod 1)"
	@echo "Comanda: ./$(TARGET) lipsa.txt copie.txt"
	@echo "=========================================================="
	-./$(TARGET) lipsa.txt copie.txt; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 5: Destinatia nu poate fi creata (folder inexistent, cod 1)"
	@echo "Comanda: ./$(TARGET) mesaj.txt nu_exista/copie.txt"
	@echo "=========================================================="
	-./$(TARGET) mesaj.txt nu_exista/copie.txt; echo "Cod de retur (echo \$$?): $$?"

script: all
	bash verifica.sh

clean:
	rm -f $(TARGET) $(OBJS) *.o *.out copie.txt erori.txt
	rm -rf backup
