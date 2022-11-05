
#include"windows.h"
#include "InitWindow.h"
#include "InputHandle.h"
#include "D3DDevice.h"
//#include "FBXLoader.h"
#include"Font2d.h"
#include"Timer.h"
#include <string>
#include <sstream>
#include"FbxLoader.h"
#include"Camera.h"
#include "Light.h"
#include "StaticXfileLoader.h"
#include"SkyBoxBasic.h"

InitWindow* window;
InputHandle* m_Input;
D3DDevice* device;
D3DXMATRIX PrespectiveProj;
FbxLoader *loader;
Camera *camera;
StaticXfileLoader *Xloader;
SkyBoxBasic *SkyBox;
//FBXLoader* fbx;
Font2d* f;
CTimer *timer;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
////////////////////////
char* str=new char[1];
char* Boxing(float f)
{
//float f = 1.0F;
sprintf(str, "%f", f);
return str;
}
////////////////////
void CreateSunLight()
{
      // Fill in a light structure defining our light
      D3DLIGHT9 light;
      ZeroMemory( &light, sizeof(D3DLIGHT9) );
      light.Type       = D3DLIGHT_DIRECTIONAL;
      light.Diffuse.r  = 1.0f;
      light.Diffuse.g  = 1.0f;
      light.Diffuse.b  = 1.0f;
      light.Diffuse.a  = 1.0f;
      light.Range      = 1000.0f;
      // Create a direction for our light - it must be normalized 
      D3DXVECTOR3 vecDir;
      vecDir = D3DXVECTOR3(0.0f,-0.3f,0.5);
      D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
      // Tell the device about the light and turn it on
      device->GetDevice()->SetLight( 0, &light );
      device->GetDevice()->LightEnable( 0, TRUE );

}
void CreatePointLight(const D3DVECTOR &pos)
{
      // Fill in a light structure defining our light
      D3DLIGHT9 light;
      ZeroMemory( &light, sizeof(D3DLIGHT9) );
      light.Type       = D3DLIGHT_POINT;
      light.Diffuse.r  = 1.0f;
      light.Diffuse.g  = 1.0f;
      light.Diffuse.b  = 1.0f;
      // Point lights have no direction but do have a position
      light.Position = pos;
      // Tell the device about the light and turn it on
      light.Attenuation0 = 0.1f;
      light.Range=200.0f;
      device->GetDevice()->SetLight( 1, &light );
      device->GetDevice()->LightEnable( 1, TRUE );

}
void CreateSpotLight(const D3DVECTOR &pos,const D3DVECTOR &dir)
{

      // Fill in a light structure defining our light
      D3DLIGHT9 light;
      ZeroMemory( &light, sizeof(D3DLIGHT9) );
      light.Type= D3DLIGHT_SPOT;
      light.Diffuse.r  = 1.0f;
      light.Diffuse.g  = 1.0f;
      light.Diffuse.b  = 1.0f;
	  light.Diffuse.a  = 1.0f;
      // Spot lights have direction and a position
      light.Position = pos;
      light.Direction = dir ; 
      // Tell the device about the light and turn it on
      light.Range=1.0f;
      light.Theta=0.5f;
      light.Phi=1.0f;
      light.Falloff=1.0f;
      light.Attenuation0= 1.0f;
      device->GetDevice()->SetLight( 1, &light );
      device->GetDevice()->LightEnable( 1, TRUE );

}
////////////////////
bool Frame()
{
	bool result;
	int mouseX, mouseY;


	// Do the input frame processing.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}

	// Get the location of the mouse from the input object,
	//m_Input->GetMouseLocation(mouseX, mouseY);

	// Do the frame processing for the graphics object.
	//result = m_Graphics->Frame(mouseX, mouseY);
	//if(!result)
	//{
	//	return false;
	//}

	// Finally render the graphics to the screen.
	//result = m_Graphics->Render();
	//if(!result)
	//{
	//	return false;
	//}
	timer->Update();
	return true;
}
void render( void )
{
    device->GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_RGBA(1,1,1,0), 1.0f, 0 );
    device->GetDevice()->BeginScene();

	
	//loader->Render();
	SkyBox->Draw();
	Xloader->Draw();
	//loader->Render();
	f->UpdateText(0,Boxing(timer->GetFPS()));
	f->Render();
	

	device->GetDevice()->EndScene();
	device->GetDevice()->Present( NULL, NULL, NULL, NULL );
}
void Display(float timeDelta,bool &done)
{
	if(m_Input->IsEscapePressed() == true)
	{
		done = true;
	}
	if(m_Input->IsKeyPressed(DIK_LEFTARROW))
		camera->yaw(-2.0f * timeDelta);
	if(m_Input->IsKeyPressed(DIK_RIGHTARROW))
		camera->yaw(2.0f * timeDelta);
	//////////
		if( m_Input->IsKeyPressed(DIK_W) )
		camera->walk(60.0f * timeDelta);

		if(m_Input->IsKeyPressed(DIK_S) )
			camera->walk(-60.0f * timeDelta);

		if(m_Input->IsKeyPressed(DIK_A) )
			camera->strafe(-60.0f * timeDelta);

		if( m_Input->IsKeyPressed(DIK_D) )
			camera->strafe(60.0f * timeDelta);

		if(m_Input->IsKeyPressed(DIK_R))
			camera->fly(60.0f * timeDelta);

		if(m_Input->IsKeyPressed(DIK_F))
			camera->fly(-60.0f * timeDelta);

		if(m_Input->IsKeyPressed(DIK_UPARROW))
			camera->pitch(1.0f * timeDelta);

		if(m_Input->IsKeyPressed(DIK_DOWNARROW) )
			camera->pitch(-1.0f * timeDelta);

		if(m_Input->IsKeyPressed(DIK_N))
			camera->roll(60.0f * timeDelta);

		if(m_Input->IsKeyPressed(DIK_M) )
			camera->roll(-60.0f * timeDelta);
		if(m_Input->IsKeyPressed(DIK_F1))
			device->GetDevice()->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		if(m_Input->IsKeyPressed(DIK_F2))
			device->GetDevice()->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
		if(m_Input->IsKeyPressed(DIK_F3))
			device->GetDevice()->SetRenderState(D3DRS_FILLMODE,D3DFILL_POINT);
		//int x=0,y=0;
		//m_Input->GetMouseLocation(x,y);
		//camera->yaw(x);
		//camera->pitch(y);
	//////////
	D3DXMATRIX V;
	camera->getViewMatrix(&V);
	device->GetDevice()->SetTransform(D3DTS_VIEW, &V);
	//////////////
	//render
	render();
	///sleep before cpu is free
	//Sleep(1);
}

