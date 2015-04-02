#include "InputDevice.h"

#define KEYDOWN(name, key) (name[key] & 0x80)

InputDevice::InputDevice(){
	directInput = NULL;
	keyboard = NULL;
}

InputDevice::~InputDevice(){

	if(directInput){
		directInput->Release();
		directInput = NULL;
	}

	if(keyboard){
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = NULL;
	}
}

bool InputDevice::Initialize(HINSTANCE hInst, HWND hWnd){
	
	if(!SUCCEEDED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL))){
		return false;
	}

	if(!SUCCEEDED(directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL))){
		return false;
	}

	if(!SUCCEEDED(keyboard->SetDataFormat(&c_dfDIKeyboard))){
		return false;
	}

	if(!SUCCEEDED(keyboard->Acquire())){
		return false;
	}

	return true;
}

bool InputDevice::Update(){

	if(!ReadKeyboard()){
		return false;
	}

	return true;
}

bool InputDevice::ReadKeyboard(){

	HRESULT result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if(!SUCCEEDED(result)){

		if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED){
			keyboard->Acquire();
		}
		else{
			return false;
		}
	}

	return true;
}

bool InputDevice::IsLeftArrowPressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_LEFT)){
		return true;
	}

	return false;

}

bool InputDevice::IsRightArrowPressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_RIGHT)){
		return true;
	}

	return false;

}

bool InputDevice::IsUpArrowPressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_UP)){
		return true;
	}

	return false;

}

bool InputDevice::IsDownArrowPressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_DOWN)){
		return true;
	}

	return false;

}

bool InputDevice::IsTabPressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_TAB)){
		return true;
	}

	return false;

}

bool InputDevice::IsWPressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_W)){
		return true;
	}

	return false;

}

bool InputDevice::IsAPressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_A)){
		return true;
	}

	return false;

}

bool InputDevice::IsSPressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_S)){
		return true;
	}

	return false;

}

bool InputDevice::IsDPressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_D)){
		return true;
	}

	return false;

}

bool InputDevice::IsSpacePressed(){

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(KEYDOWN(keyboardState, DIK_SPACE)){
		return true;
	}

	return false;

}

unsigned char* InputDevice::GetKeyboardState() {
	return keyboardState;
}


