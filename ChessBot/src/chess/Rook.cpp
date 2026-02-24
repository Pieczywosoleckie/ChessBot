#include "chess.h"




bool Chess::moveWhiteRook(const std::pair<int, int> from,
	const std::pair<int, int> to) {

	int _tmpA = abs(from.first - to.first);
	int _tmpB = abs(from.second - to.second);

	if (from.first != to.first && from.second != to.second) {
		return false;
	}

	if (_tmpA != 0) {
		for (int i = 1; i < _tmpA; i++) {
			int col = (from.first < to.first) ? from.first + i : from.first - i;
			if (this->board[col][from.second] != '.') {
				return false;
			}
		}
		if (isupper(this->board[to.first][to.second])) {
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

	if (_tmpB != 0) {
		for (int i = 1; i < _tmpB; i++) { 
			int col = (from.second < to.second) ? from.second + i : from.second - i;
			if (this->board[from.first][col] != '.') { 
				return false;
			}
		}
		if (isupper(this->board[to.first][to.second])) {
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




bool Chess::moveBlackRook(const std::pair<int, int> from, 
	const std::pair<int, int> to) {

	int _tmpA = abs(from.first - to.first);
	int _tmpB = abs(from.second - to.second);

	if (from.first != to.first && from.second != to.second) {
		return false;
	}

	if (_tmpA != 0) {
		for (int i = 1; i < _tmpA; i++) {
			int col = (from.first > to.first) ? from.first - i : from.first + i;
			if (this->board[col][from.second] != '.') {
				return false;
			}
		}
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

	if (_tmpB != 0) {
		for (int i = 1; i < _tmpB; i++) {
			int col = (from.second > to.second) ? from.second - i : from.second + i;
			if (this->board[from.first][col] != '.') {
				return false;
			}
		}
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

