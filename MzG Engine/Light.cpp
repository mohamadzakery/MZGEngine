#include "Light.h"


Light::Light(D3DDevice* _device)
{
	this->Device=_device;
	count=0;
}
Light::~Light(void)
{

	LightList.clear();
}
void Light::AddLight(Engin_Light* light)
{
	this->LightList.push_back(light);
	AddLightToDevice(count++);
}
void Light::AddLightToDevice(int i)
{
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	Engin_Light* l=LightList[i];
	light=l->Value;
	this->Device->GetDevice()->SetLight(i,&light);
}
void Light::TurnOff(int i)
{
	this->Device->GetDevice()->LightEnable(i,false);
}
void Light::TurnOn(int i)
{
	this->Device->GetDevice()->LightEnable(i,true);
}
int Light::GetLightCount()
{
	return this->count;
}
Engin_Light* Light::GetLight(int i)
{
	return LightList[i];
}
int Light::GetLightIndex(char* name)
{
	int t=0;
	    for(int j=0;j<LightList.size();j++)
		{
			if(strcmp(LightList[j]->Name,name)==0)
				return t;
			t++;
		}
		return -1;
}