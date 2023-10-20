//
// Created by paulgrahek on 1/28/19.
//

#include <algorithm>
#include "movegen.h"
#include "bitboard.h"


std::vector<Move> MoveGen::getMovesForBitboard(SquareIndex square, PieceType pieceType, Bitboard targets,Bitboard enemyBlockers, PieceType promotion) {
    std::vector<Move> moves;
    // TODO: pre-allocate space for moves

    while (targets) {
        // Set the target to the least significant bit on the attack board
        auto target = (SquareIndex)(bitScanForward(targets) - 1);

        Move mv = Move();
        mv.startSquare = square;
        mv.endSquare = target;
        mv.pieceType = pieceType;
        mv.promotionType = promotion;
        if(enemyBlockers & SquareBB[target])mv.type = Equal;
        // TODO: include reference to position and update to only allow legal moves
//        mv.stateChangedTo = MoveGen::isKingInCheckAfterMove(pos, mv, enemyColor) ? Check : Normal;
        moves.push_back(mv);

        // remove pieces from bitboard bottom up (0 to 63)
        targets &= targets - 1;
    }
    return moves;
}


std::vector<Move>
MoveGen::getCaptureMovesForBitboard(const Position &pos, SquareIndex square, PieceType pieceType, Bitboard targets,
                                    PieceColor enemyColor, PieceType promotion) {
    std::vector<Move> moves;
    // TODO: pre-allocate space for moves
    //Remove moves that do not result in captures
    targets &= pos.getColorPosition(enemyColor);
    while (targets) {
        // Set the target to the least significant bit on the attack board
        auto target = (SquareIndex)(bitScanForward(targets) - 1);

        Move mv = Move();
        mv.startSquare = square;
        mv.endSquare = target;
        mv.pieceType = pieceType;
        mv.promotionType = promotion;
        // Do not add move if it exposes the King (not a legal move)
        if (!MoveGen::isKingInCheckAfterMove(pos, mv, FlipColor(enemyColor))) {
            double SEE = pos.SEE(mv);
            if (SEE > 0)
                mv.type = Winning;
            else if (SEE < 0)
                mv.type = LosingCapture;
            else
                mv.type = Equal;
            mv.stateChangedTo = MoveGen::isKingInCheckAfterMove(pos, mv, enemyColor) ? Check : Normal;
            moves.push_back(mv);
        }

        // remove pieces from bitboard bottom up (0 to 63)
        targets &= targets - 1;
    }
    return moves;
}


std::vector<Move>
MoveGen::getNonCaptureMovesForBitboard(const Position &pos, SquareIndex square, PieceType pieceType, Bitboard targets,
                                    PieceColor enemyColor, PieceType promotion) {
    std::vector<Move> moves;
    // TODO: pre-allocate space for moves
    //Remove moves that result in captures
    targets &= ~pos.getColorPosition(enemyColor);
    while (targets) {
        // Set the target to the least significant bit on the attack board
        auto target = (SquareIndex)(bitScanForward(targets) - 1);

        Move mv = Move();
        mv.startSquare = square;
        mv.endSquare = target;
        mv.pieceType = pieceType;
        mv.promotionType = promotion;
        // Do not add move if it exposes the King (not a legal move)
        if (!MoveGen::isKingInCheckAfterMove(pos, mv, FlipColor(enemyColor))) {
//            mv.type = NonCapture;
            mv.stateChangedTo = MoveGen::isKingInCheckAfterMove(pos, mv, enemyColor) ? Check : Normal;
            mv.type = (mv.stateChangedTo == Check)? Killer : NonCapture;
            moves.push_back(mv);
        }

        // remove pieces from bitboard bottom up (0 to 63)
        targets &= targets - 1;
    }
    return moves;
}


