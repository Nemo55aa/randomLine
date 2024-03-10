#pragma once


#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

VOID OnPaint(HDC hdc);
VOID OnPaint(HDC hdc, INT x1, INT y1, INT x2, INT y2);
VOID drawDot(HDC hdc, INT x, INT y);
int GetRandom(int min, int max)
{
    return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    Pen      pen(Color(255, 0, 0, 255));
    graphics.DrawLine(&pen, 10, 10, 200, 100);

}
VOID OnPaint(HDC hdc, INT x1, INT y1, INT x2, INT y2)
{
    Graphics graphics(hdc);
    Pen      pen(Color(255, 0, 0, 255));
    graphics.DrawLine(&pen, x1, y1, x2, y2);
}

VOID drawDot(HDC hdc, INT x, INT y)
{
    Graphics graphics(hdc);
    Pen      pen(Color(255, 0, 0, 255));
    graphics.DrawRectangle(&pen, x, y, 1, 1);
}
