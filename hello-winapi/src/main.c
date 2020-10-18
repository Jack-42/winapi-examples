/**
* This application creates a window which shows a text and plays a sound.
*/

#include <windows.h>

#define CLASS_NAME TEXT("main")
#define APP_NAME TEXT("HelloWinAPI")
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow
)
{
	// create window class
	WNDCLASS wndClass = {0};
	wndClass.style = CS_HREDRAW | CS_VREDRAW; // sends WM_PAINT message for each window resize or movement
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = CLASS_NAME;

	if (!RegisterClass(&wndClass)) {
		MessageBox(NULL, TEXT("Failed to register window class!"), APP_NAME, MB_ICONERROR);
		return 1;
	}

	// create window
	HWND hWnd = CreateWindow(
		CLASS_NAME, APP_NAME, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, // default position
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, hInstance, NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd); // sends a WM_PAINT message

	// message loop
	MSG msg;
	// loop continues until WM_QUIT message is received, then the return code is 0
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// msg.wParam contains the exit code
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			PAINTSTRUCT paint;
			RECT rect;

			// draw text in center of window
			GetClientRect(hWnd, &rect);
			HDC hdc = BeginPaint(hWnd, &paint);
			DrawText(
				hdc,
				TEXT("Hello, WinAPI!"), -1, // -1 for length, i.e. that the string is null-terminated
				&rect,
				DT_SINGLELINE | DT_CENTER | DT_VCENTER
			);
			EndPaint(hWnd, &paint);

			return 0;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
