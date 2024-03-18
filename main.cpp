/* Made by tor. Posted @ https://forum.stargaze.cloud/ */

#include <windows.h>
#include <tchar.h>

#define IDT_TIMER 1
#define TIMER_INTERVAL 5000

void show_notification(HWND hwnd, const TCHAR* title, const TCHAR* message) {
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = 0;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    _tcscpy_s(nid.szTip, _countof(nid.szTip), title);

    Shell_NotifyIcon(NIM_ADD, &nid);

    nid.uFlags |= NIF_INFO;
    _tcscpy_s(nid.szInfo, _countof(nid.szInfo), message);
    _tcscpy_s(nid.szInfoTitle, _countof(nid.szInfoTitle), title);
    nid.dwInfoFlags = NIIF_INFO;
    nid.uTimeout = 10000;

    Shell_NotifyIcon(NIM_MODIFY, &nid);

    SetTimer(hwnd, IDT_TIMER, TIMER_INTERVAL, NULL);
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static NOTIFYICONDATA nid;
    switch (msg) {
    case WM_CREATE:
        show_notification(hwnd, _T("Title Of Notif"), _T("Footer Of Notif"));
        break;
    case WM_TIMER:
        KillTimer(hwnd, IDT_TIMER);
        Shell_NotifyIcon(NIM_DELETE, &nid);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    const TCHAR szClassName[] = _T("NotificationWindowClass");
    WNDCLASS wndClass = { 0 };
    wndClass.lpfnWndProc = wnd_proc;
    wndClass.hInstance = hInstance;
    wndClass.lpszClassName = szClassName;

    if (!RegisterClass(&wndClass)) {
        MessageBox(NULL, _T("Window Registration Failed!"), _T("Error"), MB_ICONERROR | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindow(szClassName, _T("Notification Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, _T("Window Creation Failed!"), _T("Error"), MB_ICONERROR | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
