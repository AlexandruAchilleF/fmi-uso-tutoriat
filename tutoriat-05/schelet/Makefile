# USO - Tutoriat 05: Makefile pentru scheletul de laborator
# FMI, Universitatea din Bucuresti
# Merge la fel pe Linux (gcc) si pe macOS (acolo "gcc" este de fapt clang).

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
TARGET = exercitiu
SRCS = exercitiu.c
OBJS = $(SRCS:.c=.o)

.PHONY: all run demo clean test script

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Directorul demo/ pe care lucram. Il facem din Makefile pentru ca git
# nu pastreaza hard link-urile si numerele de i-nod.
demo:
	rm -rf demo
	mkdir demo demo/sub
	echo "Salut, eu sunt a.txt" > demo/a.txt
	echo "Eu sunt b.txt" > demo/b.txt
	echo "Pe mine nu ma vede ls" > demo/.ascuns
	ln demo/a.txt demo/a_hard.txt
	ln -s a.txt demo/a_soft.txt

run: all demo
	./$(TARGET) demo

test: all demo
	@echo "=========================================================="
	@echo "Test 1: Prea multe argumente (asteptam mesajul de utilizare si cod 1)"
	@echo "Comanda: ./$(TARGET) demo altceva"
	@echo "=========================================================="
	-./$(TARGET) demo altceva; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 2: Listam demo/ (5 linii, fara .ascuns; ordinea poate diferi)"
	@echo "Comanda: ./$(TARGET) demo"
	@echo "=========================================================="
	./$(TARGET) demo; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 3: Comparam cu ls -i (aceleasi numere; a.txt si a_hard.txt au acelasi i-nod)"
	@echo "Comanda: ls -i1 demo"
	@echo "=========================================================="
	ls -i1 demo
	@echo ""
	@echo "=========================================================="
	@echo "Test 4: Directorul nu exista (asteptam perror: No such file or directory, cod 1)"
	@echo "Comanda: ./$(TARGET) lipsa"
	@echo "=========================================================="
	-./$(TARGET) lipsa; echo "Cod de retur (echo \$$?): $$?"
	@echo ""
	@echo "=========================================================="
	@echo "Test 5: Un fisier, nu un director (asteptam perror: Not a directory, cod 1)"
	@echo "Comanda: ./$(TARGET) Makefile"
	@echo "=========================================================="
	-./$(TARGET) Makefile; echo "Cod de retur (echo \$$?): $$?"

script: all demo
	bash legaturi.sh

clean:
	rm -rf $(TARGET) $(OBJS) *.o *.out demo lab