std::vector<Move> MoveGen::getPossibleMoves(const Position& pos, bool capturesOnly) {
    std::vector<Move> retVector;
    std::vector<Move> copyVector;
    combineVectors(retVector, getPossibleMovesForPawns(pos,pos.getCurrentMoveColor(),pos.getEnPassantSquare()));
    combineVectors(retVector, getPossibleMovesForQueens(pos,pos.getCurrentMoveColor()));
    combineVectors(retVector, getPossibleMovesForBishops(pos,pos.getCurrentMoveColor()));
    combineVectors(retVector, getPossibleMovesForRooks(pos,pos.getCurrentMoveColor()));
    combineVectors(retVector, getPossibleMovesForKnights(pos,pos.getCurrentMoveColor()));
    combineVectors(retVector, getPossibleMovesForKings(pos,pos.getCurrentMoveColor()));
    combineVectors(retVector, getCastlingMoves(pos,pos.getCurrentMoveColor()));

    if(capturesOnly){
        retVector.erase(std::remove_if(retVector.begin(),retVector.end(),[](const Move& x){
            return !((x.type == Winning) || x.type == Equal || x.type == LosingCapture);
        }),retVector.end());
    }
    return retVector;
}


bool MoveGen::areAnyMovesPossible(const Position& pos, PieceColor colorToGen) {
    // Only get moves that are legal

    if ((int) getPossibleMovesForKnights(pos, colorToGen).size() != 0)
        return true;
    else if ((int) getPossibleMovesForKings(pos, colorToGen).size() != 0)
        return true;
    else if ((int) getPossibleMovesForQueens(pos, colorToGen).size() != 0)
        return true;
    else if ((int) getPossibleMovesForRooks(pos, colorToGen).size() != 0)
        return true;
    else if ((int) getPossibleMovesForBishops(pos, colorToGen).size() != 0)
        return true;
    else if ((int) getPossibleMovesForPawns(pos, colorToGen, pos.getEnPassantSquare()).size() != 0)
        return true;
    else if (getPossibleMoveCountForCastling(pos, colorToGen) != 0)
        return true;

    return false;

    // Without branching
//    int count = (int) getPossibleMovesForKnights(pos, colorToGen).size()
//          + (int) getPossibleMovesForKings(pos, colorToGen).size()
//          + (int) getPossibleMovesForQueens(pos, colorToGen).size()
//          + (int) getPossibleMovesForRooks(pos, colorToGen).size()
//          + (int) getPossibleMovesForBishops(pos, colorToGen).size()
//          + (int) getPossibleMovesForPawns(pos, colorToGen, pos.getEnPassantSquare()).size()
//          + getPossibleMoveCountForCastling(pos, colorToGen);
//
//    return (count != 0);
}



bool MoveGen::isKingInCheckAfterMove(const Position& pos, Move move, PieceColor kingsColor) {
    PieceColor otherColor = FlipColor(kingsColor);
    Bitboard notStart = ~SquareBB[move.startSquare];
    Bitboard end = SquareBB[move.endSquare];
    Bitboard notEnd = ~end;
    PieceType destType = (move.promotionType == NoType) ? move.pieceType : move.promotionType;

    auto colorBoards = pos.getColorPositions();
    auto pieceBoards = pos.getPieceBoards();

    // 1. Move the piece for the position

    // Update color boards
    colorBoards[kingsColor] &= notStart;
    colorBoards[kingsColor] |= end;
    colorBoards[otherColor] &= notEnd;

    // Update piece boards
    pieceBoards[King] &= notEnd;
    pieceBoards[Queen] &= notEnd;
    pieceBoards[Rook] &= notEnd;
    pieceBoards[Bishop] &= notEnd;
    pieceBoards[Knight] &= notEnd;
    pieceBoards[Pawn] &= notEnd;

    pieceBoards[move.pieceType] &= notStart;
    pieceBoards[destType] |= end;

    // Dynamically update rook position
    colorBoards[kingsColor] &= ~SquareBB[move.castleRookStart]; // & not start
    colorBoards[kingsColor] |= SquareBB[move.castleRookEnd]; // | end
    pieceBoards[Rook] &= ~SquareBB[move.castleRookStart]; // & not start
    pieceBoards[Rook] |= SquareBB[move.castleRookEnd]; // | end

    // 2. Calculate attackers to King after move

    auto kingBB = pieceBoards[King] & colorBoards[kingsColor];
    if (kingBB == 0) return true; // if no King, return true
    auto kingSquare = (SquareIndex)(bitScanForward(kingBB) - 1);

    auto allBlockers = colorBoards[Black] | colorBoards[White];
    auto rooksQueens = (pieceBoards[Queen] | pieceBoards[Rook]);
    auto bishopQueens = (pieceBoards[Queen] | pieceBoards[Bishop]);
    auto attacksToKingSq = ((PawnAttacks[White][kingSquare] & pieceBoards[Pawn] & colorBoards[Black])
                         |   (PawnAttacks[Black][kingSquare] & pieceBoards[Pawn] & colorBoards[White])
                         |   (PieceAttacks[Knight][kingSquare] & pieceBoards[Knight])
                         |   (PieceAttacks[King][kingSquare] & pieceBoards[King])
                         |   (MoveGen::getPossibleMovesForBishop(kingSquare,0,allBlockers) & bishopQueens)
                         |   (MoveGen::getPossibleMovesForRook(kingSquare,0,allBlockers) & rooksQueens));


    Bitboard attacksToKing = attacksToKingSq & ~colorBoards[kingsColor];

    return (bool)attacksToKing;
}


