all: server client

server:
	gcc -g -Wall -Werror -lpthread -o bankingServer bankingServer.c

client:
	gcc -g -Wall -Werror -lpthread -o bankingClient bankingClient.c

clean: 
	rm -rf bankingServer bankingClient
