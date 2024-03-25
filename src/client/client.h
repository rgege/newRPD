#define ID_WINSOCK    9090
#define IDC_STATUS    1001
#define ID_FILE_CONN  6001
#define ID_FILE_SEND  6002
#define ID_FILE_STOP  6003
#define ID_FILE_EXIT  6009
#define PORT         "3333"
#define MAXDATASIZE   1024
#define SB_WIDTH       200

int startClient(HWND, int);
int sendData(HWND);
int shutdownClient(HWND);
