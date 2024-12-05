
#include "Board.h"
#include "cstdlib"
#include "io.h"
#include "Location.h"
#include "fstream"
#include "iostream"
#include "Guard.h"

void Board::loadBoard(std::string fileName, Player &player, std::vector<Guard> &guards, struct Location &doorLoc, int &numOfGuards) {
	auto line = std::string();
	auto file = std::ifstream(fileName);
	bool isFindPlayer = false;
	bool isFindGuard = false;
	bool isDoorExist = false;
	while (std::getline(file, line)) {
		m_board.push_back(line);
		// find player
		if (!isFindPlayer && line.find('/') < line.size()) {
			player = Player(m_board.size() - 1, line.find('/'));
			isFindPlayer = true;
		}
		// find guards
		int index = 0;
		while ((index = line.find('!', index + 1)) < line.size()) {
			isFindGuard = true;
			guards.push_back(Guard(Location(m_board.size() - 1, index)));
			numOfGuards++;
		}
		// find doors:
		if (!isDoorExist && line.find('D') < line.size()) {
			doorLoc = Location(m_board.size() - 1, line.find('D'));
			isDoorExist = true;
		}
		
	}
	
	m_victory = false;
	if (!isDoorExist || !isFindPlayer || !isFindGuard) {
		std::cerr << "Level not valid\n";
		exit(EXIT_FAILURE);
	}
}



void Board::print(int level, int points)
{
	for (int row = 0; row < m_board.size(); row++)
	{
		std::cout << m_board[row] << '\n';
	}
	m_healLocation = Location(m_board.size(), 6);
	std::cout << "Heal: 3\n";
	m_pointLocation = Location(m_board.size() + 1, 8);
	std::cout << "Points: " << points << std::endl;
	m_levelLocation = Location(m_board.size() + 2, 7);
	std::cout << "Level: " << level << std::endl;
}
 //אופרטור השמה
//Board& Board::operator=(const Board& other) {
//	return *this; 
//}

struct Location Board::isValid(struct Location newLoc, struct Location lastLoc)
{
	//added Bounds check YAIR
	if (newLoc.col > m_board[0].size() -1 ||
		newLoc.row > m_board.size() -1 )
	{
		return lastLoc;
	}
	char newPlace = m_board[newLoc.row][newLoc.col];
	if (newPlace != '#' && newPlace != '@') {
		return newLoc;
	}
	return lastLoc;
}

void Board::setPlayer(struct Location lastLoc, struct Location newLoc)
{
	m_board[lastLoc.row][lastLoc.col] = ' ';
	m_board[newLoc.row][newLoc.col] = '/';
	Screen::setLocation(lastLoc);
	std::cout << ' ';
	Screen::setLocation(newLoc);
	std::cout << '/';
}

void Board::setGuard(Location lastLoc, Location newLoc)
{
	m_board[lastLoc.row][lastLoc.col] = ' ';
	m_board[newLoc.row][newLoc.col] = '!';
	Screen::setLocation(lastLoc);
	std::cout << ' ';
	Screen::setLocation(newLoc);
	std::cout << '!';
}

void Board::playerOnBomb(struct Location bombLoc)
{ 
	m_board[bombLoc.row][bombLoc.col] = '%';
	Screen::setLocation(bombLoc);
	std::cout << '%';
}

void Board::putBombTime(Bomb bomb)
{
	//char chTime = bomb.getTime();
	char chTime = '0' + bomb.getTime();
	m_board[bomb.getLocation().row][bomb.getLocation().col] = chTime;
	Screen::setLocation(bomb.getLocation());
	std::cout << chTime;
}

void Board::bombExplodes(Bomb bomb)
{
	struct Location bombLoc = bomb.getLocation();
	m_board[bombLoc.row][bombLoc.col] = '*';
	Screen::setLocation(bomb.getLocation());
	std::cout << '*';
	
	if (m_board[bombLoc.row + 1][bombLoc.col] != '#') {
		m_board[bombLoc.row + 1][bombLoc.col] = '*';
		Screen::setLocation(Location(bombLoc.row + 1, bombLoc.col));
		std::cout << '*';
	}
	if (m_board[bombLoc.row - 1][bombLoc.col] != '#') {
		m_board[bombLoc.row - 1][bombLoc.col] = '*';
		Screen::setLocation(Location(bombLoc.row - 1, bombLoc.col));
		std::cout << '*';
	}
	if (m_board[bombLoc.row][bombLoc.col + 1] != '#') {
		m_board[bombLoc.row][bombLoc.col + 1] = '*';
		Screen::setLocation(Location(bombLoc.row, bombLoc.col + 1));
		std::cout << '*';
	}
	if (m_board[bombLoc.row][bombLoc.col - 1] != '#') {
		m_board[bombLoc.row][bombLoc.col - 1] = '*';
		Screen::setLocation(Location(bombLoc.row, bombLoc.col - 1));
		std::cout << '*';
	}
}

void Board::finishBombExploeds(Bomb bomb)
{
	struct Location bombLoc = bomb.getLocation();
	m_board[bombLoc.row][bombLoc.col] = ' ';
	Screen::setLocation(bomb.getLocation());
	std::cout << ' ';

	if (m_board[bombLoc.row + 1][bombLoc.col] != '#') {
		m_board[bombLoc.row + 1][bombLoc.col] = ' ';
		Screen::setLocation(Location(bombLoc.row + 1, bombLoc.col));
		std::cout << ' ';
	}
	if (m_board[bombLoc.row - 1][bombLoc.col] != '#') {
		m_board[bombLoc.row - 1][bombLoc.col] = ' ';
		Screen::setLocation(Location(bombLoc.row - 1, bombLoc.col));
		std::cout << ' ';
	}
	if (m_board[bombLoc.row][bombLoc.col + 1] != '#') {
		m_board[bombLoc.row][bombLoc.col + 1] = ' ';
		Screen::setLocation(Location(bombLoc.row, bombLoc.col + 1));
		std::cout << ' ';
	}
	if (m_board[bombLoc.row][bombLoc.col - 1] != '#') {
		m_board[bombLoc.row][bombLoc.col - 1] = ' ';
		Screen::setLocation(Location(bombLoc.row, bombLoc.col - 1));
		std::cout << ' ';
	}

}

int Board::size()
{
	return m_board.size();
}

void Board::setNewHeal(int heal)
{
	Screen::setLocation(m_healLocation);
	std::cout << heal;
}

void Board::setPoint(int points)
{
	Screen::setLocation(m_pointLocation);
	std::cout << points;
}

char Board::getLocationData(Location loc)
{
	return m_board[loc.row][loc.col ];
}

 

