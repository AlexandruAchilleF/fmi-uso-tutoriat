# USO - Tutoriat 06: Makefile pentru solutia completa
# FMI, Universitatea din Bucuresti
# Merge la fel pe Linux (gcc) si pe macOS (acolo "gcc" este de fapt clang).

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
TARGET = exercitiu
SRCS = exercitiu.c
OBJS = $(SRCS:.c=.o)

.PHONY: all run demo clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Directorul demo/ pe care lucram: fisiere cu drepturi diferite.
# Il facem din Makefile pentru ca git nu pastreaza drepturile (doar bitul x).
demo:
	rm -rf demo
	mkdir demo demo/sub demo/public
	echo "Salut" > demo/a.txt
	echo "parola: 1234" > demo/secret.txt
	printf '#!/bin/bash\necho "Salut din script"\n' > demo/script.sh
	cp demo/script.sh demo/prog
	chmod 644 demo/a.txt
	chmod 600 demo/secret.txt
	chmod 755 demo/script.sh demo/sub
	chmod 4755 demo/prog
	chmod 1777 demo/public

run: all demo
	./$(TARGET) demo/*

test: all demo
	@echo "=========================================================="
	@echo "Test 1: Fara argumente (asteptam mesajul de utilizare si cod 1)"
	@echo "Comanda: ./$(TARGET)"
	@echo "=========================================================="
	-./$(TARGET); echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 2: Tot ce e in demo/ (6 linii; prog are s, public are t)"
	@echo "Comanda: ./$(TARGET) demo/*"
	@echo "=========================================================="
	./$(TARGET) demo/*; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 3: Comparam cu ls -ld (aceleasi drepturi, acelasi proprietar, aceeasi marime)"
	@echo "Comanda: ls -ld demo/*"
	@echo "=========================================================="
	ls -ld demo/*
	@echo ""
	@echo "=========================================================="
	@echo "Test 4: Un fisier bun si unul lipsa (o linie + perror, cod 1)"
	@echo "Comanda: ./$(TARGET) demo/a.txt lipsa.txt"
	@echo "=========================================================="
	-./$(TARGET) demo/a.txt lipsa.txt; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 5: Un director al sistemului (proprietar root, sticky bit: drwxrwxrwt 1777)"
	@echo "Comanda: ./$(TARGET) /tmp/"
	@echo "=========================================================="
	./$(TARGET) /tmp/; echo "Cod de retur (echo \$$?): $$?"

clean:
	rm -rf $(TARGET) $(OBJS) *.o *.out demo
