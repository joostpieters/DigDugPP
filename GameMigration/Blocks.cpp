
#include "Blocks.h"
using namespace Blocks;

// Block //
void Block::setLocation(Point& pa)
{
	setPoints(pa, Point(pa.x + BLOCK_SIZE, pa.y + BLOCK_SIZE));
}

//Draw functions for each block
void Air::draw(Core::Graphics& g)
{
	// This draws black...
	// maybe we need a sky block for blue above ground
	// and a cave block for dark brown under ground
}
void Stone::draw(Core::Graphics &g)
{
	g.SetColor(RGB(121,122,122));
	fillSquare(g, a, BLOCK_SIZE);
}
void Earth::draw(Core::Graphics &g)
{
	g.SetColor(RGB(110,83,9));
	fillSquare(g, a, BLOCK_SIZE);
}
void Iron::draw(Core::Graphics &g)
{
	g.SetColor(RGB(199, 174, 141));
	fillSquare(g, a, BLOCK_SIZE);			
}
void Silver::draw(Core::Graphics &g)
{
	g.SetColor(RGB(207, 207, 207));
	fillSquare(g, a, BLOCK_SIZE);
}
void Copper::draw(Core::Graphics &g)
{
	g.SetColor(RGB(245, 151, 29));
	fillSquare(g, a, BLOCK_SIZE);
}