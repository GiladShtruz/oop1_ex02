#include <thread>
#include <chrono>

#include "io.h"
#include "Controller.h"
#include "Board.h"
#include "conio.h"
#include "cstdlib"
#include "iostream"
#include "string"
#include "vector"
#include "fstream"
using namespace std::chrono_literals;
// #include "istream"
void resetGuards(std::vector<Guard>& guards, Board& board);
int getPointFromBombedGuards(std::vector<Guard>& guards, Bomb bomb);
bool checkGuardCollide(std::vector<Guard> guards, Location playerLoc);
Controller::Controller() {}
void Controller::run()
{

	// read playlist:
	std::vector<std::string> playlist;
	auto line = std::string();
	auto file = std::ifstream("playlist.txt");
	/*if (!std::getline(file, line)) {
		std::cerr << "Can't read from file\n";
		exit(EXIT_FAILURE);
	}*/
	while (std::getline(file, line))
	{
		playlist.push_back(line);
	}
	
	bool isFailed = false;
	int playerPoints = 0;
	for (int level = 0; level < playlist.size(); level++)
	{
		Board curBoard;
		Player curPlayer;
		std::vector<Guard> curGuards;
		std::vector<Bomb> curBombs;
		int numOfGuards = 0;
		struct Location doorLocation;
		bool isFinishLevel = false;
		curBoard.loadBoard(playlist[level], curPlayer, curGuards, doorLocation, numOfGuards);
		curBoard.print(level + 1, playerPoints);
		while (true)
		{
			bool isBombPutted = false;
			struct Location newPlayerLoc = curPlayer.move(curBoard, isBombPutted);
			// get the door -> level finished
			if (newPlayerLoc == doorLocation) {
				isFinishLevel = true;
				playerPoints += numOfGuards * 3 + 25;
				break;
			}
			if (isBombPutted)
			{
				curBombs.push_back(Bomb(newPlayerLoc));
				curBoard.playerOnBomb(newPlayerLoc);
			}
			curBoard.setPlayer(curPlayer.getLocation(), newPlayerLoc);
			curPlayer.setLocation(newPlayerLoc);
			for (int bombIndex = 0; bombIndex < curBombs.size(); bombIndex++)
			{
				bool isBombExploded = false;
				bool isPlayerBombed = false;
				if (newPlayerLoc == curBombs[bombIndex].getLocation())
				{
					curBoard.playerOnBomb(newPlayerLoc);
				}
				else if (curBombs[bombIndex].getTime() > 0)
				{
					curBoard.putBombTime(curBombs[bombIndex]);
				}
				else if (curBombs[bombIndex].getTime() == 0)
				{
					curBoard.bombExplodes(curBombs[bombIndex]);
					// check if guard bombed
					int newPoints = getPointFromBombedGuards(curGuards, curBombs[bombIndex]);
					if (newPoints > 0) {
						playerPoints += newPoints;
						curBoard.setPoint(playerPoints);
					}					
					// check if player in bomb place:
					if (curPlayer.isPlayerBombed(curBombs[bombIndex]))
					{
						curPlayer.decreaseHeal();
						isPlayerBombed = true;
						if (curPlayer.getHeal() == 0) {
							isFailed = true;
							break;							
						}
						else {
							curBoard.setNewHeal(curPlayer.getHeal());
							curBoard.finishBombExploeds(curBombs[bombIndex]);
							curBoard.setPlayer(curPlayer.getLocation(), curPlayer.getStartLocation());
							curPlayer.resetLocation();
							curBombs.clear(); // reset bomb                            
							resetGuards(curGuards, curBoard); // reset guards
						}
					}
					else {
					}
				}
				else // time == -1
				{
					curBoard.finishBombExploeds(curBombs[bombIndex]);
					curBombs.erase(curBombs.begin() + bombIndex);
					bombIndex--;
					isBombExploded = true;
				}
				if (!isBombExploded && !isPlayerBombed) {
					curBombs[bombIndex].act();
				}
			}
			if (checkGuardCollide(curGuards, newPlayerLoc)) {
				curPlayer.decreaseHeal();
				curBoard.setNewHeal(curPlayer.getHeal());
				for (int guardIndex = 0; guardIndex < curGuards.size(); guardIndex++)
				{
					curBoard.setGuard(curGuards[guardIndex].getLocation(), curGuards[guardIndex].getStartLocation());
					curGuards[guardIndex].resetLocation();
				}
				curBoard.setPlayer(curPlayer.getLocation(), curPlayer.getStartLocation());
				curPlayer.resetLocation();
			}
			// move guards:
			for (int index = 0; index < curGuards.size(); index++)
			{
				// sleep:
				std::this_thread::sleep_for(50ms);
				Location newGuardLoc(curGuards[index].move(curBoard, curPlayer));
				curBoard.setGuard(curGuards[index].getLocation(), newGuardLoc);
				curGuards[index].setLocation(newGuardLoc);
			}
			// after move guards check if player bump into guard
			if (checkGuardCollide(curGuards, newPlayerLoc))
			{
				curPlayer.decreaseHeal();
				curBoard.setNewHeal(curPlayer.getHeal());
				for (int guardIndex = 0; guardIndex < curGuards.size(); guardIndex++)
				{					
					curBoard.setGuard(curGuards[guardIndex].getLocation(), curGuards[guardIndex].getStartLocation());
					curGuards[guardIndex].resetLocation();					
				}
				curBoard.setPlayer(curPlayer.getLocation(), curPlayer.getStartLocation());
				curPlayer.resetLocation();
			}
			Screen::setLocation(Location(curBoard.size(), 0));
			if (curPlayer.getHeal() <= 0) {
				isFailed = true;				
				break;
			}
		} // end while
		if (isFinishLevel) {
			system("cls");			
		}
		if (isFailed) {
			system("cls");
			std::cout << "You Failed!\nYour Points is: " << playerPoints;
			break;
		}
		

	}
	if (!isFailed)
	{
		system("cls");
		std::cout << "You Win!\nYour Points is: " << playerPoints << " \n";
	}
}

void resetGuards(std::vector<Guard>& guards, Board& board)
{
	for (int guardIndex = 0; guardIndex < guards.size(); guardIndex++)
	{
		board.setGuard(guards[guardIndex].getLocation(), guards[guardIndex].getStartLocation());
		guards[guardIndex].resetLocation();
	}

	// update board
}
int getPointFromBombedGuards(std::vector<Guard>& guards, Bomb bomb)
{
	int points = 0;
	for (int guardIndex = 0; guardIndex < guards.size(); guardIndex++)
	{
		struct Location bombLoc1 = bomb.getLocation();

		if (guards[guardIndex].isGuardBombed(bomb)) {
			points += 5;
			guards.erase(guards.begin() + guardIndex);
			guardIndex--;
		}
	}
	return points;
}

bool checkGuardCollide(std::vector<Guard> guards, Location playerLoc) {
	for (int guardIndex = 0; guardIndex < guards.size(); guardIndex++)
	{
		if (playerLoc == guards[guardIndex].getLocation()) {
			return true;
		}
	}
	return false;
}