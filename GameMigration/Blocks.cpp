#include "Blocks.h"
using namespace Blocks;
using Physics::Vector;

// Measured in pixels
const int BLOCK_SIZE = 50;

// Block //
void Block::setLocation(Point& pa)
{
	setPoints(pa, Point(pa.x + BLOCK_SIZE, pa.y + BLOCK_SIZE));
}

//Draw functions for each block
void Sky::drawAt(Core::Graphics& g, Vector& offset)
{
	Point p = a - offset;
	g.SetColor(RGB(120, 207, 227));
	fillSquare(g, p, BLOCK_SIZE);
}
void Cave::drawAt(Core::Graphics& g, Vector& offset)
{
	Point p = a - offset;
	g.SetColor(RGB(41, 24, 2));
	fillSquare(g, p, BLOCK_SIZE);
}
void Ladder::drawAt(Core::Graphics& g, Vector& offset)
{
	Point p = a - offset;
	g.SetColor(RGB(41, 24, 2));
	fillSquare(g, p, BLOCK_SIZE);
	g.SetColor(RGB(204, 151, 6));
	//Rails
	fillRectangle(g, Point(p.x + BLOCK_FIFTH, p.y), BLOCK_FIFTH, BLOCK_SIZE);
	fillRectangle(g, Point(p.x + 3*BLOCK_FIFTH, p.y), BLOCK_FIFTH, BLOCK_SIZE);
	//Rungs
	fillRectangle(g, Point(p.x + 2*BLOCK_FIFTH, p.y + BLOCK_SIXTH), BLOCK_FIFTH, BLOCK_SIXTH);
	fillRectangle(g, Point(p.x + 2*BLOCK_FIFTH, p.y + 3*BLOCK_SIXTH), BLOCK_FIFTH, BLOCK_SIXTH);
	fillRectangle(g, Point(p.x + 2*BLOCK_FIFTH, p.y + 5*BLOCK_SIXTH), BLOCK_FIFTH, BLOCK_SIXTH);
}
void Stone::drawAt(Core::Graphics &g, Vector& offset)
{
	Point p = a - offset;
	g.SetColor(RGB(121,122,122));
	fillSquare(g, p, BLOCK_SIZE);
	g.SetColor(RGB(102,102,102));
	fillSquare(g, Point(p.x + BLOCK_FIFTH, p.y + 2 * BLOCK_FIFTH), BLOCK_FIFTH * 2);
	g.SetColor(RGB(110, 110, 100));
	fillSquare(g, Point(p.x + 2* BLOCK_FIFTH, p.y + BLOCK_FIFTH), BLOCK_FIFTH * 2);
}
void Earth::drawAt(Core::Graphics &g, Vector& offset)
{
	Point p = a - offset;
	g.SetColor(RGB(110,83,9));
	fillSquare(g, p, BLOCK_SIZE);
	drawCracks(g, p, durability);
}
void Earth2::drawAt(Core::Graphics &g, Vector& offset)
{
	Point p = a - offset;
	g.SetColor(RGB(121, 122, 122));
	fillSquare(g, p, BLOCK_SIZE);
	drawCracks(g, p, durability);
}
void Grass::drawAt(Core::Graphics &g, Vector& offset)
{
	Point p = a - offset;
	g.SetColor(RGB(110,83,9));
	fillSquare(g, p, BLOCK_SIZE);
	g.SetColor(RGB(20, 219, 2));
	fillRectangle(g, p, BLOCK_SIZE, BLOCK_FIFTH);
	drawCracks(g, p, durability);
}
void Iron::drawAt(Core::Graphics &g, Vector& offset)
{
	Point p = a - offset;
	g.SetColor(RGB(121,122,122));
	fillSquare(g, p, BLOCK_SIZE);
	g.SetColor(RGB(199, 174, 141));
	fillRectangle(g, Point(p.x + BLOCK_FIFTH, p.y + BLOCK_FIFTH), 2*BLOCK_FIFTH, BLOCK_FIFTH);
	fillSquare(g, Point(p.x + 3*BLOCK_FIFTH, p.y + 2*BLOCK_FIFTH), BLOCK_FIFTH);
	fillSquare(g, Point(p.x + 2*BLOCK_FIFTH, p.y + 3 * BLOCK_FIFTH), BLOCK_FIFTH);		
	drawCracks(g, p, durability);
}
void Silver::drawAt(Core::Graphics &g, Vector& offset)
{
	Point p = a - offset;
	g.SetColor(RGB(121,122,122));
	fillSquare(g, p, BLOCK_SIZE);
	g.SetColor(RGB(207, 207, 207));
	fillRectangle(g, Point(p.x + BLOCK_FIFTH, p.y + BLOCK_FIFTH), 2*BLOCK_FIFTH, BLOCK_FIFTH);
	fillSquare(g, Point(p.x + 2* BLOCK_FIFTH, p.y + 2 * BLOCK_FIFTH), BLOCK_FIFTH);
	fillSquare(g, Point(p.x + 3* BLOCK_FIFTH, p.y + 3 * BLOCK_FIFTH), BLOCK_FIFTH);
	drawCracks(g, a, durability);
}
void Copper::drawAt(Core::Graphics &g, Vector& offset)
{
	g.SetColor(RGB(121,122,122));
	Point p = a - offset;
	fillSquare(g, p, BLOCK_SIZE);
	g.SetColor(RGB(245, 151, 29));
	fillSquare(g, Point(p.x + BLOCK_FIFTH, p.y + BLOCK_FIFTH), BLOCK_FIFTH);
	fillSquare(g, Point(p.x + 3 * BLOCK_FIFTH, p.y + 2 * BLOCK_FIFTH), BLOCK_FIFTH);
	fillSquare(g, Point(p.x + BLOCK_FIFTH, p.y +  3* BLOCK_FIFTH), BLOCK_FIFTH);
	drawCracks(g, a, durability);

}
void Sapphire::draw(Core::Graphics& g)
{
	g.SetColor(RGB(121,122,122));
	fillSquare(g, a, BLOCK_SIZE);
	g.SetColor(RGB(34, 27, 227));
	fillCircle(g, Point(a.x + BLOCK_FIFTH, a.y + BLOCK_FIFTH * 3), BLOCK_FIFTH); 
	fillCircle(g, Point(a.x + BLOCK_FIFTH, a.y + BLOCK_FIFTH), BLOCK_FIFTH);
	fillCircle(g, Point(a.x + BLOCK_FIFTH*2, a.y + BLOCK_FIFTH*2), BLOCK_FIFTH*2);
	drawCracks(g, a, durability);
}

