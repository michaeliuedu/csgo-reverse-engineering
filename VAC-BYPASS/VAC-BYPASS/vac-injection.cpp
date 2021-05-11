#include "includes.h"
#include "functions.h"
// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Densebear Private Cheats Loader");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void Cleanup(const std::string message) {
    std::cout << message << std::endl;
    system("pause");
    ExitProcess(0);
}

std::string RandomString(const size_t length)
{
    std::string r;
    static const char bet[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxyzZ1234567890" };
    srand((unsigned)time(NULL) * 5);
    for (int i = 0; i < length; ++i)
        r += bet[rand() % (sizeof(bet) - 1)];
    return r;
}
int CALLBACK WinMain(
    __in  HINSTANCE hInstance,
    __in  HINSTANCE hPrevInstance,
    __in  LPSTR lpCmdLine,
    __in  int nCmdShow
){
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Loader"),
            NULL);

        return 1;
    }

    hInst = hInstance;
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );


    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Loader"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        DWORD ProcessId = Functions::GetProcessId("csgo.exe");
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (ProcessId != NULL) {
            Sleep(10000);
            HANDLE Game = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
            uintptr_t ModuleBase = Functions::GetModuleBaseAddress(ProcessId, "client.dll");

            std::cout << "csgo.exe -> " << std::dec << ProcessId << std::endl;

            if (Functions::DoesFileExist("cheat.dll")) {

                if (!Functions::Internal::ExecuteBypass(Game)) {
                    MessageBox(NULL, "VAC FAILED", "Bypass Failed VAC", NULL);
                    ExitProcess(0);
                }

                if (Functions::LoadLibraryInject(ProcessId, "cheat.dll")) {

                    Functions::Internal::Backup(Game);
                    return 0;
                    ExitProcess(0);
                }
                else
                {
                    Functions::Internal::Backup(Game);
                    ExitProcess(0);
                }

            }
            else {
                ExitProcess(0);
            }
            exit(0);
        }
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Waiting For Game to Be Launched to run Automatic Injection");
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    


    return 0;
}