//
// Created by paulgrahek on 1/28/19.
//

#include <atomic>
#include "evaluation.h"
#include "movegen.h"

std::atomic<uint64_t> historyHeuristic[PieceColorCount][PieceTypeCount][SquareCount];
std::array<int,NoType+1> Evaluation::WeightArray;

int Evaluation::evalPosition(Position pos) {
    switch (pos.getCurrentGameState()){
        case Checkmate:
            return SEARCH_MIN;
        case Stalemate:
            return 0;
        case Check:
        case Normal:
        default:
            auto mobWhite = 0, mobBlack = 0;
            mobWhite += MoveGen::getPossibleMoveCountForKnights(pos,White);
            mobBlack += MoveGen::getPossibleMoveCountForKnights(pos,Black);
            mobWhite += MoveGen::getPossibleMoveCountForBishops(pos,White);
            mobBlack += MoveGen::getPossibleMoveCountForBishops(pos,Black);
            mobWhite += MoveGen::getPossibleMoveCountForKings(pos,White);
            mobBlack += MoveGen::getPossibleMoveCountForKings(pos,Black);
            mobWhite += MoveGen::getPossibleMoveCountForPawns(pos,White,none);
            mobBlack += MoveGen::getPossibleMoveCountForPawns(pos,Black,none);
            mobWhite += MoveGen::getPossibleMoveCountForQueens(pos,White);
            mobBlack += MoveGen::getPossibleMoveCountForQueens(pos,Black);
            mobWhite += MoveGen::getPossibleMoveCountForRooks(pos,White);
            mobBlack += MoveGen::getPossibleMoveCountForRooks(pos,Black);
            int mobility = 10 * (mobWhite - mobBlack);
            return
                    PieceColorWeight[pos.getCurrentMoveColor()] * (
                    WeightArray[King]*(numOnes64(pos.getPiecePositions(White,King)) - numOnes64(pos.getPiecePositions(Black,King))) +
                            WeightArray[Queen]*(numOnes64(pos.getPiecePositions(White,Queen)) - numOnes64(pos.getPiecePositions(Black,Queen))) +
                            WeightArray[Rook]*(numOnes64(pos.getPiecePositions(White,Rook)) - numOnes64(pos.getPiecePositions(Black,Rook))) +
                            WeightArray[Bishop]*(numOnes64(pos.getPiecePositions(White,Bishop)) - numOnes64(pos.getPiecePositions(Black,Bishop))) +
                            WeightArray[Knight]*(numOnes64(pos.getPiecePositions(White,Knight)) - numOnes64(pos.getPiecePositions(Black,Knight))) +
                            WeightArray[Pawn]*(numOnes64(pos.getPiecePositions(White,Pawn)) - numOnes64(pos.getPiecePositions(Black,Pawn))) +
                    mobility);// +
    }
}

//double Evaluation::SEE(Position pos, SquareIndex sq) {
////    double value = 0;
////    std::tuple<PieceType,SquareIndex> smallestPiece = pos.getSmallestAttacker(sq);
////    if(std::get<0>(smallestPiece) != NoType){
////        Move mv;
////        mv.pieceType = std::get<0>(smallestPiece);
////        mv.startSquare = std::get<1>(smallestPiece);
////        mv.endSquare = sq;
////        pos.move(mv);
////        value = std::max(0.0,WeightArray[mv.pieceType]-SEE(pos,sq));
////    }
////    return value;
//}

void Evaluation::init() {
    WeightArray[Pawn] = PawnWeight * EvaluationOffset;
    WeightArray[Bishop] = BishopWeight * EvaluationOffset;
    WeightArray[Knight] = KnightWeight * EvaluationOffset;
    WeightArray[Rook] = RookWeight * EvaluationOffset;
    WeightArray[Queen] = QueenWeight * EvaluationOffset;
    WeightArray[King] = KingWeight * EvaluationOffset;
    WeightArray[PieceTypeCount] = 0;
    WeightArray[NoType] = 0;
}
