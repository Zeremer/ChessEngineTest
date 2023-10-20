//
// Created by paulgrahek on 1/28/19.
//
#pragma once

#include <vector>
#include "types.h"
#include "position.h"

namespace MoveGen {

    // Note: Move generation will return moves that are legal
    // rather than pseudo-legal moves, meaning that move
    // generation will not allow moves that put the king in
    // check.

    /**
     * @brief Get possible moves for a target bitboard
     *
     * Returns all possible moves given a starting square,
     * its piece type, and a bitboard of targets
     *
     * @param square starting square of moves
     * @param pieceType piece type of attacking piece
     * @param targets bitboard identifying targets the piece can move to
     * @param promotion sets the PieceType to promote to; defaults to NoType
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
    std::vector<Move> getMovesForBitboard(SquareIndex square, PieceType pieceType, Bitboard targets, Bitboard enemyBlockers, PieceType promotion = NoType);

    /**
     * @brief Get capture moves for a given piece
     * @param pos
     * @param square
     * @param pieceType
     * @param targets
     * @param enemyBlockers
     * @param promotion
     * @return
     * @author Paul Grahek
     */
    std::vector<Move> getCaptureMovesForBitboard(const Position& pos, SquareIndex square, PieceType pieceType, Bitboard targets, PieceColor enemyColor, PieceType promotion = NoType);

    /**
     * @brief Get non-capture moves for a given piece
     * @param pos
     * @param square
     * @param pieceType
     * @param targets
     * @param enemyBlockers
     * @param promotion
     * @return
     * @author Paul Grahek
     */
    std::vector<Move> getNonCaptureMovesForBitboard(const Position& pos, SquareIndex square, PieceType pieceType, Bitboard targets, PieceColor enemyColor, PieceType promotion = NoType);

    /**
     * @brief Return all possible moves for all pieces for the current moving color
     * @param pos reference to the position object
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     */
    std::vector<Move> getPossibleMoves(const Position& pos, bool capturesOnly = false);

    /**
     * @brief Return true if any moves are available for color (legal moves)
     *
     * @param pos Reference to the position object
     * @param colorToGen Color to move
     * @return true if any moves are available
     * @author Porter Glines
     */
    bool areAnyMovesPossible(const Position& pos, PieceColor colorToGen);

    /**
     * @brief Checks if a King is in check after a potential move
     *
     * @param pos Position of the board
     * @param move potential move
     * @param kingsColor color of the King
     * @return true if King is in check after move
     * @author Porter Glines 3/10/19
     */
    bool isKingInCheckAfterMove(const Position& pos, Move move, PieceColor kingsColor);

    /**
     * @brief Get possible moves for multiple king pieces
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
    std::vector<Move> getPossibleMovesForKings(const Position& position, PieceColor colorToGen);

    /**
     * @brief Get possible move count for multiple king pieces (pseudo-legal)
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return count of possible moves
     * @author Paul Grahek
     */
    int getPossibleMoveCountForKings(const Position& position, PieceColor colorToGen);

    /**
     * @brief Get possible moves for a king piece
     *
     * @param piece square index of the attacking king piece
     * @param friendlyBlockers friendly pieces
     * @param enemyBlockers enemy pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
    Bitboard getPossibleMovesForKing(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers);


     /**
     * @brief Get possible moves for multiple queen pieces
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
    std::vector<Move> getPossibleMovesForQueens(const Position& position, PieceColor colorToGen);

    /**
     * @brief Get possible move count for multiple queen pieces (pseudo-legal)
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return count of possible moves
     * @author Paul Grahek
     */
    int getPossibleMoveCountForQueens(const Position& position, PieceColor colorToGen);

     /**
     * @brief Get possible moves for a queen piece
     *
     * @param piece square index of the attacking queen piece
     * @param friendlyBlockers friendly pieces
     * @param enemyBlockers enemy pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
     Bitboard getPossibleMovesForQueen(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers);


     /**
     * @brief Get possible moves for multiple rook pieces
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
    std::vector<Move> getPossibleMovesForRooks(const Position& position, PieceColor colorToGen);


    /**
     * @brief Get possible move count for multiple rook pieces (pseudo-legal)
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return count of possible moves
     * @author Paul Grahek
     */
    int getPossibleMoveCountForRooks(const Position& position, PieceColor colorToGen);

     /**
     * @brief Get possible moves for a rook piece
     *
     * @param piece square index of the attacking rook piece
     * @param friendlyBlockers friendly pieces
     * @param enemyBlockers enemy pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
    Bitboard getPossibleMovesForRook(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers);


     /**
     * @brief Get possible moves for multiple bishop pieces
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
    std::vector<Move> getPossibleMovesForBishops(const Position& position, PieceColor colorToGen);

    /**
     * @brief Get possible move count for multiple bishop pieces (pseudo-legal)
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return count of possible moves
     * @author Paul Grahek
     */
    int getPossibleMoveCountForBishops(const Position& position, PieceColor colorToGen);

