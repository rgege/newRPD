#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#include "server.h"

extern SOCKET listenSock;

void *get_in_addr(struct sockaddr *sa)
{
    return &(((struct sockaddr_in*)sa)->sin_addr);
}

int startServer(HWND hwnd, int id) {

    WSADATA wsa;
    int status;

    /* WinSock initialization */
    if ((status = WSAStartup(MAKEWORD(2, 2), &wsa)) != 0)
    {
        printf("WSAStartup failed with error: %d\n", status);
        return 1;
    }

    struct addrinfo hints, *res = NULL;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    /*resolving server address and port*/
    if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        printf("getaddrinfo failed with error: %d\n", status);
        WSACleanup();
        return 1;
    }

    /*creating socket for server to listen for connections*/
    if ((listenSock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }

    /*setup TCP listening socket*/
    if ((status = bind(listenSock, res->ai_addr, res->ai_addrlen)) == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(res);
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(res);

    /*starting listening for incoming connections*/
    if ((status = listen(listenSock, SOMAXCONN)) == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    if ((status = WSAAsyncSelect(listenSock, hwnd, id, FD_ACCEPT|FD_CLOSE|FD_READ|FD_WRITE)) == SOCKET_ERROR) {
        printf("WSAAsyncSelect failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    return 0;
}

int acceptConnection(HWND hwnd, int id, char *szDest)
{
    struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof their_addr;

    /*accepting client socket*/
    SOCKET clientSock = INVALID_SOCKET;
    if ((clientSock = accept(listenSock, (struct sockaddr*)&their_addr, &sin_size)) == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    /*identifying connected client address*/
    char szBuff[INET_ADDRSTRLEN];
    inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr), szBuff, sizeof szBuff);
        memcpy(szDest, szBuff, sizeof(szBuff));

    closesocket(listenSock);
    return 0;
}
    /*
    char recvbuf[MAXDATASIZE];
    int bufsize;
    int recvbuflen = MAXDATASIZE;
    int byte_count = 0;

    do {

        if ((byte_count = recv(clientSock, recvbuf, recvbuflen, 0)) > 0 ) {
            printf("server: bytes received: %d\n", byte_count);
        }
        else if (byte_count == 0)
            printf("server: connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(clientSock);
            WSACleanup();
            return 1;
        }


    } while (byte_count > 0);

    /*shutting down the connection
    if ((status = shutdown(clientSock, SD_SEND)) == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(clientSock);
        WSACleanup();
        return 1;
    }

    printf("server: shutting down...\n");

    closesocket(clientSock);
    WSACleanup(); */
