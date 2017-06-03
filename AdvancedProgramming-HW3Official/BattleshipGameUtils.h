#pragma once

enum class Direction
{
	NON,
	VERTICAL,
	HORIZONTAL,
	DEPTH
};

class BattleshipGameUtils
{
public:
	class Constants
	{
	public:
		// Constants
		static const char SPACE = ' ';

		const static int RUBBER_BOAT_POINTS = 2;
		const static int MISSILE_BOAT_POINTS = 3;
		const static int SUBMARINE_POINTS = 7;
		const static int DESTROYER_POINTS = 8;

		const static int RUBBER_BOAT_LEN = 1;
		const static int MISSILE_BOAT_LEN = 2;
		const static int SUBMARINE_LEN = 3;
		const static int DESTROYER_LEN = 4;
	};

	static int getShipLength(char ship);
};