     /**
     * @brief Get possible moves for a bishop piece
     *
     * @param piece square index of the attacking bishop piece
     * @param friendlyBlockers friendly pieces
     * @param enemyBlockers enemy pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
     Bitboard getPossibleMovesForBishop(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers);


     /**
     * @brief Get possible moves for multiple knight pieces
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
    std::vector<Move> getPossibleMovesForKnights(const Position& position, PieceColor colorToGen);

    /**
     * @brief Get possible move count for multiple knight pieces (pseudo-legal)
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return count of possible moves
     * @author Paul Grahek
     */
    int getPossibleMoveCountForKnights(const Position& position, PieceColor colorToGen);

     /**
     * @brief Get possible moves for a knight piece
     *
     * @param piece square index of the attacking knight piece
     * @param friendlyBlockers friendly pieces
     * @param enemyBlockers enemy pieces
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/10/19
     */
     Bitboard getPossibleMovesForKnight(SquareIndex piece, Bitboard friendlyBlockers, Bitboard enemyBlockers);


     /**
     * @brief Get possible moves for multiple pawn pieces
     *
     * @param position reference to the position object
     * @param color piece color of moving pieces
     * @param enpassantSq the square behind a pawn indicating that en passant is valid
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/12/19
     */
    std::vector<Move> getPossibleMovesForPawns(const Position& position, PieceColor color, SquareIndex enpassantSq);

    /**
     * @brief Get possible move count for multiple pawn pieces (pseudo-legal)
     *
     * @param position reference to the position object
     * @param colorToGen color of moving pieces
     * @return count of possible moves
     * @author Paul Grahek
     */
    int getPossibleMoveCountForPawns(const Position& position, PieceColor colorToGen,SquareIndex enpassantSq);

     /**
     * @brief Get possible moves for a pawn piece
     *
     * @param piece square index of the attacking pawn piece
     * @param color piece color of moving pieces
     * @param friendlyBlockers friendly pieces
     * @param enemyBlockers enemy pieces
     * @param enpassantSq the square behind a pawn indicating that en passant is valid
     * @return vector of Move elements (startSquare, endSquare, pieceType)
     * @author Porter Glines 2/12/19
     */
     Bitboard getPossibleMovesForPawn(SquareIndex piece, PieceColor color, Bitboard friendlyBlockers, Bitboard enemyBlockers,
                                                  SquareIndex enpassantSq);

     /**
      * @brief Get possible move count for castling moves
      *
      * @param position reference to position object
      * @param colorToGen color of moving pieces
      * @return count of possible moves
      * @author Porter Glines 3/10/19
      */
     int getPossibleMoveCountForCastling(const Position& position, PieceColor colorToGen);

     /**
      * @brief Get possible castling moves
      *
      * Moves are returned as moves in the perspective of the King
      *
      * @param position reference to the position object
      * @param colorToGen color of moving pieces
      * @return vector of possible King moves
      * @author Porter Glines 2/28/19
      */
     std::vector<Move> getCastlingMoves(const Position& position, PieceColor colorToGen);

    /**
     * @brief Get positive ray attacks for a sliding piece accounting for blockers
     *
     * Positive rays include North, NorthWest, NorthEast, and East
     *
     * This function uses a method that does not branch, therefore
     * should be faster than a similar function with branching on
     * modern CPU's with branch prediction.
     *
     * @param direction positive direction of the ray
     * @param blockers occupied squares that will block an attack
     * @param square starting square of attacking piece
     * @return bitboard representing the possible attacks the piece can make on the ray
     * @author Porter Glines 2/10/19
     */
    Bitboard getPositiveRayAttacks(Direction direction, Bitboard blockers, SquareIndex square);

     /**
     * @brief Get negative ray attacks for a sliding piece accounting for blockers
     *
     * Negative rays include South, SouthWest, SouthEast, and West
     *
     * This function uses a method that does not branch, therefore
     * should be faster than a similar function with branching on
     * modern CPU's with branch prediction.
     *
     * @param direction positive direction of the ray
     * @param blockers occupied squares that will block an attack
     * @param square starting square of attacking piece
     * @return bitboard representing the possible attacks the piece can make on the ray
     * @author Porter Glines 2/10/19
     */
    Bitboard getNegativeRayAttacks(Direction direction, Bitboard blockers, SquareIndex square);

};
