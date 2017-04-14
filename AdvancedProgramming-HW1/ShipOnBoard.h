#pragma once



class ShipOnBoard
{
public:
	ShipOnBoard() {}
	ShipOnBoard(char symbol, size_t len, int row, int col, bool onRow) : symbolOnBoard(symbol), shipLenOnBoard(len), firstRowIndex(row),
		firstColIndex(col), isOnRow(onRow) {};
	~ShipOnBoard() {}; 

	char getSymbol() { return symbolOnBoard; }
	size_t getLength() { return shipLenOnBoard; }
	int getFirstRowIndex() { return firstRowIndex; }
	int getFirstColIndex() { return firstColIndex;  }
	bool getIsOnRow() { return isOnRow;  }

	void setSymbol(char symbol) { symbolOnBoard = symbol;  }
	void setFirstRowIndex(int row) { firstRowIndex = row;  }
	void setFirstColIndex(int col) { firstColIndex = col;  }
	void setLength(size_t len) { shipLenOnBoard = len; }
	void setIsOnRow(bool onRow) { isOnRow = onRow; }

	void decreaseShipLen() { shipLenOnBoard--;  }

private:
	char symbolOnBoard; 
	size_t shipLenOnBoard;
	int firstRowIndex;
	int firstColIndex;
	bool isOnRow; 

};
