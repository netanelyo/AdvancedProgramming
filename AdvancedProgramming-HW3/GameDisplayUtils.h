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

/* a class that manages the game animation */
class GameDisplayUtils
{
private:
	static const HANDLE	m_consoleHandle;
	
	/**
	 * sets the cursor's visibility on/off accordding to the boolean show
	 *
	 * @param show - a boolean saying if the cursor's visibility should be turned on or off
	 */
	static void consoleCursorVisibility(bool show);

	/**
	* sets the cursor to x,y point 
	* 
	* @param x - the x coordinate on screen
	* @param y - the y coordinate on screen
	*/
	static void gotoxy(int x, int y);

	/**
	* sets the text's color 
	*
	* @param color - the required text color 
	*/
	static void setColor(Color color);

	/**
	* prints a character to consule in the given coordinate and in the given color
	* 
	* @param x - the x coordinate
	* @param y - the y coordinate
	* @param sq - the character to be printed
	* @param color - the color in which to print the given character
	*/
	static void printSquare(int x, int y, char sq, Color color);
	
	/**
	* returns the console to its default settings:
	* sets the cursor's visibility on and sets the text color back to white
	*/
	static void returnToDefault();

	friend class Game;


};