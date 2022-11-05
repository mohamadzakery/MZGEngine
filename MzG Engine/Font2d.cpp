#include "Font2d.h"


Font2d::Font2d(D3DDevice* d)
{
	this->device=d;
	//////////////////
	HRESULT hr;
	HDC hDC;
	int nHeight;
	int nPointSize = 18;
	hDC = GetDC( NULL );
	nHeight = -( MulDiv( nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 ) );
	ReleaseDC( NULL, hDC );
	// Create a font for statistics and help output
	hr = D3DXCreateFont( this->device->GetDevice(), nHeight, 0, FW_BOLD, 0, FALSE, 
		                 DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		                 DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), 
		                 &this->dxFont );

	if( FAILED( hr ) )
		MessageBox(NULL,"Call to D3DXCreateFont failed!", "ERROR",MB_OK|MB_ICONEXCLAMATION);
}
Font2d::~Font2d(void)
{
	delete this->dxFont;
	Array.clear();
}
void Font2d::AddText(TextProperties p)
{
	this->Array.push_back(p);
}
void Font2d::RemoveText(int i)
{
	this->Array.erase(Array.begin()+i);
}
void Font2d::RemoveText(TextProperties p)
{
	int r=-1;
	for(int i=0;i<Array.size();i++)
		if (Array[i].text==p.text && Array[i].Rect.top==p.Rect.top && Array[i].color.a==p.color.a)
		{
			r=i;
			break;
		}
		if(r!=-1)
			this->RemoveText(r);
}
void Font2d::Render()
{
	for(int i=0;i<Array.size();i++)
	{
		TextProperties p=Array[i];
		this->dxFont->DrawText(NULL,p.text,-1,&p.Rect,DT_NOCLIP,p.color);
	}
}
