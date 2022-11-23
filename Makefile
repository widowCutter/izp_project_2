test run:
	gcc -std=c99  -DNDEBUG cluster.c -o cluster -lm
	./cluster ./input.txt 5
build and run:
	gcc -std=c99 -Wall -Wextra -Werror -DNDEBUG cluster.c -o cluster -lm
	./cluster ./input.txt 5
