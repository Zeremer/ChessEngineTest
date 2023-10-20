//
// Created by paulgrahek on 2/21/19.
//

#pragma once

#include <array>
#include "types.h"
#include "position.h"

namespace ZobristHash {
    extern Bitboard zPieceTable[PieceColorCount][PieceTypeCount+1][SquareCount];
    extern Bitboard zColorTable[PieceColorCount];
    void init();

    /**
     * @brief Hashes a given position and returns the hashed position
     * @param pos
     * @return
     */
    Bitboard hash(Position pos);

};