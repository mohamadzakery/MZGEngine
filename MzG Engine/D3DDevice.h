#pragma once
#include <d3dx9.h>
#include "InitWindow.h"
class D3DDevice
{
private:
	
	InitWindow* window;
	IDirect3DDevice9* Device;
public:
	D3DDevice(void);
	~D3DDevice(void);
	bool InitDevice(HINSTANCE instance,int width, int height,bool windowed,D3DDEVTYPE deviceType,LPCTSTR WindowCaption);
	IDirect3DDevice9* GetDevice();
	void Release();
	InitWindow* GetWindow();
};

