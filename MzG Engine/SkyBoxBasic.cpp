#include "SkyBoxBasic.h"


SkyBoxBasic::SkyBoxBasic(D3DDevice *dev)
{
	this->device=dev;
	this->_dev=dev->GetDevice();
	this->Loader=new StaticXfileLoader(dev);
}
SkyBoxBasic::~SkyBoxBasic(void)
{
	delete Loader;
}
void SkyBoxBasic::Load(LPSTR FileName,LPSTR path)
{
	Loader->Load(FileName,path);
}
void SkyBoxBasic::Draw()
{
	///////////////////
	DWORD Zen=-1,Zwri=-1,Len=-1;
	_dev->GetRenderState(D3DRS_ZENABLE,&Zen);
	_dev->GetRenderState(D3DRS_ZWRITEENABLE,&Zwri);
	_dev->GetRenderState(D3DRS_LIGHTING,&Len);
	// Disable the z buffer
	_dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	_dev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	// Disable lighting
	_dev->SetRenderState( D3DRS_LIGHTING, FALSE );
	///////////////////
	D3DXMATRIX matView, matViewSave,matWorldSave ,matWorld;
	_dev->GetTransform( D3DTS_VIEW, &matViewSave );
	_dev->GetTransform( D3DTS_WORLD, &matWorldSave );
	matView = matViewSave;
	matView._41 = 0.0f; matView._42 = -0.4f; matView._43 = 0.0f;
	_dev->SetTransform( D3DTS_VIEW, &matView );
	D3DXMatrixIdentity(&matWorld);
	_dev->SetTransform( D3DTS_WORLD, &matWorld);
	this->Loader->Draw();
	_dev->SetTransform( D3DTS_VIEW, &matViewSave );
	_dev->SetTransform( D3DTS_WORLD, &matWorldSave );
	/////
	_dev->SetRenderState(D3DRS_ZENABLE,Zen);
	_dev->SetRenderState(D3DRS_ZWRITEENABLE,Zwri);
	_dev->SetRenderState(D3DRS_LIGHTING,Len);
}
