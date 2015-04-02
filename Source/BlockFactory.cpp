#include "BlockFactory.h"

BlockFactory::BlockFactory(){
	count = 0;
}

BlockFactory::~BlockFactory(){

}

Block* BlockFactory::CreateObject(ArtAssetLibrary* sprites, TiXmlElement* data, b2World* world){
	std::string objectID;
	std::stringstream ss;
	float x, y, angularVelocity;

	count++;

	ss << "BLOCK_" << count;

	ss >> objectID;

	ss.clear();

	x = (float)atof(data->Attribute("x"));
	y = (float)atof(data->Attribute("y"));
	angularVelocity = (float)atof(data->Attribute("angularVelocity"));

	bd.position.Set(RW2PW(x), RW2PW(y));

	body = world->CreateBody(&bd);

	Block* newBlock = new Block();

	std::string name = data->Attribute("name");
	std::string spriteType = "";

	if(name == "TopBlock"){
		spriteType = "TopBlock";
		newBlock->blockType = "top";

		//The 1 is so it is sorted correctly in the map, so everything renders on top of it
		ss << "A_TOP_BLOCK_" << count;
	}
	else{
		spriteType = "BottomBlock";
		newBlock->blockType = "bottom";

		ss << "A_BOTTOM_BLOCK_" << count;
	}

	ss >> objectID;

	ss.clear();

	shapefd.filter.categoryBits = entityCategory::BLOCK;

	if(newBlock->Initialize(x, y, 0, angularVelocity, linearForce, body, objectID, sprites->Search(spriteType))){
		
		if(spriteType == "TopBlock"){
			shapefd.shape = &edgeShape[1];
			body->CreateFixture(&shapefd);
		}
		/*else{
			shapefd.shape = &edgeShape[2];
			body->CreateFixture(&shapefd);

			shapefd.shape = &edgeShape[3];
			body->CreateFixture(&shapefd);
		}
*/
		body->SetAngularDamping(angularDamping);
		body->SetLinearDamping(linearDamping);
		body = NULL;
		return(newBlock);
	}
	else
		return(NULL);
}