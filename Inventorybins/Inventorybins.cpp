// Inventorybins.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Inventorybins.h"
#include "Windows.h"
#include "Windowsx.h"
#include "Commctrl.h"
#include "Winuser.h"
#include <string>
#define MAX_LOADSTRING 100
// Global var bin
bin *binptr;
wchar_t spaces[] = L"          "; // 10 spaces
// Global Variables:
HWND hWnd;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

RECT rect;
RECT Rbinselect;
RECT Rbinselectinput;
RECT RAddparts;
RECT RAddpartsin;
RECT RRemoveparts;
RECT RRemovepartsin;
RECT Rbinout;
RECT Rshowresults;

HWND binselect;
HWND binselectinput;
HWND Addparts;
HWND Addpartsin;
HWND Removeparts;
HWND Removepartsin;
HWND binout;
HWND showresults;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void displayout()
{
	wchar_t *showresultbuf = new wchar_t[10 * 64];
	for (int i = 0;i < 10; i++)
	{

		wchar_t wbinnumconv[10];
		char binnumconv[10];
		_itoa(binptr[i].Mbinnum, binnumconv, 10);
		mbstowcs(wbinnumconv, binnumconv, 10);
		if (i == 0)
			wcscpy(showresultbuf, wbinnumconv);
		else
			wcscat(showresultbuf, wbinnumconv);
		ZeroMemory(wbinnumconv, 10);
		ZeroMemory(binnumconv, 10);
		wcscat(showresultbuf, spaces);
		wcscat(showresultbuf, binptr[i].Mpartdesc);
		wcscat(showresultbuf, spaces);
		_itoa(binptr[i].Mnumofparts, binnumconv, 10);
		mbstowcs(wbinnumconv, binnumconv, 10);
		wcscat(showresultbuf,wbinnumconv);
		wcscat(showresultbuf,L"\r\n");

		
	}
	Edit_SetText(binout, showresultbuf);
}
void drawchildren()
{
	GetClientRect(hWnd, &rect);
	int width = 140;
	int height = 20;
	int offset = 20;
	binselect = CreateWindowW(L"EDIT", NULL, WS_CHILD | WS_OVERLAPPED | ES_READONLY | WS_VISIBLE | ES_CENTER, (rect.right - rect.left) / 2 - width / 2, rect.top + offset, width, height, hWnd, NULL, NULL, NULL);
	wchar_t binsel[] = { 'B','i','n',' ','S','e','l','e','c','t','i','o','n',' ','1','-','1','0','\0' };
	Edit_NoSetFocus(binselect);
	Edit_SetText(binselect, binsel);
	offset += 20;
	binselectinput = CreateWindowW(L"EDIT", NULL, WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_BORDER | ES_CENTER, (rect.right - rect.left) / 2 - width / 2, rect.top + offset, width, height, hWnd, NULL, NULL, NULL);
	offset += 20;
	showresults = CreateWindowW(L"BUTTON", NULL, BS_PUSHBUTTON | BS_CENTER | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_BORDER, ((rect.right - rect.left) / 2 - width / 2) + 140 * 2, rect.top + offset, 140, 40, hWnd, NULL, NULL, NULL);
	wchar_t buttext[] = { 'S','h','o','w',' ','R','e','s','u','l','t','s','\0' };
	Button_SetText(showresults, buttext);
	Addparts = CreateWindowW(L"EDIT", NULL, WS_CHILD | WS_OVERLAPPED | ES_READONLY | WS_VISIBLE | ES_CENTER, (rect.right - rect.left) / 2 - width / 2, rect.top + offset, width, height, hWnd, NULL, NULL, NULL);
	offset += 20;
	wchar_t add[] = { 'A','d','d',' ','P','a','r','t','s','\0' };
	Edit_NoSetFocus(Addparts);
	Edit_SetText(Addparts, add);
	Addpartsin = CreateWindowW(L"EDIT", NULL, WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_BORDER | ES_CENTER, (rect.right - rect.left) / 2 - width / 2, rect.top + offset, width, height, hWnd, NULL, NULL, NULL);
	offset += 20;
	Removeparts = CreateWindowW(L"EDIT", NULL, WS_CHILD | WS_OVERLAPPED | ES_READONLY | WS_VISIBLE | ES_CENTER, (rect.right - rect.left) / 2 - width / 2, rect.top + offset, width, height, hWnd, NULL, NULL, NULL);
	offset += 20;
	wchar_t remove[] = { 'R','e','m','o','v','e',' ','P','a','r','t','s','\0' };
	Edit_NoSetFocus(Removeparts);
	Edit_SetText(Removeparts, remove);
	Removepartsin = CreateWindowW(L"EDIT", NULL, WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_BORDER | ES_CENTER, (rect.right - rect.left) / 2 - width / 2, rect.top + offset, width, height, hWnd, NULL, NULL, NULL);
	offset += 20;
	binout = CreateWindowW(L"EDIT", NULL, WS_CHILD | WS_OVERLAPPED | ES_READONLY | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_MULTILINE |ES_WANTRETURN, rect.left, rect.top + offset, rect.right - rect.left, rect.bottom - offset, hWnd, NULL, NULL, NULL);
	offset = 0;
}

