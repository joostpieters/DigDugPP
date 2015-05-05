#ifndef STORE_H
#define STORE_H

#include "World.h"
#include "Character.h"

struct Store 
{
	void draw(Core::Graphics&);
	void update();
	void addLadder();
	void upgradePick();
	void upgradeBag();
};

#endif