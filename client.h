#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>

constexpr int PORT = 12346;
constexpr int BUFFER_SIZE = 1024;

class Client
{
public:
    Client();
    Client(bool &myTurn);

    ~Client();

    void connectServer();

    void sendMessage();
    void sendMessage(std::vector<std::vector<int>>);

    bool receiveTurn();
    std::vector<std::vector<int>> receiveMessage();

private:
    int clientSocket;
    struct sockaddr_in serverAddr;
};

#endif // CLIENT_H