void getresults()
{
	wchar_t binnumbuf[3];
	wchar_t addbuf[3];
	wchar_t rembuf[3];
	ZeroMemory(binnumbuf, 3);
	ZeroMemory(addbuf, 3);
	ZeroMemory(rembuf, 3);
	Edit_GetText(binselectinput, binnumbuf, 3);
	Edit_GetText(Addpartsin, addbuf, 3);
	Edit_GetText(Removepartsin, rembuf, 3);
	bool addbufValid = lstrlen(addbuf) > 0;
	bool rembufValid = lstrlen(rembuf) > 0;
	bool inputvalid = lstrlen(binnumbuf) > 0 && (addbufValid || rembufValid);

	if (inputvalid)
	{
		for (int i = 0;i < lstrlen(binnumbuf);i++)
		{
			if (!isdigit(binnumbuf[i]))
			{
				inputvalid = false;
				break;
			}
		}
	}

	if (inputvalid)
	{
		for (int i = 0;i < lstrlen(addbuf);i++)
		{
			if (!isdigit(addbuf[i]))
			{
				inputvalid = false;
				break;
			}
		}
	}

	if (inputvalid)
	{
		for (int i = 0;i < lstrlen(rembuf);i++)
		{
			if (!isdigit(rembuf[i]))
			{
				inputvalid = false;
				break;
			}
		}
	}


	if (inputvalid)
	{
		int Ibinnum = std::stoi(binnumbuf) - 1;
		if (Ibinnum >= 0 && Ibinnum < 10)
		{
			int Iaddbuf = addbufValid ? std::stoi(addbuf) : 0;
			int Irembuf = rembufValid ? std::stoi(rembuf) : 0;

			binptr[Ibinnum].addParts(Iaddbuf);
			binptr[Ibinnum].removeParts(Irembuf);

			displayout();
		}
		
	}
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	wchar_t *desc[] = { L"Valve",L"Bearing",L"Bushing",L"Coupling",
		L"Flange",L"Gear",L"Gear Housing",L"Vacuum Gripper",L"Cable",
		L"Rod" };
	int numofparts[] = { 10,5,15,21,7,5,5,25,18,12 };
	bin binobj[10];
	binptr = binobj;
	for (int i = 0, j = 1; i< 10; i++,j++)
	{
		ZeroMemory(binptr[i].Mpartdesc, 50);
		binptr[i].Mbinnum = j;
		wcscpy(binptr[i].Mpartdesc, desc[i]);
		binptr[i].Mnumofparts = numofparts[i];
	}
	

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_INVENTORYBINS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	//display binout
	displayout();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INVENTORYBINS));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INVENTORYBINS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_INVENTORYBINS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   drawchildren();
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
//  PURPOSE:  Processes messages for the main window.
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
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case BN_CLICKED:
			{
				if ((HWND)lParam == showresults)
				{
					getresults();
				}
				break;
			}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	
	case WM_SIZE:
		{
			int width = 140;
			int height = 20;
			int offset = 20;
			MoveWindow(binselect, LOWORD(lParam) / 2 - width / 2, offset, width, height, true);
			offset += 20;
			MoveWindow(binselectinput, LOWORD(lParam) / 2 - width / 2, offset, width, height, true);
			offset += 20;
			MoveWindow(showresults, (LOWORD(lParam) / 2 - width / 2) + width * 2, offset, width, 40, true);
			MoveWindow(Addparts, LOWORD(lParam) / 2 - width / 2, offset, width, height, true);
			offset += 20;
			MoveWindow(Addpartsin, LOWORD(lParam) / 2 - width / 2, offset, width, height, true);
			offset += 20;
			MoveWindow(Removeparts, LOWORD(lParam) / 2 - width / 2, offset, width, height, true);
			offset += 20;
			MoveWindow(Removepartsin, LOWORD(lParam) / 2 - width / 2, offset, width, height, true);
			offset += 20;
			MoveWindow(binout,0, offset, LOWORD(lParam), HIWORD(lParam) - offset, true);
			offset = 0;
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
