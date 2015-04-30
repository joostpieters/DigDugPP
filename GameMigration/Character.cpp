
#include "Character.h"

#include "Core.h"
#include "Physics.h"
#include "World.h"

using Physics::Gravity;
using Physics::Point;
using Physics::Vector;
using Physics::Rectangle;
using Physics::operator+;

// Character //
const float MAX_VELOCITY = 5.0;
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

	if (Core::Input::IsPressed(Core::Input::KEY_UP)) jump();

	//if (Core::Input::IsPressed(Core::Input::KEY_UP)) acceleration.y = -pushAcceleration;
	//else if (Core::Input::IsPressed(Core::Input::KEY_DOWN)) acceleration.y = pushAcceleration;
	//else acceleration.y = 0;
}
void Character::jump()
{
	if (!isJumping)
	{
		isJumping = true;
		// This is hard coded right now... perhaps there is some calculation to jump exactly one block
		velocity.y = -4.0;
	}
}
void Character::move()
{
	checkKeyInput();

	// Adjust for gravity
	velocity.y += Gravity::acceleration;
	// and friction
	acceleration.x += -0.02 * velocity.x;
	
	// Adjust for own fluid motion
	if (velocity.x < MAX_VELOCITY) velocity.x += acceleration.x;
	else velocity.x = MAX_VELOCITY;
	velocity.y += acceleration.y;

	// Check for collisions then move
	Block* check;
	if (velocity.x < 0) // moving left
	{
		check = &(world->getBlockAt(Point(location.x - 1, location.y)));
		if (!check->isTraversable)
		{
			if (!(*check).intersects(getBoundingBox())) location.x += velocity.x;
			else 
			{
				velocity.x = 0;
				location.x = (*check).a.x + BLOCK_SIZE;
			}
		}
		else location.x += velocity.x;
	}
	else if (velocity.x > 0) // moving right
	{
		check = &(world->getBlockAt(Point(location.x + BLOCK_SIZE, location.y)));
		if (!check->isTraversable)
		{
			if (!(*check).intersects(getBoundingBox())) location.x += velocity.x;
			else 
			{
				velocity.x = 0;
				location.x = (*check).a.x - BLOCK_SIZE;
			}
		}
		else location.x += velocity.x;
	}

	if (velocity.y < 0) // moving up
	{
		check = &(world->getBlockAt(Point(location.x, location.y - 1)));
		if (!check->isTraversable)
		{
			if (!(*check).intersects(getBoundingBox())) location.y += velocity.y;
			else 
			{
				velocity.y = 0;
				location.y = (*check).a.y + BLOCK_SIZE;
			}
		}
		else location.y += velocity.y;
	}
	else if (velocity.y > 0) // moving down
	{
		check = &(world->getBlockAt(Point(location.x, location.y + BLOCK_SIZE + 1)));
		if (!check->isTraversable)
		{
			if (!(*check).intersects(getBoundingBox())) location.y += velocity.y;
			else 
			{
				isJumping = false;
				velocity.y = 0;
				location.y = (*check).a.y - BLOCK_SIZE;
			}
		}
		else location.y += velocity.y;
	}
}
void Character::update()
{
	move();
}
void Character::draw(Core::Graphics g)
{
	// draw image eventually
	g.SetColor(RGB(10, 10, 255));
	getBoundingBox().draw(g);
	g.SetColor(RGB(255, 255, 255));
}
