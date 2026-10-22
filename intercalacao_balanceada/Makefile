CC = gcc
CPPFLAGS = -Iinclude
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
TARGET = build/intercalacao
SOURCES = src/main.c src/blocos.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES) include/blocos.h Makefile
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) $(LDFLAGS) $(LDLIBS) -o $@

run: $(TARGET)
	./$(TARGET) exemplos/data.txt build/saida.txt 3

clean:
	$(RM) $(TARGET)
