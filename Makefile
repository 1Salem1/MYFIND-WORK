CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -D_GNU_SOURCE

BIN = myfind
OBJS = src/$(BIN).o src/main.o src/is_dir.o src/is_file.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS)

check: $(OBJS)
	$(CC) -o $(BIN) $(OBJS)
	./tests/tests.sh

clean:
	$(RM) $(BIN) $(OBJS)