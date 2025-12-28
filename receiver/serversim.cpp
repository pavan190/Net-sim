// this is server code 

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

const char* reply;

void processthequery(char* buffer)
{
    std::cout<<"printing buffer in process query "<<buffer<<"\n";
    if(strcmp(buffer, "abc")==0)
    {
        reply = "i have seen this";
        return;
    }
    reply = "i didn't seen this movie";
    return;
}

int main()
{
    // 1. Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }
    std::cout << "Server socket created successfully\n";

    // Allow socket reuse (avoids "Address already in use" error)
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Bind to a particular interface (IP + Port)
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);  // Port 8080
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // Accept on all interfaces
    
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Bind failed\n";
        close(serverSocket);
        return 1;
    }
    std::cout << "Bound to port 8080\n";
    
    // 3. Listen on the binded interface
    // listen() is NOT a blocking call - it just marks socket as passive
    // accept() is the blocking call that waits for connections
    if (listen(serverSocket, 5) < 0) {  // Backlog of 5 connections
        std::cerr << "Listen failed\n";
        close(serverSocket);
        return 1;
    }
    std::cout << "Server listening on port 8080...\n";
    
    // 4. Accept incoming client connection (THIS is blocking)
    sockaddr_in clientAddress{};
    socklen_t clientAddrLen = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientAddrLen);
    if (clientSocket < 0) {
        std::cerr << "Accept failed\n";
        close(serverSocket);
        return 1;
    }
    
    // Print client info
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddress.sin_addr, clientIP, INET_ADDRSTRLEN);
    std::cout << "Client connected from " << clientIP << ":" << ntohs(clientAddress.sin_port) << "\n";
    
    // 5. Receive data from client
    char buffer[20] = {0};
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    for(int i =0;i<sizeof(buffer);i++)
    {
        //safeway
        
    }
    if (bytesReceived < 0) {
        std::cerr << "Receive failed\n";
    } else if (bytesReceived == 0) {
        std::cout << "Client disconnected\n";
    } else {
        std::cout << "Received " << bytesReceived << " bytes: " << buffer << "\n";
        processthequery(buffer);
    }
    
    // 6. Reply back to client
    const char* response = reply;
    ssize_t bytesSent = send(clientSocket, response, strlen(response), 0);
    if (bytesSent < 0) {
        std::cerr << "Send failed\n";
    } else {
        std::cout << "Sent response: " << response << "\n";
    }
    
    // 7. Close connections
    close(clientSocket);
    std::cout << "Client socket closed\n";
    
    close(serverSocket);
    std::cout << "Server socket closed\n";
    
    return 0;
}