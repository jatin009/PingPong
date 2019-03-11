#pragma once
#include <string>

class Bar;
class Ball;

class GameManager
{

public:
	GameManager();
	virtual ~GameManager();
	void runGameLoop();

private:
	bool m_GameOver;
	Bar* m_bar1; Bar* m_bar2;
	Ball* m_ball;

	void showTutorialScreen(bool show);
	void positionBars();
	void startGame();
	void getWinnerMessage(std::string& );
	void drawGameArena(bool updateBarsPos);
	bool inputMove();
	void logic(bool barinmotion);

};