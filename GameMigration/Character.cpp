
#include "Character.h"

#include "Core.h"
#include "Physics.h"
#include "World.h"

using Physics::Gravity;
using Physics::Point;
using Physics::Vector;
using Physics::Rectangle;

// Character //
Character::Character()
{
	location = Point(0, 0);
	velocity = Vector(0, 0);
	acceleration = Vector(0, 0);
	isColliding = false;
	isJumping = false;
}
Character::Character(World* w, float x, float y)
{
	world = w;
	location = Point(x, y);
	velocity = Vector(0, 0);
	acceleration = Vector(0, 0);
	isColliding = false;
	isJumping = false;
}
Physics::Rectangle Character::getBoundingBox()
{
	return Physics::Rectangle(location, Point(location.x + BLOCK_SIZE, location.y + BLOCK_SIZE));
}
void Character::checkKeyInput()
{
	float pushAcceleration = 0.2f;

	if (Core::Input::IsPressed(Core::Input::KEY_LEFT)) acceleration.x = -pushAcceleration;
	else if (Core::Input::IsPressed(Core::Input::KEY_RIGHT)) acceleration.x = pushAcceleration;
	else acceleration.x = 0;

	if (Core::Input::IsPressed(Core::Input::KEY_UP)) acceleration.y = -pushAcceleration;
	else if (Core::Input::IsPressed(Core::Input::KEY_DOWN)) acceleration.y = pushAcceleration;
	else acceleration.y = 0;
}
void Character::move()
{
	checkKeyInput();

	// Adjust for gravity
	velocity.y += Gravity::acceleration;
	// and friction
	acceleration.x += -0.02 * velocity.x;
	
	// Adjust for own fluid motion
	velocity.x += acceleration.x;
	velocity.y += acceleration.y;

	// Move
	location += velocity;
}
void Character::update()
{
	// check for collision
	move();
}
void Character::draw(Core::Graphics g)
{
	// draw image eventually
	g.SetColor(RGB(10, 10, 255));
	getBoundingBox().draw(g);
	g.SetColor(RGB(255, 255, 255));
}
