#pragma once
#include <string>
#include <vector>
#include <tuple>

struct Chess {

private:
    std::vector<std::vector<char>> board;

	std::pair<int, int> whiteKingPos = { 7, 4 };
	std::pair<int, int> blackKingPos = { 0, 4 };


    std::tuple<bool, int, int> PossibleEnPassantMove;

    bool whiteMove = true;
public:
    Chess() {

	    this->PossibleEnPassantMove = { false, -1, -1 };

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

    void SetEnPassantFalse() {
        this->PossibleEnPassantMove = { false, -1, -1 };
    }

    bool moveBlackPawn(const std::pair<int, int> from,
        const std::pair<int, int> to);

    bool moveBlackKnight(const std::pair<int, int> from,
        const std::pair<int, int> to);

    bool moveBlackBishop(const std::pair<int, int> from,
        const std::pair<int, int> to);

    bool moveBlackRook(const std::pair<int, int> from, 
        const std::pair<int, int> to);

    bool moveBlackQueen(const std::pair<int, int> from, 
        const std::pair<int, int> to);

    bool moveBlackKing(const std::pair<int, int> from, 
        const std::pair<int, int> to);

    bool moveWhitePawn(const std::pair<int, int> from,
        const std::pair<int, int> to);

    bool moveWhiteKnight(const std::pair<int, int> from,
        const std::pair<int, int> to);

    bool moveWhiteBishop(const std::pair<int, int> from,
        const std::pair<int, int> to);

    bool moveWhiteRook(const std::pair<int, int> from, 
        const std::pair<int, int> to);

    bool moveWhiteQueen(const std::pair<int, int> from,
        const std::pair<int, int> to);


    bool moveWhiteKing(const std::pair<int, int> from,
        const std::pair<int, int> to);



	bool IsCheck(bool white) const;

    bool isPathClear(std::pair<int, int> from, std::pair<int, int> to) const;


    void PlayedMoved() {
        this->whiteMove = !(this->whiteMove);
    }

    bool WhiteMove() {
        return this->whiteMove;
    }




    std::vector<std::vector<char>> getBoard() { 
        return this->board;
    }
};
