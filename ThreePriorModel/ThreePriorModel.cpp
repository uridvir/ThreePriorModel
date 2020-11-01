// ThreePriorModel.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <commdlg.h>
#include "ThreePriorModel.h"
#include "ModelBackend.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
std::string outputContents;
const char* versionText = "1.0";

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THREEPRIORMODEL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THREEPRIORMODEL));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BLUETEXAS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_THREEPRIORMODEL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_BLUETEXAS));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 640, 480, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
				{
					std::string aboutMessage = std::string("ThreePriorModel, Version ") + versionText + "\n"
						+ "Created by Uri Dvir, icon based on https://commons.wikimedia.org/wiki/File:Texas_flag_map.svg";
					MessageBoxA(hWnd, aboutMessage.c_str(), "About ThreePriorModel", MB_OK);
				}
                break;
			case IDM_OPEN:
				{
					OPENFILENAME ofn;        // common dialog box structure
					WCHAR szFile[260];       // buffer for file name
					HANDLE fileHandle; // file handle

					// Initialize OPENFILENAME
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hWnd;
					ofn.lpstrFile = szFile;
					// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
					// use the contents of szFile to initialize itself.
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = _T("Spreadsheets (*.csv)\0*.csv\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					// Display the Open dialog box. 
					if (GetOpenFileNameW(&ofn)) {
						fileHandle = CreateFile(ofn.lpstrFile,
							GENERIC_READ,
							0,
							(LPSECURITY_ATTRIBUTES)NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							(HANDLE)NULL);
						DWORD fileSize = GetFileSize(fileHandle, NULL);
						DWORD bytesRead;
						char* contents = new char[fileSize];
						if (ReadFile(fileHandle, contents, fileSize, &bytesRead, NULL)) {
							try {
								outputContents = exportCSV(runModel(getInternals(parseCSV(std::string(contents)))));
							}
							catch (std::logic_error e) {
								MessageBoxA(hWnd, e.what(), "Error", MB_OK | MB_ICONERROR);
								outputContents = "";
							}
						}
						CloseHandle(fileHandle);
					}
				}
				break;
			case IDM_SAVE:
				{
					if (outputContents == "") {
						MessageBoxA(hWnd, "Cannot export projection without input data. Select input data then try again.", "Error", MB_OK | MB_ICONERROR);
						break;
					}

					OPENFILENAME ofn;        // common dialog box structure
					WCHAR szFile[260];       // buffer for file name
					HANDLE fileHandle; // file handle

					// Initialize OPENFILENAME
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hWnd;
					ofn.lpstrFile = szFile;
					// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
					// use the contents of szFile to initialize itself.
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = _T("Spreadsheets (*.csv)\0*.csv\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
					ofn.lpstrDefExt = _T(".csv");

					// Display the Save dialog box. 
					if (GetSaveFileNameW(&ofn)) {
						fileHandle = CreateFile(ofn.lpstrFile,
							GENERIC_WRITE,
							0,
							(LPSECURITY_ATTRIBUTES)NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							(HANDLE)NULL);
						DWORD bytesWritten;
						WriteFile(fileHandle, outputContents.c_str(), outputContents.length(), &bytesWritten, NULL);
						CloseHandle(fileHandle);
					}
				}
				break;
			case IDM_TEMPLATE:
				{
					OPENFILENAME ofn;        // common dialog box structure
					WCHAR szFile[260];       // buffer for file name
					HANDLE fileHandle; // file handle

					// Initialize OPENFILENAME
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hWnd;
					ofn.lpstrFile = szFile;
					// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
					// use the contents of szFile to initialize itself.
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = _T("Spreadsheets (*.csv)\0*.csv\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
					ofn.lpstrDefExt = _T(".csv");

					// Display the Save dialog box. 
					if (GetSaveFileNameW(&ofn)) {
						fileHandle = CreateFile(ofn.lpstrFile,
							GENERIC_WRITE,
							0,
							(LPSECURITY_ATTRIBUTES)NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							(HANDLE)NULL);
						DWORD bytesWritten;
						std::string templateContents = templateText();
						WriteFile(fileHandle, templateContents.c_str(), templateContents.length(), &bytesWritten, NULL);
						CloseHandle(fileHandle);
					}
				}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}