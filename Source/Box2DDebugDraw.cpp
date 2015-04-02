#include "Box2DDebugDraw.h"

Box2DDebugDraw::Box2DDebugDraw() {
	circleAccuracy = 9;
	camera = NULL;
}

Box2DDebugDraw::~Box2DDebugDraw() {
	_dev->Release();
	_dev = nullptr; 
}

bool Box2DDebugDraw::Initialize(LPDIRECT3DDEVICE9 device) {
	if(device == nullptr) return false;

	device->AddRef();
	_dev = device;
	return true;
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
	if(checkDevice()) {
		saveMatrix();
		//box2d stores its color components in a range of 0.0 to 1.0
		//box 2d only has r, g, and b but directx allows us to use alpha as well
		D3DCOLOR dxColor = D3DCOLOR(0x00000000);
		dxColor |= ((int)(128)			<< 24);	//a -- this is only about half opacity so we can see our sprites
		dxColor |= ((int)(color.r*255)	<< 16); //r
		dxColor |= ((int)(color.g*255)	<< 8);  //g
		dxColor |= ((int)(color.b*255)	<< 0);  //b

		//copy from the box2d data structure to the directx equivalent
		
		D3DXVECTOR2 *vertex = new D3DXVECTOR2[vertexCount+1];

		for(int i=0; i<vertexCount; i++) {
			vertex[i] = D3DXVECTOR2(PW2RW(vertices[i].x) + camera->GetCenter().x, PW2RW(vertices[i].y));
		}
		//we probably want a line connecting the first vertex with the last one
		vertex[vertexCount] = vertex[0]; //so draw a line from the last to the first

		LPD3DXLINE outline;
		D3DXCreateLine(_dev, &outline);
		outline->SetPattern(0xFFFFFFFF); //solid no dash
		outline->SetAntialias(true);
		outline->SetWidth(5.0);

		//draw using our new outline object
		outline->Begin();
		outline->Draw(vertex, vertexCount+1, dxColor);
		outline->End();
		outline->Release();

		delete[] vertex;
		restoreMatrix();
	}
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
	if(checkDevice()) {
		//directx has problems with n-vertex filled polygons so just draw it like an unfilled polygon
		DrawPolygon(vertices, vertexCount, color);
	}
}

void Box2DDebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) {
	if(checkDevice()) {
		//start from our center since we don't know where else to start from
		b2Vec2 previous = center;
		//ammount to increment around the circle
		float step = (float)((3.14159 * 2) / circleAccuracy);

		//run through a full circle
		for(float i=0; i<2 * 3.14159; i += step) {
			//our current rendering position
			b2Vec2 current(center.x + radius*cos(i), center.y + radius*sin(i));
			
			//draw a line from our last position to our current position
			DrawSegment(current, previous, color);

			//update our previous position
			previous = current;
		}

		//we still need a line from our last position to our starting position
		DrawSegment(previous, center, color);


	}
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {
	if(checkDevice()) {
		//directx has problems filling n-vertex polygons so draw this like the outside of a circle
		DrawCircle(center, radius, color);
	}
}

void Box2DDebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
	if(checkDevice()) {
		saveMatrix();
		//box2d stores its color components in a range of 0.0 to 1.0
		//box 2d only has r, g, and b but directx allows us to use alpha as well
		D3DCOLOR dxColor = D3DCOLOR(0x00000000);
		dxColor |= ((int)(128)			<< 24);	//a -- this is only about half opacity so we can see our sprites
		dxColor |= ((int)(color.r*255)	<< 16); //r
		dxColor |= ((int)(color.g*255)	<< 8);  //g
		dxColor |= ((int)(color.b*255)	<< 0);  //b

		//copy from the box2d data structure to the directx equivalent
		D3DXVECTOR2 *vertex = new D3DXVECTOR2[2];
		vertex[0] = D3DXVECTOR2(PW2RW(p1.x) + camera->GetCenter().x, PW2RW(p1.y));
		vertex[1] = D3DXVECTOR2(PW2RW(p2.x) + camera->GetCenter().x, PW2RW(p2.y));

		LPD3DXLINE outline;
		D3DXCreateLine(_dev, &outline);
		outline->SetPattern(0xffffffff); //solid no dash
		outline->SetAntialias(true);
		outline->SetWidth(3.0);

		//draw using our new outline object
		outline->Begin();

		outline->Draw(vertex, 2, dxColor);

		outline->End();

		outline->Release();

		delete[] vertex;
		restoreMatrix();
	}
}

void Box2DDebugDraw::DrawTransform(const b2Transform &xf) {
	if(checkDevice()) {
		saveMatrix();
		//lets start by breaking up the b2Transform into its parts for directx

		//box2d stores its color components in a range of 0.0 to 1.0
		//box 2d only has r, g, and b but directx allows us to use alpha as well
		D3DCOLOR dxColor = D3DCOLOR(0x00000000);
		dxColor |= ((int)(128)			<< 24);	//a -- this is only about half opacity so we can see our sprites
		dxColor |= ((int)(1*255)		<< 16); //r
		dxColor |= ((int)(0.5*255)		<< 8);  //g
		dxColor |= ((int)(0.25*255)		<< 0);  //b

		//copy from the box2d data structure to the directx equivalent
		
		D3DXVECTOR2 *vertex = new D3DXVECTOR2[2];
		vertex[0] = D3DXVECTOR2(PW2RW(xf.p.x) + camera->GetCenter().x, PW2RW(xf.p.y));
		vertex[1] = D3DXVECTOR2(PW2RW(xf.p.x + xf.q.c) + camera->GetCenter().x, PW2RW(xf.p.y + xf.q.s)); //c is the cos and s is the sin

		LPD3DXLINE outline;
		D3DXCreateLine(_dev, &outline);
		outline->SetPatternScale(2.0);
		outline->SetPattern(0xffffffff); //solid no dash

		//draw using our new outline object
		outline->Begin();
		outline->Draw(vertex, 2, dxColor);
		outline->End();
		outline->Release();

		delete[] vertex;
		restoreMatrix();
	}
}

//true if device is good
//false if device is nullptr
bool Box2DDebugDraw::checkDevice() {
	return _dev != nullptr;
}

void Box2DDebugDraw::saveMatrix() {
	_dev->GetTransform(D3DTS_WORLD, &backupMatrix);
}

void Box2DDebugDraw::restoreMatrix() {
	_dev->SetTransform(D3DTS_WORLD, &backupMatrix);
}


/*
private:
	LPDIRECT3DDEVICE9* _dev;
*/