#include <stdio.h>
#include <Windows.h>
#include <math.h>
using namespace std;
void drawCircleBres2(HDC hdc, int xc, int yc, int R, COLORREF color);

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    int x, y;
    switch (m)
    {
    case WM_LBUTTONDOWN:
        hdc = GetDC(hwnd);
        x = LOWORD(lp);
        y = HIWORD(lp);
        drawCircleBres2(hdc, x, y, 100, RGB(255, 0, 0));
        ReleaseDC(hwnd, hdc);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default: return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}
int Round(double x) {
    return (int)(x + 0.5);
}
void draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color) {
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc - x, yc - y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc - y, yc - x, color);
    SetPixel(hdc, xc + y, yc - x, color);
}



void drawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1, dy = y2 - y1;

    if (abs(dy) <= abs(dx)){
        if (dx == 0) {
            SetPixel(hdc, x1, y1, color);
            return;
        }
        if(x2 < x1) {
            // swap(x1, x2, y1, y2)
        }
        int x = x1;
        double y = y1; // m is accumulated so we want a double
        SetPixel(hdc, x1, y1, color);
        double m = dy / (double)dx;

        while (x < x2) {
            x++;
            y += m;
            SetPixel(hdc, x, round(y), color);
        }
    } else {
        if (y2 < y1) {
            //swap(x1, x2, y1, y2);
        }
        int y = y1;
        double x = x1;
        SetPixel(hdc, x1, y1, color);
        double mi = dx / (double )dy; // m inverse

        while (y < y2) {
            y++;
            x += mi;
            SetPixel(hdc, round(x), y, color);
        }
    }
}


double computeYIntercept(double m, int x, int y) {
    return y - m * x;;
}
int computeY(double m, int x, double b) {
    return m * x + b;
}

void drawTangentLine(HDC hdc, int x, int y, int xc, int yc, COLORREF color) {
    double mi = -(x / (double)y);
    int lineStartXPoint = x - 300;
    int lineEndXPoint = x + 300;
    double b = computeYIntercept(mi, x, y);
    int lineStartYPoint = computeY(mi, lineStartXPoint, b);
    int lineEndYPoint = computeY(mi, lineEndXPoint, b);
    drawLineDDA(hdc, lineStartXPoint + xc, lineStartYPoint + yc,
               lineEndXPoint + xc, lineEndYPoint + yc, color);

}
void drawCircleBres2(HDC hdc, int xc, int yc, int R, COLORREF color) {
    int x = 0, y = R;
    int d = 1 - R, ch1 = 3, ch2 = 5 - 2 * R;
    draw8Points(hdc, xc, yc, x, y, color);
    drawTangentLine(hdc, x, y, xc, yc, color);
    while (x < y) {
        if (d < 0) {
            d += ch1;
            ch2 += 2;
        }
        else {
            d += ch2;
            ch2 += 4;
            y--;
        }
        ch1 += 2;
        x++;
        draw8Points(hdc, xc, yc, x, y, color);
        drawTangentLine(hdc, x, y, xc, yc, color);
    }
}




int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR c, int ns)
{

    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hInstance = hi;

    wc.lpfnWndProc = WndProc;

    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("MyClass", "Hello World", WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, hi, 0);

    ShowWindow(hwnd, ns);
    UpdateWindow(hwnd);
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
