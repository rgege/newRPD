#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#include "server.h"

SOCKET listenSock = INVALID_SOCKET;
SOCKET clientSock = INVALID_SOCKET;

const char g_szClassName[] = "windowClass";

/*window procedure*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    
    switch(msg)
    {
        case ID_WINSOCK:
            switch(LOWORD(lParam))
            {
                case FD_READ:
                {
                    char recvB[MAXDATASIZE];
                    memset(&recvB, 0, sizeof recvB);
                    recieveMsg(hwnd, ID_WINSOCK, recvB);
                    HWND hstat = GetDlgItem(hwnd, IDC_STATUS);
                    SendMessage(hstat, SB_SETTEXT, 1, (LPARAM)recvB);
                }
                break;
                case FD_ACCEPT:
                {
                    char szDest[16]; // INET_ADDRSTRLEN
                    char szSBMsg[SB_WIDTH] = "got connection from: ";
                    acceptConnection(hwnd, ID_WINSOCK, szDest);
                    strcat(szSBMsg, szDest);
                    HWND hstat = GetDlgItem(hwnd, IDC_STATUS);
                    SendMessage(hstat, SB_SETTEXT, 0, (LPARAM)szSBMsg);
                }
                break;
                case FD_CLOSE:
                {
                    shutdownServer(hwnd, ID_WINSOCK);
                    HWND hstat = GetDlgItem(hwnd, IDC_STATUS);
                    SendMessage(hstat, SB_SETTEXT, 0, (LPARAM)"connection closed, shutting down");
                }
                break;
            }
        break;
        case WM_CREATE:
        {
            HMENU hMenu, hSubMenu;

            hMenu = CreateMenu();
            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING | MF_POPUP, ID_FILE_START, "Start");
            AppendMenu(hSubMenu, MF_STRING | MF_POPUP, ID_FILE_EXIT, "Exit");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "File");
            SetMenu(hwnd, hMenu);
        }
        break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ID_FILE_START:
                {
                    HWND hstat = GetDlgItem(hwnd, IDC_STATUS);
                    SendMessage(hstat, SB_SETTEXT, 0, (LPARAM)"waiting for connection ...");
                    startServer(hwnd, ID_WINSOCK);
                }
                break;
                case ID_FILE_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
            }
        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main()
{
    HINSTANCE hInstance = GetModuleHandle(0);
    WNDCLASSEX wc;
    HWND hwnd, hstatus;
    MSG Msg;

    //FreeConsole();

    /*registering windows class*/
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    /*creating window hierarchy*/
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Server",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 340, 220,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hstatus = CreateWindowEx(0, STATUSCLASSNAME, NULL,
        WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
        hwnd, (HMENU)IDC_STATUS, GetModuleHandle(NULL), NULL);

    int statwidths[] = {SB_WIDTH, -1};
    SendMessage(hstatus, SB_SETPARTS, sizeof(statwidths)/sizeof(int), (LPARAM)statwidths);

    ShowWindow(hwnd, 1);
    UpdateWindow(hwnd);

    /*message loop*/ 
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
