checkers: checkers.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f checkers

run: checkers
	./$<