void Release()
{
	timer->Stop();
	device->Release();
	delete m_Input;
	delete timer;
}

int Run()
{
	bool done=false, result=false;
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	while(!done)
	{
		static float lastTime = (float)timeGetTime();
		if(PeekMessage(&msg,NULL,0, 0, PM_REMOVE))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		///
		result = Frame();
		if(!result)
		{
			MessageBox(window->MainWindowHandle, "Frame Processing Failed","Error", MB_OK);
			done = true;
		}
		/////check for escape
		float currTime  = (float)timeGetTime();
		float timeDelta = (currTime - lastTime)*0.001f;
		Display(timeDelta,done);
		lastTime = currTime;
		///////////////end while
	}
	Release();
	return 0;
}

int WINAPI WinMain(HINSTANCE instance,HINSTANCE prev,PSTR Command,int Count)
{
	////////////////////setup 3d device
	device=new D3DDevice();
	bool bl= device->InitDevice(instance,800,600,true,D3DDEVTYPE_HAL,"MzG Engine01");
	window=device->GetWindow();
	////////////create prespective matrix
	camera=new Camera(Camera::CameraType::LANDOBJECT);
	D3DXMATRIX proj;
	camera->SetUpPrespectiveMatrix(&proj,window->Width,window->Height);
	device->GetDevice()->SetTransform(D3DTS_PROJECTION, &proj);
	device->GetDevice()->SetRenderState( D3DRS_LIGHTING, false );
	device->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	///////////|/////////create Input
	m_Input = new InputHandle();
	bool result = m_Input->Initialize(window->Handle, window->MainWindowHandle, window->GetClientScreenWidth(), window->GetClientScreenHeight());
	if(!result)
	{
		MessageBox(window->MainWindowHandle, "Could not initialize the input object.", "Error", MB_OK);
		return false;
	}
	f=new Font2d(device);
	f->AddText(TextProperties(10,10,400,400,D3DXCOLOR(1,1,1,1),"Salam"));
	timer=new CTimer();
	timer->Start();
	////////////////
	loader=new FbxLoader(device);
	bool bbl=loader->LoadFbx("E:\\MzG Engine\\Debug\\wall3.fbx");
	//D3DXVECTOR3 center=loader->GetCenter();
	//camera->setPosition(&center);
	//////////setup light
	///x file
	Xloader=new StaticXfileLoader(device);
	//Xloader->Load("E:\\Graphics\\3d file\\x\\level.x");
	Xloader->Load("E:\\MzG Engine\\Debug\\bigship1.x");
	//Xloader->Load("md8.x","E:\\MzG Engine\\Debug\\x\\");
	//D3DXVECTOR3 center=Xloader->Models[0]->center;//D3DXVECTOR3(302, 17, 111);
	//camera->setPosition(&center);
	//////
	//CreateSunLight();
	//CreatePointLight(center);
	//CreateSpotLight(center,D3DXVECTOR3(0.0f,-0.3f,0.5));
	//////skybox
	SkyBox=new SkyBoxBasic(device);
	SkyBox->Load("skybox.x","E:\\MzG Engine\\Debug\\x\\");
	/////////////////////////go to Render loop
	if(bl)
	return Run();
	/////////////////end
}

