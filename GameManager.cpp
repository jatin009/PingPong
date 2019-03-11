#include "GameManager.h"
#include <iostream>
#include <Windows.h>
#include "Bar.h"
#include "Ball.h"
#include <thread>
#include "Common.h"
#include <thread>

//static int MINLOOPTIME = 45;
static int LOOPTIME = 60;
//static int MAXLOOPTIME = 75;
//static int INCLOOPTIME = 15;

static void setCursorPosition(int x, int y, int color = 15)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };

	SetConsoleCursorPosition(hOut, coord);	//jump cursor to this position
	SetConsoleTextAttribute(hOut, color);	//color console with this color

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hOut, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hOut, &cursorInfo);	//set cursor visibility
}

static void beepTone(int freq, int dur)
{
	Beep(freq, dur);
}

GameManager::GameManager() :m_GameOver(false), m_bar1(new Bar(3, 8)), m_bar2(new Bar(76, 8)), m_ball(new Ball(27, 8, Ball::UPRIGHT))
{
	/*setCursorPosition(0, 10);
	std::cout << "0, 0";
	setCursorPosition(79, 11);
	std::cout << "79,11";
	setCursorPosition(79, 1);
	std::cout << "79,11";
	setCursorPosition(40, 25);
	std::cout << "79,11";*/
}

GameManager::~GameManager()
{
	SAFE_DELETE(m_bar1);
	SAFE_DELETE(m_bar2);
	SAFE_DELETE(m_ball);
}

void GameManager::positionBars()
{
	static PingPong::IntPair prevBar1Pos = std::make_pair<int, int>(0, 0);
	static PingPong::IntPair prevBar2Pos = std::make_pair<int, int>(0, 0);
	PingPong::IntPair bar1Pos = m_bar1->getLocation();
	PingPong::IntPair bar2Pos = m_bar2->getLocation();

	auto lambda = [bar1Pos, bar2Pos](int i) {

		setCursorPosition(prevBar1Pos.first, prevBar1Pos.second + i, PingPong::BAR1COLOR);
		std::cout << PingPong::CLEARBLOCK;

		setCursorPosition(bar1Pos.first, bar1Pos.second + i, PingPong::BAR1COLOR);
		std::cout << PingPong::FULLBLOCK;

		setCursorPosition(prevBar2Pos.first, prevBar2Pos.second + i, PingPong::BAR2COLOR);
		std::cout << PingPong::CLEARBLOCK;

		setCursorPosition(bar2Pos.first, bar2Pos.second + i, PingPong::BAR2COLOR);
		std::cout << PingPong::FULLBLOCK;
	};

	if (bar1Pos.second > prevBar1Pos.second || bar2Pos.second > prevBar2Pos.second)
		for (int i = PingPong::BARLENGTH - 1; i >= 0; i--)
		{
			lambda(i);
		}
	else if (bar1Pos.second < prevBar1Pos.second || bar2Pos.second < prevBar2Pos.second)
		for (int i = 0; i < PingPong::BARLENGTH; i++)
		{
			lambda(i);
		}

	prevBar1Pos = bar1Pos;
	prevBar2Pos = bar2Pos;
}

void GameManager::drawGameArena(bool updateBarsPos)
{
	if (updateBarsPos)
	{
		positionBars();
	}
	static PingPong::IntPair prevBallPos = std::make_pair<int, int>(0, 0);
	PingPong::IntPair ballPos = m_ball->getLocation();
	
	setCursorPosition(prevBallPos.first, prevBallPos.second);
	std::cout << PingPong::CLEARBLOCK;
	setCursorPosition(ballPos.first, ballPos.second);
	std::cout << PingPong::BALL;

	prevBallPos = ballPos;
}

bool GameManager::inputMove()
{
	bool barmoved = false;
	if (GetAsyncKeyState(VK_UP))	//bar2 move up
	{
		if (m_bar2->getLocation().second == PingPong::ENDMARGIN) barmoved = false;
		else barmoved = m_bar2->move(Bar::UP);
	}
	if (GetAsyncKeyState(VK_DOWN))	//bar2 move down
	{
		if (m_bar2->getLocation().second == PingPong::CONSOLEHEIGHT - PingPong::BARLENGTH - PingPong::ENDMARGIN) barmoved = false;
		else barmoved = m_bar2->move(Bar::DOWN);
	}
	if (GetAsyncKeyState('Q') || GetAsyncKeyState('q'))	//bar1 move up
	{
		if (m_bar1->getLocation().second == PingPong::ENDMARGIN) barmoved = false;
		else barmoved = m_bar1->move(Bar::UP);
	}
	if (GetAsyncKeyState('A') || GetAsyncKeyState('a'))	//bar1 move down
	{
		if (m_bar1->getLocation().second == PingPong::CONSOLEHEIGHT - PingPong::BARLENGTH - PingPong::ENDMARGIN) barmoved = false;
		else barmoved = m_bar1->move(Bar::DOWN);
	}
	return barmoved;
}

