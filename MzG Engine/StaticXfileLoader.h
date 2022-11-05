#pragma once
#include "D3DDevice.h"
#include<vector>
using namespace std;
struct Model
{
    DWORD numMaterials;
    LPD3DXMESH Mesh;
    LPDIRECT3DTEXTURE9* Texture;
    D3DMATERIAL9* Material;
	D3DXMATRIX *worldMatrix;
	D3DXVECTOR3 center;
	float radius;
};

class StaticXfileLoader
{
private:
	D3DDevice* device;
	IDirect3DDevice9* _Dev;
	char *tmp;
public:
	StaticXfileLoader(D3DDevice*);
	~StaticXfileLoader(void);
	void Load(LPCSTR File,LPCSTR path,bool Translate=false,float x=0,float y=0,float z=0);
	void Load(LPCSTR File,bool Translate=false,float x=0,float y=0,float z=0);
	void Draw();
	vector<Model*> Models;
};

