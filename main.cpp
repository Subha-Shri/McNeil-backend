#include <windows.h>
#include <commdlg.h>
#include <wininet.h>
#include <string>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "wininet.lib")

// UI Element Control IDs
#define ID_BTN_BROWSE 1
#define ID_BTN_CLOUD 2
#define ID_STATIC_PATH 3
#define ID_STATIC_STATUS 4

HWND hStaticPath;
HWND hStaticStatus;
std::string selectedFilePath = "";

// Function to open native Windows Explorer file dialog
void OpenFileBrowser(HWND hwnd) {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0PDF Files (*.pdf)\0*.pdf\0CSV Files (*.csv)\0*.csv\0Text Files (*.txt)\0*.txt\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        selectedFilePath = szFile;
        std::string displayMsg = "Selected File: " + selectedFilePath;
        SetWindowTextA(hStaticPath, displayMsg.c_str());
        SetWindowTextA(hStaticStatus, "Status: File loaded via Windows Explorer.");
    }
}

// Function to call your live Render cloud backend
void CallCloudBackend(HWND hwnd) {
    SetWindowTextA(hStaticStatus, "Status: Connecting to Render cloud backend...");
    
    HINTERNET hInternet = InternetOpenA("McNeilCppClient", INTERNET_OPEN_TYPE_DIRECT_CONNECT, NULL, NULL, 0);
    if (!hInternet) {
        SetWindowTextA(hStaticStatus, "Status: Error opening internet connection.");
        return;
    }

    // Connect to your live Render web service
    HINTERNET hConnect = InternetConnectA(hInternet, "mcneil-backend.onrender.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        SetWindowTextA(hStaticStatus, "Status: Failed to connect to Render server.");
        return;
    }

    // Request the loop sheet generation endpoint
    HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", "/generate-loop-sheet/TAG-101", NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        SetWindowTextA(hStaticStatus, "Status: Failed to create HTTP request.");
        return;
    }

    if (HttpSendRequestA(hRequest, NULL, 0, NULL, 0)) {
        SetWindowTextA(hStaticStatus, "Status: Success! Cloud PDF requested.");
        MessageBoxA(hwnd, "Loop Sheet PDF successfully requested and processed from Render cloud!", "McNeil C++ Client", MB_OK | MB_ICONINFORMATION);
    } else {
        SetWindowTextA(hStaticStatus, "Status: HTTP Request failed.");
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

// Window Event Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        // Browse File Button
        CreateWindowA("BUTTON", "Browse File", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            40, 30, 180, 40, hwnd, (HMENU)ID_BTN_BROWSE, NULL, NULL);

        // Generate Cloud PDF Button
        CreateWindowA("BUTTON", "Generate Cloud Loop Sheet", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            240, 30, 220, 40, hwnd, (HMENU)ID_BTN_CLOUD, NULL, NULL);

        // File Path Display Label