CC=clang
CFLAGS=-Werror=vla -Wextra -Wall -Wshadow -Wswitch-default -std=c11 -g
CFLAG_SAN=$(CFLAGS) -fsanitize=address
DEPS=
OBJ=office.o

%.o: %.c %.h $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS_SAN)
	
clean:
	rm *.o
