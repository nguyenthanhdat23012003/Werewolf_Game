CC = g++
CFLAGS = -Wall -Wextra

all: server client

server: werewolf_server.cpp
	$(CC) $(CFLAGS) -o werewolf_server werewolf_server.cpp

client: werewolf_client.cpp
	$(CC) $(CFLAGS) -o werewolf_client werewolf_client.cpp

clean:
	rm -f werewolf_server werewolf_client