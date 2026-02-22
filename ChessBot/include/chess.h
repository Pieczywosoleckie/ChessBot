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

  bool moveBlackPawn(const std::pair<int, int> from,
      const std::pair<int, int> to);

  bool moveBlackKnight(const std::pair<int, int> from,
      const std::pair<int, int> to) {
	  return false;
  }

  bool moveBlackBishop(const std::pair<int, int> from,
                  const std::pair<int, int> to) {
    return false;
  }
  bool moveBlackRook(const std::pair<int, int> from, const std::pair<int, int> to) {
    return false;
  }
  bool moveBlackQueen(const std::pair<int, int> from, const std::pair<int, int> to) {
    return false;
  }
  bool moveBlackKing(const std::pair<int, int> from, const std::pair<int, int> to) {
    return false;
  }






  bool moveWhitePawn(const std::pair<int, int> from,
      const std::pair<int, int> to);

  bool moveWhiteKnight(const std::pair<int, int> from,
      const std::pair<int, int> to) {
      return false;
  }

  bool moveWhiteBishop(const std::pair<int, int> from,
      const std::pair<int, int> to) {
      return false;
  }
  bool moveWhiteRook(const std::pair<int, int> from, const std::pair<int, int> to) {
      return false;
  }
  bool moveWhiteQueen(const std::pair<int, int> from, const std::pair<int, int> to) {
      return false;
  }
  bool moveWhiteKing(const std::pair<int, int> from, const std::pair<int, int> to) {
      return false;
  }

  std::vector<std::vector<char>> getBoard() { return this->board; }
};
