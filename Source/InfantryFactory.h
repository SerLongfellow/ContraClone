#ifndef INFANTRY_FACTORY_H
#define INFANTRY_FACTORY_H

#include "Infantry.h"
#include "ActorFactory.h"
#include <sstream>

class InfantryFactory: public ActorFactory{

public:
	InfantryFactory();
	~InfantryFactory();

	Infantry* CreateObject(ArtAssetLibrary* sprites, TiXmlElement* data, b2World* world);


};

#endif