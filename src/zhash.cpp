//
// Created by paulgrahek on 2/21/19.
//

#include <random>
#include "zhash.h"
#include "coutsync.h"

Bitboard ZobristHash::zPieceTable[PieceColorCount][PieceTypeCount+1][64];// = std::array<std::array<Bitboard,12>,64>();
Bitboard ZobristHash::zColorTable[PieceColorCount];
void ZobristHash::init() {
    std::mt19937_64 rd(232554);

    for(int i = 0; i < PieceColorCount; i++){
        for(int j = 0; j < PieceTypeCount+1; j++){
            for(int k = 0; k < 64; k++){
                Bitboard val = rd();
                if(j == NoType) val = 0;
                zPieceTable[i][j][k] = val;
            }
        }
    }
    zColorTable[White] = 0;
    zColorTable[Black] = rd();
//    auto table = zPieceTable;
}

Bitboard ZobristHash::hash(Position pos) {
    Bitboard hash = 0;
    for(PieceColor pc = White; pc != PieceColorCount; ++pc){
        for(PieceType pt = Pawn; pt != PieceTypeCount; ++pt){
            for(SquareIndex ind : getSquareIndexesForBitboard(pos.getPiecePositions(pc,pt))){
                hash ^= zPieceTable[pc][pt][ind];
            }
        }
    }
    hash ^= zColorTable[pos.getCurrentMoveColor()];
    return hash;
}
