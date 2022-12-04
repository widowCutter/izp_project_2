debug:
	gcc -std=c99  cluster.c -o cluster -lm -g
	./cluster objekty 1
relase:
	gcc -std=c99 -Wall -Wextra -Werror -DNDEBUG cluster.c -o cluster -lm
	./cluster objekty 5
