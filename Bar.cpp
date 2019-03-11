#include "Bar.h"

Bar::Bar(int x, int y) : m_location(std::make_pair(x,y))
{
}

Bar::~Bar()
{
}

bool Bar::move(DIR dir)
{
	if (dir == UP)
	{
		m_location.second -= PingPong::BARDISPLACEMENT;
	}
	else if (dir == DOWN)
	{
		m_location.second += PingPong::BARDISPLACEMENT;
	}
	m_direction = dir;
	return true;
}