void GameManager::logic(bool barinmotion)
{
	PingPong::IntPair ballPos = m_ball->getLocation();
	PingPong::IntPair bar1Pos = m_bar1->getLocation();
	PingPong::IntPair bar2Pos = m_bar2->getLocation();

	static PingPong::IntPair ballPace;
	ballPace.first = 2; ballPace.second = 2;	

	if (ballPos.first >= PingPong::RIGHTEND || ballPos.first <= PingPong::LEFTEND)	//reflection from side walls
	{
		m_GameOver = true;
	}	
	else if ((ballPos.second >= bar1Pos.second && ballPos.second <= bar1Pos.second + PingPong::BARLENGTH) && 
		(ballPos.first == bar1Pos.first + ballPace.first) )	//collision with bar1
	{		
		std::thread th(beepTone, 600, 80);	//playing sound asynchronously
		th.detach();
		/*if (barinmotion)
		{
			if (m_ball->getDirection() == Ball::DOWNLEFT && m_bar1->getDirection() == Bar::DOWN)
			{
				m_ball->changePace(Ball::Y_ORIENTED);	//slowing down the ball
				ballPace.first = 1; ballPace.second = 2;
				if (LOOPTIME < MAXLOOPTIME)
					LOOPTIME += INCLOOPTIME;
			}
			else if (m_ball->getDirection() == Ball::UPLEFT && m_bar1->getDirection() == Bar::DOWN)
			{
				m_ball->changePace(Ball::X_ORIENTED);	//pacing up the ball
				ballPace.first = 2; ballPace.second = 1;
				if (LOOPTIME > MINLOOPTIME)
					LOOPTIME -= INCLOOPTIME;
			}
			else if (m_ball->getDirection() == Ball::DOWNLEFT && m_bar1->getDirection() == Bar::UP)
			{
				m_ball->changePace(Ball::X_ORIENTED);	//pacing up the ball
				ballPace.first = 2; ballPace.second = 1;
				if (LOOPTIME > MINLOOPTIME)
					LOOPTIME -= INCLOOPTIME;
			}
			else if (m_ball->getDirection() == Ball::UPLEFT && m_bar1->getDirection() == Bar::UP)
			{
				m_ball->changePace(Ball::Y_ORIENTED);	//slowing down the ball
				ballPace.first = 1; ballPace.second = 2;
				if (LOOPTIME < MAXLOOPTIME)
					LOOPTIME += INCLOOPTIME;
			}
		}
		*/
		if (ballPos.second >= PingPong::BOTTOMEND || ballPos.second <= PingPong::TOPEND)	//collision right at corner
			m_ball->changeDirection(Ball::REVERSE_CORNER);
		else
			m_ball->changeDirection(Ball::BARS);
	}
	else if ((ballPos.second >= bar2Pos.second && ballPos.second <= bar2Pos.second + PingPong::BARLENGTH) && 
		(ballPos.first == bar2Pos.first - ballPace.first))	//collision with bar2
	{
		std::thread th(beepTone, 500, 80);	//playing sound asynchronously
		th.detach();
		/*if (barinmotion)
		{
			if (m_ball->getDirection() == Ball::DOWNRIGHT && m_bar2->getDirection() == Bar::DOWN)
			{
				m_ball->changePace(Ball::Y_ORIENTED);	//slowing down the ball
				ballPace.first = 1; ballPace.second = 2;
				if (LOOPTIME < MAXLOOPTIME)
					LOOPTIME += INCLOOPTIME;
			}
			else if (m_ball->getDirection() == Ball::UPRIGHT && m_bar2->getDirection() == Bar::DOWN)
			{
				m_ball->changePace(Ball::X_ORIENTED);	//pacing up the ball
				ballPace.first = 2; ballPace.second = 1;
				if (LOOPTIME > MINLOOPTIME)
					LOOPTIME -= INCLOOPTIME;
			}
			else if (m_ball->getDirection() == Ball::DOWNRIGHT && m_bar2->getDirection() == Bar::UP)
			{
				m_ball->changePace(Ball::X_ORIENTED);	//pacing up the ball
					ballPace.first = 2; ballPace.second = 1;
				if (LOOPTIME > MINLOOPTIME)
					LOOPTIME -= INCLOOPTIME;
			}
			else if (m_ball->getDirection() == Ball::UPRIGHT && m_bar2->getDirection() == Bar::UP)
			{
				m_ball->changePace(Ball::Y_ORIENTED);	//slowing down the ball
				ballPace.first = 1; ballPace.second = 2;
				if (LOOPTIME < MAXLOOPTIME)
					LOOPTIME += INCLOOPTIME;
			}
		}
		*/
		if (ballPos.second >= PingPong::BOTTOMEND || ballPos.second <= PingPong::TOPEND)	//collision right at corner
			m_ball->changeDirection(Ball::REVERSE_CORNER);
		else
			m_ball->changeDirection(Ball::BARS);
	}	
	else if (ballPos.second >= PingPong::BOTTOMEND || ballPos.second <= PingPong::TOPEND)	//reflection from ceiling and floor
	{
		std::thread th(beepTone, 300, 80);
		th.detach();
		m_ball->changeDirection(Ball::CEILING_FLOOR);
	}

	if ( !m_GameOver)
		m_ball->move();
}

