CC=gcc
CFLAGS=-Wall -Wextra -Werror
DEST_FOLDER=compiled
SCANNER_DEPENDENCIES=scanner.h string.h
MAIN_DEPENDENCIES=ifj20go.c ifj20go.h scanner.c scanner.h error.h parser.h parser.c codegenerator.h codegenerator.c string_dyn.h string_dyn.c expression.h expression.c

ifj20go: $(MAIN_DEPENDENCIES) ifj20go.h
	$(CC) -o $@ ifj20go.c scanner.c parser.c codegenerator.c string_dyn.c expression.c -lm

clean: 
	rm -rf ifj20go