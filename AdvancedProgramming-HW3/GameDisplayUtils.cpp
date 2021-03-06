#include "GameDisplayUtils.h"

const HANDLE GameDisplayUtils::m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

void GameDisplayUtils::consoleCursorVisibility(bool show)
{
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(m_consoleHandle, &cursorInfo);
	cursorInfo.bVisible = show; 
	SetConsoleCursorInfo(m_consoleHandle, &cursorInfo); 
}

void GameDisplayUtils::gotoxy(int x, int y)
{
	COORD coord; 

	coord.X = x; 
	coord.Y = y;
	SetConsoleCursorPosition(m_consoleHandle, coord); 
}

void GameDisplayUtils::setColor(Color color)
{
	int col;
	switch (color)
	{
	case Color::BLUE:
		col = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;

	case Color::GREEN:
		col = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;

	case Color::RED:
		col = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;

	default:
		col = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		break;
	}

	SetConsoleTextAttribute(m_consoleHandle, col);
}

void GameDisplayUtils::printSquare(int x, int y, char sq, Color color)
{
	setColor(color);
	gotoxy(x, y);
	std::cout << sq;
}

void GameDisplayUtils::returnToDefault()
{
	consoleCursorVisibility(true);
	setColor(Color::WHITE);
}
