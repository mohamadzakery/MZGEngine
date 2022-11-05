#pragma once
#include <d3dx9.h>
class Camera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT };
	Camera(CameraType cameraType);
	Camera(void);
	~Camera(void);
	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward
	
	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	void getViewMatrix(D3DXMATRIX* V); 
	void setCameraType(CameraType cameraType); 
	void getPosition(D3DXVECTOR3* pos); 
	void setPosition(D3DXVECTOR3* pos); 

	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);
	void SetUpPrespectiveMatrix(D3DXMATRIX *proj,int W,int H);
private:
	CameraType  _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;
};

