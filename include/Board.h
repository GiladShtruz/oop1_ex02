#pragma once
#include "vector"
#include "string"


class Board {
public:
	Board();
	Board(std::string fileName);
	//void loadBoard();
	//void print() const;


private:
	std::vector<std::string> m_board;


};