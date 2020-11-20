CC=cc
CPP=g++

BUILD_CONFIG = -g -DDEBUG

INCLUDES = -Iinclude -Isrc/


CFLAGS= $(BUILD_CONFIG) -fPIC -Wall $(INCLUDES) -std=gnu99 -pedantic 

SOURCES = $(wildcard src/*.c) $(wildcard src/*/*.c)
TEST_SOURCES = $(wildcard tests/*.c)
TEST_SOURCES_CPP = $(wildcard Decompiler/*.cpp) DeviceTreeCtor/AMLDecompilerInterface.cpp
#$(wildcard DeviceTreeCtor/*.cpp) 

OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE= libacpi.so
TEST_EXEC = UnitTests
TEST_CPP = decompiler

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


test:
	$(CC)  $(CFLAGS) $(TEST_SOURCES) -L. -lacpi -o $(TEST_EXEC)

decomp:
	$(CPP) -std=gnu++14 $(INCLUDES) $(TEST_SOURCES_CPP) -IDeviceTreeCtor -L. -lacpi -o $(TEST_CPP)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(EXECUTABLE)
	rm -f $(TEST_CPP)