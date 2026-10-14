#include <iostream>
#include <string>
#include <winsock2.h>

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr = {AF_INET, htons(8080)};
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int server_len = sizeof(server_addr);

    std::string msg;
    std::cout << "Enter an UPPERCASE string to send: ";
    std::getline(std::cin, msg);

    sendto(client_socket, msg.c_str(), msg.length(), 0, (sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[1024] = {0};
    recvfrom(client_socket, buffer, 1024, 0, (sockaddr*)&server_addr, &server_len);
    
    std::cout << "Server replied with: " << buffer << "\n";

    closesocket(client_socket);
    WSACleanup();
    return 0;
} 
// g++ Client.cpp -o Client.exe -lws2_32
//.\Client.exe  