CC=cc

BUILD_CONFIG = -g -DDEBUG

INCLUDES = -Iinclude -Isrc/


CFLAGS= $(BUILD_CONFIG) -fPIC -Wall $(INCLUDES) -std=gnu99 -pedantic 
SOURCES = $(wildcard src/*.c) $(wildcard src/*/*.c)


OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE= libacpi.so

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ -shared

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@


install:
	cp $(EXECUTABLE) /usr/local/lib
	mkdir -p /usr/local/include/libacpi
	cp include/*.h /usr/local/include/libacpi/
	ldconfig


clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(EXECUTABLE)