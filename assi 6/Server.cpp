#include <iostream>
#include <string>
#include <cctype> 
#include <winsock2.h>

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr = {AF_INET, htons(8080)};
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));

    std::cout << "UDP Server waiting for messages...\n";

    char buffer[1024];
    sockaddr_in client_addr;
    int client_len = sizeof(client_addr);

    while (true) {
        memset(buffer, 0, sizeof(buffer)); 
        
        recvfrom(server_socket, buffer, 1024, 0, (sockaddr*)&client_addr, &client_len);
        std::string msg(buffer);
        std::cout << "Received from client: " << msg << "\n";

        for (char &c : msg) {
            c = std::tolower(c);
        }

        sendto(server_socket, msg.c_str(), msg.length(), 0, (sockaddr*)&client_addr, client_len);
        std::cout << "Converted and sent back: " << msg << "\n\n";
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}

//g++ Server.cpp -o server.exe -lws2_32
//.\server.exe  
