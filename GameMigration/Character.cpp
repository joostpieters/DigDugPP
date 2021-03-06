
#include "Character.h"

#include "Core.h"
#include "Physics.h"
#include "World.h"
#include "Fill.h"
#include <time.h>
#include <string>
#include "Sound.h"
#include "Controls.h"

using Physics::Gravity;
using Physics::Point;
using Physics::Vector;
using Physics::Rectangle;
using Physics::operator+;
using Physics::operator-;

// Character //
Point Character::spawnPoint = Point(3, 3);
string Character::notification = "";
const float MAX_SPEED = BLOCK_SIZE / 10;

time_t lastMineMillis = time(nullptr) * 1000;

Character::Character(World& world)
{
	Character::world = &world;
	setLocation(spawnPoint);
	velocity = Vector(0, 0);
	isJumping = false;
	isOnLadder = false;
	pickStrength = 1;
	inventory.empty();
	notification = "";
	
	// Five ladders to start
	inventory.ladderCount = 5;
}
Physics::Rectangle Character::getBoundingBox()
{
	return Physics::Rectangle(Point(location.x + BLOCK_FIFTH, location.y + 2), Point(location.x + BLOCK_SIZE - BLOCK_FIFTH, location.y + BLOCK_SIZE - 2));
}

Point Character::getHeadPoint()
{
	Point p = getCenterPoint();
	return Point(p.x, p.y - (getBoundingBox().b.y - getBoundingBox().a.y) / 2);
}

Point Character::getCenterPoint()
{
	return getBoundingBox().getCenterPoint();
}

// Sets an x and y value based on the grid (BLOCK_SIZE)
void Character::setLocation(Point& spot)
{
	location.x = spot.x * BLOCK_SIZE;
	location.y = spot.y * BLOCK_SIZE;
}
void Character::reset()
{
	world->shouldFlash = true;
	Sound::playDeath();
	notification = "";
	inventory.empty();
	setLocation(spawnPoint);
}

void Character::jump()
{
	if (!isJumping)
	{
		isJumping = true;
		velocity.y = -sqrt(2 * Gravity::acceleration * BLOCK_SIZE);
	}
}

void Character::mine(int dir)
{		

	Physics::Rectangle* box = &getBoundingBox();
	Point& p = getCenterPoint();

	Block* b;
	switch (dir)
	{
	case 0:
		//down
		b = &(world->getBlockAt(Point(p.x, box->b.y + MAX_SPEED - 1)));
		break;
	case 1:
		//right
		b = &(world->getBlockAt(Point(box->a.x - MAX_SPEED + 1, p.y)));
	
		break;
	case 2:
		//up
		b = &(world->getBlockAt(Point(p.x, box->a.y - MAX_SPEED + 1)));
		break;
	case 3:
		//left
		b = &(world->getBlockAt(Point(box->b.x + MAX_SPEED, p.y)));
		break;
	default:
		b = nullptr;
		break;
	}

	BreakableBlock* b2 = dynamic_cast<BreakableBlock*>(b);
	if (b2 != nullptr)
	{
		int dura = b2->takeDamage(pickStrength);
		if (dura <= 0)
		{
			world->destroyBlockAt(b->a);
		} 
		else if (dura == 101)
		{
			notification = "My pick is too weak!";
		}
	}

	
}