std::vector<Move> MoveGen::getPossibleMovesForKings(const Position& position, PieceColor colorToGen) {
    std::vector<Move> moves;
    // TODO: pre-allocate space for moves

    Bitboard pieces = position.getPiecePositions(colorToGen, King);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);

    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard =  getPossibleMovesForKing(piece, friendlyBlockers, enemyBlockers);

        //Get passive moves for bitboard
        std::vector<Move> newMoves = getNonCaptureMovesForBitboard(position,piece, King, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

        //Get capture moves for bitboard
        newMoves = getCaptureMovesForBitboard(position,piece, King, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moves;
}


int MoveGen::getPossibleMoveCountForKings(const Position& position, PieceColor colorToGen) {
    int moveCount = 0;

    Bitboard pieces = position.getPiecePositions(colorToGen, King);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);

    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard =  getPossibleMovesForKing(piece, friendlyBlockers, enemyBlockers);
        moveCount += numOnes64(attackBoard);
        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moveCount;
}


Bitboard MoveGen::getPossibleMovesForKing(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers) {
    Bitboard attackBoard = PieceAttacks[King][piece];

    // Remove blockers from attackboard by subtracting a mask
    attackBoard = (attackBoard & ~friendlyBlockers);

    return attackBoard;
}


std::vector<Move> MoveGen::getPossibleMovesForQueens(const Position& position, PieceColor colorToGen) {
    std::vector<Move> moves;
    // TODO: pre-allocate space for moves
    Bitboard pieces = position.getPiecePositions(colorToGen, Queen);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);

    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard =  getPossibleMovesForQueen(piece, friendlyBlockers, enemyBlockers);

        //Get passive moves for bitboard
        std::vector<Move> newMoves = getNonCaptureMovesForBitboard(position,piece, Queen, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

        //Get capture moves for bitboard
        newMoves = getCaptureMovesForBitboard(position,piece, Queen, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moves;
}


int MoveGen::getPossibleMoveCountForQueens(const Position& position, PieceColor colorToGen) {
    int moveCount = 0;
    // TODO: pre-allocate space for moves
    Bitboard pieces = position.getPiecePositions(colorToGen, Queen);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);

    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard =  getPossibleMovesForQueen(piece, friendlyBlockers, enemyBlockers);

        // Append new moves to total moves vector
        moveCount += numOnes64(attackBoard);

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moveCount;
}


