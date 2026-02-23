#include "chess.h"


bool Chess::moveWhiteBishop(const std::pair<int, int> from,
    const std::pair<int, int> to) {

	int _tmpA = abs(from.first - to.first);
	int _tmpB = abs(from.second - to.second);


	if (_tmpA != _tmpB) {
		return false;
	}

	int offsetA = from.first > to.first ? -1 : 1;
	int offsetB = from.second > to.second ? -1 : 1;
	
	for (int i = 1; i < _tmpA; i++) {
		if (this->board[from.first + (i * offsetA)][from.second + (i * offsetB)] != '.'
			) {

			return false;
		}
	}
	if (isupper(this->board[to.first][to.second])) {
		return false;
	}

	this->board[to.first][to.second] = this->board[from.first][from.second];
	this->board[from.first][from.second] = '.';
	return true;

}




bool Chess::moveBlackBishop(const std::pair<int, int> from,
    const std::pair<int, int> to) {

	int _tmpA = abs(from.first - to.first);
	int _tmpB = abs(from.second - to.second);


	if (_tmpA != _tmpB) {
		return false;
	}

	int offsetA = from.first > to.first ? -1 : 1;
	int offsetB = from.second > to.second ? -1 : 1;

	for (int i = 1; i < _tmpA; i++) {
		if (this->board[from.first + (i * offsetA)][from.second + (i * offsetB)] != '.'
			) {

			return false;
		}
	}
	if (!isupper(this->board[to.first][to.second]) and this->board[to.first][to.second] != '.') {
		return false;
	}

	this->board[to.first][to.second] = this->board[from.first][from.second];
	this->board[from.first][from.second] = '.';
	return true;
	
}
