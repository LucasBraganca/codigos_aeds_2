CC := gcc
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Werror -g

OBJ := patricia.o
TESTE := test_patricia
PROGRAMA := programa

.PHONY: all test clean valgrind

all: $(PROGRAMA) $(TESTE)

$(OBJ): src/patricia.c include/patricia.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

src/main.o: src/main.c include/patricia.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(PROGRAMA): $(OBJ) src/main.o
	$(CC) $(CFLAGS) $^ -o $@

tests/test_patricia.o: tests/test_patricia.c include/patricia.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TESTE): $(OBJ) tests/test_patricia.o
	$(CC) $(CFLAGS) $^ -o $@

test: $(TESTE)
	./$(TESTE)

valgrind: $(TESTE)
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(TESTE)

clean:
	rm -f $(OBJ) src/main.o tests/test_patricia.o $(PROGRAMA) $(TESTE)
