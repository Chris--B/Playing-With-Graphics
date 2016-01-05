#include "CommonDefs.hpp"

#define UNICODE
#include <windows.h>

#include <future>

HWND hMainWnd = 0;


bool initWindowsApp(HINSTANCE instance, int show);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void message(LPCWSTR msg, LPCWSTR caption = 0) {
    MessageBox(0, msg, caption, MB_OK);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR      pCmdLine,
                   int       nShowCmd) {
    if (!initWindowsApp(hInstance, nShowCmd)) {
        return 0;
    }

    MSG msg = { 0 };
    do {
        if (!PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            continue;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    } while (msg.message != WM_QUIT);

    return as<int>(msg.wParam);
}

bool initWindowsApp(HINSTANCE instance, int show) {
    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = instance;
    wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  = 0;
    wc.lpszClassName = L"BasicWndClass";

    if (!RegisterClass(&wc)) {
        message(L"Register class FAILED.");
        return false;
    }

    hMainWnd = CreateWindow(L"BasicWndClass",
                            L"Playing with DirectX",
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            0,
                            0,
                            instance,
                            0);
    if (hMainWnd == 0) {
        message(L"CreateWindow FAILED");
        return false;
    }

    ShowWindow(hMainWnd, show);
    UpdateWindow(hMainWnd);

    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_LBUTTONDOWN:
        message(L"Hello World!", L"Hello!");
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hMainWnd);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}