#pragma once

#include <windows.h>
#include <iostream>

enum class Color
{
	RED,
	GREEN,
	BLUE,
	WHITE
};

class GameDisplayUtils
{
private:
	static const HANDLE	m_consoleHandle;
	
	static void consoleCursorVisibility(bool show);

	static void gotoxy(int x, int y);

	static void setColor(Color color);

	static void printSquare(int x, int y, char sq, Color color);

	static void returnToDefault();

	friend class Game;


};