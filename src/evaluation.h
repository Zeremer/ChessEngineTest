//
// Created by paulgrahek on 1/28/19.
//

#pragma once

#include "position.h"
#include <atomic>

namespace Evaluation {
    constexpr int KingWeight = 10000;
    constexpr int QueenWeight = 12;
    constexpr int RookWeight = 6;
    constexpr int BishopWeight =4;
    constexpr int KnightWeight = 4;
    constexpr int PawnWeight = 1;
    constexpr int EvaluationOffset = 100;
    extern std::array<int,NoType+1> WeightArray;
    extern std::atomic<uint64_t > historyHeuristic[PieceColorCount][PieceTypeCount][SquareCount];

    /**
     * @brief Initiates any object associated with Evaluation
     * @author Paul Grahek
     */
    void init();

    /**
      * @brief Calculates the heuristic value of the given position
      * @param pos
      * @return
      * @author Porter Glines 2/10/19
      */
    int evalPosition(Position pos);

//    /**
//     * @brief Static Exchange Evaluation: Determines if exchanges on a square will result in a net gain or loss in material
//     * @param sq
//     * @return
//     * @author Paul Grahek
//     */
//    double SEE(Position pos,SquareIndex sq);


};
