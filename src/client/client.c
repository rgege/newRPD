#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include "client.h"

extern SOCKET connectSock;

int startClient(HWND hwnd, int id)
{
    WSADATA wsa;
    int status;

    /*initializing Winsock*/
    if ((status = WSAStartup(MAKEWORD(2,2), &wsa)) != 0) {
        printf("WSAStartup failed with error: %d\n", status);
        return 1;
    }

    struct addrinfo hints, *res = NULL;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /*resolving server address and port*/
    if ((status = getaddrinfo("localhost", PORT, &hints, &res)) != 0) {
        printf("getaddr failed with error: %d\n", status);
        WSACleanup();
        return 1;
    }

    struct addrinfo *ptr = NULL;
    /*attempting to connect until succsess */
    for (ptr = res; ptr != NULL; ptr=ptr->ai_next) {

        /*creating socket for connecting to server*/
        if((connectSock = socket(ptr->ai_family, ptr->ai_socktype, 
                                 ptr->ai_protocol)) == INVALID_SOCKET) {
            printf ("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
        }

        /*making the socket asynchronous*/ 
        if ((status = WSAAsyncSelect(connectSock, hwnd, id, FD_WRITE|FD_CLOSE|FD_CONNECT)) == SOCKET_ERROR) {
            printf("WSAAsyncSelect failed with error: %d\n", WSAGetLastError());
            closesocket(connectSock);
            WSACleanup();
            return 1;
        }

        /*connecting to server*/
        if ((status = connect(connectSock, ptr->ai_addr, ptr->ai_addrlen)) == SOCKET_ERROR) {
                printf("connect failed with error: %d\n", WSAGetLastError());
                closesocket(connectSock);
                connectSock = INVALID_SOCKET;
                continue;
        }
    }

    freeaddrinfo(res);

    if (connectSock == INVALID_SOCKET) {
        printf("unable to connect to server!\n");
        WSACleanup();
    }

    printf("client: connected to server...\n");
    return 0;
}

int sendData(HWND hwnd)
{

    /*sending data to server*/
    POINT pt;
    GetCursorPos(&pt);
    char sendBuff[MAXDATASIZE];
    sprintf(sendBuff, "%d::%d\n", pt.x, pt.y);
    int status;
    if ((status = send(connectSock, sendBuff, (int)sizeof sendBuff, 0)) == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(connectSock);
        WSACleanup();
        return 1;
    }
        
    printf("client: sending data...\n");
    return 0;
}

int shutdownClient(HWND hwnd)
{
    int status;
    if ((status = shutdown(connectSock, SD_SEND)) == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(connectSock);
        WSACleanup();
        return 1;
    }

    printf("client: data sent, shuting down...\n");

    closesocket(connectSock);
    WSACleanup();

    return 0;
}
