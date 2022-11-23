run:
	gcc -std=c99  -DNDEBUG cluster.c -o cluster -lm
	./cluster objekty 5
build:
	gcc -std=c99 -Wall -Wextra -Werror -DNDEBUG cluster.c -o cluster -lm
	./cluster ./objekty 5
