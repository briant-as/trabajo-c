CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
LDLIBS = -lm
SOURCES = main.c ascenso.c propulsion.c vuelo_libre.c reportes.c validaciones.c
ifeq ($(OS),Windows_NT)
TARGET = programa.exe
else
TARGET = programa
endif

$(TARGET): $(SOURCES) simulacion.h validaciones.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDLIBS)

.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	-del /Q programa.exe
else
	rm -f programa
endif
