#include "StaticXfileLoader.h"


StaticXfileLoader::StaticXfileLoader(D3DDevice* dev)
{
	this->device=dev;
	this->_Dev=dev->GetDevice();
}
StaticXfileLoader::~StaticXfileLoader(void)
{
	int size=this->Models.size();
	Model *m;
	for(int i=0;i<size;i++)
	{
		delete Models[i];
	}
	Models.clear();
	delete []tmp;
}
void StaticXfileLoader::Load(LPCSTR File,LPCSTR path,bool Translate,float x,float y,float z)
{
	int lenB=strlen(path);
	tmp=new char[lenB];
	strcpy(tmp,path);
	strcat(tmp,File);
	Model* model=new Model;
	LPD3DXBUFFER bufMaterial;
	IDirect3DDevice9 *d3ddev=device->GetDevice();
	D3DXLoadMeshFromX(tmp, D3DXMESH_SYSTEMMEM, d3ddev, NULL, &bufMaterial, NULL, &model->numMaterials, &model->Mesh); 
	//OutputDebugString(L"LOAD MESH \n"); 
	D3DXMATERIAL* tempMaterials = (D3DXMATERIAL*)bufMaterial->GetBufferPointer(); 
	//OutputDebugString(L"GET BUFFER\n"); 
	model->Material = new D3DMATERIAL9[model->numMaterials]; 
	model->Texture = new LPDIRECT3DTEXTURE9[model->numMaterials]; 
	//OutputDebugString(L"LOAD MESH \n"); 
	for(DWORD index = 0; index < model->numMaterials; index++) 
	{ 
		model->Material[index] = tempMaterials[index].MatD3D; 
		model->Material[index].Ambient = model->Material[index].Diffuse; 
		// if there is a texture to load, load it 
		LPSTR base=tempMaterials[index].pTextureFilename;
		int lenA=strlen(base);
		tmp=new char[lenB];
		//char *t1=new char[lenB];
		strcpy(tmp,path);
		strcat(tmp,base);
		//strcat(tmp,path);
	 if(FAILED(D3DXCreateTextureFromFileA(d3ddev, tmp, &model->Texture[index]))) 
		model->Texture[index] = NULL;// if there is no texture, set the texture to NULL 
	}  
	if(Translate)
	{
		model->worldMatrix=new D3DXMATRIX;
		D3DXMatrixTranslation(model->worldMatrix, x, y, z);
	}
	else
		model->worldMatrix=NULL;
	///////////bounding sphere
	HRESULT hr = 0;
	BYTE* v = 0;
	model->Mesh->LockVertexBuffer(0, (void**)&v);
	hr = D3DXComputeBoundingSphere(
			(D3DXVECTOR3*)v,
			model->Mesh->GetNumVertices(),
			D3DXGetFVFVertexSize(model->Mesh->GetFVF()),
			&model->center,
			&model->radius);
	model->Mesh->UnlockVertexBuffer();
		///////////
	//ID3DXBuffer* adjBuffer  = 0;
	//	hr = model->Mesh->OptimizeInplace(		
	//	D3DXMESHOPT_ATTRSORT |
	//	D3DXMESHOPT_COMPACT  |
	//	D3DXMESHOPT_VERTEXCACHE,
	//	(DWORD*)adjBuffer->GetBufferPointer(),
	//	0, 0, 0);
		//delete adjBuffer;
	//////////
	///////////
	this->Models.push_back(model);
}
void StaticXfileLoader::Draw()
{
	int size=this->Models.size();
	Model *m;
	for(int i=0;i<size;i++)
	{
		m=Models[i];
		if(m->worldMatrix!=NULL)
			_Dev->SetTransform(D3DTS_WORLD, m->worldMatrix);
		int count=m->numMaterials;
		for(int j=0;j<count;j++)
		{
			_Dev->SetMaterial(&m->Material[j]);
			if(m->Texture!=NULL)
			_Dev->SetTexture(0,m->Texture[j]);
			m->Mesh->DrawSubset(j);
		}
	}
}
void StaticXfileLoader::Load(LPCSTR File,bool Translate,float x,float y,float z)
{
	Model* model=new Model;
	LPD3DXBUFFER bufMaterial;
	IDirect3DDevice9 *d3ddev=device->GetDevice();
	D3DXLoadMeshFromX(File, D3DXMESH_SYSTEMMEM, d3ddev, NULL, &bufMaterial, NULL, &model->numMaterials, &model->Mesh); 
	//OutputDebugString(L"LOAD MESH \n"); 
	D3DXMATERIAL* tempMaterials = (D3DXMATERIAL*)bufMaterial->GetBufferPointer(); 
	//OutputDebugString(L"GET BUFFER\n"); 
	model->Material = new D3DMATERIAL9[model->numMaterials]; 
	model->Texture = new LPDIRECT3DTEXTURE9[model->numMaterials]; 
	//OutputDebugString(L"LOAD MESH \n"); 
	for(DWORD index = 0; index < model->numMaterials; index++) 
	{ 
		model->Material[index] = tempMaterials[index].MatD3D; 
		model->Material[index].Ambient = model->Material[index].Diffuse; 
		// if there is a texture to load, load it 
	 if(FAILED(D3DXCreateTextureFromFileA(d3ddev,tempMaterials[index].pTextureFilename, &model->Texture[index]))) 
		model->Texture[index] = NULL;// if there is no texture, set the texture to NULL 
	}  
	if(Translate)
	{
		model->worldMatrix=new D3DXMATRIX;
		D3DXMatrixTranslation(model->worldMatrix, x, y, z);
	}
	else
		model->worldMatrix=NULL;
	///////////bounding sphere
	HRESULT hr = 0;
	BYTE* v = 0;
	model->Mesh->LockVertexBuffer(0, (void**)&v);
	hr = D3DXComputeBoundingSphere(
			(D3DXVECTOR3*)v,
			model->Mesh->GetNumVertices(),
			D3DXGetFVFVertexSize(model->Mesh->GetFVF()),
			&model->center,
			&model->radius);
	model->Mesh->UnlockVertexBuffer();
	///////////
	//ID3DXBuffer* adjBuffer  = 0;
	//	hr = model->Mesh->OptimizeInplace(		
	//	D3DXMESHOPT_ATTRSORT |
	//	D3DXMESHOPT_COMPACT  |
	//	D3DXMESHOPT_VERTEXCACHE,
	//	(DWORD*)adjBuffer->GetBufferPointer(),
	//	0, 0, 0);
	//	delete adjBuffer;
	//////////
	this->Models.push_back(model);
}
