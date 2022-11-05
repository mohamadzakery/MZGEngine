#include "InitWindow.h"
#include <windows.h>

InitWindow::~InitWindow(void)
{
}
InitWindow::InitWindow(int _Width,int _Height,int _Top,int _Left,LPCSTR _WindowClassName,LPCSTR _WindowTitle,HINSTANCE _Handle,int _show,bool _FullScreenSet,bool _CenterScreenDrawed)
{
	this->Width=_Width;
	this->Height=_Height;
	this->Top=_Top;
	this->Left=_Left;
	this->WindowClassName=_WindowClassName;
	this->WindowTitle=_WindowTitle;
	this->Handle=_Handle;
	this->show=_show;
	this->FullScreenSet=_FullScreenSet;
	this->MainWindowHandle=0;
	this->CenterScreenDrawed=_CenterScreenDrawed;
	::ZeroMemory(&msg, sizeof(MSG));
}
bool InitWindow::DoInit(void)
{
	WNDCLASS wc; 
	DEVMODE dmScreenSettings;
	int posX, posY;
	ApplicationWindowInit=this;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = this->Handle;
	wc.hIcon         = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = 
	static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName  = 0;
	wc.lpszClassName = this->WindowClassName;


	if(!::RegisterClass(&wc)) 
	{
		::MessageBox(0, "RegisterClass - Failed", 0, 0);
		return false;
	}
	///////////////////////////////
	int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	////
	if(this->FullScreenSet)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		this->Top=0;this->Left=0;
	    this->Width=screenWidth;
	    this->Height=screenHeight;
		//////////////////////////////////Create FullScreen
			this->MainWindowHandle= ::CreateWindow(this->WindowClassName,this->WindowTitle,WS_POPUP|WS_VISIBLE|WS_SYSMENU,0, 0,this->Width,this->Height,0, 0, this->Handle,0);
			//SetWindowLong(this->MainWindowHandle, GWL_STYLE, 0); 
		//////////////////////////////////
	}
	else
	{
	if(CenterScreenDrawed)
	{
		this->CalculateCenterClientPosition(this->Top,this->Left);
		if(this->Top<0)
			this->Top=0;
		if(this->Left<0)
			this->Left=0;
	}
		//////////////////////////////////Create NormalScreen
			this->MainWindowHandle= ::CreateWindow(this->WindowClassName,this->WindowTitle,WS_OVERLAPPED  | WS_SYSMENU,this->Left, this->Top,this->Width,this->Height,0, 0, this->Handle,0);
		//////////////////////////////////
	}
	///////////////////////////////

	if(MainWindowHandle == 0)
	{
		::MessageBox(0, "CreateWindow - Failed", 0, 0);
		return false;
	}
	::ShowWindow(MainWindowHandle, show);
	::UpdateWindow(MainWindowHandle);

	return true;
}
void InitWindow::ReturnClientScreenSize(int &With,int &height)
{
	With = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
}
void InitWindow::CalculateCenterClientPosition(int &CenterTop,int &CenterLeft)
{
	int cszW=0,cszH=0;
	InitWindow::ReturnClientScreenSize(cszW,cszH);
	CenterTop=(cszH-(this->Height))/2;
	CenterLeft=(cszW-(this->Width))/2;
}
int InitWindow::GetClientScreenWidth()
{
	return GetSystemMetrics(SM_CXSCREEN);
}
int InitWindow::GetClientScreenHeight()
{
	return GetSystemMetrics(SM_CYSCREEN);
}
LRESULT CALLBACK InitWindow::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umessage, wparam, lparam);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}
		//case WM_KEYDOWN:
		//if( wparam == VK_ESCAPE )
		//	DestroyWindow(hwnd);

		// All other messages pass to the message handler in the system class.
		default:
		{
			
				return ApplicationWindowInit->MessageHandler(hwnd,umessage,wparam,lparam);
		}
	}
}
bool InitWindow::GetMessageEnded()
{
	    if(PeekMessage(&msg,NULL,0, 0, PM_REMOVE))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
		if(msg.message == WM_QUIT)
		{
			return true;
		}
		return false;
}
