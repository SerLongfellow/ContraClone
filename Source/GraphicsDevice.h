#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include <d3d9.h>
#include <d3dx9core.h>

class GraphicsDevice{

public:
	GraphicsDevice();
	~GraphicsDevice();

	//Direct3D Functions
	bool Initialize(HWND hWnd, bool windowed);
	void Clear(D3DCOLOR color);
	void Begin();
	void End();
	void Present();

	void PrintHeaderText(char* str, int x, int y, DWORD color);
	void PrintSmallText(char* str, int x, int y, DWORD color);

	LPDIRECT3DDEVICE9 GetDevice();

private:
	LPDIRECT3D9 direct3d;
	LPDIRECT3DDEVICE9 device;
	LPD3DXFONT headerFont, smallFont;

};

#endif