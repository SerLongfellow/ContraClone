#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include <Box2D\Box2D.h>
#include <iostream>
#include "Actor.h"

class ContactListener: public b2ContactListener {

public:

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif