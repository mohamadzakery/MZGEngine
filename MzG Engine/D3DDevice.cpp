#include "D3DDevice.h"


D3DDevice::D3DDevice(void)
{
	this->Device=NULL;
	window=NULL;
}


D3DDevice::~D3DDevice(void)
{
	delete window;
	delete Device;
}
bool D3DDevice::InitDevice(HINSTANCE instance,int width, int height,bool windowed,D3DDEVTYPE deviceType,LPCTSTR WindowCaption)
{
	window=new InitWindow(width,height,0,0,"MzG Engine",WindowCaption,instance,1,!windowed,true);
	bool bl= window->DoInit();
	if(!bl)
	{
		::MessageBox(0,"Faild To Create Window!!!","MzG Enigne",0);
		return false;
	}
	HRESULT hr = 0;
	IDirect3D9* d3d9 = 0;
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
    if( !d3d9 )
	{
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}
	// Step 2: Check for hardware vp.
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = window->MainWindowHandle;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
	// Step 4: Create the device.
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT,deviceType,window->MainWindowHandle,vp,&d3dpp,&this->Device);
	if( FAILED(hr) )
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT,deviceType,window->MainWindowHandle,vp,&d3dpp,&this->Device);
		if( FAILED(hr) )
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object
	return true;
}
void D3DDevice::Release()
{
	Device->Release();
	delete window;
}
InitWindow* D3DDevice::GetWindow()
{
	return window;
}
IDirect3DDevice9* D3DDevice::GetDevice()
{
	return this->Device;
}
