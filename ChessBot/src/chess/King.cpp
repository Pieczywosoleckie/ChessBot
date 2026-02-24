#include "chess.h"


bool Chess::moveWhiteKing(const std::pair<int, int> from,
    const std::pair<int, int> to) {

	int _tmpA = abs(from.first - to.first);
	int _tmpB = abs(from.second - to.second);

	if (_tmpA <= 1 and _tmpB <= 1) {
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


	
	return false;
}




bool Chess:: moveBlackKing(const std::pair<int, int> from,
    const std::pair<int, int> to) {


	int _tmpA = abs(from.first - to.first);
	int _tmpB = abs(from.second - to.second);

	if (_tmpA <= 1 and _tmpB <= 1) {
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


	return false;
}