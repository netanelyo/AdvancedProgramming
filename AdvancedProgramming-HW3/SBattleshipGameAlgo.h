#pragma once
#include "CommonBattleshipGameAlgo.h"
#include <set>
#include <map>

/*
* class SBattleshipGameAlgo - a derived class of CommonBattleshipGameAlgo that plays
* a battleship game in a smart way
*/
class SBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:
	SBattleshipGameAlgo() : CommonBattleshipGameAlgo(),	m_currentAttackingShip(-1, -1),
						m_sequentialState(true), m_lastAttackDirection(Direction::NON) {}
	SBattleshipGameAlgo(const SBattleshipGameAlgo& b) = delete;
	SBattleshipGameAlgo& operator=(const SBattleshipGameAlgo& b) = delete;

	/**
	* @overrides CommonBattleshipGameAlgo::notifyOnAttackResult virtual function
	*
	* handles the result of the current attack. 
	* in case of a miss - we update our board so we wouldn't attack there 
	* in case of a hit - if it is a hit on our ship, we only update our board. 
	*					 if it is the opponent's ship:
	*					 if we are in sequential mode or the opponent is the current player,
	*					 we also save the square's coordinates so we could use it 
	*					 in order to drown this ship and update our state (sequential vs. attacking mode)
	*					 if we are the current player, we simply update the direction of the ship we currently attacking
	* in case of a sink - if it is a sink of our ship, we only update our board.
	*					  if it is the opponent's ship, we also remove all its related coordinates
	*					  from our next potential ships to attack and mark the bordering coordinates of this ship so we 
	*					  won't attack there
	*
	* @param player - the index of the player who currently played
	* @param row - the row coordinate of the attack
	* @param col - the column coordinate of the attack
	* @param result - the result of the attack
	*/
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;

	/**
	* @overrides CommonBattleshipGameAlgo::attack virtual function
	* 
	* returns the next attacking square.
	* if we are in a sequential state, we just find the next empty square (same as in CommonBattleshipGameAlgo)
	* if we are in an attacking mode, we scan according to the known direction of the current 
	* ship we are attacking for its next potential square (we find the next empty square in the ship's direction  
	*
	* @return 
	* <-1, -1> in case no empty square was found
	* otherwise, the coordinates of the next attack
	*/
	std::pair<int, int> attack() override;

	bool isSequntial() const { return m_sequentialState; }

private:
	std::pair<int, int>		m_currentAttackingShip;
	bool		m_sequentialState;
	Direction	m_lastAttackDirection;
	std::set<std::pair<int, int>> m_preferredAttackSquares;
	std::map<std::pair<int, int>, Direction> m_attackHelperMap;

	/**
	* a recursive function that marks the oppenent's sunk ship and the bordering squares of this ship on our board, 
	* and removes the ship's squares from our potential ships to attack 
	*
	* @param row - the row coordinate
	* @param col - the col coordinate
	* @param changed - a boolean we mark if the find the current sunk ship is the one we were currently attacking
	* 
	*/
	void markOppSankShip(int row, int col, bool& changed);

	/**
	* a helper function of markOppSankShip 
	*/
	void markRecursiveCall(int row, int col, bool& changed);

	/**
	* finds our next move after if the result of the previous attack was a sink of a ship we where currently attacking
	* If we have no potential ships to attack, we return to sequantial state 
	* Otherwise, we choose one of the potential ships
	*/
	void findNextMove();

	/**
	* a helper function of notifyOnAttackResult which handles the hit result as described in notifyOnAttackResult
	*
	* @param player - the index of the player who currently played
	* @param row - the row coordinate of the attack
	* @param col - the column coordinate of the attack
	*/
	void hitNotify(int row, int col, int player);

	/**
	* a helper function of notifyOnAttackResult which handles the sink result as described in notifyOnAttackResult
	*
	* @param player - the index of the player who currently played
	* @param row - the row coordinate of the attack
	* @param col - the column coordinate of the attack
	*/
	void sinkNotify(int row, int col, int player);

	/**
	* a helper function of attack which finds the next empty square in the direction of 
	* the current attacking ship or the next bordering empty square of the current attacking ship
	* in case it doesn't have a direction yet, as decribed in attack. 
	* 
	* @param row - the row coordinate 
	* @param col - the column coordinate
	* @param nextAttackDirection - the direction of the current attacking ship
	*/
	std::pair<int, int> findNextEmptySquare(int row, int col, Direction nextAttackDirection);

	/**
	* a helper function of findNextEmptySquare which finds the next empty square in the ship's direction
	* in case all its borders are not empty (the direction of the ship must be known)
	*
	* @param row - the row coordinate
	* @param col - the column coordinate
	* @param nextAttackDirection - the direction of the current attacking ship
	*/
	std::pair<int, int> findNextIterative(int row, int col, Direction nextAttackDirection) const;
};
