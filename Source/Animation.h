#ifndef ANIMATION_H
#define ANIMATION_H

#include <windows.h>
#include <vector>
#include <string>
#include <Box2D\Box2D.h>

class Animation{

public:
	Animation();
	~Animation();

	bool LoadAnimation(const char* animationFile, float scale);
	void UpdateAnimation(int actorState, RECT& srcRect, b2Body* body, b2FixtureDef* shapefd);

	int GetWidth();
	int GetHeight();

private:
	RECT sourceRect;
	int currentState;

	struct State {
		int startX, startY, width, height, currentState, numStates;
		std::string order;
	};

	std::vector<State*> states;
};

#endif