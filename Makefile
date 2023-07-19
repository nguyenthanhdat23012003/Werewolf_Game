CC = g++
CFLAGS = -Wall -Wextra

all: server client app

server: werewolf_server.cpp
	$(CC) $(CFLAGS) -o werewolf_server werewolf_server.cpp

client: werewolf_client.cpp
	$(CC) $(CFLAGS) -o werewolf_client werewolf_client.cpp

app: app.cpp
	$(CC) $(CFLAGS) -o app app.cpp -lSDL2 -lSDL2_ttf

clean:
	rm -f werewolf_server werewolf_client app