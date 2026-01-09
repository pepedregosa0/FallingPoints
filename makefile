CC=gcc
CFLAGS=-Wall -Wextra -Werror -g $(shell sdl2-config --cflags)
LIBS=$(shell sdl2-config --libs)

NAME=points

SRC=$(NAME).c
OPTIMIZED_SRC=$(NAME)optimized.c

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

optimized: $(OPTIMIZED_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(NAME)
