compile:
	gcc -g -o chttpserver -Wall -Wextra src/server.c src/main.c

compile_test:
	gcc -g -o chttpserver_test -Wall -Wextra src/server.c tests/tests.c tests/main.c -lm -L./tests/lib -lctestfmk
