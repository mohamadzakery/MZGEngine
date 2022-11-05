#pragma once
#include "StaticXfileLoader.h"
class SkyBoxBasic
{
private:
	IDirect3DDevice9 *_dev;
	D3DDevice *device;
	StaticXfileLoader *Loader;
public:
	SkyBoxBasic(D3DDevice *dev);
	~SkyBoxBasic(void);
	void Load(LPSTR FileName,LPSTR path);
	void Draw();
};

