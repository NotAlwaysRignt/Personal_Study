// 按键精灵.cpp : 定义应用程序的入口点。
#include "stdafx.h"
#include "按键精灵.h"
#include<stdio.h>
#include<windows.h>
#include"Imitate_kb_and_mou.h"
#define MAX_LOADSTRING 100

#define ID_EDIT_X 1
#define ID_EDIT_Y 2
#define ID_EDIT_KEY 3

#define ID_BUT_SHOW 100
#define ID_BUT_CLEAR 101
#define ID_BUT_LEFT 102
#define ID_BUT_RIGHT 103
#define ID_BUT_KEY 99
// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING]=TEXT("按键精灵");			// 主窗口类名
static HWND hwndx,hwndy,hwndkey,hwndshow,hwndclear,hwndleft,hwndright,hwndBkey;
static int cXchar,cYchar;//存储系统默认字符长宽
static Imitate_kb_and_mou test;//把按键精灵封装进类里
void InitialWindow(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);
void InitialWindowSize(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);
typedef HHOOK(__stdcall * Hooktest)();
// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// 执行应用程序初始化:
	HMODULE hdll=LoadLibrary(L"HookTest.dll");//加载我们的dll库
	if(hdll)
	{
		Hooktest MyHooktest=(Hooktest)GetProcAddress(hdll,"HookKeyBoard");//拿到进程(此处为dll中的HookKeyBoard函数)的地址，即拿到函数的地址
		if(MyHooktest==0)//加载失败
		{
			MessageBox(NULL,L"加载失败",L"ERROR",MB_OK);
		}
	}
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));


	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd,hwndx;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	TCHAR strMessage[8];
	for (int i=1;i<8;i++)
		strMessage[i]='/0';
	switch (message)
	{
	case WM_CREATE://别把createwiindow写到WM_PAINT里去了

			InitialWindow(hWnd,message,wParam,lParam);
			//sprintf(strMessage,"成功， cxchar is %d,and cychar is %d",cXchar,cYchar);
			//MessageBoxA(hWnd,strMessage,"show",MB_OK);
			break;
	case WM_SIZE://调用了MoveWindow后才会显示
		 InitialWindowSize( hWnd, message, wParam, lParam);

	 case WM_KEYDOWN://接收按键消息
		 switch(wParam)
		{
		 case VK_F11:
			 test.Lmouse_record();
			 test.show_record();
			 break;
		 case VK_F12:
			 test.Rmouse_record();
			 test.show_record();
			 break;
		 case VK_F1:test.clear_record();
			 //test.show_record();
			 break;
		 case VK_F2:test.Info_indicate();
			 break;
		 default:break;
		 }	
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUT_CLEAR:
			test.clear_record();
			 test.show_record();
			break;
		/*case ID_BUT_SHOW:	我们没有添加这个按键，如果添加这段代码，程序会莫名其妙报错
			test.show_record();
			break;*/
		case  ID_BUT_LEFT :
			Sleep(3000);
			test.Lmouse_record();
			 test.show_record();
			break;
		case ID_BUT_RIGHT:
			Sleep(3000);
			test.Lmouse_record();
			test.show_record();
			break;
		 case ID_BUT_KEY:
			 test.key_record();
			 test.show_record();
			 break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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

void InitialWindow(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam)
{
			cXchar=LOWORD(GetDialogBaseUnits());
			cYchar=HIWORD(GetDialogBaseUnits());

			hwndx=CreateWindow(TEXT("edit"),TEXT("x坐标"),WS_CHILD|WS_VISIBLE|WS_BORDER|
							ES_LEFT,0,0,0,0,hWnd,(HMENU) ID_EDIT_X,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndy=CreateWindow(TEXT("edit"),TEXT("y坐标"),WS_CHILD|WS_VISIBLE|WS_BORDER|
							ES_LEFT,0,0,0,0,hWnd,(HMENU) ID_EDIT_Y,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndkey=CreateWindow(TEXT("edit"),TEXT("按键"),WS_CHILD|WS_VISIBLE|WS_BORDER|
							ES_LEFT,0,0,0,0,hWnd,(HMENU) ID_EDIT_KEY,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndshow=CreateWindow(TEXT("edit"),TEXT("显示指令"),WS_CHILD|WS_VISIBLE|WS_BORDER|
							ES_LEFT,0,0,0,0,hWnd,(HMENU)ID_BUT_SHOW,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndclear=CreateWindow(TEXT("button"),TEXT("清除指令"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON
							,0,0,0,0,hWnd,(HMENU)ID_BUT_CLEAR,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndleft=CreateWindow(TEXT("button"),TEXT("点击左键"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON
							,0,0,0,0,hWnd,(HMENU)ID_BUT_LEFT,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			hwndright=CreateWindow(TEXT("button"),TEXT("点击右键"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON
							,0,0,0,0,hWnd,(HMENU)ID_BUT_RIGHT,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndBkey=CreateWindow(TEXT("button"),TEXT("键盘消息"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON
							,0,0,0,0,hWnd,(HMENU)ID_BUT_KEY,((LPCREATESTRUCT)lParam)->hInstance,NULL);
}

void InitialWindowSize(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam)
{
			MoveWindow(hwndx,cXchar,cYchar*2*0,20*cXchar,7*cYchar/4,TRUE);//Y坐标在循环中可以用cYchar*2i，此时会大于7/4*cYchar，就不会重叠了
			MoveWindow(hwndy,cXchar,cYchar*2*1,20*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndkey,cXchar,cYchar*2*2,20*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndshow,cXchar,cYchar*2*3,20*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndclear,cXchar,cYchar*2*4,8*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndleft,13*cXchar,cYchar*2*4,8*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndright,25*cXchar,cYchar*2*4,8*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndBkey,37*cXchar,cYchar*2*4,8*cXchar,7*cYchar/4,TRUE);

}