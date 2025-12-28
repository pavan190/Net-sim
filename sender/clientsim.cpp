// this is client 

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "pktstruct.cpp"

int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

void sendrequest()
{
    pktstruct pkt;
    pkt.header=5;
    pkt.length=15;
    pkt.message=new char[15];
    //first 5 bytes are header, next 10bytes are message
    std::memcpy(pkt.message, "HEAD", 4);                      // Copy "HEAD" (4 chars) to start
    std::memcpy(pkt.message + pkt.header, "body", 4);         // Copy "body" (4 chars) starting at offset 5
    
    // Serialize the entire packet: header + length + message
    size_t total_size = sizeof(pkt.header) + sizeof(pkt.length) + pkt.length;
    char* packet_buffer = new char[total_size];
    
    // Pack the packet  : [header][length][message]
    std::memcpy(packet_buffer, &pkt.header, sizeof(pkt.header));
    std::memcpy(packet_buffer + sizeof(pkt.header), &pkt.length, sizeof(pkt.length));
    std::memcpy(packet_buffer + sizeof(pkt.header) + sizeof(pkt.length), pkt.message, pkt.length);
    
    ssize_t bytesSent = send(clientSocket, packet_buffer, total_size, 0);
    if (bytesSent < 0) {
        std::cerr << "Failed to send data\n";
    } else {
        std::cout << "Sent " << bytesSent << " bytes (header=" << pkt.header 
                  << ", length=" << pkt.length << ", message=" << pkt.message << ")\n";
    }
    
    delete[] packet_buffer;  // Free serialized buffer
    delete[] pkt.message;    // Free message buffer
}

void receiveresponse()
{
    std::cout<<"in receive response client\n";
    char buffer[100];
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived > 0) {
        std::cout << "Received from server: " << buffer << "\n";
    }
    else
    {
        std::cout<<" received bytes less than 0\n";
    }
}

int main()
{
    // 1. Create socket
    if (clientSocket < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }
    std::cout << "Socket created successfully\n";
    
    // 2. Define server address (presumably you already know the server address)
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);  // Server port
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP (localhost)
    
    // 3. Connect to server
    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Connection failed\n";
        close(clientSocket);
        return 1;
    }
    std::cout << "Connected to server successfully\n";
    // 4. Send data to the server
    sendrequest();
    
    // Optional: Receive response from server
    receiveresponse();
    
    // 5. Close the socket
    close(clientSocket);
    std::cout << "Socket closed\n";
    
    return 0;
}