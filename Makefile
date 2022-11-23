CFLAGS=-std=c99 -g -Wall -Werror -pedantic -lm
OBJECTS=$(SOURCES:.c=.o)
SOURCES=$(wildcard *.c)
TARGET=IFJ22

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)
