CC=clang
CFLAGS=-Werror=vla -Wextra -Wall -Wshadow -Wswitch-default -std=c11
CFLAG_SAN=$(CFLAGS) -fsanitize=address
DEPS=
OBJ=battleships.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

battleships: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	
battleships_asan: $(OBJ)
	$(CC) -o $@ $^ $(CFLAG_SAN)
	
clean:
	rm *.o battleships battleships_asan
