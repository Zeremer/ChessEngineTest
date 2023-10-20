//
// Created by paulgrahek on 4/10/19.
//

#pragma once

#include "types.h"
#include "position.h"

void testUndoMove(){
    Position pos;
    Position nextPos(pos);
    Move mv;
    mv.startSquare = g1;
    mv.endSquare = h3;
    mv.pieceType = Knight;
    Move mv2;
    mv2.startSquare = g7;
    mv2.endSquare = g5;
    mv2.pieceType = Pawn;
    Move mv3;
    mv3.startSquare = h3;
    mv3.endSquare = g5;
    mv3.pieceType = Knight;
    nextPos.move(mv);
    nextPos.move(mv2);
    nextPos.move(mv3);
    pos.move(mv);
    pos.move(mv2);
//    pos.undoLastMove();
//    pos.undoLastMove();
//    nextPos.undoLastMove();
//    nextPos.undoLastMove();
    nextPos.undoLastMove();
    assert(nextPos.getHash() == pos.getHash());
    assert(nextPos.getPieceBoards() == pos.getPieceBoards());
    assert(nextPos.getColorPositions() == pos.getColorPositions());
}