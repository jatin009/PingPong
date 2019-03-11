#pragma once

#include <utility>

#define SAFE_DELETE(x) delete x;\
x = nullptr;

namespace PingPong{
	
	static const int BARLENGTH = 8;
	static const int BAR1COLOR = 10;
	static const int BAR2COLOR = 9;
	
	static const char* FULLBLOCK = "\xDB";
	static const char* CLEARBLOCK = " ";
	static const char* BALL = "O";

	static const int CONSOLEWIDTH = 80;
	static const int CONSOLEHEIGHT = 25;
	
	static const int BOTTOMEND = 24;
	static const int TOPEND = 0;
	static const int RIGHTEND = 79;
	static const int LEFTEND = 0;
	static const int BARDISPLACEMENT = 1;
	static const int ENDMARGIN = 0;

	typedef std::pair<int, int> IntPair;
}
