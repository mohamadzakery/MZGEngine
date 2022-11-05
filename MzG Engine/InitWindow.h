#pragma once
#include <windows.h>

class InitWindow
{
	public:
	int Width;int Height;int Top;int Left;LPCSTR WindowClassName;LPCSTR WindowTitle;HINSTANCE Handle;int show;bool FullScreenSet;bool CenterScreenDrawed;
	bool DoInit(void);
	HWND MainWindowHandle;
	InitWindow(int Width,int Height,int Top,int Left,LPCSTR WindowClassName,LPCSTR WindowTitle,HINSTANCE Handle,int show,bool FUllScreenSet,bool CenterScreenDrawed);
	~InitWindow(void);
	void ReturnClientScreenSize(int &With,int &height);
	void CalculateCenterClientPosition(int &CenterTop,int &CenterLeft);
	int GetClientScreenWidth();
	int GetClientScreenHeight();
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
	bool GetMessageEnded();
	MSG msg;
};
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
static InitWindow *ApplicationWindowInit=0;



