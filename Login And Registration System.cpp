#include <windows.h>
#include <fstream>
#include <string>

using namespace std;

HWND hUsername, hPassword;

// Function to save user
void registerUser(string username, string password) {
    ofstream outFile("users.txt", ios::app);
    if (outFile.is_open()) {
        outFile << username << " " << password << endl;
        outFile.close();
        MessageBox(NULL, "Registration successful!", "Info", MB_OK);
    } else {
        MessageBox(NULL, "Unable to open file.", "Error", MB_OK);
    }
}

// Function to login user
void loginUser(string username, string password) {
    ifstream inFile("users.txt");
    string u, p;
    bool success = false;

    if (inFile.is_open()) {
        while (inFile >> u >> p) {
            if (u == username && p == password) {
                success = true;
                break;
            }
        }
        inFile.close();
    }

    if (success) {
        MessageBox(NULL, "Login successful!", "Info", MB_OK);
    } else {
        MessageBox(NULL, "Login failed. Invalid credentials.", "Error", MB_OK);
    }
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    char username[100], password[100];

    switch (msg) {
        case WM_CREATE:
            CreateWindow("STATIC", "Username:", WS_VISIBLE | WS_CHILD,
                         20, 20, 80, 20, hwnd, NULL, NULL, NULL);
            hUsername = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                     110, 20, 150, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Password:", WS_VISIBLE | WS_CHILD,
                         20, 60, 80, 20, hwnd, NULL, NULL, NULL);
            hPassword = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
                                     110, 60, 150, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("BUTTON", "Register", WS_VISIBLE | WS_CHILD,
                         50, 100, 80, 30, hwnd, (HMENU)1, NULL, NULL);
            CreateWindow("BUTTON", "Login", WS_VISIBLE | WS_CHILD,
                         150, 100, 80, 30, hwnd, (HMENU)2, NULL, NULL);
            break;

        case WM_COMMAND:
            if (wParam == 1 || wParam == 2) {
                GetWindowText(hUsername, username, 100);
                GetWindowText(hPassword, password, 100);
                string u = username;
                string p = password;

                if (wParam == 1)
                    registerUser(u, p);
                else
                    loginUser(u, p);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                   LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hInstance = hInst;
    wc.lpszClassName = L"UserAuthGUI";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc))
        return -1;

    CreateWindowW(L"UserAuthGUI", L"Login & Register", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  100, 100, 320, 200, NULL, NULL, NULL, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

