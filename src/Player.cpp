#include "io.h"

#include "conio.h"

#include "Player.h"
#include "Board.h"
struct Location handleSpecialKey(Board& board, struct Location loc);



Player::Player() {}

Player::Player(int row, int col)
{
	m_location.row = row;
	m_location.col = col;
	m_startLocation = m_location;
	m_heal = 3;
}

int Player::getHeal()
{
	return m_heal;
}

void Player::decreaseHeal()
{
	m_heal--;
}

bool Player::isPlayerBombed(Bomb bomb)
{
	struct Location bombLoc1 = bomb.getLocation();
	struct Location bombLoc2 = Location(bombLoc1.row + 1, bombLoc1.col);
	struct Location bombLoc3 = Location(bombLoc1.row - 1, bombLoc1.col);
	struct Location bombLoc4 = Location(bombLoc1.row, bombLoc1.col + 1);
	struct Location bombLoc5 = Location(bombLoc1.row, bombLoc1.col - 1);
	if (m_location == bombLoc1 || m_location == bombLoc2 || m_location == bombLoc3 || m_location == bombLoc4 || m_location == bombLoc5)
	{
		return true;
	}
	return false;
}

void Player::resetLocation()
{
	m_location = m_startLocation;
}

struct Location Player::getLocation()
{
	return m_location;
}

Location Player::getStartLocation()
{
	return m_startLocation;
}

Location Player::move(Board& board, bool& isBombPutted)
{

	bool toTryAgain = true;
	struct Location lastLoc = m_location;
	struct Location newLoc = m_location;
	while (toTryAgain) {
		const auto c = _getch();

		switch (c)
		{
		case 0:
		case ' ':
			isBombPutted = false;
			toTryAgain = false;
			break;
		case Keys::SPECIAL_KEY:
			newLoc = handleSpecialKey(board, lastLoc);
			toTryAgain = newLoc == lastLoc;
			break;
		case 'b':
		case 'B':
			isBombPutted = true;
			toTryAgain = false;
			break;

		default:
			break;
		}
		// handle bomb
	}

	return newLoc;
}
void Player::setLocation(Location newLoc)
{
	m_location = newLoc;
}
struct Location handleSpecialKey(Board& board, struct Location lastLoc) {
	const auto c = _getch();
	Location newLoc = lastLoc;
	switch (c)
	{
	case SpecialKeys::UP:
		newLoc.row = lastLoc.row - 1;
		return board.isValid(newLoc, lastLoc);
		//std::cout << "UP\n";
		break;
	case SpecialKeys::DOWN:
		newLoc.row = lastLoc.row + 1;
		return board.isValid(newLoc, lastLoc);
		//std::cout << "DOWN\n";
		break;
	case SpecialKeys::RIGHT:
		newLoc.col = lastLoc.col + 1;
		return board.isValid(newLoc, lastLoc);
		//std::cout << "RIGHT\n";
		break;
	case SpecialKeys::LEFT:
		newLoc.col = lastLoc.col - 1;
		return board.isValid(newLoc, lastLoc);
		//std::cout << "LEFT\n";
		break;
	default:
		break;
	}
}

