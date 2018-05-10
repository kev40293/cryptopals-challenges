
.SUFFIXES:.o.c

CFLAGS=-std=c++14 -g -Iinclude

default: all

all: build/main

build/%.o: src/%.c
	$(CXX) -c $(CFLAGS) -o $@ $<

build/%.o: src/%.cpp
	$(CXX) -c $(CFLAGS) -o $@ $<

build/main: build/b64.o build/xorTest.o build/xorEnc.o build/main.o build/aes.o build/detectECB.o build/utils.o build/encOracle.o build/breakECB.o
	$(CXX) $^ -o $@ -lcrypto -lssl

.PHONY: clean

clean:
	rm -f build/*.o
	rm -f build/main
