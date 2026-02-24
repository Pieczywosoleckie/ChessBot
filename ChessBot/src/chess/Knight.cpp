#include "chess.h"



bool Chess::moveBlackKnight(const std::pair<int, int> from,
	const std::pair<int, int> to) {

	int _tmpA = abs(from.first - to.first);
	int _tmpB = abs(from.second - to.second);

	if (_tmpA == 0 or _tmpB == 0 or _tmpA > 2 or _tmpB > 2 or (_tmpA - _tmpB) == 0){
		return false;
	}
	// We have only 2 possible values for _tmpA and _tmpB
	// 1. _tmpA = 2 and _tmpB = 1
	// 2. _tmpA = 1 and _tmpB = 2
	// we can just check if the destination is empty or has a black piece 

	if (!isupper(this->board[to.first][to.second]) and this->board[to.first][to.second] != '.') {
		return false;
	}
	this->board[to.first][to.second] = this->board[from.first][from.second];
	this->board[from.first][from.second] = '.';

	if (IsCheck(false)) {
		this->board[from.first][from.second] = this->board[to.first][to.second];
		this->board[to.first][to.second] = '.';
		return false;
	}

	return true;
}



bool Chess::moveWhiteKnight(const std::pair<int, int> from,
	const std::pair<int, int> to) {

	int _tmpA = abs(from.first - to.first);
	int _tmpB = abs(from.second - to.second);

	if (_tmpA == 0 or _tmpB == 0 or _tmpA > 2 or _tmpB > 2 or (_tmpA - _tmpB) == 0) {
		return false;
	}
	// We have only 2 possible values for _tmpA and _tmpB
	// 1. _tmpA = 2 and _tmpB = 1
	// 2. _tmpA = 1 and _tmpB = 2
	// we can just check if the destination is empty or has a white piece 

	if (isupper(this->board[to.first][to.second]) and this->board[to.first][to.second] != '.') {
		return false;
	}
	this->board[to.first][to.second] = this->board[from.first][from.second];
	this->board[from.first][from.second] = '.';

	if (IsCheck(true)) {
		this->board[from.first][from.second] = this->board[to.first][to.second];
		this->board[to.first][to.second] = '.';
		return false;
	}

	return true;
}