
#include "chess.h"
#include <iostream>

#include <math.h>

bool Chess::IsCheck(bool isWhite) const {
    std::pair<int, int> kingPos;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            char piece = this->board[y][x];
            if (isWhite) {
                if (piece == 'K') { kingPos = { y, x }; }
            }
            else {
                if (piece == 'k') { kingPos = { y, x }; }
            }
        }
    }

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            char piece = this->board[y][x];

            if (piece == '.' or (isWhite ? isupper(piece) : !isupper(piece))) {
                continue;
            }

            std::pair<int, int> from = { y, x };
            std::pair<int, int> to = kingPos;

            if (piece == 'n' or piece == 'N') {
                int dy = abs(y - to.first);
                int dx = abs(x - to.second);
                if ((dy == 2 and dx == 1) or (dy == 1 and dx == 2)) {
                    return true;
                }
            }

            if (piece == 'r' or piece == 'R' or piece == 'q' or piece == 'Q') {
                if (y == to.first or x == to.second) { 
                    if (isPathClear(from, to)) {
                        return true;
                    }
                }
            }

            if (piece == 'b' or piece == 'B' or piece == 'q' or piece == 'Q') {
                if (abs(y - to.first) == abs(x - to.second)) {
                    if (isPathClear(from, to)) {
                        return true;
                    }
                }
            }

            if (piece == 'p' or piece == 'P') {
                int direction = (piece == 'p') ? 1 : -1; 
                int attackY = y + direction;

                if (attackY == to.first and abs(x - to.second) == 1) {
                    return true; 
                }
            }

            if (piece == 'k' or piece == 'K') {
                if (abs(y - to.first) <= 1 and abs(x - to.second) <= 1) {
                    return true;
                }
            }
        }
    }

    return false; 
}

bool Chess::isPathClear(std::pair<int, int> from, std::pair<int, int> to) const {
    int dy = to.first - from.first;
    int dx = to.second - from.second;

    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);

    int steps = std::max(abs(dy), abs(dx));

    for (int i = 1; i < steps; ++i) {
        int checkY = from.first + i * stepY;
        int checkX = from.second + i * stepX;
        if (this->board[checkY][checkX] != '.') {
            return false;
        }
    }
    return true;
}