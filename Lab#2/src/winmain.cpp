#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <commctrl.h>
#include "../include/resource.h"

INT_PTR CALLBACK AboutDialogProc(HWND, UINT, WPARAM, LPARAM);                   // About dialog
LPSTR szClassName = "Lab2Class";
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);                   // Window Procedure

HINSTANCE hInst;
int focused = 0, idFocus;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    WNDCLASSEX wnd;
    MSG msg;
    HWND hwnd;
    hInst = hInstance;
    HACCEL hAccel;

    /* The Window Structure */
    wnd.hInstance = hInst;
    wnd.lpszClassName = szClassName;
    wnd.lpfnWndProc = WindowProcedure;
    wnd.style = CS_HREDRAW | CS_VREDRAW;
    wnd.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wnd.hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 0, 0, LR_SHARED);
    wnd.hIconSm = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, LR_SHARED);
    wnd.hCursor = LoadCursor(NULL, IDC_HAND);                                   // Default arrow mouse cursor
    wnd.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);                           // No menu
    wnd.cbClsExtra = 0;                                                         // No extra bytes after the window class
    wnd.cbWndExtra = 0;                                                         //  structure or the window instance

    wnd.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);

    hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));     // Loading the accelerators

    if(!RegisterClassEx(&wnd))                                                  // Register the WNDCLASSEX
    {
        MessageBox(NULL, "This Program Requires Windows NT", "Error", MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        (DWORD)NULL,
        szClassName,
        "Lab#1",                                                                // Window title
        WS_OVERLAPPEDWINDOW | WS_BORDER,                                        // Basic window style
        CW_USEDEFAULT, CW_USEDEFAULT,                                           // Set starting point to default value
        520, 250,                                                               // Set all the dimensions to default value
        NULL,                                                                   //no parent window
        NULL,                                                                   //no menu
        hInst,
        NULL);                                                                  //no parameters to pass

    ShowWindow(hwnd, iCmdShow);                                                 //display the window on the screen
    UpdateWindow(hwnd);                                                         //make sure the window is updated correctly

    while(GetMessage(&msg, NULL, 0, 0))                                         //message loop
    {
        if (! TranslateAccelerator(hwnd, hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndAddButton, hwndCurrentTask, hwndTextInput, hwndList, hwndTextCounter;
    static HWND hwndScroll[3], hwndScrollValue[3];
    RECT rect;
    PAINTSTRUCT ps;
    HDC hdc;
    LPSTR title = TEXT("  A simple task tracker.");
    LRESULT textSize;
    HBRUSH color, hBrushStatic;
    char * message = new char[100];
    char * placeholder = new char[26];
    placeholder = "Type here the new task...";
    int screenW;
    int screenH;
    char * itemCnt;
    int index, items, i, cyChar;
    static int colors[3];                                                        // Red, Green, Blue [0, 255]
    static COLORREF crPrim[3] = {RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255)};
    COLORREF taskColor = RGB(0, 0, 0);


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

            hwndAddButton = CreateWindowEx(
                (DWORD)NULL,
                TEXT("button"),                                                 // The class name required is button
                TEXT("Add task to the list"),                                   // the caption of the button
                WS_CHILD |WS_VISIBLE | BS_PUSHBUTTON,                           // the styles
                257, 175,                                                       // the left and top co-ordinates
                130, 20,                                                        // width and height
                hwnd,                                                           // parent window handle
                (HMENU)IDC_ADD_BUTTON,                                          // the ID of your button
                hInst,                                                          // the instance of your application
                NULL);

            hwndCurrentTask = CreateWindowEx(
                (DWORD)NULL,
                TEXT("static"),
                TEXT("AE"),
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                392, 20,
                115, 40,
                hwnd,
                (HMENU)IDC_CURRENT_TASK,
                hInst,
                NULL);

            hwndTextInput = CreateWindowEx(
                (DWORD)NULL,
                TEXT("edit"),                                                   // The class name required is edit
                TEXT(placeholder),                                              // Default text.
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,             // Textbox styles
                5, 175,                                                         // the left and top co-ordinates
                242, 20,                                                        // width and height
                hwnd,                                                           // parent window handle
                (HMENU)IDC_TEXT_INPUT,                                          // the ID of your editbox
                hInst,                                                          // the instance of your application
                NULL);                                                          // extra bits you dont really need

            hwndList = CreateWindowEx(
                (DWORD)NULL,
                TEXT("listbox"),
                "",
                WS_CHILD | WS_VISIBLE | LBS_STANDARD | ES_AUTOVSCROLL | WS_BORDER,
                5, 20,
                382, 150,
                hwnd,
                (HMENU)IDC_TASK_LIST,
                hInst,
                NULL);

            hwndTextCounter = CreateWindowEx(
                (DWORD)NULL,
                TEXT("static"),
                NULL,
                WS_CHILD | WS_VISIBLE | SS_RIGHT,
                328, 0,
                60, 20,
                hwnd,
                (HMENU)IDC_TEXT_COUNTER,
                hInst,
                NULL);

            for (i = 0 ; i < 3 ; i++)
            {
                hwndScroll[i] = CreateWindowEx(
                    (DWORD)NULL,
                    TEXT ("scrollbar"),
                    NULL,
                    WS_CHILD | WS_VISIBLE | SBS_VERT,
                    (400 + i * 40), 65,
                    20, 100,
                    hwnd,
                    (HMENU)(105 + i),                                           // Defined in resource.h IDC_{COLOR}_SCROLL 105-107
                    hInst,
                    NULL) ;

                SetScrollRange(hwndScroll[i], SB_CTL, 0, 255, FALSE);
                SetScrollPos(hwndScroll[i], SB_CTL, 255 - 0, FALSE);            // 255 - 0 because we use zero as coordinate

                hwndScrollValue[i] = CreateWindowEx(
                    (DWORD)NULL,
                    TEXT ("static"),
                    TEXT ("0"),
                    WS_CHILD | WS_VISIBLE | SS_CENTER,
                    (395 + i * 40), 175,
                    30, 20,
                    hwnd,
                    (HMENU)(108 + i),
                    hInst,
                    NULL);
            }
            hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
            cyChar = HIWORD(GetDialogBaseUnits ());
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                /*case IDC_COUNT_BUTTON:
                    if(HIWORD(wParam) == BN_CLICKED)
                    {
                        items = SendMessage(hwndList, LB_GETCOUNT, 0, 0);
                        sprintf(message, "There are %d tasks in the list.", items);
                        MessageBox(hwnd, message, "Tasks counter", MB_OK);
                        ScrollWindowEx(hwnd, -20, -20, NULL, NULL, NULL, NULL, 2 );
                    }
                    break;
*/
                case IDC_ADD_BUTTON:
                    textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM)message);
                    message[textSize] = _T('\0');

                    if(strlen(message) && strcmp(message, placeholder))
                    {
                        char *item = new char[200];
                        strcpy(item, " - ");                                    // Managing the new string
                        strcat(item, message);
                        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)item);
                        delete [] item;                                         // Managing the memory

                        SendMessage(
                            hwndTextInput,
                            WM_SETTEXT,
                            TRUE,
                            (LPARAM)placeholder);                               // Recovering the placeholder
                        focused = 0;
                    }
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    break;
                case IDC_TEXT_INPUT:
                    if(HIWORD(wParam) == EN_SETFOCUS)
                    {
                        textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM)message);
                        message[textSize] = _T('\0');
                        if(!strcmp(message, placeholder))
                        {
                            SendMessage(hwndTextInput, WM_SETTEXT, TRUE,(LPARAM)"");// Clearing the text input
                            focused = 1;
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
                        }
                    }
                    else if(HIWORD(wParam) == EN_KILLFOCUS)
                    {
                        textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM)message);
                        message[textSize] = _T('\0');
                        if(!strcmp(message, ""))
                        {
                            SendMessage(
                                hwndTextInput,
                                WM_SETTEXT,
                                TRUE,
                                (LPARAM)placeholder);                           // Recovering the placeholder
                            focused = 0;
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
                        }
                    }
                    break;
                case ID_HELP_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTDIALOG), hwnd, &AboutDialogProc);
                    break;

                case ID_FILE_EXIT:
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    break;

                case IDC_TASK_LIST:
                    switch(HIWORD(wParam))
                    {
                        case LBN_SELCHANGE:
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                            break;

                        case LBN_DBLCLK:
                            index = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                            SendMessage(hwndList, LB_DELETESTRING, (WPARAM)index, 0);
                            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                            break;
                    }
                    break;

                case ID_DATA_CLEAR:
                    SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    break;
            }
            break;
        case WM_CTLCOLOREDIT:
            switch(GetDlgCtrlID((HWND)lParam))
            {
                case IDC_TEXT_INPUT:
                {
                    hdc = (HDC)wParam;                                          //Get handles
                    if(focused)
                    {
                        color = CreateSolidBrush(RGB(255, 255, 255));
                        SetTextColor(hdc, RGB(0, 0, 0));                        // Text color
                        SetBkMode(hdc, TRANSPARENT);                            // EditBox Backround Mode
                        SetBkColor(hdc,(LONG)color);                            // Backround color for EditBox
                    }
                    else
                    {
                        color = CreateSolidBrush(RGB(255, 255, 255));
                        SetTextColor(hdc, RGB(150, 150, 150));                  // Text color
                        SetBkMode(hdc, TRANSPARENT);                            // EditBox Backround Mode
                        SetBkColor(hdc,(LONG)color);                            // Backround color for EditBox
                    }
                    return (LONG)color;                                         // Paint it
                }
            }
            break;

        case WM_CTLCOLORSTATIC:
            if ((HWND)lParam == GetDlgItem(hwnd, IDC_TEXT_COUNTER))
            {
                hdc = (HDC)wParam;                                              //Get handles
                SetBkMode((HDC)wParam, TRANSPARENT);
                SetTextColor((HDC)wParam, RGB(0, 100, 0));
                return (BOOL)GetStockObject(NULL_BRUSH);
            }
            if(((HWND)lParam == GetDlgItem(hwnd, IDC_RED_LABEL))
                || ((HWND)lParam == GetDlgItem(hwnd, IDC_GREEN_LABEL))
                || ((HWND)lParam == GetDlgItem(hwnd, IDC_BLUE_LABEL)))
            {
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (BOOL)GetStockObject(NULL_BRUSH);
            }

            if((HWND)lParam == GetDlgItem(hwnd, IDC_CURRENT_TASK))
            {
                SetBkMode((HDC)wParam, TRANSPARENT);
                SetTextColor((HDC)wParam, taskColor);
                return (BOOL)GetStockObject(NULL_BRUSH);
            }
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);                                         // Getting coordinates of window client area
            SetBkMode(hdc, TRANSPARENT);                                        // Set the background to be transparent
            SetTextColor(hdc, RGB(152, 0, 16));                                 // Setting the text color to a dark red
            DrawText(hdc, title, -1, &rect, DT_LEFT | DT_TOP);                  // Drawind the text on top aligning it to center
            SetTextColor(hdc, RGB(0, 100, 0));                                  // Setting color to a dark green
            itemCnt = new char[10];
            items = SendMessage(hwndList, LB_GETCOUNT, 0, 0);
            sprintf(itemCnt, "%d", items);                                      // Creating the counter string
            SetWindowText(hwndTextCounter, itemCnt);                            // Showing the counter

            index = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
            if(index != -1)
            {
                SendMessage(hwndList, LB_GETTEXT, (WPARAM)index, (LPARAM)message);
                message += 3;                                                   // Skipping first three characters of the string
                SetWindowText(hwndCurrentTask, message);
            }
            else
                SetWindowText(hwndCurrentTask, "No task selected");

            SetTextColor(hdc, RGB(0, 0, 0));                                    // Resetting the color to black
            EndPaint(hwnd, &ps);
            break;

        case WM_SETFOCUS :
            SetFocus (hwndScroll[idFocus]);
            return 0;

        case WM_VSCROLL :
            i = GetWindowLong ((HWND) lParam, GWL_ID) - 105;

            switch (LOWORD (wParam))
            {
                case SB_PAGEDOWN:
                    colors[i] -= 15;
                case SB_LINEDOWN:
                    colors[i] = max(0, colors[i] - 1);
                    break;

                case SB_PAGEUP:
                    colors[i] += 15;
                case SB_LINEUP:
                    colors[i] = min(255, colors[i] + 1);
                    break;

                case SB_TOP:
                    colors[i] = 255;
                    break;

                case SB_BOTTOM:
                    colors[i] = 0;
                    break;

                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    colors[i] = 255 - HIWORD (wParam);
                    break;

                default:
                    break;
            }
            SetScrollPos(hwndScroll[i], SB_CTL, 255 - colors[i], TRUE) ;
            sprintf(message, "%d", colors[i]) ;
            SetWindowText (hwndScrollValue[i], message) ;
            taskColor = RGB(colors[1], colors[2], colors[3]);

            GetClientRect(hwndScrollValue[i], &rect);
            InvalidateRect(hwndScrollValue[i], &rect, TRUE) ;
            MapWindowPoints(hwndScrollValue[i], hwnd, (POINT *) &rect, 2);
            RedrawWindow(hwnd, &rect, NULL, RDW_ERASE | RDW_INVALIDATE);

            return 0 ;

        case WM_CTLCOLORSCROLLBAR :
            i = GetWindowLong((HWND)lParam, GWL_ID) - 105;
            return (LRESULT)CreateSolidBrush(crPrim[i]);

        case WM_CLOSE:
            if(MessageBox(hwnd, "Are you sure that you want to close this task tracker?", "Lab#1", MB_OKCANCEL | MB_ICONWARNING) == IDOK)
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

INT_PTR CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                {
                    EndDialog(hwndDlg, (INT_PTR) LOWORD(wParam));
                    return (INT_PTR) TRUE;
                }
            }
            break;
        }

        case WM_INITDIALOG:
            return (INT_PTR) TRUE;
    }
    return (INT_PTR) FALSE;
}

