// desktopWizardTest.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "desktopWizardTest.h"

// -------------- ownlibs including --------------
#include "drawing.h"
#include "winsockWrp.h"


// -------------- win32api --------------
#include<stdio.h>
#include <locale.h>
// button id definition
#define SEND_BUTTON_ID 1
#define OPEN_BUTTON_ID 2
#define SAVE_BUTTON_ID 3
#define EDIT_ID 10

void doingFileStuff(LPWSTR str);


#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
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

    // TODO: ここにコードを挿入してください。

    // --------------------- GDIpFuncs ---------------------
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // --------------------- wsockFuncs ---------------------
    //SOCKET s;
    //initWsock(s);


    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DESKTOPWIZARDTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DESKTOPWIZARDTEST));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Finalize GDI+.
    GdiplusShutdown(gdiplusToken);
    return (int) msg.wParam;
} // WinMain



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DESKTOPWIZARDTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DESKTOPWIZARDTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
bool isPressing = false;
int timerCnt = 0;
SOCKET s;
char buf[32];
unsigned int    threadID;
rcvDataStr		myrcvData;
HANDLE rcvFncHd = 0;
static HWND edit;
LPWSTR strText;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    setlocale(LC_ALL, "ja_JP");

    switch (message)
    {
    case WM_CREATE:
        // --------------------- wsockFuncs ---------------------
        printWsockStatus(hWnd, initWsock(s));
        memset(buf, 0, sizeof(buf));
        rcvStopFlag = FALSE;
        myrcvData = { s, buf, sizeof(buf) };
        rcvFncHd = (HANDLE)_beginthreadex(NULL, 0, rcvFunc, &myrcvData, 0, &threadID);


        // --------------------- win32apiFuncs ---------------------
        SetTimer(hWnd, 1, 10, NULL);
        CreateWindow(
            L"BUTTON",
            L"send",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 210, 100, 25,
            hWnd,
            (HMENU)SEND_BUTTON_ID,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        CreateWindow(
            L"BUTTON",
            L"open",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 240, 100, 25,
            hWnd,
            (HMENU)OPEN_BUTTON_ID,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        CreateWindow(
            L"BUTTON",
            L"save",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            110, 240, 100, 25,
            hWnd,
            (HMENU)SAVE_BUTTON_ID,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);

        edit = CreateWindow(
            TEXT("EDIT"), NULL,
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | ES_MULTILINE,
            0, 0, 400, 200, hWnd, (HMENU)EDIT_ID,
            ((LPCREATESTRUCT)(lParam))->hInstance, NULL
        );
        break;
    case WM_LBUTTONDOWN:
        {
            isPressing = true;
            CHAR chStr[128];
            wsprintf((LPWSTR)chStr, L"clicked on: %d, %d", LOWORD(lParam), HIWORD(lParam));
        }
    break;
    case WM_LBUTTONUP:
        {
            isPressing = false;
        }
    break;
    case WM_MOUSEMOVE:
        {
            PAINTSTRUCT ps;
            HDC hdc = GetDC(hWnd);
            CHAR chStr[128];
            wsprintf((LPWSTR)chStr, L"mouse at: %d, %d, \npressing: %d", LOWORD(lParam), HIWORD(lParam), isPressing);
            TextOut(hdc, 10, 430, (LPWSTR)chStr, sizeof(chStr));

            if (isPressing) {
                drawDot(hdc, LOWORD(lParam), HIWORD(lParam));
            }
            ReleaseDC(hWnd, hdc);
        }
    break;
    case WM_TIMER:
    {
        PAINTSTRUCT ps;
        HDC hdc = GetDC(hWnd);
        CHAR chStr[128];
        
        //OnPaint(hdc, GetRandom(10, 1000), GetRandom(10, 1000), GetRandom(10, 1000), GetRandom(10, 1000));
        
        wsprintf((LPWSTR)chStr, L"timerCnt: %d", timerCnt++);
        TextOut(hdc, 10, 270, (LPWSTR)chStr, sizeof(chStr));
        
        //(void)wcstombs_s(NULL, (wchar_t*)chStr, (size_t)sizeof(buf), (const char*)buf, 32);
        (void)mbstowcs_s(NULL, (wchar_t*)chStr, (size_t)sizeof(buf), (const char*)buf, 32);

        TextOut(hdc, 10, 290, (LPWSTR)chStr, sizeof(buf));

        ReleaseDC(hWnd, hdc);
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case SEND_BUTTON_ID:
                send(s, "hoge", sizeof("hoge"), 0);
                break;
            case SAVE_BUTTON_ID:
                strText = (LPWSTR)malloc(32767);
                GetWindowText(edit, strText, 32767);
                doingFileStuff(strText);
                free(strText);

                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            //OnPaint(hdc, 10,100, 200,300); 
            TextOut(hdc, 10, 200, L"hello", strlen("hello"));

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        closesocket(s);
        WSACleanup();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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


void doingFileStuff(LPWSTR str) {
    char string[100];
    FILE* sfile;
    _wfopen_s(&sfile, L"./test.txt", L"a");
    if (sfile == NULL) {
        fprintf(stderr, "cannot open %s\n", string);
        exit(1);
    }
    //fprintf(sfile, L"%s\n", str);
    fwprintf(sfile, L"%s\n", str);
    fclose(sfile);
}