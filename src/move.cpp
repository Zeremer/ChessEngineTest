//
// Created by paulgrahek on 2/6/19.
//

#include "move.h"

Move Moves::parseMoveString(std::string strMove) {
    for(int i = 0; i < strMove.size();i++){
        strMove[i] = (char)tolower(strMove[i]);
    }
    Move move = Move();
    if(strMove.size() < 4) return move;
    for(int i = 0; i < SquareCount;i++){
        //Check to make sure the square is not present twice in the move
        size_t validSquare = strMove.find(SquareIndexStrings[i]);
        if(validSquare != std::string::npos){
            if(validSquare < 2){
                move.startSquare = (SquareIndex)i;
            }
            else{
                move.endSquare = (SquareIndex)i;
            }
        }
    }
    if(move.startSquare == none || move.endSquare == none) return move;
    // It it is not another piece, assume it is a pawn.
    else move.pieceType = Pawn;
    for(int i = 0; i < PieceTypeCount; i++){
        if(!PieceTypeLowercaseShortStrings[i].empty()){
            std::string pieceTypeStr = PieceTypeLowercaseShortStrings[i];
            size_t firstInd = strMove.find_first_of(pieceTypeStr);
            if(firstInd != std::string::npos){
                if(pieceTypeStr == "b"){
                    // If bb is found, it is a bishop move
                    if(strMove.find_first_of("bb") == 0){
                        move.pieceType = Bishop;
                        break;
                    }
                    // If b is last in the string, it is a pawn promotion.
                    else if(strMove.find_last_of("b") == strMove.size()-1){
                        move.promotionType = Bishop;
                        move.pieceType = Pawn;
                        break;
                    }
                }
                //If the piecetypestring is at the front, it is a move
                else if(firstInd == 0){
                    move.pieceType = (PieceType)i;
                    break;
                }
                //If the piecetypestring is at the back, it is a promotion
                else if(firstInd == strMove.size()-1){
                    move.promotionType = (PieceType)i;
                    move.pieceType = Pawn;
                    break;
                }
            }

        }
    }

    // If it is a castling move, return as such.
    if(move.pieceType == Pawn &&
        ((move.startSquare == e1 && move.endSquare == g1) ||
         (move.startSquare == e1 && move.endSquare == c1) ||
         (move.startSquare == e8 && move.endSquare == g8) ||
         (move.startSquare == e8 && move.endSquare == c8))){
        move.pieceType = King;
        move.promotionType = NoType;
        return move;
    }
    //Nullmove: 0000
    return move;
}

std::string Moves::createMoveString(Move move){
    std::string moveString;
    if(move.pieceType == Pawn){
        moveString += SquareIndexStrings[move.startSquare];
        moveString += SquareIndexStrings[move.endSquare];
        moveString += PieceTypeShortStrings[move.promotionType];
    }
    else{
        // If it is a castling move
        if(move.pieceType != King ||
           !((move.startSquare == e1 && move.endSquare == g1) ||
            (move.startSquare == e1 && move.endSquare == c1) ||
            (move.startSquare == e8 && move.endSquare == g8) ||
            (move.startSquare == e8 && move.endSquare == c8))){
            moveString += PieceTypeShortStrings[move.pieceType];
        }
        moveString += SquareIndexStrings[move.startSquare];
        moveString += SquareIndexStrings[move.endSquare];
    }
    return moveString;
}