// Used for collision detection
Block* neighbor1;
Block* neighbor2;
Physics::Rectangle* box;
// Step forward horizontally, then resolve collisions
// Do not let the character leave the world
void Character::moveX()
{
	// Move
	location.x += velocity.x;

	// Resolve collisions
	box = &getBoundingBox();
	if (velocity.x < 0) // moving left
	{
		neighbor1 = &(world->getBlockAt(Point(box->a.x, box->a.y)));
		neighbor2 = &(world->getBlockAt(Point(box->a.x, box->b.y)));
		if (!neighbor1->isTraversable || !neighbor2->isTraversable) // collision incoming
		{
			velocity.x = 0;
			location.x += neighbor1->b.x - box->a.x;
		}
		else if (box->a.x < 0) location.x += -box->a.x;
	}
	else if (velocity.x > 0) // moving right
	{
		neighbor1 = &(world->getBlockAt(Point(box->b.x, box->a.y)));
		neighbor2 = &(world->getBlockAt(Point(box->b.x, box->b.y)));
		if (!neighbor1->isTraversable || !neighbor2->isTraversable)
		{
			velocity.x = 0;
			location.x += neighbor1->a.x - box->b.x - 1;
		}
		else if (box->b.x > world->blocks.size() * BLOCK_SIZE) location.x += -(box->b.x - world->blocks.size() * BLOCK_SIZE);
	}
}
// Step forward vertically, then resolve collisions
// Do not let the character leave the bottom of the world
// Kill the character if the fall is too great
void Character::moveY()
{
	// Move
	location.y += velocity.y;

	// Resolve collisions
	box = &getBoundingBox();
	if (velocity.y > 0) // moving down
	{
		isJumping = true; // If the character is falling he should not be able to jump

		neighbor1 = &(world->getBlockAt(Point(box->a.x, box->b.y + velocity.y)));
		neighbor2 = &(world->getBlockAt(Point(box->b.x, box->b.y + velocity.y))); // why do these "+ velocity.y" need to be there
		if (!neighbor1->isTraversable || !neighbor2->isTraversable)
		{
			isJumping = false;
			if (velocity.y >= Gravity::deathVelocity)
			{
				reset();
				velocity.y = 0;
				return;
			}
			velocity.y = 0;
			location.y += neighbor1->a.y - box->b.y - 1;
		}
		else if (box->b.y > world->blocks[0].size() * BLOCK_SIZE) location.y += -(box->b.y - world->blocks[0].size() * BLOCK_SIZE);
	}
	else if (velocity.y < 0) // moving up
	{
		neighbor1 = &(world->getBlockAt(Point(box->a.x, box->a.y)));
		neighbor2 = &(world->getBlockAt(Point(box->b.x, box->a.y)));
		if (!neighbor1->isTraversable || !neighbor2->isTraversable)
		{
			velocity.y = 0;
			location.y += neighbor1->b.y - box->a.y;
		}
		// No need for boundaries with the top of the screen
	}
}

void Character::placeLadder()
{
	if (inventory.ladderCount > 0)
	{
		if (!isJumping)
		{
			// This will lower the ladderCount
			world->placeLadderAt(getHeadPoint());
		}
	}
	else notification = "Out of ladders.";
}

void Character::checkKeyInput()
{
	if (keyPressed(DIE)) reset();

	if (keyPressed(PLACE_LADDER)) placeLadder();
	
	if (keyPressed(MINE_MODE))
	{
		velocity.x = 0;
		if (isOnLadder) velocity.y = 0;
		if (!isJumping)
		{
			if (keyPressed(MOVE_DOWN)) mine(0);
			else if (keyPressed(MOVE_RIGHT)) mine(3);
			else if (keyPressed(MOVE_LEFT)) mine(1);
			else if (keyPressed(MOVE_UP)) mine(2);
		}
	}
	else
	{
		if (keyPressed(MOVE_LEFT)) velocity.x = -MAX_SPEED;
		else if (keyPressed(MOVE_RIGHT)) velocity.x = MAX_SPEED;
		else velocity.x = 0;

		Block* b = &world->getBlockAt(getCenterPoint());
		Ladder* ladder = dynamic_cast<Ladder*>(b);
		isOnLadder = (ladder != nullptr);
		// If on a ladder, w and s move up and down the ladder...
		if (isOnLadder)
		{
			isJumping = false;
			if (keyPressed(MOVE_UP)) velocity.y = -MAX_SPEED;
			else if (keyPressed(MOVE_DOWN)) velocity.y = MAX_SPEED;
			else velocity.y = 0;
		}
		// Else, the s key does nothing and w jumps
		else if (keyPressed(MOVE_UP)) jump();
	}
	
	if (keyPressed(OPEN_SHOP))
	{
		Point& p = getCenterPoint();
		Physics::Rectangle* box = &getBoundingBox();
		Block* b;
		b = &(world->getBlockAt(Point(box->a.x - MAX_SPEED+1, p.y)));
		StoreRight* s = dynamic_cast<StoreRight*>(b);
		if(s != nullptr) storeOpen = true, inventory.sellMinerals(), Sound::playSell();
	}
}

