#include "client.h"

Client::Client()
{
    connectServer();
}

Client::Client(bool &myTurn)
{
    connectServer();
    myTurn = receiveTurn();
}

Client::~Client()
{
    // Close the client socket
    close(clientSocket);
}

void Client::connectServer()
{
    // Create a socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Use the server's IP address

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("connect");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to the server." << std::endl;

}

void Client::sendMessage()
{
    // Send a message to the server
    const char* messageToSend = "Message from a client!";
    send(clientSocket, messageToSend, strlen(messageToSend), 0);
    std::cout << "Sent message to the server: " << messageToSend << std::endl;
}

void Client::sendMessage(std::vector<std::vector<int>> field)
{

    int* flatArr = new int[field.size() * field[0].size()];
    int index = 0;
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[0].size(); ++j) {
            flatArr[index++] = field[i][j];
        }
    }


//    char* messageToSend = new char(sizeof(flatArr));
//    std::memcpy(messageToSend, flatArr, sizeof(flatArr));
    send(clientSocket, flatArr, field.size()*field[0].size()*sizeof(int), 0);
//    char** fieldArr = (char*)malloc(siz)
    delete[] flatArr;
}

std::vector<std::vector<int>> Client::receiveMessage()
{
        // Receive the linear array
        int* flatArray = new int[10 * 10];
        std::vector<std::vector<int>> res;

        // Receive the server's response
        ssize_t bytesRead = recv(clientSocket, flatArray, 10 * 10 * sizeof(int), 0);
        if (bytesRead <= 0) {
            perror("recv");
        }
        else {

            std::vector<int> row;
            for(int i = 0; i < 10; i++){
                row.clear();
                for(int j = 0; j < 10; j++){
                    row.push_back(flatArray[i * 10 + j]);
                    std::cout << "cell " << (i*10)+j <<" = " << flatArray[i * 10 + j] << std::endl;
                }
                res.push_back(row);
            }
        }

        delete[] flatArray;
        return res;
}

bool Client::receiveTurn()
{
        // Receive the integer
        int val = 0;
        bool res = false;

        // Receive the server's response
        ssize_t bytesRead = recv(clientSocket, &val, sizeof(int), 0);
        if (bytesRead <= 0) {
            perror("recv");
        }
        else {
            if (val == 1)
                res = true;
            else
                res = false;
        }

        return res;
}

//std::string Client::receiveMessage()
//{
//    char buffer[BUFFER_SIZE];
//    std::string res;

//    // Receive the server's response
//    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
//    if (bytesRead <= 0) {
//        perror("recv");
//    } else {
//        buffer[bytesRead] = '\0';
//                std::cout << "Received message from the server: " << buffer << std::endl;
//        res = std::string(buffer);
//    }

//    return res;

//}
