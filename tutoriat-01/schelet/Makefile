# USO - Tutoriat 01: Makefile pentru compilarea scheletului
# FMI, Universitatea din Bucuresti

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
TARGET = exercitiu
SRCS = exercitiu.c
OBJS = $(SRCS:.c=.o)

.PHONY: all run clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET) date.in

test: all
	@echo "=== Test 1: Rulare fara argumente (asteptat: cod de eroare 1) ==="
	-./$(TARGET)
	@echo "Cod iesire: $$?"
	@echo ""
	@echo "=== Test 2: Rulare cu date.in (asteptat: cod de succes 0) ==="
	./$(TARGET) date.in
	@echo "Cod iesire: $$?"

clean:
	rm -f $(TARGET) $(OBJS) *.o
