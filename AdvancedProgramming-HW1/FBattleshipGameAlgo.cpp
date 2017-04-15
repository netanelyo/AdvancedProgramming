#include "FBattleshipGameAlgo.h"


void FBattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols)
{
	for (int i = 0; i < numRows; i++)
		for (int j = 0; j < numCols; j++)
			setBoardCoord(i, j, board[i][j]);



	//int i/*ndex = 0;
	//bool isRow = false;
	//char sym;

	//for (int i = 0; i < BOARD_SIZE; i++)
	//{
	//	for (int j = 0; j < BOARD_SIZE; j++)
	//	{
	//		sym = board[i][j];
	//		if (sym != '0')
	//		{
	//			setBoardCoord(i, j, index + 1);
	//			if ((i < BOARD_SIZE - 1) && (board[i+1][j] == board[i][j]))
	//			{	
	//				isRow = false; 
	//			}

	//			if ((j < BOARD_SIZE - 1) && (board[i][j + 1] == board[i][j]))
	//			{
	//				isRow = true; 
	//			}

	//			myShips[index].setSymbol(sym); 
	//			myShips[index].setIsHorizontal(isRow); 
	//			switch (sym)
	//			{
	//			case 'B':
	//			case 'b':
	//				myShips[index].setLength(RUBBER_BOAT_LEN); 
	//				myShips[index].setLengthOnBoard(RUBBER_BOAT_LEN); 
	//				break;
	//			case 'P':
	//			case 'p':
	//				myShips[index].setLength(MISSILE_BOAT_LEN);
	//				myShips[index].setLengthOnBoard(MISSILE_BOAT_LEN); 
	//			case 'M':
	//			case 'm':
	//				myShips[index].setLength(SUBMARINE_LEN);
	//				myShips[index].setLengthOnBoard(SUBMARINE_LEN); 
	//				break;
	//			case 'D':
	//			case 'd':
	//				myShips[index].setLength(DESTROYER_LEN);
	//				myShips[index].setLengthOnBoard(DESTROYER_LEN);
	//				break;
	//			default:
	//				break;
	//			}

	//			myShips[index].setFirstCoord(i, j); 
	//		}
	//	}
	//}*/

}

std::pair<int, int> FBattleshipGameAlgo::attack()
{
	std::pair<int, int> attackCoordinate(-1, -1);
	std::string			line;

	/* Reads attacks file until a valid line or EOF have been reached */
	while (!std::getline(playerMoves, line).eof())
	{
		if (parseLineAndValidate(line, attackCoordinate))
			break;
	}

	/* If EOF reached then current player is done */
	if (playerMoves.eof())
	{
		isDone = true;
	}
	
	return attackCoordinate;
}

void FBattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	if (board[row][col] != '0' && board[row][col] != 'X')
	{
		switch (result)
		{
		case AttackResult::Sink:
			shipCounter--;
		case AttackResult::Hit:
			board[row][col] = 'X';
			break;
		default:
			break;
		}
	}
	
}


