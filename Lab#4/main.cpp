#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define ID_TIMER 201
#define X_INC 5
#define Y_OFFSET 10
#define WINDOW_W 900
#define WINDOW_H 300
#define FISH_W 85
#define FISH_H 50

LRESULT CALLBACK WindowProcedure( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ReDraw(void);
void DrawFish(HDC hdc, int direction, int tail, int XOff, int YOff);
void DrawNyan(HDC hdc);

HWND hwnd=NULL;
int curX=0;
int direction = 1;
int tail = 0;
int timer = 100;
bool flipped = false;
COLORREF fishcolor = RGB(255, 118, 0);
HBITMAP nyan[6];
HBITMAP rainbow;
int nyanXPos = 0;
HDC hdcMem;
int rainbowPos;
bool rainbowUp = false;

LPSTR szClassName = "Lab4Class";
HINSTANCE hInst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    WNDCLASSEX wnd;
    MSG msg;
    hInst = hInstance;

    /* The Window Structure */
    wnd.hInstance = hInst;
    wnd.lpszClassName = szClassName;
    wnd.lpfnWndProc = WindowProcedure;
    wnd.style = NULL;
    wnd.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);                                // Default icon
    wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);                              // Default icon
    wnd.hCursor = LoadCursor(NULL, IDC_ARROW);                                  // Default arrow mouse cursor
    wnd.lpszMenuName = NULL;                                                    // No menu
    wnd.cbClsExtra = 0;                                                         // No extra bytes after the window class
    wnd.cbWndExtra = 0;                                                         //  structure or the window instance

    wnd.hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH );

    if(!RegisterClassEx(&wnd))                                                  // Register the WNDCLASSEX
    {
        MessageBox(NULL, "This Program Requires Windows NT", "Error", MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        (DWORD)NULL,
        szClassName,
        "Lab#4",                                                                // Window title
        WS_OVERLAPPEDWINDOW,                                                    // Basic window style
        CW_USEDEFAULT, CW_USEDEFAULT,                                           // Set starting point to default value
        WINDOW_W, WINDOW_H,                                                               // Set all the dimensions to default value
        NULL,                                                                   //no parent window
        NULL,                                                                   //no menu
        hInst,
        NULL);                                                                  //no parameters to pass

    ShowWindow(hwnd, SW_SHOWNORMAL);                                            //display the window on the screen
    SetActiveWindow(hwnd);
    UpdateWindow(hwnd);                                                         //make sure the window is updated correctly
    SetTimer(hwnd, ID_TIMER, timer, NULL);

    while(GetMessage(&msg, NULL, 0, 0))                                         //message loop
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static RECT UpdateRect = {0, Y_OFFSET, FISH_W, Y_OFFSET+FISH_H+1};

  switch (message)
  {
    case WM_CREATE:
      for(int i = 0; i < 6; i++)
      {
        char str[11];
        sprintf(str, "nyan/%d.bmp", i+1);
        nyan[i] = (HBITMAP)LoadImage(hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      }

      rainbow = (HBITMAP)LoadImage(hInst, "nyan/rainbow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_CTLCOLORSTATIC:
      return (LRESULT)GetStockObject( WHITE_BRUSH );

    case WM_PAINT:
      ReDraw();
      return 0;

    case WM_TIMER:
      if (wParam == ID_TIMER)
      {
        curX += X_INC * direction;
        tail = (tail + 1) % 2;
        if (curX > WINDOW_W - FISH_W)
        {
          curX = WINDOW_W - FISH_W;
          direction = -1;
          flipped = true;
        }

        if (curX < 0)
        {
          curX = 0;
          direction = 1;
          flipped = true;
        }
        InvalidateRect(hwnd, &UpdateRect, 1);
        UpdateWindow(hwnd);
        UpdateRect.left = curX;
        UpdateRect.right = curX + FISH_W + 1;
        InvalidateRect(hwnd, &UpdateRect, 1);
        UpdateWindow( hwnd );
      }
      return 0;
    case WM_MOUSEWHEEL:
      // Changing the animation speed.
      if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)
      {
          if(timer < 500)
            timer += 5;
          else
            return 0;
      }
      else
      {
          if(timer > 5)
            timer -= 5;
          else
            return 0;
      }
      KillTimer(hwnd, ID_TIMER);
      SetTimer(hwnd, ID_TIMER, timer, NULL);
      return 0;
  }
  return (DefWindowProc(hWnd, message, wParam, lParam));
}

void ReDraw()
{
  HDC hdc;
  PAINTSTRUCT ps;

  hdc = BeginPaint(hwnd, (LPPAINTSTRUCT)&ps);
  DrawFish(hdc, direction, tail, curX, Y_OFFSET);
  DrawNyan(hdc);
  EndPaint(hwnd, (LPPAINTSTRUCT)&ps);
}

/*
direction: directionection. 1: facing right; -1: facing left;
tail:position of tail. 1 or 0.
XOff, YOff: x and y offset.
*/
void DrawFish(HDC hdc, int direction, int tail, int XOff, int YOff)
{
  HPEN gpen, wpen,oldpen;
  HBRUSH gbrush, wbrush, oldbrush;
  POINT tail_1[3] = {{35, 20}, {5, 0}, {0, 40}};
  POINT tail_2[3] = {{35, 20}, {0, 0}, {5, 40}};
  POINT mouth[3] = {{65, 20}, {85, 30}, {85, 10}};

  srand (time(NULL));

  int i;
  if (direction < 0)
  {
    for (i=0; i<3; i++)
    {
      tail_1[i].x = FISH_W - tail_1[i].x;
      tail_2[i].x = FISH_W - tail_2[i].x;
      mouth[i].x = FISH_W - mouth[i].x;
    }
  }

  for (i=0; i<3; i++)
  {
    tail_1[i].x += XOff;
    tail_1[i].y += YOff;
    tail_2[i].x += XOff;
    tail_2[i].y += YOff;
    mouth[i].x += XOff;
    mouth[i].y += YOff;
  }

  if(flipped)
  {
    fishcolor = RGB(rand() % 256, rand() % 256, rand() % 256);
    flipped = false;
  }

  gpen = CreatePen(PS_SOLID, 1, fishcolor);
  oldpen = (HPEN)SelectObject(hdc, gpen);
  gbrush = CreateSolidBrush(fishcolor);
  wbrush = (HBRUSH)GetStockObject( WHITE_BRUSH );
  wpen = (HPEN)GetStockObject( WHITE_PEN );
  oldbrush = (HBRUSH)SelectObject(hdc, gbrush);

  if (direction < 0)
    Ellipse(hdc, XOff, 0 + YOff, 70 + XOff, 40 + YOff);
  else
    Ellipse(hdc, 15 + XOff, 0 + YOff, 85 + XOff, 40 + YOff);

  if (tail)
    Polygon(hdc, (CONST POINT *)&tail_1, 3);
  else
    Polygon(hdc, (CONST POINT *)&tail_2, 3);

  SelectObject(hdc, wpen);
  SelectObject(hdc, wbrush);
  Polygon(hdc, (CONST POINT *)&mouth, 3);

  int rndVal = rand() % 10;
  // Blinking with a probability of 0.2
  if(rndVal > 1)
  {
    if (direction < 0)
      Ellipse(hdc, 20 + XOff, 6 + YOff, 25 + XOff, 12 + YOff);
    else
      Ellipse(hdc, 60 + XOff, 6 + YOff, 65 + XOff, 12 + YOff);
  }

  SelectObject(hdc, oldpen);
  DeleteObject( (HGDIOBJ)gpen );
  SelectObject(hdc, oldbrush);
  DeleteObject( (HGDIOBJ)gbrush );
}

void DrawNyan(HDC hdc)
{
  RECT rect;
  rect.top = 100;
  rect.left = 0;
  rect.bottom = 281;
  rect.right = nyanXPos;
  HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
  hdc = GetDC(hwnd);
  hdcMem = CreateCompatibleDC(hdc);

  if(nyanXPos > WINDOW_W)
  {
    nyanXPos = 0;
    rect.right = WINDOW_W;
    FillRect(hdc, &rect, hBrush);
  }

  int rainDir = (rainbowUp)?-1:1;
  SelectObject (hdcMem, rainbow);

  if((nyanXPos % 50) == 0)
    rainbowPos += rainDir;

  if(rainbowPos > 3)
    rainbowUp = true;
  else if (rainbowPos < 0)
    rainbowUp = false;

  BitBlt(hdc, nyanXPos - 5, 130 + rainbowPos * 5, 5, 96, hdcMem, 0, 0, SRCCOPY);

  SelectObject (hdcMem, nyan[ (nyanXPos/5) % 6 ]);
  BitBlt(hdc, nyanXPos, 100, 260, 181, hdcMem, 0, 0, SRCCOPY);
  ReleaseDC(hwnd, hdc);
  DeleteDC(hdcMem);

  DeleteObject(hBrush);
  nyanXPos += 5;
}
