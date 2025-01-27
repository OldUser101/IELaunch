#include "IELaunch.h"
#include <windows.h>
#include <strsafe.h>
#include <exdisp.h>
#include <mshtmhst.h>

#define MAX_LOADSTRING 100
#define WM_START_IE 1001
#define DEFAULT_NAV_STR L"about:home"
#define IEFRAME_RES_ICON 190

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
IWebBrowser2* pWebBrowser = NULL;
INT call = 0;
HMODULE hIeFrame;
HICON ieIcon;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance) noexcept;
BOOL                InitInstance(HINSTANCE, int) noexcept;
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT             StartIE();
HRESULT             InitAX() noexcept;
void                ShowErrorMessage(UINT, UINT);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ const HINSTANCE hPrevInstance,
                     _In_ const LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HMODULE hIeFrame = LoadLibrary(L"ieframe.dll");

    if (!hIeFrame)
    {
        ShowErrorMessage(IDC_ERR_MODLOAD, GetLastError());
        return 1;
    }

    ieIcon = LoadIcon(hIeFrame, MAKEINTRESOURCE(IEFRAME_RES_ICON));
    if (!ieIcon) 
    {
        ShowErrorMessage(IDC_ERR_ICONLOAD, GetLastError());
        return 1;
    }

    InitAX();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, &szTitle[0], MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IELAUNCH, &szWindowClass[0], MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IELAUNCH));

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

    DestroyIcon(ieIcon);
    FreeLibrary(hIeFrame);
    CoUninitialize();
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance) noexcept
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = ieIcon;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = GetSysColorBrush(COLOR_BTNFACE);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = &szWindowClass[0];
    wcex.hIconSm        = ieIcon;

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) noexcept
{
   hInst = hInstance; // Store instance handle in our global variable


   INT screenX = (GetSystemMetrics(SM_CXSCREEN) - 300) / 2;
   INT screenY = (GetSystemMetrics(SM_CYSCREEN) - 50) / 2;

   HWND hWnd = CreateWindowW(&szWindowClass[0], &szTitle[0], WS_POPUP,
       screenX, screenY, 300, 50, nullptr, nullptr, hInstance, nullptr);

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
    case WM_START_IE:
        StartIE();
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        PostMessage(hWnd, WM_START_IE, NULL, NULL);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            DrawIcon(hdc, 10, 10, ieIcon);

            RECT rect{};
            rect.left = 60;
            rect.right = 300;
            rect.bottom = 300;
            rect.top = 18;
            SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));

            WCHAR dispStr[MAX_LOADSTRING];
            LoadString(hInst, IDC_DISPTEXT, dispStr, MAX_LOADSTRING);

            DrawText(hdc, dispStr, -1, &rect, DT_LEFT);

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

HRESULT InitAX() noexcept
{
    HRESULT hr = CoInitialize(NULL);  // Initialize COM
    if (FAILED(hr)) {
        ShowErrorMessage(IDC_ERR_COM_INIT, hr);
        return hr;
    }
    
    hr = CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_ALL, IID_IWebBrowser2, reinterpret_cast<void**>(&pWebBrowser));  // Create a web browser instance
    if (FAILED(hr)) {
        ShowErrorMessage(IDC_ERR_INSTANCE, hr);
        CoUninitialize();
        return hr;
    }
    return 0;
}

HRESULT StartIE()
{
    VARIANT vtFlags, vtTargetFrameName, vtPostData, vtHeaders; 
    VariantInit(&vtFlags); 
    VariantInit(&vtTargetFrameName); 
    VariantInit(&vtPostData); 
    VariantInit(&vtHeaders); 

    vtFlags.vt = VT_I4; 
    vtFlags.lVal = navOpenInNewWindow; 

    BSTR url = SysAllocString(DEFAULT_NAV_STR); 

    const HRESULT hr = pWebBrowser->Navigate(url, &vtFlags, &vtTargetFrameName, &vtPostData, &vtHeaders);
    if (FAILED(hr)) 
    { 
        ShowErrorMessage(IDC_ERR_NAV, hr);
    }

    SysFreeString(url); 

    pWebBrowser->Quit();

    VariantClear(&vtFlags); 
    VariantClear(&vtTargetFrameName); 
    VariantClear(&vtPostData); 
    VariantClear(&vtHeaders);
    return 0;
}

void ShowErrorMessage(UINT uID) 
{
    WCHAR title[MAX_LOADSTRING];
    WCHAR errorMsg[MAX_LOADSTRING];

    LoadString(hInst, IDC_ERROR, title, MAX_LOADSTRING);
    LoadString(hInst, uID, errorMsg, MAX_LOADSTRING);

    MessageBox(NULL, errorMsg, title, MB_ICONERROR);
}

void ShowErrorMessage(UINT uID, UINT uError)
{
    WCHAR title[MAX_LOADSTRING];
    WCHAR errorFmt[MAX_LOADSTRING];
    WCHAR errorStr[MAX_LOADSTRING];

    LoadString(hInst, IDC_ERROR, title, MAX_LOADSTRING);
    LoadString(hInst, uID, errorFmt, MAX_LOADSTRING);

    StringCchPrintf(errorStr, MAX_LOADSTRING, errorFmt, uError);

    MessageBox(NULL, errorStr, title, MB_ICONERROR);
}