void BreakableBlock::drawCracks(Core::Graphics& g, Point &p, int durability)
{
	if (durability < 100)
	{
		//A little damaged
		g.SetColor(RGB(0,0,0));
		g.DrawLine(p.x + 2*BLOCK_FIFTH, p.y + BLOCK_FIFTH, p.x + 2*BLOCK_FIFTH, p.y + 2*BLOCK_FIFTH);
		g.DrawLine(p.x + 2*BLOCK_FIFTH, p.y + 2*BLOCK_FIFTH, p.x + 3*BLOCK_FIFTH, p.y + 2*BLOCK_FIFTH);
		g.DrawLine(p.x + 3*BLOCK_FIFTH, p.y + 2*BLOCK_FIFTH, p.x + 3*BLOCK_FIFTH, p.y + 4*BLOCK_FIFTH);
		if (durability < 50)
		{
			//more damaged
			g.DrawLine(p.x + BLOCK_FIFTH, p.y + 2*BLOCK_FIFTH, p.x + 2 * BLOCK_FIFTH, p.y + 2*BLOCK_FIFTH);
			g.DrawLine(p.x + 2*BLOCK_FIFTH, p.y + 2*BLOCK_FIFTH, p.x + 2*BLOCK_FIFTH, p.y + 3*BLOCK_FIFTH);
			g.DrawLine(p.x + 3*BLOCK_FIFTH, p.y + 3*BLOCK_FIFTH, p.x + 4*BLOCK_FIFTH, p.y + 3*BLOCK_FIFTH);
			if (durability < 25)
			{
				//very damaged
				g.DrawLine(p.x, p.y + 2*BLOCK_FIFTH, p.x + 4*BLOCK_FIFTH, p.y + 2*BLOCK_FIFTH);
				g.DrawLine(p.x + BLOCK_FIFTH, p.y + 3*BLOCK_FIFTH, p.x + 2*BLOCK_FIFTH, p.y + 3*BLOCK_FIFTH);
				g.DrawLine(p.x + 2*BLOCK_FIFTH, p.y, p.x + 2*BLOCK_FIFTH, p.y + 4*BLOCK_FIFTH);
				g.DrawLine(p.x + 3*BLOCK_FIFTH, p.y + BLOCK_FIFTH, p.x + 3*BLOCK_FIFTH, + p.y + 5*BLOCK_FIFTH);
				g.DrawLine(p.x +3*BLOCK_FIFTH, p.y + 3* BLOCK_FIFTH, p.x + 5* BLOCK_FIFTH, p.y + 3*BLOCK_FIFTH);
				g.DrawLine(p.x + 4*BLOCK_FIFTH, p.y + BLOCK_FIFTH, p.x + 4*BLOCK_FIFTH, p.y+ 2*BLOCK_FIFTH);


			}
		}
	}
}

int BreakableBlock::takeDamage(int damage)
{
	durability -= damage;
	return durability;
}