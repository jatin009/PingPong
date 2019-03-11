#pragma once

#include "Common.h"

class Ball
{
public:
	enum DIR
	{
		UPRIGHT,
		DOWNRIGHT,
		DOWNLEFT,
		UPLEFT
	};

	enum PACE
	{
		DEFAULT,
		X_ORIENTED,
		Y_ORIENTED
	};

	enum CHANGEDIR
	{		
		BARS,
		CEILING_FLOOR,
		REVERSE_CORNER
	};

	Ball(int x, int y, DIR dir);

	PingPong::IntPair getLocation() const { return m_location; }
	DIR getDirection() const { return m_direction; }

	void changePace(PACE pace);

	void changeDirection(CHANGEDIR changeCause);	//changing direction after reflection from ceiling, floor and bars
	void move();

private:
	DIR m_direction;
	PingPong::IntPair m_location;
	PingPong::IntPair m_pace;
};