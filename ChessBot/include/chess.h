#pragma once
#include <string>
#include <vector>


struct Chess {
private:
  std::vector<std::vector<char>> board;
  std::vector<std::vector<bool>> WhiteAtackPos;
  std::vector<std::vector<bool>> BlackAtackPos;

public:
  Chess() {
    this->board = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
    };
  }

  bool movePawn(const std::pair<int, int> from, const std::pair<int, int> to) {
    return false;
  }
  bool moveKnight(const std::pair<int, int> from,
                  const std::pair<int, int> to) {
    return false;
  }
  bool moveBishop(const std::pair<int, int> from,
                  const std::pair<int, int> to) {
    return false;
  }
  bool moveRook(const std::pair<int, int> from, const std::pair<int, int> to) {
    return false;
  }
  bool moveQueen(const std::pair<int, int> from, const std::pair<int, int> to) {
    return false;
  }
  bool moveKing(const std::pair<int, int> from, const std::pair<int, int> to) {
    return false;
  }

  std::vector<std::vector<char>> getBoard() { return this->board; }
};
