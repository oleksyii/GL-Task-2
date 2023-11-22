#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <vector>
#include <mutex>
#include <chrono>
#include <cstring>
#include <cstdlib>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

constexpr int PORT = 12346;
constexpr int BUFFER_SIZE = 1024;
int countClients = 0;
std::vector<std::vector<int>> *playersFields = new std::vector<std::vector<int>>();
// std::vector<int*> *playersFieldsPlain = new std::vector<int*>();
int** playersFieldsPlain = new int*[2];

std::mutex consoleMutex;
std::mutex countClientsMutex;

// int mainArr[10000];

// Function to handle communication with a client
void handleClient(int clientSocket) {

    int hits[100];
    int currentClient = countClients;
    int opponent = (currentClient == 1 ? 0 : 1);
    std::cout<< "opponent is " << opponent << std::endl;
    std::cout<< "countClients is " << countClients << std::endl;
    if(countClients == 0){
        int i = 1;
        send(clientSocket, &i, sizeof(int), 0);
    } else {
        int i = 0;
        std::cout<< "sending order " << std::endl;
        send(clientSocket, &i, sizeof(int), 0);
        std::cout<< "order sent " << std::endl;
    }
    {
        std::lock_guard<std::mutex> lock(countClientsMutex);
        countClients++;
        std::cout<< "clearing the field " << std::endl;
        playersFields[currentClient].clear();       
    }

    // Send 1 or 0 if client is going first or second

    // Receive and process messages from the client
    while (true) {
        // Receive the linear array
        int* flatArray = new int[10 * 10];
        std::cout<< "starting to recv " << std::endl;
        ssize_t bytesRead = recv(clientSocket, flatArray, 10 * 10 * sizeof(int), 0);
        // ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            // Client disconnected or error occurred
            break;
        }

        if(playersFields[currentClient].empty()){
            std::vector<int> row;
            for(int i = 0; i < 10; i++){
                row.clear();
                for(int j = 0; j < 10; j++){
                    row.push_back(flatArray[i * 10 + j]);
                    std::cout << "cell " << (i*10)+j <<" = " << flatArray[i * 10 + j] << std::endl;
                }
                { 
                    std::lock_guard<std::mutex> lock(countClientsMutex);
                    playersFields[currentClient].push_back(row);
                }
            }
            int* arr1 = new int[10*10];
            playersFieldsPlain[currentClient] = arr1;
            // delete[] arr1;

            continue;
        } else {
            for(int i = 0; i < 10*10; i++){
                if((flatArray[i] == playersFieldsPlain[opponent][i] == 1)){
                    hits[i] = 1;
                    
                } else{
                    flatArray[i] = 0;
                }
            }
        }
        // // Process the received message
        // {
        //     std::lock_guard<std::mutex> lock(mainArrMutex);
        //     std::memcpy(mainArr, buffer, sizeof(buffer));
            
        // }

        // buffer[bytesRead] = '\0';
        // message = buffer;



        {
            std::lock_guard<std::mutex> lock(consoleMutex);
            for(int i = 0; i < 100; i++){
                std::cout << "cell " << i+1 << " " << flatArray[i] << std::endl; 
            }
            // std::cout << "Received from client " << clientSocket << ": " << message << std::endl;
        }

        // Echo the message back to the client
        send(clientSocket, flatArray, 10 * 10 * sizeof(int), 0);
        delete[] flatArray;
    }

    // Close the client socket
    close(clientSocket);

    {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "Client " << clientSocket << " disconnected." << std::endl;
    }
        {
        std::lock_guard<std::mutex> lock(countClientsMutex);
        countClients--;
        
    }
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == -1) {
        perror("listen");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Main server loop
    while (true) {
        // Accept a client connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("accept");
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(consoleMutex);
            std::cout << "Client connected from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
        }

        // Start a new thread to handle communication with the client
        std::thread(handleClient, clientSocket).detach();
    }

    // Close the server socket (this part of the code is unreachable in this example)
    close(serverSocket);
    delete playersFields;
    for (int i = 0; i < 2; i++){
        delete[] playersFieldsPlain[i];
    }
    delete[] playersFieldsPlain;
    return 0;
}
