#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

class InputDevice{

public:
	InputDevice();
	~InputDevice();

	bool Initialize(HINSTANCE hInst, HWND hWnd);
	bool Update();

	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();
	bool IsUpArrowPressed();
	bool IsDownArrowPressed();
	bool IsTabPressed();
	bool IsWPressed();
	bool IsAPressed();
	bool IsSPressed();
	bool IsDPressed();
	bool IsSpacePressed();

	unsigned char* GetKeyboardState();

private:
	bool ReadKeyboard();
	void ProcessInput();

	IDirectInput8* directInput;
	IDirectInputDevice8* keyboard;

	unsigned char keyboardState[256];

};

#endif