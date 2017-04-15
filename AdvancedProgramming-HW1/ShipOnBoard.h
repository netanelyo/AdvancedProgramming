#pragma once



class ShipOnBoard
{
public:
	ShipOnBoard() {}
	~ShipOnBoard() {}

	char getSymbol() const { return symbolOnBoard; }
	size_t getLengthOnBoard() const { return shipLenOnBoard; }
	size_t getLength() const { return shipLen; }
	std::pair<int, int> getFirstCoord() const { return firstCoord; }
	bool getIsHorizontal() const { return isHorizontal;  }

	void setSymbol(char symbol) { symbolOnBoard = symbol;  }
	void setFirstCoord(int i, int j) { firstCoord = std::pair<int, int>(i, j); }
	void setLengthOnBoard(size_t len) { shipLenOnBoard = len; }
	void setLength(size_t len) { shipLen = len; }
	void setIsHorizontal(bool onRow) { isHorizontal = onRow; }

	void decreaseShipLen() { shipLenOnBoard--; }

private:
	char symbolOnBoard; 
	size_t shipLenOnBoard;
	size_t shipLen;
	std::pair<int, int> firstCoord;
	bool isHorizontal; 

};