void Character::update()
{
	checkKeyInput();
	
	// Move the window if needed
	Window* window = &(world->window); // The larger, viewable screen
	Physics::Rectangle* deadzone = &(world->window.deadzone); // The invisible small box that the character cannot move out of
	Physics::Rectangle* character = &(getBoundingBox()); // The character's bounding box

	// If the character is outside the deadzone, shift the window (which also shifts the deadzone)
	if (character->a.x < deadzone->a.x) window->shift(Vector(character->a.x - deadzone->a.x, 0));
	else if (character->b.x > deadzone->b.x) window->shift(Vector(character->b.x - deadzone->b.x, 0));
	if (character->a.y < deadzone->a.y) window->shift(Vector(0, character->a.y - deadzone->a.y));
	else if (character->b.y > deadzone->b.y) window->shift(Vector(0, character->b.y - deadzone->b.y));

	// Move the character
	if (abs(velocity.x) > abs(velocity.y))
	{
		moveX();
		moveY();
	}
	else
	{
		moveY();
		moveX();
	}

	// Adjust for gravity
	if (!isOnLadder) velocity.y += Gravity::acceleration;
}

void Character::draw(Core::Graphics& g)
{
	drawAt(g, world->window.box.a);
}
void Character::drawAt(Core::Graphics& g, Vector& displacement)
{
	Point p = location - displacement;

	g.SetColor(RGB(28, 212, 52));

	fillSquare(g, Point(p.x + BLOCK_FIFTH * 2, p.y + BLOCK_FIFTH), BLOCK_FIFTH);
	fillSquare(g, Point(p.x + BLOCK_FIFTH * 2, p.y + BLOCK_FIFTH*2), BLOCK_FIFTH);

	g.SetColor(RGB(247, 214, 143));
	fillSquare(g, Point(p.x + 2 * BLOCK_FIFTH, p.y), BLOCK_FIFTH);

	// Hands
	if(velocity.x > 0)
	{
		fillSquare(g, Point(p.x + 4 * BLOCK_FIFTH, p.y + BLOCK_FIFTH), BLOCK_FIFTH);
		g.SetColor(RGB(28, 212, 52));
		fillSquare(g, Point(p.x + BLOCK_FIFTH * 3, p.y + BLOCK_FIFTH), BLOCK_FIFTH);
	} 
	else if (velocity.x < 0)
	{
		fillSquare(g, Point(p.x, p.y + BLOCK_FIFTH), BLOCK_FIFTH);
		g.SetColor(RGB(28, 212, 52));
		fillSquare(g, Point(p.x + BLOCK_FIFTH, p.y + BLOCK_FIFTH), BLOCK_FIFTH);
	} 
	else 
	{
		fillSquare(g, Point(p.x + BLOCK_FIFTH, p.y + BLOCK_FIFTH * 2), BLOCK_FIFTH);
		fillSquare(g, Point(p.x + 3 * BLOCK_FIFTH, p.y + BLOCK_FIFTH * 2), BLOCK_FIFTH);
		g.SetColor(RGB(28, 212, 52));
		fillSquare(g, Point(p.x + BLOCK_FIFTH * 3, p.y + BLOCK_FIFTH), BLOCK_FIFTH);
		fillSquare(g, Point(p.x + BLOCK_FIFTH, p.y + BLOCK_FIFTH), BLOCK_FIFTH);
	}
	g.SetColor(RGB(38, 88, 158));
	fillRectangle(g, Point(p.x + BLOCK_FIFTH * 2, p.y + BLOCK_FIFTH * 3), BLOCK_FIFTH, BLOCK_FIFTH * 2);
	//g.SetColor(RGB(10, 10, 255));

	// Draw the notifications
	static int count;
	if (count == 0)
	{
		if (notification != "") count = 90; // make this equal to framerate * 2? 2 seconds?
		// Then it's just been changed to some text, we need to show it
	}
	else
	{
		g.SetColor(RGB(255, 255, 255));
		g.DrawString(p.x, p.y + BLOCK_SIZE + BLOCK_FIFTH, notification.c_str());
		count--;
		// if we've just reached the end of the two seconds, set the string to ""
		if (count == 0) notification = "";
	}
	
	//getBoundingBox().draw(g);
}
