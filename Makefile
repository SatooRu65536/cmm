CC=gcc
CFLAGS=-O

cmm-lang: main.c
	$(CC) main.c $(CFLAGS) -o cmm
