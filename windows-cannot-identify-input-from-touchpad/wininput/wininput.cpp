
#include "stdafx.h"

#include <sstream>
#include <string>
#include "wininput.h"

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                      // current instance
WCHAR szTitle[MAX_LOADSTRING];        // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];  // the main window class name

HWND output;

void AppendText(wstringstream& wss) {
  wss << endl;
  DWORD StartPos, EndPos;
  SendMessage(output, EM_GETSEL, reinterpret_cast<WPARAM>(&StartPos),
              reinterpret_cast<WPARAM>(&EndPos));
  int outLength = GetWindowTextLength(output);
  SendMessage(output, EM_SETSEL, outLength, outLength);
  SendMessage(output, EM_REPLACESEL, TRUE,
              reinterpret_cast<LPARAM>(wss.str().c_str()));
  SendMessage(output, WM_VSCROLL, SB_BOTTOM, 0);
}

void AppendText(wstring ws) {
  wstringstream wss;
  wss << ws;
  AppendText(wss);
}

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_WININPUT, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable =
      LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WININPUT));

  MSG msg;

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WININPUT));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WININPUT);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;  // Store instance handle in our global variable

  HWND hWnd =
      CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd) {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  EnableMouseInPointer(true);

  return TRUE;
}

void LogPointerEvent(UINT32 pointer_id, wstring name) {
  POINTER_INFO pt_info;
  
  bool success = GetPointerInfo(pointer_id, &pt_info);

  POINTER_DEVICE_INFO device;
  GetPointerDevice(pt_info.sourceDevice, &device); // Get Error Code 6 Here.

  if (!success) {
    return;
  }

  wstringstream wss;
  wss << L"Event: " << name << " id:" << pointer_id << " device: " << pt_info.sourceDevice
      << ", type=";
  switch (pt_info.pointerType) {
  case PT_POINTER:
    wss << "PT_POINTER";
    break;
  case PT_TOUCH:
    wss << "PT_TOUCH";
    break;
  case PT_PEN:
    wss << "PT_PEN";
    break;
  case PT_MOUSE:
    wss << "PT_MOUSE";
    break;
  case PT_TOUCHPAD:
    wss << "PT_TOUCHPAD";
    break;
  default:
    break;
  }
  AppendText(wss);
}

LRESULT CALLBACK WndProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam) {
  UINT32 pointer_id;
  switch (message) {
    case WM_POINTERUPDATE:
      pointer_id = GET_POINTERID_WPARAM(wParam);
      LogPointerEvent(pointer_id, L"WM_POINTERUPDATE");
      break;
    case WM_POINTERWHEEL:
      pointer_id = GET_POINTERID_WPARAM(wParam);
      LogPointerEvent(pointer_id, L"WM_POINTERWHEEL");
      break;
    case WM_POINTERHWHEEL:
      pointer_id = GET_POINTERID_WPARAM(wParam);
      LogPointerEvent(pointer_id, L"WM_POINTERHWHEEL");
      break;
	  case WM_POINTERDOWN:
      pointer_id = GET_POINTERID_WPARAM(wParam);
      LogPointerEvent(pointer_id, L"WM_POINTERDOWN");
      break;
    case WM_POINTERACTIVATE:
      pointer_id = GET_POINTERID_WPARAM(wParam);
      LogPointerEvent(pointer_id, L"WM_POINTERACTIVATE");
      break;
    case WM_CREATE: {
      output = CreateWindow(L"EDIT", NULL,
                            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
                                ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
                            5, 5, 500, 500, hWnd, NULL, NULL, NULL);
	  break;
    }
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      // Parse the menu selections:
      switch (wmId) {
        case IDM_ABOUT:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
          break;
        case IDM_EXIT:
          DestroyWindow(hWnd);
          break;
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
      }
    } break;
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);

      EndPaint(hWnd, &ps);
    } break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
    case WM_INITDIALOG:
      return (INT_PTR)TRUE;

    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}