Bitboard MoveGen::getPossibleMovesForQueen(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers) {
    Bitboard allBlockers = friendlyBlockers | enemyBlockers;
    Bitboard attackBoard = getPositiveRayAttacks(North, allBlockers, piece)
                           | getPositiveRayAttacks(East, allBlockers, piece)
                           | getNegativeRayAttacks(South, allBlockers, piece)
                           | getNegativeRayAttacks(West, allBlockers, piece)
                           | getPositiveRayAttacks(NorthEast, allBlockers, piece)
                           | getPositiveRayAttacks(NorthWest, allBlockers, piece)
                           | getNegativeRayAttacks(SouthEast, allBlockers, piece)
                           | getNegativeRayAttacks(SouthWest, allBlockers, piece);
    attackBoard &= ~friendlyBlockers;

    return attackBoard;
}


std::vector<Move> MoveGen::getPossibleMovesForRooks(const Position& position, PieceColor colorToGen) {
    std::vector<Move> moves;
    // TODO: pre-allocate space for moves
    Bitboard pieces = position.getPiecePositions(colorToGen, Rook);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);
    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard = getPossibleMovesForRook(piece, friendlyBlockers, enemyBlockers);
        //Get passive moves for bitboard
        std::vector<Move> newMoves = getNonCaptureMovesForBitboard(position,piece, Rook, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

        //Get capture moves for bitboard
        newMoves = getCaptureMovesForBitboard(position,piece, Rook, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));
        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moves;
}


int MoveGen::getPossibleMoveCountForRooks(const Position& position, PieceColor colorToGen) {
    int moveCount = 0;
    // TODO: pre-allocate space for moves
    Bitboard pieces = position.getPiecePositions(colorToGen, Rook);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);
    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard = getPossibleMovesForRook(piece, friendlyBlockers, enemyBlockers);
        // Append new moves to total moves vector
        moveCount += numOnes64(attackBoard);

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moveCount;
}


Bitboard MoveGen::getPossibleMovesForRook(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers) {
    Bitboard allBlockers = friendlyBlockers | enemyBlockers;
    Bitboard attackBoard = getPositiveRayAttacks(North, allBlockers, piece)
                           | getPositiveRayAttacks(East, allBlockers, piece)
                           | getNegativeRayAttacks(South, allBlockers, piece)
                           | getNegativeRayAttacks(West, allBlockers, piece);
    attackBoard &= ~friendlyBlockers;

    return attackBoard;
}


std::vector<Move> MoveGen::getPossibleMovesForBishops(const Position& position, PieceColor colorToGen) {
    std::vector<Move> moves;
    // TODO: pre-allocate space for moves

    Bitboard pieces = position.getPiecePositions(colorToGen, Bishop);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);
    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard = getPossibleMovesForBishop(piece, friendlyBlockers, enemyBlockers);

        //Get passive moves for bitboard
        std::vector<Move> newMoves = getNonCaptureMovesForBitboard(position,piece, Bishop, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

        //Get capture moves for bitboard
        newMoves = getCaptureMovesForBitboard(position,piece, Bishop, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moves;
}


int MoveGen::getPossibleMoveCountForBishops(const Position& position, PieceColor colorToGen) {
    int moveCount = 0;
    // TODO: pre-allocate space for moves

    Bitboard pieces = position.getPiecePositions(colorToGen, Bishop);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);
    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard = getPossibleMovesForBishop(piece, friendlyBlockers, enemyBlockers);

        // Append new moves to total moves vector
        moveCount += numOnes64(attackBoard);

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moveCount;
}


Bitboard MoveGen::getPossibleMovesForBishop(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers) {
    Bitboard allBlockers = friendlyBlockers | enemyBlockers;
    Bitboard attackBoard = getPositiveRayAttacks(NorthEast, allBlockers, piece)
                         | getPositiveRayAttacks(NorthWest, allBlockers, piece)
                         | getNegativeRayAttacks(SouthEast, allBlockers, piece)
                         | getNegativeRayAttacks(SouthWest, allBlockers, piece);
    attackBoard &= ~friendlyBlockers;

    return attackBoard;
}


