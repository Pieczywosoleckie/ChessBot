
#include "chess.h"

#include <math.h>

bool Chess::moveWhitePawn(const std::pair<int, int> from,
	const std::pair<int, int> to){
	
	int _tmpA = from.first - to.first;
	int _tmpB = abs(from.second - to.second);

	if (_tmpA == 2) {
		if (from.first != 6) {
			return false;
		}
		if (_tmpB != 0) {
			return false;
		}
		if (this->board[to.first][to.second] != '.') {
			return false;
		}
		this->board[to.first][to.second] = this->board[from.first][from.second];
		this->board[from.first][from.second] = '.';
		return true;
	}
	else if (_tmpA == 1) {
		if (_tmpB == 0) {
			if (this->board[to.first][to.second] != '.') {
				return false;
			}
			else {
				this->board[to.first][to.second] = this->board[from.first][from.second];
				this->board[from.first][from.second] = '.';
				return true;
			}
		}
		else if (_tmpB == 1) {
			if (this->board[to.first][to.second] == '.' or
				isupper(this->board[to.first][to.second])) {
				return false;
			}
			else {
				this->board[to.first][to.second] = this->board[from.first][from.second];
				this->board[from.first][from.second] = '.';
				return true;
			}
		}
		else {
			return false;
		}
	}	
	else {
		return false;
	}

}
bool Chess::moveBlackPawn(const std::pair<int, int> from,
	const std::pair<int, int> to) {
	int _tmpA = to.first - from.first;
	int _tmpB = abs(from.second - to.second);

	if (_tmpA == 2) {
		if (from.first != 1) {
			return false;
		}
		if (_tmpB != 0) {
			return false;
		}
		if (this->board[to.first][to.second] != '.') {
			return false;
		}
		this->board[to.first][to.second] = this->board[from.first][from.second];
		this->board[from.first][from.second] = '.';
		return true;
	}
	else if (_tmpA == 1) {
		if (_tmpB == 0) {
			if (this->board[to.first][to.second] != '.' ) {
				return false;
			}
			else {
				this->board[to.first][to.second] = this->board[from.first][from.second];
				this->board[from.first][from.second] = '.';
				return true;
			}
		}
		else if (_tmpB == 1) {
			if (this->board[to.first][to.second] == '.' or 
				!(isupper(this->board[to.first][to.second]))
				) {
				return false;
			}
			else {
				this->board[to.first][to.second] = this->board[from.first][from.second];
				this->board[from.first][from.second] = '.';
				return true;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}

}