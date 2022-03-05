CC=clang
CFLAGS=-Werror=vla -Wextra -Wall -Wshadow -Wswitch-default -std=c11
CFLAG_SAN=$(CFLAGS) -fsanitize=address
DEPS=
OBJ=robots.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

robots: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	
robots_asan: $(OBJ)
	$(CC) -o $@ $^ $(CFLAG_SAN)
	
clean:
	rm *.o robots robots_asan