std::vector<Move> MoveGen::getPossibleMovesForKnights(const Position& position, PieceColor colorToGen) {
    std::vector<Move> moves;
    // TODO: pre-allocate space for moves
    Bitboard pieces = position.getPiecePositions(colorToGen, Knight);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);
    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard = getPossibleMovesForKnight(piece, friendlyBlockers, enemyBlockers);
        //Get passive moves for bitboard
        std::vector<Move> newMoves = getNonCaptureMovesForBitboard(position,piece, Knight, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

        //Get capture moves for bitboard
        newMoves = getCaptureMovesForBitboard(position,piece, Knight, attackBoard,otherColor);
        // Append new moves to total moves vector
        moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moves;
}


int MoveGen::getPossibleMoveCountForKnights(const Position& position, PieceColor colorToGen) {
    int moveCount = 0;
    // TODO: pre-allocate space for moves
    Bitboard pieces = position.getPiecePositions(colorToGen, Knight);
    PieceColor otherColor = FlipColor(colorToGen);
    Bitboard friendlyBlockers = position.getColorPosition(colorToGen);
    Bitboard enemyBlockers = position.getColorPosition(otherColor);
    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard attackBoard = getPossibleMovesForKnight(piece, friendlyBlockers, enemyBlockers);
        // Append new moves to total moves vector
        moveCount += numOnes64(attackBoard);

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moveCount;
}


Bitboard MoveGen::getPossibleMovesForKnight(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers) {
    Bitboard attackBoard = PieceAttacks[Knight][piece];
    // Remove blockers from attackboard by subtracting a mask
    attackBoard = (attackBoard & ~friendlyBlockers);

    return attackBoard;
}


std::vector<Move> MoveGen::getPossibleMovesForPawns(const Position &position, PieceColor color, SquareIndex enpassantSq) {
    std::vector<Move> moves;
    Bitboard pieces = position.getPiecePositions(color, Pawn);
    PieceColor otherColor = FlipColor(color);
    Bitboard friendlyBlockers = position.getColorPosition(color);
    Bitboard enemyBlockers = position.getColorPosition(FlipColor(color));
    Bitboard promotionRank = (color == White?Rank8BB:Rank1BB);
    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard moveBoard = getPossibleMovesForPawn(piece, color, friendlyBlockers, enemyBlockers, enpassantSq);
        Bitboard promotionBoard = moveBoard & promotionRank;
        {
            // Non-promotion moves
            //Get passive moves for bitboard
            std::vector<Move> newMoves = getNonCaptureMovesForBitboard(position,piece, Pawn, moveBoard,otherColor,NoType);
            // Append new moves to total moves vector
            moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

            //Get capture moves for bitboard
            newMoves = getCaptureMovesForBitboard(position,piece, Pawn, moveBoard,otherColor,NoType);
            // Append new moves to total moves vector
            moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));
        }
        {
            // Promotion moves
            std::vector<PieceType> promotionTypes { Knight, Rook, Bishop, Queen};
            for(auto promotion : promotionTypes){
                //Get passive moves for bitboard
                std::vector<Move> newMoves = getNonCaptureMovesForBitboard(position,piece, Pawn, promotionBoard,otherColor,promotion);
                // Append new moves to total moves vector
                moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));

                //Get capture moves for bitboard
                newMoves = getCaptureMovesForBitboard(position,piece, Pawn, promotionBoard,otherColor,promotion);
                // Append new moves to total moves vector
                moves.insert(std::end(moves), std::begin(newMoves), std::end(newMoves));
            }
        }

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moves;
}


int MoveGen::getPossibleMoveCountForPawns(const Position &position, PieceColor color, SquareIndex enpassantSq) {
    int moveCount = 0;
    Bitboard pieces = position.getPiecePositions(color, Pawn);
    Bitboard friendlyBlockers = position.getColorPosition(color);
    Bitboard enemyBlockers = position.getColorPosition(FlipColor(color));

    // Iterate over pieces
    while (pieces) {
        auto piece = (SquareIndex)(bitScanForward(pieces) - 1);
        Bitboard moveBoard = getPossibleMovesForPawn(piece, color, friendlyBlockers, enemyBlockers, enpassantSq);
        // Append new moves to total moves vector
        moveCount += numOnes64(moveBoard);

        // remove pieces from pieces bitboard
        pieces &= pieces - 1;
    }
    return moveCount;
}


