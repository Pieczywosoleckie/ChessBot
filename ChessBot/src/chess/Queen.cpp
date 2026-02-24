#include "chess.h"



bool Chess::moveBlackQueen(const std::pair<int, int> from, const std::pair<int, int> to) {

	int _tmpA = abs(from.first - to.first);		//	classic ;)
	int _tmpB = abs(from.second - to.second);

	if (_tmpA == _tmpB) { // Bishop like move ;)

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

		if (IsCheck(false)) {
			this->board[from.first][from.second] = this->board[to.first][to.second];
			this->board[to.first][to.second] = '.';
			return false;
		}
		return true;
	}

	if (_tmpA == 0 or _tmpB == 0) { // you know what I am famous for ?????         THE ROOOOOOOOOOOK !!!!!!!!
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
	return false;
}



bool Chess::moveWhiteQueen(const std::pair<int, int> from, const std::pair<int, int> to) {
	int _tmpA = abs(from.first - to.first);		
	int _tmpB = abs(from.second - to.second);


	if (_tmpA == _tmpB) { 

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


		if (IsCheck(true)) {
			this->board[from.first][from.second] = this->board[to.first][to.second];
			this->board[to.first][to.second] = '.';
			return false;
		}
		return true;

	}

	if (_tmpA == 0 or _tmpB == 0) {

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












	return false;
}