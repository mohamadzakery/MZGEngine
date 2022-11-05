#pragma once
#include <fbxsdk.h>
#include "EngineFbxManager.h"
#include "BaseMesh.h"
#include "Light.h"
struct Object
{
	string				name;
	BaseMesh				*mesh;
	double				*matrix;	// transfomation data for the ENTIRE object, GLOBAL space
	Object()
	{
		mesh = NULL;
		matrix = NULL;
	}
};
class FbxLoader
{
private:
	Object *tmp;
	bool LoadContent(FbxScene *lScene, FbxNode *pNode);
	bool LoadMesh(FbxNode *Node,bool Bl);
	bool LoadMesh(FbxNode *pNode);
	bool LoadLight(FbxNode *pNode);
	D3DDevice *device;
	FbxTexture* FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex);
public:
	vector<Object*> Objects;
	FbxLoader(D3DDevice *Device);
	~FbxLoader(void);
	bool LoadFbx(LPCSTR filename);
	D3DXVECTOR3 GetCenter();
	void Render();
};

