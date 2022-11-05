#pragma once
#include "D3DDevice.h"
#include <vector>
#include <mmsystem.h>
#include "string"
#include <iostream>
using namespace std ;
enum LightTypes
{
    LIGHT_TYPE_DIRECTIONAL = 0,
	LIGHT_TYPE_SPOT,
	LIGHT_TYPE_POINT,
};
struct LightValue
{
	struct DIRECTIONAL
	{
			float _Diffuse_r;
			float _Diffuse_g;
			float _Diffuse_b;
			float _Diffuse_a;
			float _Range;
			D3DLIGHTTYPE _Type;
		    D3DXVECTOR3 _Direction;
	}directional;
	struct SPOT
	{
			float _Diffuse_r;
			float _Diffuse_g;
			float _Diffuse_b;
			float _Diffuse_a;
			float _Range;
		    D3DLIGHTTYPE _Type;
            D3DXVECTOR3 _Position;
            D3DXVECTOR3 _Direction;
            float _Theta;
            float _Phi;
            float _Falloff;
            float _Attenuation0;
	}spot;
	struct POINT
	{
			float _Diffuse_r;
			float _Diffuse_g;
			float _Diffuse_b;
			float _Diffuse_a;
			float _Range;
		    D3DLIGHTTYPE _Type;
            D3DXVECTOR3 _Position;
            float _Attenuation1;
	}point;
};
struct Engin_Light
{
	//LightTypes Type;
	//LightValue Value;
	D3DLIGHT9 Value;
	char* Name;
};
class Light
{
private:
	vector<Engin_Light*> LightList;
	D3DDevice * Device;
	void AddLightToDevice(int);
	int count;
public:
	Light(D3DDevice*);
	~Light(void);
	Engin_Light* GetLight(int);
	int GetLightIndex(char* name);
	int GetLightCount();
	void AddLight(Engin_Light*);
	void Release();
	void TurnOn(int i);
	void TurnOff(int i);
};

