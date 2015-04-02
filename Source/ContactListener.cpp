#include "ContactListener.h"
#include "Player.h"

void ContactListener::BeginContact(b2Contact* contact){
}

void ContactListener::EndContact(b2Contact* contact){}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
	//Handle pre-solve event 
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	void* bodyAUserData = bodyA->GetUserData();
	void* bodyBUserData = bodyB->GetUserData();

	Actor* actorA = NULL;
	Actor* actorB = NULL;

	if(bodyAUserData){
		actorA = static_cast<Actor*>(bodyAUserData);
	}
	if(bodyBUserData){
		actorB = static_cast<Actor*>(bodyBUserData);
	}

	if(actorA && actorB){

		if(actorA->GetObjectType() == "Infantry" && actorB->GetObjectType() == "Player"){
			contact->SetEnabled(false);

			Player* player = static_cast<Player*>(bodyBUserData);

			if(player->invincibleTime <= 0.0f){
				player->health--;
				player->invincibleTime = 1.49f;
			}
		}
		else if(actorA->GetObjectType() == "Player" && actorB->GetObjectType() == "Infantry"){
			contact->SetEnabled(false);

			Player* player = static_cast<Player*>(bodyAUserData);

			if(player->invincibleTime <= 0.0f){
				player->health--;
				player->invincibleTime = 1.49f;
			}
		}
		else if(actorA->GetObjectType() == "Player" && actorB->GetObjectType() == "Block"){
			

			Player* player = static_cast<Player*>(bodyAUserData);
			float yVel = player->GetBody()->GetLinearVelocity().y;

			if(yVel < -0.01f){
				contact->SetEnabled(false);
			}
		}
		else if(actorA->GetObjectType() == "Block" && actorB->GetObjectType() == "Player"){

			Player* player = static_cast<Player*>(bodyBUserData);
			float yVel = player->GetBody()->GetLinearVelocity().y;

			if(yVel < -0.01f){
				contact->SetEnabled(false);
			}
		}
		else if((actorA->GetObjectType() == "Bullet" && actorB->GetObjectType() == "Infantry") || (actorA->GetObjectType() == "Infantry" && actorB->GetObjectType() == "Bullet")){
			actorA->SetDead(true);
			actorB->SetDead(true);
		}
		else if(actorA->GetObjectType() == "EnemyBullet" && actorB->GetObjectType() == "Player"){
			contact->SetEnabled(false);

			actorA->SetDead(true);
			
			Player* player = static_cast<Player*>(bodyBUserData);

			if(player->invincibleTime <= 0.0f){
				player->health--;
				player->invincibleTime = 1.49f;
			}
		}
		else if (actorA->GetObjectType() == "Player" && actorB->GetObjectType() == "EnemyBullet"){
			contact->SetEnabled(false);

			actorB->SetDead(true);
			
			Player* player = static_cast<Player*>(bodyAUserData);

			if(player->invincibleTime <= 0.0f){
				player->health--;
				player->invincibleTime = 1.49f;
			}
		}
	}

}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){}