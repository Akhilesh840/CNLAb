
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main(void) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server,
                sizeof(server)) == SOCKET_ERROR) {
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Connected to server. Type exit to quit.\n");

    while (1) {
        printf("Client: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            break;

        int len = (int)strlen(buffer);
        int sent = 0;

        while (sent < len) {
            int n = send(sock, buffer + sent, len - sent, 0);

            if (n <= 0)
                goto cleanup;

            sent += n;
        }

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        memset(buffer, 0, sizeof(buffer));

        int n = recv(sock, buffer, BUFFER_SIZE - 1, 0);

        if (n <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buffer[n] = '\0';

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server ended the chat.\n");
            break;
        }

        printf("Server: %s", buffer);
    }

cleanup:
    closesocket(sock);
    WSACleanup();

    return 0;
}


// g++ Client.cpp -o Client.exe -lws2_32
//.\Client.exe  