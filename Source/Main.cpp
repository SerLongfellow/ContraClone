#include <Windows.h>
#include "Game.h"

bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int x, int y,  int width, int height, HWND& hWnd);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	HWND hWnd;

	Game* game;

	//AllocConsole();

	if(GenerateWindow(hInstance, nCmdShow, "Win32Window", "CPSC 4399 Final Project", 150, 150, 1280, 720, hWnd)){

		MSG msg;
		memset(&msg, 0, sizeof(msg));

		game = new Game();

		if(game->Initialize(hInstance, hWnd)){

			while(TRUE){

				while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if(msg.message == WM_QUIT){
					break;
				}
				else{
					if(!game->Run()){
						break;
					}
				}
			}
		}

		delete game;

		return msg.wParam;
	}

	return 0;
}

bool GenerateWindow(HINSTANCE hInstance, int nCmdShow, LPCSTR className, LPCSTR windowTitle, int x, int y, int width, int height, HWND& hWnd){

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = className;
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if(!RegisterClassEx(&wc))
		return false;

	hWnd = CreateWindowEx(NULL, className,
		windowTitle,
		WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_MINIMIZEBOX,
		x,
		y,
		width, 
		height, 
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	return true;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	switch(message){

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}