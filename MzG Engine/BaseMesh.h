#pragma once
#include"D3DDevice.h"
#include <string>
#include <vector>
#include <fbxsdk.h>
using namespace std;
///////////////////////////vertex Declateion
	#define FVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
	struct Vertex
	{
	Vertex(){}
	Vertex(
		float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
    float _x, _y, _z;
    float _nx, _ny, _nz;
    float _u, _v; // texture coordinates
};
///////////////////////////Material Declaration
///////////////////////////Face Declaration
	struct Face
	{
		int vertices[3];
		int normals[3];
		int UVs[3];
		D3DXVECTOR3 faceCenter;
		D3DXVECTOR3 faceNormal;
	};
//////////////////////////Animation 
	struct Frame
	{
	KFbxXMatrix		*bones;
	double			*weights;
	Frame()
	{
		bones = NULL;
		weights = NULL;
	}
	};
class BaseMesh
{
private:
	D3DDevice	*device;
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	IDirect3DDevice9 *__device;
public:
	D3DXVECTOR3 matTrans,matRotation,matScale;
	D3DXMATRIX World;
	D3DMATERIAL9 *Material;
	LPDIRECT3DTEXTURE9 Texture;
	BaseMesh(D3DDevice *_device,string name = "");
	~BaseMesh(void);
	void AddFace(int v1,int v2,int v3,int n1,int n2,int n3,int u1,int u2,int u3);
	void AddMaterialAndTexture(LPCSTR textureFile,D3DMATERIAL9 *material);
	void AddFace(Face* f);
	void InitVertexBuffer();
	void DrawFace(int ind);
	void DrawFace();
	////////
	vector<Face *> &getFaces(void);
	D3DXVECTOR3 *getVertices(void);
	D3DXVECTOR3 *getNormals(void);
	D3DXVECTOR3 *getUVs(void);
	////////
	D3DXVECTOR3 *vertices;
	D3DXVECTOR3 *normals;
	D3DXVECTOR3 *UVs;
	vector<Face*> Faces;
	int	numVertices;
	string name;
	////Animations
	float			frameStart;
	float			frameEnd;
	float			frame;
};

