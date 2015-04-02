#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <d3dx9.h>
#include "InputDevice.h"
#include <Box2D\Box2D.h>

class Camera{
public:

	Camera();
	~Camera();

	bool Initialize(float x, float y, float z, float angle, HWND hWnd);
	void Update(float gameTime, InputDevice* inputDevice, b2Vec2* playerPos);
	void Reset();

	D3DXVECTOR3 GetCenter();
	float GetAngle();
	void setCenter(float x, float y);

private:
	D3DXVECTOR3 position, center, cameraOffset;

	float speed;

	float angle;
	int screenWidth, screenHeight;
	int halfScreenWidth, halfScreenHeight;

	bool initialized;
};

#endif