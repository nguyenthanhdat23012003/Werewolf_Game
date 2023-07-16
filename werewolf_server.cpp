#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <thread>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "lib/Message.h"
#include "lib/Lobby.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 512

using namespace std;

vector<Lobby> lobbies;

void parseHTTPRequest(const char *httpRequest, int httpRequestLength, char *requestBody, int &contentLength);
void handleRequest(const string &request, int sd, int *clientSockets, int maxClients);
string getContentOfChatRequestBody(const std::string &message);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("./server <PORT>\n");
        return 1;
    }

    int serverSocket, clientSockets[MAX_CLIENTS], opt, maxSockets, activity, valread, sd;
    int maxClients = MAX_CLIENTS;

    fd_set readfds;
    char buffer[BUFFER_SIZE]; // +2 for the delimiter and null terminator

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("Failed to create socket");
        exit(1);
    }

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(atoi(argv[1]));

    // Bind the server socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Bind failed");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, maxClients) < 0)
    {
        perror("Listen failed");
        exit(1);
    }
    else
        printf("Server is listening on port %s\n", argv[1]);

    // Accept the incoming connection
    int addressLength = sizeof(serverAddress);

    // Initialize clientSockets array
    for (int i = 0; i < maxClients; i++)
    {
        clientSockets[i] = 0;
    }

    while (true)
    {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add server socket to the set
        FD_SET(serverSocket, &readfds);
        maxSockets = serverSocket;

        // Add child sockets to the set
        for (int i = 0; i < maxClients; i++)
        {
            sd = clientSockets[i];
            if (sd > 0)
            {
                FD_SET(sd, &readfds);
            }
            if (sd > maxSockets)
            {
                maxSockets = sd;
            }
        }

        // Wait for activity on any socket
        activity = select(maxSockets + 1, &readfds, NULL, NULL, NULL);
        printf("Activity detected\n");
        if ((activity < 0) && (errno != EINTR))
        {
            perror("Select error");
        }

        // If the server socket is set, it means a new connection is coming
        if (FD_ISSET(serverSocket, &readfds))
        {
            printf("New connection\n");
            int newSocket = accept(serverSocket, (struct sockaddr *)&serverAddress, (socklen_t *)&addressLength);
            if (newSocket < 0)
            {
                perror("Accept failed");
                exit(1);
            }

            // Add new socket to the array of client sockets
            for (int i = 0; i < maxClients; i++)
            {
                if (clientSockets[i] == 0)
                {
                    clientSockets[i] = newSocket;
                    break;
                }
            }
        }

        // Print clients list
        for (int i = 0; i < maxClients; i++)
        {
            printf("%d ", clientSockets[i]);
        }
        printf("\n");

        // Check other sockets for incoming data
        for (int i = 0; i < maxClients; i++)
        {
            sd = clientSockets[i];
            if (FD_ISSET(sd, &readfds))
            {
                // Check if it was a closing request or actual data
                valread = read(sd, buffer, sizeof(buffer));
                if (valread == 0)
                {
                    // Close the socket and mark as 0 in the array for reuse
                    printf("Closing a socket ...\n");
                    close(sd);
                    clientSockets[i] = 0;
                }
                else
                {
                    // Process the received data
                    buffer[valread] = '\0'; // Add null terminator to the end
                    char requestBody[BUFFER_SIZE];
                    requestBody[0] = '\0';
                    int contentLength = 0;
                    parseHTTPRequest(buffer, strlen(buffer), requestBody, contentLength);
                    cout << "Request Body Parsed: " << requestBody << endl;
                    // is this multithreading?
                    thread handleRequestThread(handleRequest, requestBody, sd, clientSockets, maxClients);
                    handleRequestThread.detach();
                    break;
                    //handleRequest(requestBody, sd, clientSockets, maxClients);
                }
            }
        }
    }
    return 0;
}

