// ��������.cpp : ����Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "��������.h"
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
// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING]=TEXT("��������");			// ����������
static HWND hwndx,hwndy,hwndkey,hwndshow,hwndclear,hwndleft,hwndright,hwndBkey;
static int cXchar,cYchar;//�洢ϵͳĬ���ַ�����
static Imitate_kb_and_mou test;//�Ѱ��������װ������
void InitialWindow(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);
void InitialWindowSize(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);
typedef HHOOK(__stdcall * Hooktest)();
// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// ִ��Ӧ�ó����ʼ��:
	HMODULE hdll=LoadLibrary(L"HookTest.dll");//�������ǵ�dll��
	if(hdll)
	{
		Hooktest MyHooktest=(Hooktest)GetProcAddress(hdll,"HookKeyBoard");//�õ�����(�˴�Ϊdll�е�HookKeyBoard����)�ĵ�ַ�����õ������ĵ�ַ
		if(MyHooktest==0)//����ʧ��
		{
			MessageBox(NULL,L"����ʧ��",L"ERROR",MB_OK);
		}
	}
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));


	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd,hwndx;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
	case WM_CREATE://���createwiindowд��WM_PAINT��ȥ��

			InitialWindow(hWnd,message,wParam,lParam);
			//sprintf(strMessage,"�ɹ��� cxchar is %d,and cychar is %d",cXchar,cYchar);
			//MessageBoxA(hWnd,strMessage,"show",MB_OK);
			break;
	case WM_SIZE://������MoveWindow��Ż���ʾ
		 InitialWindowSize( hWnd, message, wParam, lParam);

	 case WM_KEYDOWN://���հ�����Ϣ
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
		// �����˵�ѡ��:
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
		/*case ID_BUT_SHOW:	����û����������������������δ��룬�����Ī�������
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
		// TODO: �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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

			hwndx=CreateWindow(TEXT("edit"),TEXT("x����"),WS_CHILD|WS_VISIBLE|WS_BORDER|
							ES_LEFT,0,0,0,0,hWnd,(HMENU) ID_EDIT_X,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndy=CreateWindow(TEXT("edit"),TEXT("y����"),WS_CHILD|WS_VISIBLE|WS_BORDER|
							ES_LEFT,0,0,0,0,hWnd,(HMENU) ID_EDIT_Y,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndkey=CreateWindow(TEXT("edit"),TEXT("����"),WS_CHILD|WS_VISIBLE|WS_BORDER|
							ES_LEFT,0,0,0,0,hWnd,(HMENU) ID_EDIT_KEY,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndshow=CreateWindow(TEXT("edit"),TEXT("��ʾָ��"),WS_CHILD|WS_VISIBLE|WS_BORDER|
							ES_LEFT,0,0,0,0,hWnd,(HMENU)ID_BUT_SHOW,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndclear=CreateWindow(TEXT("button"),TEXT("���ָ��"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON
							,0,0,0,0,hWnd,(HMENU)ID_BUT_CLEAR,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndleft=CreateWindow(TEXT("button"),TEXT("������"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON
							,0,0,0,0,hWnd,(HMENU)ID_BUT_LEFT,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			hwndright=CreateWindow(TEXT("button"),TEXT("����Ҽ�"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON
							,0,0,0,0,hWnd,(HMENU)ID_BUT_RIGHT,((LPCREATESTRUCT)lParam)->hInstance,NULL);

			hwndBkey=CreateWindow(TEXT("button"),TEXT("������Ϣ"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON
							,0,0,0,0,hWnd,(HMENU)ID_BUT_KEY,((LPCREATESTRUCT)lParam)->hInstance,NULL);
}

void InitialWindowSize(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam)
{
			MoveWindow(hwndx,cXchar,cYchar*2*0,20*cXchar,7*cYchar/4,TRUE);//Y������ѭ���п�����cYchar*2i����ʱ�����7/4*cYchar���Ͳ����ص���
			MoveWindow(hwndy,cXchar,cYchar*2*1,20*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndkey,cXchar,cYchar*2*2,20*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndshow,cXchar,cYchar*2*3,20*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndclear,cXchar,cYchar*2*4,8*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndleft,13*cXchar,cYchar*2*4,8*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndright,25*cXchar,cYchar*2*4,8*cXchar,7*cYchar/4,TRUE);
			MoveWindow(hwndBkey,37*cXchar,cYchar*2*4,8*cXchar,7*cYchar/4,TRUE);

}