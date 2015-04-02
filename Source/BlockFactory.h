#ifndef Block_FACTORY_H
#define Block_FACTORY_H

#include "Block.h"
#include "ActorFactory.h"
#include <sstream>

class BlockFactory: public ActorFactory{

public:
	BlockFactory();
	~BlockFactory();

	Block* CreateObject(ArtAssetLibrary* sprites, TiXmlElement* data, b2World* world);


};

#endif