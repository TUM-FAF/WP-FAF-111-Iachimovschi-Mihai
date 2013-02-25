#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

#define IDC_ADD_BUTTON 100
#define IDC_COUNT_BUTTON 101
#define IDC_TEXT_LIST 102
#define IDC_TEXT_INPUT 103

LPSTR szClassName = "Lab1Class";
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;
int items, focused = 0;

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
        400, 260,                                                               // Set all the dimensions to default value
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
    static HWND hwndAddButton, hwndCountButton, hwndTextList, hwndTextInput;
    RECT rect ;
    PAINTSTRUCT ps ;
    HDC hdc;
    LPSTR title = _T("A simple task tracker.");
    LRESULT textSize;
    char * message = new char[100];
    char * placeholder = new char[26];
    placeholder = "Type here the new task...";
    int screenW;
    int screenH;
    HBRUSH color;
    char * itemCnt;

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
                257, 170,                                                       // the left and top co-ordinates
                130, 20,                                                        // width and height
                hwnd,                                                           // parent window handle
                (HMENU)IDC_ADD_BUTTON,                                          // the ID of your button
                hInst,                                                          // the instance of your application
                NULL);

            hwndCountButton = CreateWindowEx(
                (DWORD)WS_EX_CLIENTEDGE,
                TEXT("button"),                                                 // The class name required is button
                TEXT("Count all tracked tasks"),                                // the caption of the button
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,                          // the styles
                5, 200,                                                         // the left and top co-ordinates
                382, 30,                                                        // width and height
                hwnd,                                                           // parent window handle
                (HMENU)IDC_COUNT_BUTTON,                                        // the ID of your button
                hInst,                                                          // the instance of your application
                NULL);                                                          // extra bits you dont really need

            hwndTextList = CreateWindowEx(
                (DWORD)NULL,
                TEXT("edit"),                                                   // The class name required is edit
                TEXT(""),                                                       // Default text.
                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL |
                ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,                    // Textbox styles
                5, 20,                                                          // the left and top co-ordinates
                382, 140,                                                       // width and height
                hwnd,                                                           // parent window handle
                (HMENU)IDC_TEXT_LIST,                                           // the ID of your editbox
                hInst,                                                          // the instance of your application
                NULL);                                                          // extra bits you dont really need

            hwndTextInput = CreateWindowEx(
                (DWORD)NULL,
                TEXT("edit"),                                                   // The class name required is edit
                TEXT(placeholder),                                              // Default text.
                WS_VISIBLE | WS_CHILD | WS_BORDER,                              // Textbox styles
                5, 170,                                                         // the left and top co-ordinates
                242, 20,                                                        // width and height
                hwnd,                                                           // parent window handle
                (HMENU)IDC_TEXT_INPUT,                                          // the ID of your editbox
                hInst,                                                          // the instance of your application
                NULL);                                                          // extra bits you dont really need
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDC_COUNT_BUTTON:
                    if(HIWORD(wParam) == BN_CLICKED){
                        sprintf(message, "There are %d tasks in the list.", items);
                        MessageBox(hwnd, message, "Tasks counter", MB_ICONINFORMATION);
                    }
                    break;

                case IDC_ADD_BUTTON:
                    textSize = SendMessage(hwndTextInput, WM_GETTEXT, 100, (LPARAM)message);
                    message[textSize] = _T('\0');

                    if(strlen(message) && strcmp(message, placeholder))
                    {
                        char *item = new char[200];
                        if(items)
                            strcpy(item, "\r\n - ");
                        else
                            strcpy(item, " - ");                                // Managing the new string
                        strcat(item, message);
                        SendMessage(hwndTextList, EM_REPLACESEL,
                            TRUE, (LPARAM)item);                                // Appending a new item in the list
                        SendMessage(hwndTextInput, WM_SETTEXT, TRUE,(LPARAM)"");// Clearing the text input
                        delete [] item;                                         // Managing the memory
                        items += 1;                                             // Incrementing the number of items
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
            }
            break;
        case WM_CTLCOLOREDIT:
            if(GetDlgCtrlID((HWND)lParam) == IDC_TEXT_INPUT)
            {
                hdc = (HDC)wParam;                                              //Get handles
                if(focused)
                {
                    color = CreateSolidBrush(RGB(255, 255, 255));
                    SetTextColor(hdc, RGB(0, 0, 0));                            // Text color
                    SetBkMode(hdc, TRANSPARENT);                                // EditBox Backround Mode
                    SetBkColor(hdc,(LONG)color);                                // Backround color for EditBox
                }
                else
                {
                    color = CreateSolidBrush(RGB(255, 255, 255));
                    SetTextColor(hdc, RGB(150, 150, 150));                      // Text color
                    SetBkMode(hdc, TRANSPARENT);                                // EditBox Backround Mode
                    SetBkColor(hdc,(LONG)color);                                // Backround color for EditBox
                }
                return (LONG)color;                                             // Paint it
            }
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);                                         // Getting coordinates of window client area
            SetBkMode(hdc, TRANSPARENT);                                        // Set the background to be transparent
            SetTextColor(hdc, RGB(152, 0, 16));                                 // Setting the text color to a dark red
            DrawText(hdc, title, -1, &rect, DT_CENTER | DT_TOP);                // Drawind the text on top aligning it to center
            SetTextColor(hdc, RGB(0, 100, 0));                                  // Setting color to a dark green
            itemCnt = new char[10];
            sprintf(itemCnt, "%d  ", items);                                    // Creating the counter string
            DrawText(hdc, itemCnt, -1, &rect, DT_RIGHT | DT_TOP);               // Drawing the text on top aligning it to right
            SetTextColor(hdc, RGB(0, 0, 0));                                    // Resetting the color to black
            EndPaint(hwnd, &ps);
            break;
        case WM_CLOSE:
            if(MessageBox(hwnd, "Are you sure that you want to close this task tracker?", "Lab#1", MB_OKCANCEL) == IDOK)
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

