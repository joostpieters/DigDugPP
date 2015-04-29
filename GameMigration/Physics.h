
#ifndef PHYSICS_H
#define PHYSICS_H

#include "Core.h"

namespace Physics
{
	struct Gravity
	{
		static float acceleration;
	};

	// In a typical convention, this would be called Vector2f (vector - 2d - float)
	struct Vector
	{
		float x;
		float y;
		Vector();
		Vector(float nx, float ny);
		
		void operator=(const Vector& v);
		void operator+=(const Vector& right);
	};

	// They hold the same data, but have different meaning:
	// Point = location
	// Vector = displacement
	typedef Vector Point; 
	
	struct Line
	{
		Point a;
		Point b;

		Line();
		Line(Point pa, Point pb);
	};

	/*
		A - upper left
		B - lower right
	*/
	struct Rectangle
	{
		Point a, b;
		Rectangle(Point pa, Point pb);

		bool contains(Point& p);
		bool intersects(Rectangle& r);

		void draw(Core::Graphics& g);
	};

	struct Triangle
	{
		Point lowerLeft;
		Point top;
		Point lowerRight;

		Vector velocity;
		Vector acceleration;

		bool isColliding;

		Triangle(Point pLowerLeft, Point pTop, Point pLowerRight);

		Rectangle getBoundingBox();
		void draw(Core::Graphics& g);
		void move();
		//void resolveCollision(Line& edge);
	};

	class Character
	{
	public:
		Point location;
		Vector velocity;
		Vector acceleration;
		bool isColliding;
		bool isJumping;

		Character();
		Character(Point&);

		Rectangle getBoundingBox();
		void move();
		void draw(Core::Graphics);
	};
}

#endif