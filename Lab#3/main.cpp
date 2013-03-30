#include <windows.h>
#include <windowsx.h>

#define IDB_TOOLS_GROUP 100
#define IDB_PENCIL_TOOL 101
#define IDB_ELLIPSE_TOOL 102
#define IDB_RECTANGLE_TOOL 103
#define IDB_LINE_TOOL 104
#define IDB_ERASER_TOOL 105
#define IDB_FILL_CHECK 106
#define IDB_BORDER_WIDTH 107
#define IDB_RESET_BUTTON 108

LPSTR szClassName = "Lab3Class";
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;

void updateColorControls(HDC, COLORREF, int, int);

COLORREF g_rgbBackground=RGB(0,0,0);
COLORREF g_rgbCustom[16] = {0};

void colorSelect(HWND hwnd)
{
    CHOOSECOLOR cc = {sizeof(CHOOSECOLOR)};

    cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR;
    cc.hwndOwner = hwnd;
    cc.rgbResult = g_rgbBackground;
    cc.lpCustColors = g_rgbCustom;

    if(ChooseColor(&cc))
    {
        g_rgbBackground = cc.rgbResult;
    }
    //InvalidateRect(hwnd, NULL,FALSE);
}

// Redirecting child messages to parent window
WNDPROC GroupBoxProc;
LONG CALLBACK GroupRelay(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(msg == WM_COMMAND || msg == WM_NOTIFY){
        return SendMessage(GetParent(hwnd), msg, wParam, lParam);
    }

    return CallWindowProc(GroupBoxProc, hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    WNDCLASSEX wnd;
    MSG msg;
    HWND hwnd;
    hInst = hInstance;

    /* The Window Structure */
    wnd.hInstance = hInst;
    wnd.lpszClassName = szClassName;
    wnd.lpfnWndProc = WindowProcedure;
    wnd.style = CS_HREDRAW | CS_VREDRAW;
    wnd.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);                                // Default icon
    wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);                              // Default icon
    wnd.hCursor = LoadCursor(NULL, IDC_ARROW);                                  // Default arrow mouse cursor
    wnd.lpszMenuName = NULL;                                                    // No menu
    wnd.cbClsExtra = 0;                                                         // No extra bytes after the window class
    wnd.cbWndExtra = 0;                                                         //  structure or the window instance

    wnd.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);

    if(!RegisterClassEx(&wnd))                                                  // Register the WNDCLASSEX
    {
        MessageBox(NULL, "This Program Requires Windows NT", "Error", MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        (DWORD)NULL,
        szClassName,
        "Lab#1",                                                                // Window title
        WS_OVERLAPPED | WS_BORDER | WS_SYSMENU,                                 // Basic window style
        CW_USEDEFAULT, CW_USEDEFAULT,                                           // Set starting point to default value
        800, 515,                                                               // Set all the dimensions to default value
        NULL,                                                                   //no parent window
        NULL,                                                                   //no menu
        hInst,
        NULL);                                                                  //no parameters to pass

    ShowWindow(hwnd, iCmdShow);                                                 //display the window on the screen
    UpdateWindow(hwnd);                                                         //make sure the window is updated correctly

    while(GetMessage(&msg, NULL, 0, 0))                                         //message loop
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndToolsGroup;
    static HWND hwndPencilTool, hwndEllipseTool, hwndRectangleTool;
    static HWND hwndLineTool;
    static HWND hwndEraserTool;
    static HWND hwndFillCheck;
    static HWND hwndBorderWidth;
    static HWND hwndEraserWidth;
    static HWND hwndResetButton;

    static HWND hwndConfigGroup;
    RECT rect ;
    int screenW;
    int screenH;
    PAINTSTRUCT ps;
    HDC hdc;

    // Color preview rectangles
    HBRUSH hBrush;
    int xFillPreview   = 115;
    int yFillPreview   = 180;
    int xStrokePreview = 115;
    int yStrokePreview = 230;


    HDC hdcMem;
    BITMAP bitmap;
    HBITMAP hbmpDesignerImage = NULL;
    hbmpDesignerImage = (HBITMAP)LoadImage(hInst, "designer.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmpDesignerImage, sizeof(bitmap), &bitmap);

    // Drawing area rect
    static RECT drawingArea = {170, 17, 780, 475};

    switch(msg)
    {
        case WM_CREATE:
            screenW = GetSystemMetrics(SM_CXSCREEN);
            screenH = GetSystemMetrics(SM_CYSCREEN);
            GetWindowRect(hwnd, &rect);
            SetWindowPos(
                    hwnd, 0,
                    (screenW - rect.right)/2,
                    (screenH - rect.bottom)/2,
                    0, 0, SWP_NOZORDER|SWP_NOSIZE);

            // Drawing tools group
            hwndToolsGroup = CreateWindowEx(
                0,
                "Button",
                "Tools",
                WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                15, 10,
                140, 120,
                hwnd,
                (HMENU)IDB_TOOLS_GROUP,
                hInst,
                NULL);

            // Pen tool
            hwndPencilTool = CreateWindowEx(
                0,
                "Button",
                "Pencil",
                WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON,
                10, 15,
                120, 20,
                hwndToolsGroup,
                (HMENU)IDB_PENCIL_TOOL,
                hInst,
                NULL);
            Button_SetCheck(hwndPencilTool, BST_CHECKED);

            // Ellipse tool
            hwndEllipseTool = CreateWindowEx(
                0,
                "Button",
                "Ellipse",
                WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON,
                10, 35,
                120, 20,
                hwndToolsGroup,
                (HMENU)IDB_ELLIPSE_TOOL,
                hInst,
                NULL);

            // Rectangle tool
            hwndRectangleTool = CreateWindowEx(
                0,
                "Button",
                "Rectangle",
                WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON,
                10, 55,
                120, 20,
                hwndToolsGroup,
                (HMENU)IDB_RECTANGLE_TOOL,
                hInst,
                NULL);

            // Line tool
            hwndLineTool = CreateWindowEx(
                0,
                "Button",
                "Line",
                WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON,
                10, 75,
                120, 20,
                hwndToolsGroup,
                (HMENU)IDB_LINE_TOOL,
                hInst,
                NULL);

            // Eraser tool
            hwndEraserTool = CreateWindowEx(
                0,
                "Button",
                "Eraser",
                WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON,
                10, 95,
                120, 20,
                hwndToolsGroup,
                (HMENU)IDB_ERASER_TOOL,
                hInst,
                NULL);

            // Config group
            hwndConfigGroup = CreateWindowEx(
                0,
                "Button",
                "Config",
                WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                15, 140,
                140, 180,
                hwnd,
                (HMENU)IDB_TOOLS_GROUP,
                hInst,
                NULL);

            // Fill checkbox
            hwndFillCheck = CreateWindowEx(
                0,
                "Button",
                "Fill with color",
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                10, 15,
                120, 20,
                hwndConfigGroup,
                (HMENU)IDB_FILL_CHECK,
                hInst,
                NULL);

            // Fill with label
            CreateWindowEx(
                0,
                "Static",
                "Fill with",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                10, 40,
                90, 20,
                hwndConfigGroup,
                (HMENU)0,
                hInst,
                NULL);

            // Border width label
            CreateWindowEx(
                0,
                "Static",
                "Border width",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                10, 65,
                100, 20,
                hwndConfigGroup,
                (HMENU)0,
                hInst,
                NULL);

            // Border width input
            hwndBorderWidth = CreateWindowEx(
                0,
                "Edit",
                "1",
                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                100, 65,
                25, 20,
                hwndConfigGroup,
                (HMENU)0,
                hInst,
                NULL);

            // Border color label
            CreateWindowEx(
                0,
                "Static",
                "Border color",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                10, 90,
                90, 20,
                hwndConfigGroup,
                (HMENU)0,
                hInst,
                NULL);

            // Eraser width label
            CreateWindowEx(
                0,
                "Static",
                "Eraser width",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                10, 115,
                100, 20,
                hwndConfigGroup,
                (HMENU)0,
                hInst,
                NULL);

            // Eraser width input
            hwndEraserWidth = CreateWindowEx(
                0,
                "Edit",
                "1",
                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                100, 115,
                25, 20,
                hwndConfigGroup,
                (HMENU)0,
                hInst,
                NULL);

            // Reset Button
            hwndResetButton = CreateWindowEx(
                (DWORD)WS_EX_CONTROLPARENT,
                "Button",
                "Reset",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY,
                10, 140,
                120, 30,
                hwndConfigGroup,
                (HMENU)IDB_RESET_BUTTON,
                hInst,
                NULL);

            // Redirecting child messages to parent window
            GroupBoxProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hwndConfigGroup, GWLP_WNDPROC));
            SetWindowLongPtr(hwndConfigGroup, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(GroupRelay));

            GroupBoxProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hwndToolsGroup, GWLP_WNDPROC));
            SetWindowLongPtr(hwndToolsGroup, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(GroupRelay));
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDB_RESET_BUTTON:
                    if(HIWORD(wParam) == BN_CLICKED)
                    {
                        // Temporary. Proof of concept.
                        colorSelect(hwnd);
                    }
                    break;
            }
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            // Fill color preview rectangle
            updateColorControls(hdc, RGB(255, 255, 255), xFillPreview, yFillPreview);

            // Stroke color preview rectangle
            updateColorControls(hdc, RGB(0, 0, 0), xStrokePreview, yStrokePreview);

            // Blank draw area
            SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0,0,0)));
            SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            Rectangle(hdc, drawingArea.left, drawingArea.top, drawingArea.right, drawingArea.bottom);

            // Draw image
            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hbmpDesignerImage);
            BitBlt(hdc, 15, 335, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);

            EndPaint(hwnd, &ps);
            break;
        case WM_CLOSE:
            if(MessageBox(hwnd, "Are you sure that you want to close this task tracker?", "Lab#1", MB_YESNO) == IDYES)
                DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void updateColorControls(HDC hdc, COLORREF rgb, int xLeft, int yTop) {
    HBRUSH hBrush = CreateSolidBrush(rgb);
    HPEN hPen = CreatePen(PS_INSIDEFRAME, 2, RGB(10, 40, 140));

    SelectObject(hdc, hBrush);
    SelectObject(hdc, hPen);

    Rectangle(hdc, xLeft, yTop, xLeft + 25, yTop + 20);

    hBrush = CreateSolidBrush(RGB(10, 40, 140));
    SelectObject(hdc, hBrush);

    Rectangle(hdc, xLeft+18, yTop+13, xLeft + 25, yTop + 20);

    DeleteObject(hPen);
    DeleteObject(hBrush);
}