Bitboard MoveGen::getPossibleMovesForPawn(SquareIndex piece, PieceColor color, Bitboard friendlyBlockers,
                                                   Bitboard enemyBlockers, SquareIndex enpassantSq) {
    Bitboard allBlockers = friendlyBlockers | enemyBlockers;
    Bitboard moveBoard;
    Bitboard attackBoard;

    if (color == White) {
        // Move piece up for white
        // If on starting position, allow 2 squares of movement
        moveBoard = (SquareBB[piece] & Rank2BB) ? SquareBB[piece] << 8 | SquareBB[piece] << (8 * 2) :
                                                  SquareBB[piece] << 8;

        // Forward movement is blocked for any piece
        moveBoard = (moveBoard & getPositiveRayAttacks(North, allBlockers, piece) & ~(allBlockers));

        // Get precompiled attack options for pawn; & it with enemy pieces
        attackBoard = PawnAttacks[color][piece] & (enemyBlockers | SquareBB[enpassantSq]);

    }
    else {
         // Move piece down for black
        // If on starting position, allow 2 squares of movement
        moveBoard = (SquareBB[piece] & Rank7BB) ? SquareBB[piece] >> 8 | SquareBB[piece] >> (8 * 2) :
                                                  SquareBB[piece] >> 8;

        // Forward movement is blocked for any piece
        moveBoard = (moveBoard & getNegativeRayAttacks(South, allBlockers, piece) & ~(allBlockers));

        // Get precompiled attack options for pawn; & it with enemy pieces
        attackBoard = PawnAttacks[color][piece] & (enemyBlockers | SquareBB[enpassantSq]);

   }

     // Combine pawn move and pawn capture
    moveBoard |= attackBoard;

    // Remove friendly blockers from attackboard by subtracting a mask
    moveBoard = (moveBoard & ~friendlyBlockers);

//    // TODO: Remove branch if possible
//    // Handle enpassant
//    if (enpassantSq != none) {
////        Bitboard enpassantTarget = (color == White) ? SquareBB[enpassantSq] << 8 : SquareBB[enpassantSq] >> 8;
//        Bitboard  enpassantTarget = SquareBB[enpassantSq];
//
//        // Combine enpassant target if not occupied by friendly blocker
//        moveBoard |= (enpassantTarget & ~friendlyBlockers);
//    }
    return moveBoard;
}


int MoveGen::getPossibleMoveCountForCastling(const Position& position, PieceColor colorToGen) {
    return (int)getCastlingMoves(position, colorToGen).size();
}


