#include <windows.h>
#include <commctrl.h>

#define IDC_STATUS 1001
#define ID_TIMER   5001

#define MAX_POS 10

const char g_szClassName[] = "windowClass";

POINT g_curPos;

char *ptos(POINT *p, char b[]) {
    wsprintf(b, TEXT("%d::%d\n"), p->x, p->y, MAX_POS);
    return b;
}

/*window procedure*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
        {
            int ret;
            if ((ret = SetTimer(hwnd, ID_TIMER, 30, NULL) == 0))
               MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
        }
        break;
        case WM_TIMER:
        {
            GetCursorPos(&g_curPos);  
            char szBuff[MAX_POS];
            HWND hstat = GetDlgItem(hwnd, IDC_STATUS);
            SendMessage(hstat, SB_SETTEXT, 0, (LPARAM)ptos(&g_curPos, szBuff));
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd, hstatus;
    MSG Msg;

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
        "newRPD",
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

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /*message loop*/ 
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}