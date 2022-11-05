#pragma once
#include"D3DDevice.h"
#include <vector>
#include <mmsystem.h>
#include <iostream>
using namespace std ;
struct TextProperties
{
	RECT Rect;
	D3DXCOLOR color;
	char* text;
	TextProperties(int xLeft,int yTop,int xRight,int yBottom,D3DXCOLOR Color,char* _text)
	{
		SetRect(&Rect,xLeft,yTop,xRight,yBottom);
		color=Color;
		int len=strlen(_text);
		text=new char[len+1];
		strcpy(text,_text);
		strcat(text,"\0");
	}
};
class Font2d
{
private:
	vector<TextProperties> Array;
	D3DDevice* device;
	LPD3DXFONT dxFont;
public:
	Font2d(D3DDevice*);
	~Font2d(void);
	void AddText(TextProperties p);
	void RemoveText(int i);
	void RemoveText(TextProperties p);
	void Render();
	void UpdateText(int i,char* newText)
	{

		int len=strlen(newText);
		delete this->Array[i].text;
		this->Array[i].text=new char[len+1];
		strcpy(this->Array[i].text,newText);
		strcat(this->Array[i].text,"\0");
	}
};

