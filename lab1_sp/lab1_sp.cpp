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
int miniStep = 1;
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
    wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));

    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL,
            L"Registration Failed",
            L"Failed to register window class.",
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
            L"Creation Failed",
            L"Failed to create window.",
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
    Image temp(L"a.png");
    if (temp.GetLastStatus() != Ok) {
        MessageBox(NULL, L"Failed to load image!", L"Error", MB_ICONERROR);
   
        return;
    }
    spriteHeight = temp.GetHeight();
    spriteWidth = temp.GetWidth();
}

void EnsureSpriteInsideScreen()
{
    if (x < spriteWidth / 2)
    {
        x = spriteWidth / 2;
    }
    else if (x > clientWidth - spriteWidth / 2)
    {
        x = clientWidth - spriteWidth / 2;
    }

    if (y < spriteHeight / 2)
    {
        y = spriteHeight / 2;
    }
    else if (y > clientHeight - spriteHeight / 2)
    {
        y = clientHeight - spriteHeight / 2;
    }
}

void ResizeWindow(LPARAM lParam)
{
    clientWidth = LOWORD(lParam);
    clientHeight = HIWORD(lParam);
    EnsureSpriteInsideScreen();
}


void MouseDown(HWND hWnd, LPARAM lParam)
{
    KillTimer(hWnd, 1);
    started = true;
    paused = true;
    mousePressed = true;
    x = LOWORD(lParam);
    y = HIWORD(lParam);

    EnsureSpriteInsideScreen();

    int angle = rand() % 360;
    while ((angle >= 0 && angle <= 89) || (angle >= 91 && angle <= 179) || (angle >= 181 && angle <= 269) || (angle >= 271 && angle <= 359)) {
        angle = rand() % 360;
    }
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

        if (y <= spriteHeight / 2 + miniStep)
        {
            y = spriteHeight / 2 + miniStep;
        }

        if (y >= clientHeight - spriteHeight / 2 - miniStep)
        {
            y = clientHeight - spriteHeight / 2;
        }

        if (x <= spriteWidth / 2 - miniStep)
        {
            x = spriteWidth / 2 - miniStep;
        }

        if (x >= clientWidth - spriteWidth / 2 + miniStep)
        {
            x = clientWidth - spriteWidth / 2 + miniStep;
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
            if (y >= spriteHeight / 2 + step)
            {
                y -= step;
                InvalidateRect(hWnd, NULL, true);
            }
            else if (y >= spriteHeight / 2+ miniStep) {
                y -= miniStep;
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        }
        case VK_DOWN:
        {
            if (y <= clientHeight - spriteHeight / 2 - step)
            {
                y += step;
                InvalidateRect(hWnd, NULL, true);
            }
            else if (y <= clientHeight - spriteHeight / 2- miniStep) {
                y += miniStep;
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        }
        case VK_LEFT:
        {
            if (x >= spriteWidth / 2 + step)
            {
                x -= step;
                InvalidateRect(hWnd, NULL, true);
            }
            else if (x >= spriteWidth / 2 + miniStep) {
                x -= miniStep;
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        }
        case VK_RIGHT:
        {
            if (x <= clientWidth - spriteWidth / 2 - step)
            {
                x += step;
                InvalidateRect(hWnd, NULL, true);
            }
            else if (x <= clientWidth - spriteWidth / 2- miniStep) {
                x += miniStep;
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

    if (LOWORD(wParam) == MK_SHIFT)
    {
        if (wheelDelta > 0)
        {
            if (x <= clientWidth - spriteWidth / 2 - step)
            {
                x += step;
            }
            else if (x <= clientWidth - spriteWidth / 2 - miniStep) 
            {
                x += miniStep;
            }

        }
        else if (wheelDelta < 0)
        {
            if (x >= spriteWidth / 2 + step)
            {
                x -= step;
            }
            else if (x >= spriteWidth / 2 + miniStep)
            {
                x -= miniStep;
            }
        }
    }
    else
    {
        if (wheelDelta > 0)
        {
            if (y >= spriteHeight / 2 + step)
            {
                y -= step;
            }
            else if (y >= spriteHeight / 2 + miniStep)
            {
                y -= miniStep;
            }

        }
        else if (wheelDelta < 0)
        {       
            if (y <= clientHeight - spriteHeight / 2 - step)
            {
                y += step;
            }
            else if (y <= clientHeight - spriteHeight / 2 - miniStep) 
            {
                y += miniStep;
            }
        }
    }

    InvalidateRect(hWnd, NULL, true);
}


void Timer(HWND hWnd)
{
   
    if (x >= clientWidth - spriteWidth / 2)
    {
        dX = -abs(dX);
    }

    if (y >= clientHeight - spriteHeight / 2)
    {
        dY = -abs(dY);
    }

    if (x <= spriteWidth / 2 )
    {
        dX = abs(dX);
    }

    if (y <= spriteHeight / 2)
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

   PatBlt(memDC, 0, 0, rect.right, rect.bottom, WHITENESS);

    Graphics graphics(memDC);
    Image image(L"a.png");
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
        case WM_ERASEBKGND:
            return(1);
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
        case WM_GETMINMAXINFO:
        {
            MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
            pInfo->ptMinTrackSize.x = 130; 
            pInfo->ptMinTrackSize.y = 170; 
            break;
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        return (LRESULT)NULL;
}

