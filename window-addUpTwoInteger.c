#define UNICODE
#define _UNICODE
#include <windows.h>
#include <ctype.h>
#include <wchar.h>
#include <stdlib.h>

const wchar_t CLASS_NAME[] = L"MyWindow";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct Window {
    HWND hwnd;
    HWND textbox1;
    HWND textbox2;
} theWindow;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEXW wc = {};

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = 0;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = NULL;

    if (!RegisterClassExW(&wc)) {
        MessageBox(
            NULL,
            L"Failed to Register Class",
            L"FAILURE",
            MB_OK
        );
        return 0;
    }

    theWindow.hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"An Ordinary Window",
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (theWindow.hwnd == NULL) {
        MessageBox(
            NULL,
            L"Failed to Create Window",
            L"FAILURE",
            MB_OK
        );
        return 0;
    }

    ShowWindow(theWindow.hwnd, SW_SHOW);

    theWindow.textbox1 = CreateWindowExW(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        0,
        0,
        200,
        30,
        theWindow.hwnd,
        (HMENU) 0,
        hInstance,
        NULL
    );

    theWindow.textbox2 = CreateWindowExW(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        200,
        0,
        200,
        30,
        theWindow.hwnd,
        (HMENU) 0,
        hInstance,
        NULL
    );

    CreateWindowExW(
        0,
        L"BUTTON",
        L"Sum",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        100,
        30,
        100,
        30,
        theWindow.hwnd,
        (HMENU) 1,
        hInstance,
        NULL
    );

    CreateWindowExW(
        0,
        L"BUTTON",
        L"BUTTON 2",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        200,
        30,
        100,
        30,
        theWindow.hwnd,
        (HMENU) 2,
        hInstance,
        NULL
    );

    MSG msg;
    
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_COMMAND:
            if (HIWORD(wParam) == EN_UPDATE) {
                HWND focused = GetFocus();

                if (focused == theWindow.textbox1 || focused == theWindow.textbox2) {
                    int length = GetWindowTextLength(focused);

                    if (length > 5) {
                        wchar_t buffer[length];
                        GetWindowText(focused, buffer, length);
                        
                        for (int i = length; i > 5; i--) {
                            buffer[i] = L'\0';
                            SetWindowText(focused, buffer);
                            SendMessage(focused, EM_SETSEL, i, i);
                        }

                    } else {
                        wchar_t buffer[6];
                        GetWindowText(focused, buffer, 6);
                        
                        for (int i = 0; buffer[i] != '\0'; i++) {
                            if (!iswdigit(buffer[i]) && buffer[i] != '\b') {
                                buffer[i] = L'\0';
                                SetWindowText(focused, buffer);
                                SendMessage(focused, EM_SETSEL, i, i);
                            }
                        }
                    }
                }
            }

            switch (LOWORD(wParam)) {
                case 1:
                    wchar_t* string1 = malloc(6 * sizeof(wchar_t));
                    wchar_t* string2 = malloc(6 * sizeof(wchar_t));

                    if (string1 == NULL || string2 == NULL) {
                        return 1;
                    }

                    GetWindowText(theWindow.textbox1, string1, 6);
                    GetWindowText(theWindow.textbox2, string2, 6); // 5 + (1 for null)
                    int number1 = (int) wcstol(string1, NULL, 10);
                    int number2 = (int) wcstol(string2, NULL, 10);

                    free(string1);
                    free(string2);

                    wchar_t output[7]; // (6 because ) + (1 for null)
                    swprintf(output, 7, L"%d", number1 + number2); // swprintf because of wide character

                    MessageBox(
                        hwnd,
                        output,
                        L"Sum",
                        MB_OK
                    );
                    break;
                case 2:
                    MessageBox(
                        hwnd,
                        L"This is button 2...",
                        L"No.",
                        MB_OK
                    );
                    break;
            }
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}