CC = gcc
CFLAGS = -Og

.PHONY: main run clean

RAW_SOURCE_FILE = main.c basememt.c question1.c question2.c question3.c question4.c question5.c extra.c

SOURCE_FILE = $(addprefix src/, $(RAW_SOURCE_FILE))

INCLUDE_PATH = -Ilib/include -Iinclude

main: $(SOURCE_FILE) lib/src/extmem.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(SOURCE_FILE) lib/src/extmem.c -o main

run: main
	./main

clean:
	rm main