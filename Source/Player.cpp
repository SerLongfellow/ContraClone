#include "Player.h"

Player::Player(){
	timeSinceBulletShot = 0.1f;
	timeSinceUpdateAnim = 1.0f;
	timeSinceJumped = 0.3f;
	invincibleTime = 0.0f;
	timeSinceLanded = 0.5f;
	cameraOffsetX = 0.0f;
	objectType = "Player";
	state = 1;
	lastState = 0;
	animation = NULL;
}

Player::~Player(){

}

void Player::Update(float gameTime, InputDevice* inputDevice, ActorVector* objectsToAdd){

	timeSinceBulletShot += gameTime;
	timeSinceJumped += gameTime;

	//Make sure player hasn't fallen in gap
	if(body->GetPosition().y > RW2PW(SCREEN_HEIGHT) || health <= 0){
		isDead = true;
	}

	if(invincibleTime > 0.0f){
		invincibleTime -= gameTime;
	}

	b2Vec2 vel = body->GetLinearVelocity();

	lastState = state;

	if(vel.y <= RW2PW(1.0f) && vel.y >= RW2PW(-1.0f)){
		if(state == JUMP_LEFT){
			state = STILL_LEFT;
			timeSinceLanded += gameTime;
		}
		else if(state == JUMP_RIGHT){
			state = STILL_RIGHT;
			timeSinceLanded += gameTime;
		}
	}
	else{
		if(state != JUMP_LEFT && state != JUMP_RIGHT){
			if(state == RUN_RIGHT || state == STILL_RIGHT){
				state = JUMP_RIGHT;
			}
			else{
				state = JUMP_LEFT;
			}
		}
	}

	if(inputDevice->IsWPressed()){
		if(timeSinceJumped > 0.3f && timeSinceLanded >= 0.05f){
			if(state != JUMP_LEFT && state != JUMP_RIGHT){
				//Jump

				/*miter = sounds->find("jump");
				if(miter != sounds->end()){
					miter->second->PlaySoundA(DSBVOLUME_MAX, 0);
				}*/

				sounds->PlaySoundA("jump", DSBVOLUME_MAX, 0);

				b2Vec2 force(0.0f, -130.0f);

				body->ApplyForceToCenter(force, true);

				if(state == STILL_LEFT || state == RUN_LEFT){
					state = JUMP_LEFT;
				}
				else if(state == STILL_RIGHT || state == RUN_RIGHT){
					state = JUMP_RIGHT;
				}

				timeSinceJumped = 0.0f;
				timeSinceLanded = 0.0f;
			}
		}
	}

	if(inputDevice->IsAPressed()){
		//Move left
		b2Vec2 vel = body->GetLinearVelocity();

		if(body->GetPosition().x > cameraOffsetX + RW2PW(20)){			
			vel.x = -2.0f;
			body->SetLinearVelocity(vel);

			if(state == STILL_LEFT || state == STILL_RIGHT || state == RUN_RIGHT){
				state = RUN_LEFT;
				timeSinceLanded += gameTime;
			}
			else if(state == JUMP_RIGHT){
				state = JUMP_LEFT;
			}
		}
		else{
			vel.x = 0.0f;
			body->SetLinearVelocity(vel);
		}
	}

	if(inputDevice->IsDPressed()){
		//Move right
		b2Vec2 vel = body->GetLinearVelocity();
		vel.x = 2.0f;
		body->SetLinearVelocity(vel);

		//if(state == STILL_LEFT || state == STILL_RIGHT){
		if(state != JUMP_LEFT && state != JUMP_RIGHT){
			state = RUN_RIGHT;
			timeSinceLanded += gameTime;
		}
		else if(state == JUMP_LEFT){
			state = JUMP_RIGHT;
		}
	}

	if(!inputDevice->IsDPressed() && !inputDevice->IsAPressed()){
		//Stop moving
		b2Vec2 vel = body->GetLinearVelocity();
		vel.x = 0.0f;
		body->SetLinearVelocity(vel);

		if(state == RUN_LEFT){
			state = STILL_LEFT;
		}
		else if(state == RUN_RIGHT){
			state = STILL_RIGHT;
		}

		if(state != JUMP_LEFT && state != JUMP_RIGHT){
			timeSinceLanded += gameTime;
		}
	}

	if(inputDevice->IsSpacePressed()){

		//If time elapsed since the last bullet shot is 10 milliseconds
		if(timeSinceBulletShot > 0.25f){

			/*miter = sounds->find("shoot");
			if(miter != sounds->end()){
				miter->second->PlaySoundA(DSBVOLUME_MAX, 0);
			}*/

			sounds->PlaySoundA("shoot", DSBVOLUME_MAX, 0);

			objectToAdd newBullet;

			newBullet.objectType = "Bullet";

			b2Vec2 playerPos(body->GetPosition());

			b2Vec2 vel(0.0f,0.0f);

			if(state == STILL_RIGHT || state == RUN_RIGHT || state == JUMP_RIGHT){
				newBullet.position.x = PW2RW(playerPos.x) + 15.0f;
				vel.x = 3.5f;
			}
			else if(state == STILL_LEFT || state == RUN_LEFT || state == JUMP_LEFT){
				newBullet.position.x = PW2RW(playerPos.x) - 15.0f;
				vel.x = -3.5f;
			}

			newBullet.position.y = PW2RW(playerPos.y) - 8.0f;
			newBullet.position.z = 0.0f;

			newBullet.angle = body->GetAngle();

			newBullet.vel = vel;

			objectsToAdd->push_back(newBullet);

			timeSinceBulletShot = 0.0f;
		}
	}
}

void Player::Draw(float gameTime, Camera* camera){
	timeSinceUpdateAnim += gameTime;

	if(animation && (timeSinceUpdateAnim > 0.1f || lastState != state)){
		animation->UpdateAnimation(state, srcRect, body, shapefd);
		timeSinceUpdateAnim = 0.0f;
	}

	if( (invincibleTime <= 0.0f) ||
		(invincibleTime >= 0.0f && invincibleTime <= 0.25f) ||
		(invincibleTime >= 0.5f && invincibleTime <= 0.75f) ||
		(invincibleTime >= 1.0f && invincibleTime <= 1.25f) ||
		(invincibleTime >= 1.5f && invincibleTime <= 1.75f)){

			if(sprite){	
				b2Vec2 pos = body->GetPosition();	

				D3DXVECTOR3 position;
				position.x = PW2RW(pos.x)-animation->GetWidth()/2.0f;
				position.y = PW2RW(pos.y)-animation->GetHeight()/2.0f;
				position.z = 0.0f;

				//Get the angle
				float angle = body->GetAngle();

				//Draw the sprite
				//sprite->Draw(gameTime, position, D3DXVECTOR3(RW2PW(pos.x), RW2PW(pos.y), 0.0f), angle, camera, &srcRect);
				sprite->Draw(gameTime, position, D3DXVECTOR3(PW2RW(pos.x), PW2RW(pos.y), 0.0f), angle, camera,  &srcRect);
			}
	}

	cameraOffsetX = RW2PW(-camera->GetCenter().x);
}
