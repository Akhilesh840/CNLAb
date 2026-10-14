
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8081
#define BUFFER_SIZE 1024

int main(void) {
    WSADATA wsa;
    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // 1. Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed: %d\n",
               WSAGetLastError());
        return 1;
    }

    // 2. Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_fd == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n",
               WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // 3. Configure server address
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    // 4. Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // 5. Listen for clients
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);
    printf("Waiting for client connection...\n");

    // 6. Accept one client
    client_socket = accept(server_fd,
                           (struct sockaddr *)&address,
                           &addrlen);

    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Client connected!\n");

    // 7. Chat loop
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        int valread = recv(client_socket, buffer,
                           BUFFER_SIZE - 1, 0);

        if (valread <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[valread] = '\0';

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        printf("Client: %s", buffer);

        printf("Server: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        int len = (int)strlen(buffer);
        int sent = 0;

        while (sent < len) {
            int n = send(client_socket, buffer + sent,
                         len - sent, 0);

            if (n == SOCKET_ERROR || n == 0) {
                printf("Send failed: %d\n",
                       WSAGetLastError());
                goto cleanup;
            }

            sent += n;
        }

        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }
    }

cleanup:
    // 8. Close sockets and clean up Winsock
    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}

//g++ Server.cpp -o server.exe -lws2_32
//.\server.exe                         