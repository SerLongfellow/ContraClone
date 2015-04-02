#include "Sprite.h"

Sprite::Sprite(){

	width = 0;
	height = 0;

	color = D3DCOLOR_ARGB(255,255,255,255);

	initialized = false;
}

Sprite::~Sprite(){

	if(sprite){
		sprite->Release();
		sprite = NULL;
	}

	if(tex){
		tex->Release();
		tex = NULL;
	}
}

bool Sprite::Initialize(LPDIRECT3DDEVICE9 device, std::string file, UINT width, UINT height){

	this->width = width;
	this->height = height;

	if(!SUCCEEDED(D3DXCreateTextureFromFileEx(
		device,
		file.c_str(),
		width,
		height,
		D3DX_DEFAULT, 
		0, 
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, 
		D3DX_DEFAULT, 
		D3DX_DEFAULT, 
		0, 
		NULL, 
		NULL, 
		&tex))){

			std::string s = "There was an issue creating the texture. Make sure the requested image is available. Requested image: " + file;
			MessageBox(NULL, s.c_str(), NULL, NULL);
			return(false);
	}

	if(!SUCCEEDED(D3DXCreateSprite(device, &sprite))){

		MessageBox(NULL, "There was an issue creating the sprite.", NULL, MB_OK);
	}

	initialized = true;

	return true;
}

bool Sprite::IsInitialized(){
	return(initialized);
}

void Sprite::Draw(float gameTime, D3DXVECTOR3 objectPosition, D3DXVECTOR3 objectCenter, float objectAngle, Camera* camera, RECT* srcRect){

	if(sprite && tex){
		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		objectPosition += camera->GetCenter();
		 
		sprite->Draw(tex, srcRect, NULL, &objectPosition, color);

		sprite->End();
	}
}

UINT Sprite::GetWidth(){
	return(width);
}

UINT Sprite::GetHeight(){
	return(height);
}