std::vector<Move> MoveGen::getCastlingMoves(const Position &position, PieceColor colorToGen) {
    std::vector<Move> moves;
    Bitboard castlingBoard = position.getCastlingBoard();
    Bitboard king = position.getPiecePositions(colorToGen, King) & castlingBoard;
    Bitboard rooks = position.getPiecePositions(colorToGen, Rook) & castlingBoard;
    Bitboard blockers = position.getColorPosition(White) | position.getColorPosition(Black);
    Bitboard moveBoard = 0;

    if (king == 0 || rooks == 0) {
        return moves;
    }

    auto mv = Move();
    mv.pieceType = King;
    mv.startSquare = (SquareIndex)(bitScanForward(king) - 1);

    // East Castle Move
    Bitboard eastRay = rayAttacks[East][bitScanForward(king) - 1];
    bool isEastValid = true;

    // Check if east rook has moved
    isEastValid = (bool)(rooks & FileHBB);

    // Check for pieces blocking path
    isEastValid = (isEastValid) ? (bool)!(eastRay & (blockers & ~rooks)) : false;

    // Check if path is safe for King
    if (isEastValid) {
        eastRay &= (FileFBB | FileGBB);  // only check path of King

        while (eastRay) {
            // Set the target to the least significant bit on the attack board
            auto target = (SquareIndex)(bitScanForward(eastRay) - 1);

            // Get enemy attackers to square
            Bitboard attackers = position.AttacksToSquare(target, blockers) & ~position.getColorPosition(colorToGen);

            // If attackers are present
            if (attackers) {
                isEastValid = false;
                break;
            }

            // remove pieces from bitboard bottom up (0 to 63)
            eastRay &= eastRay - 1;
        }
    }
    if (isEastValid) {
        // King moves two squares towards rook
        mv.endSquare = (SquareIndex)(bitScanForward(king << 2) - 1);
        // Rook moves two spaces towards king
        mv.castleRookStart = (SquareIndex)(bitScanForward(rooks & FileHBB) - 1);
        mv.castleRookEnd = (SquareIndex)(bitScanForward((rooks & FileHBB) >> 2) - 1);
        moves.push_back(mv);
    }


    // West Castle Move
    Bitboard westRay = rayAttacks[West][bitScanForward(king) - 1];
    bool isWestValid = true;

    // Check if west rook has moved
    isWestValid = (bool)(rooks & FileABB);

    // Check for pieces blocking path
    isWestValid = (isWestValid) ? (bool)!(westRay & (blockers & ~rooks)) : false;

    // Check if path is safe for King
    if (isWestValid) {
        westRay &= (FileCBB | FileDBB);  // only check path of King

        while (westRay) {
            // Set the target to the least significant bit on the attack board
            auto target = (SquareIndex)(bitScanForward(westRay) - 1);

            // Get enemy attackers to square
            Bitboard attackers = position.AttacksToSquare(target, blockers) & ~position.getColorPosition(colorToGen);

            // If attackers are present
            if (attackers) {
                isWestValid = false;
                break;
            }

            // remove pieces from bitboard bottom up (0 to 63)
            westRay &= westRay - 1;
        }
    }
    if (isWestValid) {
        // King moves two squares towards rook
        mv.endSquare = (SquareIndex)(bitScanForward(king >> 2) - 1);
        // Rook moves three squares towards king
        mv.castleRookStart = (SquareIndex)(bitScanForward(rooks & FileABB) - 1);
        mv.castleRookEnd = (SquareIndex)(bitScanForward((rooks & FileABB) << 3) - 1);
        moves.push_back(mv);
    }

    return moves;
}


Bitboard MoveGen::getPositiveRayAttacks(Direction direction, Bitboard blockers, SquareIndex square) {
    Bitboard attacks = rayAttacks[direction][square];
    Bitboard realBlockers = attacks & blockers;

    // To ensure that we don't run a forward bitscan on an empty board, we let it scan
    // any outer square (we'll pick h8). This does not affect our final
    // attack options whether or not h8 is a blocker or in the right direction

    // We set square to the closest blocker from our attacking piece
    square = (SquareIndex)(bitScanForward(realBlockers | 0x8000000000000000) - 1);

    // We run xor on our original attack vector (attacks) and a ray in the
    // same direction but starting at the closest blocker
    return attacks ^ rayAttacks[direction][square];
}


Bitboard MoveGen::getNegativeRayAttacks(Direction direction, Bitboard blockers, SquareIndex square) {
    Bitboard attacks = rayAttacks[direction][square];
    Bitboard realBlockers = attacks & blockers;

    // To ensure that we don't run a reverse bitscan on an empty board, we let it scan
    // any outer square (we'll pick a1). This does not affect our final
    // attack options whether or not h8 is a blocker or in the right direction

    // We set square to the closest blocker from our attacking piece
    square = (SquareIndex)(bitScanReverse(realBlockers | 1) - 1);

    // We run xor on our original attack vector (attacks) and a ray in the
    // same direction but starting at the closest blocker
    return attacks ^ rayAttacks[direction][square];
}

