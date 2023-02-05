#include <Windows.h>
#include <vector>

// Utility function to get the resolution of all connected monitors
std::vector<RECT> getMonitorResolutions() {
    std::vector<RECT> resolutions;
    EnumDisplayMonitors(NULL, NULL, [](HMONITOR hMonitor, HDC hdc, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
        std::vector<RECT>* pResolutions = (std::vector<RECT>*)dwData;
        pResolutions->push_back(*lprcMonitor);
        return TRUE;
    }, (LPARAM)&resolutions);
    return resolutions;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    POINT p;
    switch (uMsg) {
        case WM_LBUTTONDOWN: 
            GetCursorPos(&p);
            break;
        case WM_LBUTTONUP: 
            GetCursorPos(&p);
            break;
        case WM_CLOSE: 
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wndClass = { 0 };
    wndClass.lpfnWndProc = WindowProc;
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszClassName = L"SemiTransparentWindowClass";
    wndClass.hCursor = LoadCursor(NULL, IDC_CROSS);
    RegisterClass(&wndClass);

    // Get the resolution of all connected monitors
    std::vector<RECT> monitorResolutions = getMonitorResolutions();
    for (const RECT& monitorRes : monitorResolutions) {
        // Create a window for each monitor
        HWND hWnd = CreateWindow(
            wndClass.lpszClassName,
            L"Semi-Transparent Window",
            WS_POPUP | WS_VISIBLE,
            monitorRes.left,
            monitorRes.top,
            monitorRes.right - monitorRes.left,
            monitorRes.bottom - monitorRes.top,
            NULL,
            NULL,
            wndClass.hInstance,
            NULL
        );
        RegisterHotKey(hWnd, 1, MOD_ALT, VK_TAB);
        // Set the window as top-most
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        // Set the window's transparency
        SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
        SetLayeredWindowAttributes(hWnd, RGB(22, 22, 23), 152, LWA_COLORKEY | LWA_ALPHA);
        // Show the window
        ShowWindow(hWnd, SW_SHOW);
    }

    POINT p;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        Sleep(1);
        if (msg.message == WM_LBUTTONDOWN) {
            GetCursorPos(&p);
            break;
        }
        if (msg.message == WM_KEYDOWN && (msg.wParam == VK_LWIN || msg.wParam == VK_TAB || msg.wParam == VK_CONTROL || msg.wParam == VK_ESCAPE)) {
            break;
        }
        if (msg.message == WM_SYSKEYDOWN && msg.wParam == VK_MENU) {
            break;
        }
    }
    return 0;
}
