main: main.c server/server.c clients/clients.c socket/socket.c logger/logger.c
	gcc -w -o main main.c logger/logger.c server/server.c socket/socket.c clients/clients.c clients/actions.c -lm

clean:
	rm -rf main
	rm -rf output.txt