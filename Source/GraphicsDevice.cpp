#include "GraphicsDevice.h"
#include "Definitions.h"

GraphicsDevice::GraphicsDevice(){
}

GraphicsDevice::~GraphicsDevice(){
	//Release all graphics resources

	if(device){
		device->Release();
		device = NULL;
	}

	if(direct3d){
		direct3d->Release();
		direct3d = NULL;
	}
}

bool GraphicsDevice::Initialize(HWND hWnd, bool windowed){

	direct3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS presentationParams;
	ZeroMemory(&presentationParams, sizeof(presentationParams));
	presentationParams.Windowed = windowed;
	presentationParams.hDeviceWindow = hWnd;
	presentationParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentationParams.BackBufferFormat = D3DFMT_X8R8G8B8;
	presentationParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	//Create the device
	HRESULT result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_MIXED_VERTEXPROCESSING, &presentationParams, &device);

	if(result != D3D_OK){
		switch(result){
		case D3DERR_INVALIDCALL:
			MessageBox(hWnd, "Invalid call", "", MB_OK);
			break;
		case D3DERR_DEVICELOST:
			MessageBox(hWnd, "Device Lost", "", MB_OK);
			break;
		case D3DERR_NOTAVAILABLE:
			MessageBox(hWnd, "Not available", "", MB_OK);
			break;
		}
		return false;
	}

	D3DXCreateFont(device,    // the D3D Device
		90,    // font height
		0,    // default font width
		FW_NORMAL,    // font weight
		1,    // not using MipLevels
		false,    // italic font
		DEFAULT_CHARSET,    // default character set
		OUT_DEFAULT_PRECIS,    // default OutputPrecision,
		DEFAULT_QUALITY,    // default Quality
		DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
		"Impact",    // Font name
		&headerFont);    // the font object

	D3DXCreateFont(device,    // the D3D Device
		40,    // font height
		0,    // default font width
		FW_NORMAL,    // font weight
		1,    // not using MipLevels
		false,    // italic font
		DEFAULT_CHARSET,    // default character set
		OUT_DEFAULT_PRECIS,    // default OutputPrecision,
		DEFAULT_QUALITY,    // default Quality
		DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
		"Impact",    // Font name
		&smallFont);    // the font object

	return true;
}

void GraphicsDevice::Clear(D3DCOLOR color){

	//2nd param is set to NULL so it clears the entire back buffer
	device->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
}

void GraphicsDevice::Begin(){
	device->BeginScene();
}

void GraphicsDevice::End(){
	device->EndScene();
}

void GraphicsDevice::Present(){
	device->Present(NULL, NULL, NULL, NULL);
}

LPDIRECT3DDEVICE9 GraphicsDevice::GetDevice(){
	return(device);
}

//This code was lifted from http://cboard.cprogramming.com/game-programming/116614-directx-%7C-drawing-text.html
void GraphicsDevice::PrintHeaderText(char* str, int x, int y, DWORD color){


	static RECT textbox;
	SetRect(&textbox, x, y, SCREEN_WIDTH, SCREEN_HEIGHT);

	headerFont->DrawTextA(NULL,
		str,
		strlen(str),
		&textbox,
		DT_LEFT | DT_TOP,
		color);
}

void GraphicsDevice::PrintSmallText(char* str, int x, int y, DWORD color){


	static RECT textbox;
	SetRect(&textbox, x, y, SCREEN_WIDTH, SCREEN_HEIGHT);

	smallFont->DrawTextA(NULL,
		str,
		strlen(str),
		&textbox,
		DT_LEFT | DT_TOP,
		color);
}