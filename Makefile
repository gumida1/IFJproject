CC=gcc
CFLAGS=-Wall -Wextra -Werror
DEST_FOLDER=compiled
SCANNER_DEPENDENCIES=scanner.h

run: 
	./$(DEST_FOLDER)/scanner

scanner: $(SCANNER_DEPENDENCIES)
	$(CC) $(CFLAGS) -o $(DEST_FOLDER)/$@ scanner.c

test: $(SCANNER_DEPENDENCIES)
	$(CC) -o $(DEST_FOLDER)/$@ test.c scanner.c

clean: 
	rm -rf $(DEST_FOLDER)/scanner