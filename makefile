CFLAGS = -Wall -Wextra -pedantic

main:
	gcc $(CFLAGS) lexer.c
	./a.out < test
	rm a.out

debug:
	gcc $(CFLAGS) -g lexer.c