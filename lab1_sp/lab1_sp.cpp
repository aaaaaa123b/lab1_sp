#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <gdiplus.h>


using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

static wchar_t  CLASS_NAME[] = L"DesktopApp";
static wchar_t  szTitle[] = L"My Application";



HINSTANCE hInst;

int clientWidth;
int clientHeight;
int spriteWidth;
int spriteHeight;
int x, y;
//bool started = true;

LRESULT CALLBACK WindowProc(HWND , UINT , WPARAM , LPARAM );

int CALLBACK WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow)

{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(RGB(0, 0, 0));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL,
            L"Call to RegisterClassEx failed!",
            L"Windows Desktop Guided Tour",
            NULL);

        return 1;
    }
    
    hInst = hInstance;


    HWND hwnd = CreateWindow(
        CLASS_NAME,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
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

void ResizeWindow(LPARAM lParam)
{
    clientWidth = LOWORD(lParam);
    clientHeight = HIWORD(lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        ResizeWindow(lParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
       OnPaint(hwnd, wParam, x - spriteWidth / 2, y - spriteHeight / 2, spriteWidth, spriteHeight);   
       break;
    }
    case WM_CREATE:
        InitializeSpriteSize();
        break;
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return (LRESULT)NULL;
}

