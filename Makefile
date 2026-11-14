CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

OBJS = main.c ascenso.c vuelo_libre.c reportes.c validaciones.c

simulador: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o simulador $(LDFLAGS)

clean:
	del /Q simulador.exe