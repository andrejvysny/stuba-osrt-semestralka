main: main.c server/server.c clients/clients.c socket/socket.c logger/logger.c
	gcc -w -o main main.c logger/logger.c server/server.c socket/socket.c clients/clients.c

clean:
	rm -rf main