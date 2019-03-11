#pragma once
#include "Common.h"
#include <vector>

class Bar
{
public:

	enum DIR
	{
		UP,
		DOWN
	};

private:
	PingPong::IntPair m_location;
	DIR m_direction;

public:

	Bar(int x, int y);
	virtual ~Bar();

	DIR getDirection() const { return m_direction; }
	PingPong::IntPair getLocation() const { return m_location; }
	bool move(DIR dir);
};
