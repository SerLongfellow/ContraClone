#ifndef Block_H
#define Block_H

#include <map>
#include "Actor.h"

class Block: public Actor{

	friend class BlockFactory;

public:
	Block();
	~Block();

	void Update(float gameTime, InputDevice* inputDevice, ActorVector* objectsToAdd);
	//void Draw(float gameTime, Camera* camera);
	
private:
	std::string blockType;
};

#endif