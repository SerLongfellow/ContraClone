#ifndef SPRITE_H
#define SPRITE_H

#include <d3dx9.h>
#include <Windows.h>
#include <string>
#include "Camera.h"

class Sprite{
	
public:
	Sprite();
	~Sprite();

	bool Initialize(LPDIRECT3DDEVICE9 device, std::string file, UINT width, UINT height);
	bool IsInitialized();
	virtual void Draw(float gameTime, D3DXVECTOR3 objectPosition, D3DXVECTOR3 objectCenter, float objectAngle, Camera* camera, RECT* srcRect);

	UINT GetWidth();
	UINT GetHeight();
	
private:
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;
	D3DCOLOR color;
	bool initialized;

	UINT width;
	UINT height;
};

#endif