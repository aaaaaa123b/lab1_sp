#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <gdiplus.h>


using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

int spriteWidth;
int spriteHeight;
int x, y;
bool started = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              
        CLASS_NAME,                     
        L"Learn to Program Windows",    
        WS_OVERLAPPEDWINDOW,            
        CW_USEDEFAULT, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
           
                OnPaint(hwnd, wParam, x - spriteWidth / 2, y - spriteHeight / 2, spriteWidth, spriteHeight);
            
    }
    return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OnPaint(HWND hWnd, WPARAM wParam, int x, int y, int w, int h)
{
    RECT rect;
    if (!GetClientRect(hWnd, &rect))
    {
        GetLastError();
    }
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBmp = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    HBITMAP mOldBmp = (HBITMAP)SelectObject(memDC, hBmp);

    Graphics graphics(memDC);
    Image image(L"1pushd.png");
    Rect destRect(x, y, w, h);
    graphics.DrawImage(&image, destRect);

    BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
    SelectObject(memDC, mOldBmp);
    DeleteObject(hBmp);
    DeleteDC(memDC);
    EndPaint(hWnd, &ps);
}


void InitializeSpriteSize()
{
    Image temp(L"1pushd.png");
    spriteHeight = temp.GetHeight();
    spriteWidth = temp.GetWidth();
}