void parseHTTPRequest(const char *httpRequest, int httpRequestLength, char *requestBody, int &contentLength)
{
    cout << "Parsing...\n";
    cout << "Received Length: " << httpRequestLength << endl;
    istringstream iss(httpRequest);
    char line[BUFFER_SIZE];
    bool isReadingBody = false;

    // Iterate over each line of the HTTP request
    while (iss.getline(line, BUFFER_SIZE))
    {
        if (line[1] == '\0')
        {
            // Empty line indicates the end of the headers and the start of the body
            isReadingBody = true;
        }
        else if (isReadingBody)
        {
            // Copy the line to the request body
            int lineLength = strlen(line);
            strncat(requestBody, line, BUFFER_SIZE - contentLength);
            contentLength += lineLength;

            // Stop reading the body once the specified content length is reached
            if (contentLength >= httpRequestLength)
            {
                break;
            }
        }
        else
        {
            // Check if the line contains the Content-Length header
            if (strncmp(line, "Content-Length:", 15) == 0)
            {
                // Extract the content length value
                contentLength = atoi(line + 16);
            }
        }
    }
}

void handleRequest(const string &request, int sd, int *clientSockets, int maxClients)
{
    static int clientIdCounter = 100;
    static unordered_map<int, int> clientToLobbyMap;
    cout << "In \'handleRequest\' funtction\n";
    cout << "Received body: " << request << endl;
    // Perform request handling based on the type of request
    if (request == "")
    {
        write(sd, "Blank request", strlen("Blank request"));
    }
    else if (request.substr(0, 3) == to_string(CommandMessage::NEW))
    {
        write(sd, to_string(sd).c_str(), to_string(sd).length());
        clientIdCounter++;
    }
    else if (request.substr(0, 3) == to_string(CommandMessage::CREATE))
    {
        Lobby lobby;
        lobby.addPlayer(sd); // Add the player to the lobby
        lobbies.push_back(lobby); // Add the created lobby to the lobbies vector
        int lobbyId = lobby.getId();
        clientToLobbyMap[sd] = lobbyId; // Update the mapping
        write(sd, to_string(lobbyId).c_str(), to_string(lobbyId).length());
    }
    else if (request.substr(0, 3) == to_string(CommandMessage::JOIN))
    {
        for (const Lobby &lobby : lobbies)
        {
            if (lobby.hasPlayer(sd))
            {
                write(sd, "You are already in a lobby", strlen("You are already in a lobby"));
                return;
            }
        }

        // Extract the lobby ID from the request
        int pos = request.find(' ');
        if (pos != string::npos)
        {
            string lobbyIdStr = request.substr(pos + 1);
            int lobbyId;
            try
            {
                lobbyId = stoi(lobbyIdStr);
            }
            catch (const exception &e)
            {
                // Invalid lobby ID format
                write(sd, "Invalid lobby ID format", strlen("Invalid lobby ID format"));
                return;
            }
            // Find the lobby with the given ID
            auto it = find_if(lobbies.begin(), lobbies.end(), [&](const Lobby &lobby)
                              { return lobby.getId() == lobbyId; });

            if (it != lobbies.end())
            {
                // Lobby found, add the client to the lobby
                it->addPlayer(sd);
                clientToLobbyMap[sd] = lobbyId; // Update the mapping
                write(sd, "Joined the lobby successfully", strlen("Joined the lobby successfully"));
            }
            else
            {
                // Lobby not found
                write(sd, "Lobby not found", strlen("Lobby not found"));
            }
        }
        else
        {
            // Invalid request format
            write(sd, "Invalid format", strlen("Invalid format"));
        }
    }
    else if (request.substr(0, 3) == std::to_string(CommandMessage::LEAVE))
    {
        for (Lobby &lobby : lobbies)
        {
            if (lobby.hasPlayer(sd))
            {
                lobby.removePlayer(sd);
                clientToLobbyMap.erase(sd); // Remove the mapping
                write(sd, "Left the lobby successfully", strlen("Left the lobby successfully"));
                return;
            }
        }

        write(sd, "You are not in any lobby", strlen("You are not in any lobby"));
    }
    else if (request.substr(0, 3) == std::to_string(CommandMessage::READY))
    {
        for (Lobby &lobby : lobbies)
        {
            if (lobby.hasPlayer(sd))
            {
                lobby.setPlayerStatus(sd, PlayerStatus::Ready);
                write(sd, "Set ready status successfully", strlen("Set ready status successfully"));
                return;
            }
        }

        write(sd, "You are not in any lobby", strlen("You are not in any lobby"));
    }
    else if (request.substr(0, 3) == std::to_string(CommandMessage::START))
    {
        for (Lobby &lobby : lobbies)
        {
            if (lobby.hasPlayer(sd))
            {
                lobby.startGame();
                // Additional handling for starting the game, if needed
                return;
            }
        }

        write(sd, "You are not in any lobby", strlen("You are not in any lobby"));
    }
    else if (request.substr(0, 3) == std::to_string(GameMessage::NIGHT_ACTION))
    {
        if (clientToLobbyMap.find(sd) != clientToLobbyMap.end()) {
            write(sd, "You perform a night action", strlen("You perform a night action"));
            string messageCode, targetIndexStr;
            istringstream iss(request.substr(0));
            iss >> messageCode >> targetIndexStr;
            int targetIndex = atoi(targetIndexStr.c_str());
            int lobbyId = clientToLobbyMap[sd]; // Get the lobby ID for the player
            for (Lobby& lobby : lobbies) {
                if (lobby.getId() == lobbyId) {
                    lobby.performNight(sd, targetIndex);
                    break;
                }
            }
        }
        else
        {
            write(sd, "Error: you are expected to be in a game", strlen("Error: you are expected to be in a game"));
        }
    }
    else if (request.substr(0, 3) == std::to_string(GameMessage::DAY_ACTION))
    {
        if (clientToLobbyMap.find(sd) != clientToLobbyMap.end()) {
            write(sd, "You perform a day action", strlen("You perform a day action"));
            string messageCode, targetIndexStr;
            istringstream iss(request.substr(0));
            iss >> messageCode >> targetIndexStr;
            int targetIndex = atoi(targetIndexStr.c_str());
            int lobbyId = clientToLobbyMap[sd]; // Get the lobby ID for the player
            for (Lobby& lobby : lobbies) {
                if (lobby.getId() == lobbyId) {
                    lobby.performDay(sd, targetIndex);
                    break;
                }
            }
        }
        else
        {
            write(sd, "Error: you are expected to be in a game", strlen("Error: you are expected to be in a game"));
        }
    }
    else if (request.substr(0, 3) == std::to_string(GameMessage::VOTE))
    {
        if (clientToLobbyMap.find(sd) != clientToLobbyMap.end()) {
            write(sd, "You voted", strlen("You voted"));
            string messageCode, targetIndexStr;
            istringstream iss(request.substr(0));
            iss >> messageCode >> targetIndexStr;
            int targetIndex = atoi(targetIndexStr.c_str());
            int lobbyId = clientToLobbyMap[sd]; // Get the lobby ID for the player
            for (Lobby& lobby : lobbies) {
                if (lobby.getId() == lobbyId) {
                    lobby.performVote(sd, targetIndex);
                    break;
                }
            }
        }
        else
        {
            write(sd, "Error: you are expected to be in a game", strlen("Error: you are expected to be in a game"));
        }
    }
    else if (request.substr(0, 3) == to_string(GameMessage::CHAT))
    {
        cout << "This is a chat request!\n";

        // Send response to all clients
        string response = getContentOfChatRequestBody(request);
        cout << "Content: " << response << endl;
        for (int i = 0; i < maxClients; i++)
        {
            if (clientSockets[i] != 0)
            {
                write(clientSockets[i], response.c_str(), response.length());
            }
        }
    }
    else
    {
        // Handle unrecognized request
        write(sd, "Unrecognized request", strlen("Unrecognized request"));
    }
}

string getContentOfChatRequestBody(const std::string &body)
{
    size_t startPos = body.find(to_string(GameMessage::CHAT));
    if (startPos != std::string::npos)
    {
        std::string content = body.substr(startPos + 4);
        return content;
    }
    return "";
}