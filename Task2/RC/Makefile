CC=clang
CFLAGS=-Werror=vla -Wextra -Wall -Wshadow -Wswitch-default -std=c11
CFLAG_SAN=$(CFLAGS) -fsanitize=address
DEPS=
OBJ=rc.o

%.o: %.c %.h $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
clean:
	rm *.o
