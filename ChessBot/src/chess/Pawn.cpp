
#include "chess.h"
#include <iostream>

#include <math.h>

bool Chess::moveWhitePawn(const std::pair<int, int> from,
	const std::pair<int, int> to) {

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

		if (IsCheck(true)) {
			this->board[from.first][from.second] = this->board[to.first][to.second];
			this->board[to.first][to.second] = '.';
			return false;
		}

		this->PossibleEnPassantMove = { true, to.first - 1, to.second };
		std::cout << "Possible en-pasant : " << get<1>(this->PossibleEnPassantMove) << ":" << get<2>(this->PossibleEnPassantMove) << std::endl;
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

				if (IsCheck(true)) {
					this->board[from.first][from.second] = this->board[to.first][to.second];
					this->board[to.first][to.second] = '.';
					return false;
				}
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

				if (IsCheck(true)) {
					this->board[from.first][from.second] = this->board[to.first][to.second];
					this->board[to.first][to.second] = '.';
					return false;
				}
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
		if (IsCheck(false)) {
			this->board[from.first][from.second] = this->board[to.first][to.second];
			this->board[to.first][to.second] = '.';
			return false;
		}

		this->PossibleEnPassantMove = { true, to.first + 1, to.second };
		std::cout << "Possible en-pasant : " << get<1>(this->PossibleEnPassantMove) << ":" << get<2>(this->PossibleEnPassantMove) << std::endl;
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

				if (IsCheck(false)) {
					this->board[from.first][from.second] = this->board[to.first][to.second];
					this->board[to.first][to.second] = '.';
					return false;
				}
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
				if (IsCheck(false)) {
					this->board[from.first][from.second] = this->board[to.first][to.second];
					this->board[to.first][to.second] = '.';
					return false;
				}

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