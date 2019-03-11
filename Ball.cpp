#include "Ball.h"

Ball::Ball(int x, int y, DIR dir): m_location (std::make_pair(x, y) ), m_direction (dir), m_pace(std::make_pair<int, int>(2,2))
{
}

void Ball::changeDirection(CHANGEDIR changeCause)
{
	if (changeCause == BARS)
	{
		if (m_direction == DOWNRIGHT)
			m_direction = DOWNLEFT;
		else if (m_direction == DOWNLEFT)
			m_direction = DOWNRIGHT;
		else if (m_direction == UPRIGHT)
			m_direction = UPLEFT;
		else if (m_direction == UPLEFT)
			m_direction = UPRIGHT;
	}
	else if (changeCause == CEILING_FLOOR)
	{
		if (m_direction == DOWNRIGHT)
			m_direction = UPRIGHT;
		else if (m_direction == DOWNLEFT)
			m_direction = UPLEFT;
		else if (m_direction == UPRIGHT)
			m_direction = DOWNRIGHT;
		else if (m_direction == UPLEFT)
			m_direction = DOWNLEFT;
	}	
	else if (changeCause == REVERSE_CORNER)
	{
		if (m_direction == DOWNRIGHT)
			m_direction = UPLEFT;
		else if (m_direction == DOWNLEFT)
			m_direction = UPRIGHT;
		else if (m_direction == UPRIGHT)
			m_direction = DOWNLEFT;
		else if (m_direction == UPLEFT)
			m_direction = DOWNRIGHT;
	}
}

void Ball::changePace(PACE pace)
{
	switch (pace)
	{
	case DEFAULT:
		m_pace.first = 1;
		m_pace.second = 1;
		break;
	case X_ORIENTED:
		m_pace.first = 2;
		m_pace.second = 1;
		break;
	case Y_ORIENTED:
		m_pace.first = 1;
		m_pace.second = 2;
		break;
	}
}

void Ball::move()
{
	switch (m_direction)
	{
	case DOWNRIGHT:
		m_location.first += m_pace.first;
		m_location.second += m_pace.second;
		break;
	case UPRIGHT:
		m_location.first += m_pace.first;
		m_location.second -= m_pace.second;
		break;
	case UPLEFT:
		m_location.first -= m_pace.first;
		m_location.second -= m_pace.second;
		break;
	case DOWNLEFT:
		m_location.first -= m_pace.first;
		m_location.second += m_pace.second;
		break;
	}
}