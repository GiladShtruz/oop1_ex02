#include "Guard.h"
#include "Location.h"
#include "Bomb.h"
#include "Player.h"
#include "vector"
#include <iostream>
#include <random>
#include <io.h>
#include <Board.h>

Guard::Guard(){}

Guard::Guard(Location loc) : m_location(loc), m_startLocation(loc), m_isExist(true){}

Location Guard::getLocation()
{
	return m_location;
}

Location Guard::getStartLocation()
{
	return m_startLocation;
}

bool Guard::isGuardBombed(Bomb bomb)
{
	struct Location bombLoc1 = bomb.getLocation();
	struct Location bombLoc2 = Location(bombLoc1.row + 1, bombLoc1.col);
	struct Location bombLoc3 = Location(bombLoc1.row - 1, bombLoc1.col);
	struct Location bombLoc4 = Location(bombLoc1.row, bombLoc1.col + 1);
	struct Location bombLoc5 = Location(bombLoc1.row, bombLoc1.col - 1);
	if (m_location == bombLoc1 || m_location == bombLoc2 || m_location == bombLoc3 || m_location == bombLoc4 || m_location == bombLoc5)
	{
		return true;
		m_isExist = false;
	}
	return false;
}

void Guard::resetLocation()
{
	m_location = m_startLocation;
}
void Guard::setLocation(Location newLoc)
{
	m_location.row = newLoc.row;
	m_location.col = newLoc.col;
}
Location Guard::randMove()
{
	int a = rand() % 3;
	switch (a)
	{
	case 0:
		return Location(m_location.row + 1, m_location.col);
	case 1:
		return Location(m_location.row - 1, m_location.col);
	case 2:
		return Location(m_location.row, m_location.col + 1);
	case 3:
		return Location(m_location.row , m_location.col - 1);
	}
	return m_location;
}
Location Guard::smartMove(Player& player)
{
	////player location
	Location player_loc = player.getLocation();
	if(player_loc.row > m_location.row)
		return Location(m_location.row +1, m_location.col );
	else if(player_loc.row < m_location.row)
		return Location(m_location.row -1, m_location.col);
	if(player_loc.col > m_location.col)
		return Location(m_location.row, m_location.col +1);
	if (player_loc.col < m_location.col)
		return Location(m_location.row, m_location.col -1);
	return m_location;
	

}
Location Guard::move(Board &board, Player &player)
{

	Location newLoc;
	if (rand() % 2)
	{
		newLoc = randMove();
	}
	else newLoc = smartMove(player);
	if (newLoc == board.isValid(newLoc, m_location) &&
		(board.getLocationData(newLoc) != 'D'))
	{
			return newLoc;
	}
	return m_location;
}

bool Guard::getIsExist()
{
	return m_isExist;
}
