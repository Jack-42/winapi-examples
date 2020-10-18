/**
* This application creates a window which shows a text and plays a sound.
*/

#include <windows.h>

#define APP_NAME TEXT("HelloWinAPI")

BOOL createWindowClass(WNDCLASS* wndClass, HINSTANCE hInstance);
HWND createWindow(WNDCLASS* wndClass, int width, int height, HINSTANCE hInstance);
int runMessageLoop();

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void drawText(HWND hWnd, LPCWSTR text);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow
)
{
    WNDCLASS wndClass;
    if (!createWindowClass(&wndClass, hInstance)) {
        MessageBox(NULL, TEXT("Failed to create window class!"), APP_NAME, MB_ICONERROR);
        return 1;
    }

    HWND hWnd = createWindow(&wndClass, 800, 600, hInstance);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd); // sends a WM_PAINT message

    return runMessageLoop();
}

BOOL createWindowClass(WNDCLASS* wndClass, HINSTANCE hInstance)
{
    ZeroMemory(wndClass, sizeof(WNDCLASS));

    wndClass->style = CS_HREDRAW | CS_VREDRAW; // sends WM_PAINT message for each window resize or movement
    wndClass->lpfnWndProc = wndProc;
    wndClass->cbClsExtra = 0;
    wndClass->cbWndExtra = 0;
    wndClass->hInstance = hInstance;
    wndClass->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass->hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass->lpszMenuName = NULL;
    wndClass->lpszClassName = TEXT("main");

    if (!RegisterClass(wndClass)) {
        return FALSE;
    }

    return TRUE;
}

HWND createWindow(WNDCLASS* wndClass, int width, int height, HINSTANCE hInstance)
{
    return CreateWindow(
        wndClass->lpszClassName, APP_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, // default position
        width, height,
        NULL, NULL, hInstance, NULL
    );
}

int runMessageLoop()
{
    MSG msg;
    // loop continues until WM_QUIT message is received
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    // for WM_QUIT message, wParam contains the exit code
    return (int)msg.wParam;
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_CREATE: {
            // play a system sound asynchrounously
            PlaySound(TEXT("SystemAsterisk"), NULL, SND_ALIAS | SND_ASYNC);
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0); // sends a WM_QUIT message with specified exit code
            return 0;
        }
        case WM_PAINT: {
            drawText(hWnd, TEXT("Hello, WinAPI!"));
            return 0;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void drawText(HWND hWnd, LPCWSTR text)
{
    PAINTSTRUCT paint;
    RECT rect;

    // draw text in center of window
    GetClientRect(hWnd, &rect);
    HDC hdc = BeginPaint(hWnd, &paint);
    DrawText(
        hdc,
        text, -1, // -1 for length because string is null-terminated
        &rect,
        DT_SINGLELINE | DT_CENTER | DT_VCENTER
    );
    EndPaint(hWnd, &paint);
}