CC=gcc
CFLAGS=-Wall -Wextra -Werror
DEST_FOLDER=compiled
SCANNER_DEPENDENCIES=scanner.h string.h
MAIN_DEPENDENCIES=test.c test.h scanner.c scanner.h error.h parser.h parser.c codegenerator.h codegenerator.c string_dyn.h string_dyn.c expression.h expression.c

run: 
	./$(DEST_FOLDER)/scanner

scanner: $(SCANNER_DEPENDENCIES)
	$(CC) $(CFLAGS) -o $(DEST_FOLDER)/$@ scanner.c

test: $(MAIN_DEPENDENCIES) test.h
	$(CC) -o $@ test.c scanner.c parser.c codegenerator.c string_dyn.c expression.c

clean: 
	rm -rf $(DEST_FOLDER)/scanner