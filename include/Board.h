#pragma once
#include "vector"
#include "Location.h"
#include "string"
#include "Player.h"
#include "Guard.h"
#include "Bomb.h"

class Board {
public:
	Board() {};
	void print(int level, int points);
	void loadBoard(std::string fileName, Player& player, std::vector<Guard>& guards, struct Location& doorLoc, int& numOfGuards);
	//Board& operator=(const Board& other);
	struct Location isValid(struct Location lastLoc, struct Location newLoc);
	void setPlayer(struct Location lastLoc, struct Location newLoc);
	void setGuard(struct Location lastLoc, struct Location newLoc);
	void playerOnBomb(struct Location bombLoc);
	void putBombTime(Bomb bomb);
	void bombExplodes(Bomb bomb);
	void finishBombExploeds(Bomb bomb);
	int size();
	void setNewHeal(int heal);
	void setPoint(int points);
	char getLocationData(Location loc);

	//bool getVictory();
	//void setVictory();

	
private:
	std::vector<std::string> m_board;
	bool m_victory = false;
	struct Location m_healLocation;
	struct Location m_pointLocation;
	struct Location m_levelLocation;
};