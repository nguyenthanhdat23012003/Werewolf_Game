CC = g++
CFLAGS = -Wall -Wextra
LIBS = -lSDL2 -lSDL2_ttf

# Directories for source files and object files
SRCDIR = .
LIBDIR = lib
OBJDIR = obj

# Source files for each target
SERVER_SRCS = $(SRCDIR)/werewolf_server.cpp
CLIENT_SRCS = $(SRCDIR)/werewolf_client.cpp
APP_SRCS = $(SRCDIR)/app.cpp $(LIBDIR)/RequestHelper.cpp $(LIBDIR)/gui/LobbyListState.cpp $(LIBDIR)/gui/InLobbyState.cpp $(LIBDIR)/gui/InGameState.cpp $(LIBDIR)/gui/SDLRenderer.cpp $(LIBDIR)/gui/GameState.cpp

# Object files for each target
SERVER_OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SERVER_SRCS))
CLIENT_OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CLIENT_SRCS))
APP_OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(APP_SRCS))

# Targets to build
all: werewolf_server werewolf_client app

werewolf_server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJS)

werewolf_client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJS)

app: $(APP_OBJS)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJS) $(LIBS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the obj directory exists before compiling
$(shell mkdir -p $(OBJDIR) >/dev/null)

clean:
	rm -f werewolf_server werewolf_client app $(OBJDIR)/*.o