void GameManager::startGame()
{
	bool updateBarPos = true;

	while (!m_GameOver)
	{
		drawGameArena(updateBarPos);
		updateBarPos = inputMove();
		logic(updateBarPos);
		std::this_thread::sleep_for(std::chrono::milliseconds(LOOPTIME));
	}
}

void GameManager::getWinnerMessage(std::string& msg)
{
	if (m_ball->getLocation().first >= PingPong::RIGHTEND)
		msg = "Player1 wins!";
	else if (m_ball->getLocation().first <= PingPong::LEFTEND)
		msg = "Player2 wins!";
}

void GameManager::showTutorialScreen(bool show)
{
	if (show)
	{
		positionBars();

		char up = 24, down = 25;

		//Game instruction
		setCursorPosition(33, 8);
		std::cout << "PING PONG GAME";
		setCursorPosition(25, 11);
		std::cout << "Press SPACE to start the game!";

		//Player1 movement tutorials
		setCursorPosition(7, 8);
		std::cout << up;
		setCursorPosition(9, 8);
		std::cout << "Key 'Q'";

		setCursorPosition(9, 11);
		std::cout << "Player1";

		setCursorPosition(7, 15);
		std::cout << down;
		setCursorPosition(9, 15);
		std::cout << "Key 'A'";

		//Player2 movement tutorials
		setCursorPosition(72, 8);
		std::cout << up;
		setCursorPosition(63, 8);
		std::cout << "Up Arrow";

		setCursorPosition(64, 11);
		std::cout << "Player2";

		setCursorPosition(72, 15);
		std::cout << down;
		setCursorPosition(61, 15);
		std::cout << "Down Arrow";
	}
	else
	{
		for (int row = 7; row <= 72; row++)
		{
			for (int col = 8; col <= 15; col++)
			{
				setCursorPosition(row, col);
				std::cout << PingPong::CLEARBLOCK;
			}
		}
	}
}

void GameManager::runGameLoop()
{
	//home screen
	showTutorialScreen(true);
	while (!GetAsyncKeyState(VK_SPACE)) {}	//wait for start until SPACE is pressed
	showTutorialScreen(false);

	bool gameRestarted = false;
	std::string gameWinMsg;

	do
	{
		if (gameRestarted)
		{
			m_GameOver = false;
			SAFE_DELETE(m_bar1);
			SAFE_DELETE(m_bar2);
			SAFE_DELETE(m_ball);

			m_bar1 = new Bar(3, 8);
			m_bar2 = new Bar(76, 8);
			m_ball = new Ball(27, 8, Ball::UPRIGHT);

			gameRestarted = false;
			system("cls");
		}
		startGame();

		system("cls");
		setCursorPosition(28, 6);
		getWinnerMessage(gameWinMsg);
		std::cout << gameWinMsg.c_str() << " Game Over!";
		setCursorPosition(17, 9);
		std::cout << "Press SPACE to restart the game; END to exit.";

		while (!GetAsyncKeyState(VK_SPACE) && !GetAsyncKeyState(VK_END)) {}	//wait until SPACE to restart or END to exit the game is pressed
		gameRestarted = true;		

	} while (GetAsyncKeyState(VK_SPACE));	//loop until player wishes to exit
}
