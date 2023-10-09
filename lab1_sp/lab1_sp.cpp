#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <gdiplus.h>
#include <cmath>


using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

static wchar_t  CLASS_NAME[] = L"DesktopApp";
static wchar_t  szTitle[] = L"My Application";

HINSTANCE hInst;

int clientWidth;
int clientHeight;
int spriteWidth;
int spriteHeight;
int step = 5;
int x, y;
double dX, dY;

bool started = false;
bool paused = false;
bool mousePressed = false;

LRESULT CALLBACK WindowProc(HWND , UINT , WPARAM , LPARAM );

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
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

    if (!hwnd)
    {
        MessageBox(NULL,
            L"Call to RegisterClassEx failed!",
            L"Windows Desktop Guided Tour",
            NULL);
        return 1;
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


void InitializeSpriteSize()
{
    Image temp(L"1pushd.png");
    if (temp.GetLastStatus() != Ok) {
        // Обработка ошибки загрузки изображения
        MessageBox(NULL, L"Failed to load image!", L"Error", MB_ICONERROR);
        // Можно предпринять другие действия, например, выход из приложения или установка альтернативного изображения
        return;
    }
    spriteHeight = temp.GetHeight();
    spriteWidth = temp.GetWidth();
}


void ResizeWindow(LPARAM lParam)
{
    clientWidth = LOWORD(lParam);
    clientHeight = HIWORD(lParam);
}


void MouseDown(HWND hWnd, LPARAM lParam)
{
    KillTimer(hWnd, 1);
    started = true;
    paused = true;
    mousePressed = true;

    int angle = rand() % 360;

    x = LOWORD(lParam);
    y = HIWORD(lParam);

    dX = 5 * cos(angle);
    dY = 5 * sin(angle);

    InvalidateRect(hWnd, NULL, TRUE);
}

void MouseMove(HWND hWnd, LPARAM lParam)
{
    if (mousePressed)
    {
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        if (y < spriteHeight / 2 + step)
        {
            y = spriteHeight / 2 + step;
        }

        if (y > clientHeight - spriteHeight / 2 - step)
        {
            y = clientHeight - spriteHeight / 2;
        }

        if (x < spriteWidth / 2 - step)
        {
            x = spriteWidth / 2 - step;
        }

        if (x > clientWidth - spriteWidth / 2 + step)
        {
            x = clientWidth - spriteWidth / 2 + step;
        }

        InvalidateRect(hWnd, NULL, true);
    }
}


void KeyDown(HWND hWnd, WPARAM wParam)
{
    if (wParam == VK_SPACE)
    {
        if (paused==true)
        {
            SetTimer(hWnd, 1, 42, NULL);
            paused = false;
        }
        else
        {
            KillTimer(hWnd, 1);
            paused = true;
        }
    }

    if (paused==true)
    {
        switch (wParam)
        {
        case VK_UP:
        {
            if (y > spriteHeight / 2 + step)
            {
                y -= step;
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        }
        case VK_DOWN:
        {
            if (y < clientHeight - spriteHeight / 2 - step)
            {
                y += step;
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        }
        case VK_LEFT:
        {
            if (x > spriteWidth / 2 - step)
            {
                x -= step;
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        }
        case VK_RIGHT:
        {
            if (x < clientWidth - spriteWidth / 2 + step)
            {
                x += step;
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        }
        break;
        }
    }
}

void MouseWheel(HWND hWnd, WPARAM wParam)
{
    int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    if (LOWORD(wParam) == MK_CONTROL)
    {
        for (; wheelDelta > WHEEL_DELTA; wheelDelta -= WHEEL_DELTA)
        {
            if (x < clientWidth - spriteWidth / 2)
            {
                x += step;
                InvalidateRect(hWnd, NULL, true);
            }
        }
        for (; wheelDelta < 0; wheelDelta += WHEEL_DELTA)
        {
            if (x > step)
            {
                x -= step;
                InvalidateRect(hWnd, NULL, true);
            }
        }
    }
    else
    {
        for (; wheelDelta > WHEEL_DELTA; wheelDelta -= WHEEL_DELTA)
        {
            if (y < clientHeight - spriteHeight / 2 - step)
            {
                y += step;
                InvalidateRect(hWnd, NULL, true);
            }
        }
        for (; wheelDelta < 0; wheelDelta += WHEEL_DELTA)
        {
            if (y > spriteHeight / 2 + step)
            {
                y -= step;
                InvalidateRect(hWnd, NULL, true);
            }
        }
    }
}

void Timer(HWND hWnd)
{
    if (x > clientWidth - spriteWidth / 2 + step)
    {
        dX = -abs(dX);
    }

    if (y > clientHeight - spriteHeight / 2 + step)
    {
        dY = -abs(dY);
    }

    if (x < spriteWidth / 2 - step)
    {
        dX = abs(dX);
    }

    if (y < spriteHeight / 2 + step)
    {
        dY = abs(dY);
    }

    x += (int)dX;
    y += (int)dY;

    InvalidateRect(hWnd, NULL, true);
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

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_SIZE:
            ResizeWindow(lParam);
            break;
        case WM_LBUTTONDOWN:
            MouseDown(hWnd, lParam);
            break;
        case WM_MOUSEMOVE:
            MouseMove(hWnd, lParam);
            break;
        case WM_LBUTTONUP:
            mousePressed = false;
            break;
        case WM_KEYDOWN:
            KeyDown(hWnd, wParam);
            break;
        case WM_MOUSEWHEEL:
            MouseWheel(hWnd, wParam);
            break;
        case WM_TIMER:
            Timer(hWnd);
            break;
        case WM_PAINT:
            if (started)
            {
                OnPaint(hWnd, wParam, x - spriteWidth / 2, y - spriteHeight / 2, spriteWidth, spriteHeight);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_CREATE:
            InitializeSpriteSize();
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        return (LRESULT)NULL;
}

