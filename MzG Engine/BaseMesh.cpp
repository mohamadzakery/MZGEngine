#include "BaseMesh.h"


BaseMesh::BaseMesh(D3DDevice *_device,string name)
{
	this->device=_device;
	this->name=name;
	vertices = NULL;
	normals = NULL;
	UVs = NULL;
	frameStart= 0;
	frameEnd= 0;
	frame= 0;
	numVertices = 0;
	VertexBuffer = NULL;
	__device=this->device->GetDevice();
}
BaseMesh::~BaseMesh(void)
{
	// delete the arrays
	delete [] vertices;
	delete [] normals;
	delete [] UVs;
	// delete our faces
	for (int i = 0; i < (int)Faces.size(); i++)
		delete Faces[i];
	Faces.clear();
}
void BaseMesh::AddFace(int v1,int v2,int v3,int n1,int n2,int n3,int u1,int u2,int u3)
{
	Face* f=new Face;
	f->vertices[0]=v1;
	f->vertices[1]=v2;
	f->vertices[2]=v3;
	f->normals[0]=n1;
	f->normals[1]=n2;
	f->normals[2]=n3;
	f->UVs[0]=u1;
	f->UVs[1]=u2;
	f->UVs[2]=u3;
	AddFace(f);
}
void BaseMesh::AddFace(Face* f)
{
	Faces.push_back(f);
}
vector<Face *> &BaseMesh::getFaces(void)
{
	return Faces;
}
D3DXVECTOR3 *BaseMesh::getVertices(void)
{
	return vertices;
}
D3DXVECTOR3 *BaseMesh::getNormals(void)
{
	return normals;
}
D3DXVECTOR3 *BaseMesh::getUVs(void)
{
	return UVs;
}
void BaseMesh::InitVertexBuffer()
{
	Vertex *ver;
	ver=new Vertex[Faces.size()*3];
	for(register  int i=0;i<this->Faces.size();i++)
	{
		Face *face = Faces[i];
		for(int v=0;v<3;v++)
		{
			//ver[i]=new Vertex(vertices[ face->vertices[v] ].x,vertices[ face->vertices[v] ].y,vertices[ face->vertices[v] ].z,normals[ face->normals[v] ].x,normals[ face->normals[v] ].y,normals[ face->normals[v] ].z,UVs[ face->UVs[v] ].x, UVs[ face->UVs[v] ].y);
			ver[i+v]._x=vertices[ face->vertices[v] ].x;
			ver[i+v]._y=vertices[ face->vertices[v] ].y;
			ver[i+v]._z=vertices[ face->vertices[v] ].z;
			ver[i+v]._nx=normals[ face->normals[v] ].x;
			ver[i+v]._ny=normals[ face->normals[v] ].y;
			ver[i+v]._nz=normals[ face->normals[v] ].z;
			ver[i+v]._u=UVs[ face->UVs[v] ].x;
			ver[i+v]._v=UVs[ face->UVs[v] ].y;
		}
	}
	IDirect3DDevice9 *device=this->device->GetDevice();
	int FaceSize=Faces.size();
	int StructSize=sizeof(Vertex);
	int BufferSize=StructSize*FaceSize*3;
	device->CreateVertexBuffer(BufferSize,D3DUSAGE_DYNAMIC,FVF_VERTEX,D3DPOOL_DEFAULT,&this->VertexBuffer,NULL);
	void *pVertices = NULL;

	VertexBuffer->Lock(0, 0,(void**)&pVertices, 0);//sizeof(ver),
	memcpy( pVertices, ver, BufferSize );
	VertexBuffer->Unlock();
	/////////
    D3DXMATRIX MTrans;
	D3DXMATRIX MRotation;
	D3DXMATRIX MScale;
	D3DXMatrixTranslation( &MTrans,this->matTrans.x,this->matTrans.y,this->matTrans.z);
	D3DXMatrixScaling(&MScale,this->matScale.x,this->matScale.y,this->matScale.z);
	D3DXMatrixRotationYawPitchRoll( &MRotation, D3DXToRadian(this->matRotation.x), D3DXToRadian(this->matRotation.y), D3DXToRadian(this->matRotation.z) );
	//
	World=MScale*MRotation*MTrans;
	//
	delete [] ver;
}
void BaseMesh::DrawFace(int i)
{
	//__device->SetStreamSource(0,this->VertexBuffer,0,sizeof(Vertex));
	//__device->SetFVF(FVF_VERTEX);
	//	if(Faces[i]->Material!=NULL)
	//	__device->SetMaterial(Faces[i]->Material);
	//if(Faces[i]->Texture !=NULL)
	//	__device->SetTexture(0,Faces[i]->Texture);
	//__device->DrawPrimitive(D3DPT_TRIANGLESTRIP,i,2);
}
void BaseMesh::DrawFace()
{
	//__device->SetTransform( D3DTS_WORLD, &this->World);

	__device->SetStreamSource( 0, VertexBuffer, 0, sizeof(Vertex) );

	if(Material!=NULL)
	__device->SetMaterial(Material);
	if(Texture !=NULL)
	__device->SetTexture(0,Texture);

	__device->SetFVF(FVF_VERTEX);


	__device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,Faces.size());

}
void BaseMesh::AddMaterialAndTexture(LPCSTR textureFile,D3DMATERIAL9 *material)
{
	this->Material=material;
	if(textureFile!=NULL && textureFile!="")
	D3DXCreateTextureFromFile(this->device->GetDevice(),textureFile,&this->Texture);
	else this->Texture=NULL;
}