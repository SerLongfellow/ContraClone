#include "Camera.h"
#include "GameFunctions.h"

Camera::Camera(){
	center.x = 0;
	center.y = 0;
	center.z = 0;
	angle = 0;
	screenWidth = 1280;
	screenHeight = 720;
	halfScreenWidth = screenWidth / 2;
	halfScreenHeight = screenHeight / 2;
	initialized = false;
}

Camera::~Camera(){
}

bool Camera::Initialize(float x, float y, float z, float angle, HWND hWnd){

	center.x = 0;//(float)halfScreenWidth;
	center.y = 0;//(float)halfScreenHeight;
	center.z = 0;

	cameraOffset = center;

	this->angle = angle;

	initialized = true;

	return true;
}

void Camera::Reset(){
	center.x = center.y = 0;
}

D3DXVECTOR3 Camera::GetCenter(){
	return(center);
}

float Camera::GetAngle(){
	return(angle);
}

void Camera::setCenter(float x, float y){
	center.x = x;
	center.y = y;
}

void Camera::Update(float gameTime, InputDevice* inputDevice, b2Vec2* playerPos){
	//Scroll the screen
	float playerX = PW2RW(playerPos->x);

	if(playerX + center.x >= ((screenWidth / 2) - 100.0f)){
		center.x -= 3.8f;
	}
}