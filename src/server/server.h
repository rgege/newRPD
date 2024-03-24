#define ID_WINSOCK    9090
#define IDC_STATUS    1001
#define ID_FILE_START 6001
#define ID_FILE_EXIT  6009
#define PORT         "3333"
#define MAXDATASIZE   1024
#define SB_WIDTH       200

int startServer(HWND, int);
int acceptConnection(HWND, int, char*);
int shutdownServer(HWND, int);
int recieveMsg (HWND, int, char*);