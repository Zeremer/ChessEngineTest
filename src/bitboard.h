//
// Created by paulp on 1/16/2019.
//

#pragma once

#include "types.h"

namespace Bitboards {

    /**
     * @brief Initialize bitboards, including precompiling attack vectors
     */
    void init();


    /**
     * @brief Precompile attack vectors on an empty board.
     *
     * Fills the PieceAttacks 2D array [piece][square] with
     * bitmaps representing valid movements on an empty board
     * given a starting square
     *
     * @author Porter Glines 1/24/19
     */
    void compileAttacks();


    /**
     * @brief Precompile basic rays on an empty board
     *
     * Fills all the basic rays arrays (e.g. NorthEastRays[square]),
     * all basic arrays include North/East/South/West and the 4
     * diagonals in between.
     *
     * @author Porter Glines 1/28/19
     */
    void compileRays();


    /**
     * @brief Precompile sliding piece attacks (Rook, Bishop, and Queen)
     *
     * Utilizes the basic rays which should be compiled before this
     * function is called. Fills the PieceAttack array for the
     * Rook, Bishop, and Queen piece types at all 64 squares
     * with attack bitboards
     *
     * @author Porter Glines 1/31/19
     */
    void compileSlidingAttacks();


    /**
     * @brief Precompile knight attacks
     *
     * Utilizes simple bit shifts to compute a knights
     * attacks at every square. Fills the PieceAttack
     * array for the Knight piece type at all 64 squares
     * with attack bitboards
     *
     * @author Porter Glines 1/31/19
     */
    void compileKnightAttacks();


    /**
     * @brief Precompile Pawn attacks (black and white)
     *
     * Black and white pawns are compiled separately as their
     * attacks are based on the direction they face. White is
     * assumed to start at the bottom of the board facing north.
     * Fills us the WhitePawnAttacks and BlackPAwnAttacks arrays
     * with pawn attack bitboards at all 64 squares.
     *
     * En passant is calculated in move generation and is not
     * precompiled.
     *
     * @author Porter Glines 1/31/19
     */
    void compilePawnAttacks();


    /**
     * @brief Precompile King attacks
     *
     * Utilizes the "move one in a direction" functions
     * like northOne() and southEastOne()
     *
     * @author Porter Glines 1/31/19
     */
    void compileKingAttacks();


    /**
     * @brief Shifts a bitboard North one square
     * @param bb bitboard to shift
     * @return a bitboard that has been shifted
     * @author Porter Glines 1/31
     */
    Bitboard northOne(Bitboard bb);
     /**
     * @brief Shifts a bitboard East one square
     * @param bb bitboard to shift
     * @return a bitboard that has been shifted
     * @author Porter Glines 1/31
     */
    Bitboard eastOne(Bitboard bb);
     /**
     * @brief Shifts a bitboard South one square
     * @param bb bitboard to shift
     * @return a bitboard that has been shifted
     * @author Porter Glines 1/31
     */
    Bitboard southOne(Bitboard bb);
     /**
     * @brief Shifts a bitboard West one square
     * @param bb bitboard to shift
     * @return a bitboard that has been shifted
     * @author Porter Glines 1/31
     */
    Bitboard westOne(Bitboard bb);

    // TODO: Optimize so they are diagonals are only 1 bitshift operation
      /**
     * @brief Shifts a bitboard North East one square
     * @param bb bitboard to shift
     * @return a bitboard that has been shifted
     * @author Porter Glines 1/31
     */
    Bitboard northEastOne(Bitboard bb);
      /**
     * @brief Shifts a bitboard South East one square
     * @param bb bitboard to shift
     * @return a bitboard that has been shifted
     * @author Porter Glines 1/31
     */
    Bitboard southEastOne(Bitboard bb);
      /**
     * @brief Shifts a bitboard South West one square
     * @param bb bitboard to shift
     * @return a bitboard that has been shifted
     * @author Porter Glines 1/31
     */
    Bitboard southWestOne(Bitboard bb);
      /**
     * @brief Shifts a bitboard North West one square
     * @param bb bitboard to shift
     * @return a bitboard that has been shifted
     * @author Porter Glines 1/31
     */
    Bitboard northWestOne(Bitboard bb);


    /**
     * @brief Precompile SquareBB mapping for a given square
     *
     * Fills SquareBB with mappings from a given square to a bitboard.
     *
     * @author Paul Grahek 1/28/2019
     */
    void compileSquareBB();

    /**
     * @brief Precompile arrays for EnPassant bitboards
     *
     * @author Paul Grahek
     */
    void compileEnPassantBB();
}

constexpr Bitboard wholeBoard = 0xFFFFFFFFFFFFFFFF;
constexpr Bitboard FileABB = 0x0101010101010101;
constexpr Bitboard FileBBB = FileABB << 1;
constexpr Bitboard FileCBB = FileABB << 2;
constexpr Bitboard FileDBB = FileABB << 3;
constexpr Bitboard FileEBB = FileABB << 4;
constexpr Bitboard FileFBB = FileABB << 5;
constexpr Bitboard FileGBB = FileABB << 6;
constexpr Bitboard FileHBB = FileABB << 7;

constexpr Bitboard Rank1BB = 0x00000000000000FF;
constexpr Bitboard Rank2BB = Rank1BB << (8 * 1);
constexpr Bitboard Rank3BB = Rank1BB << (8 * 2);
constexpr Bitboard Rank4BB = Rank1BB << (8 * 3);
constexpr Bitboard Rank5BB = Rank1BB << (8 * 4);
constexpr Bitboard Rank6BB = Rank1BB << (8 * 5);
constexpr Bitboard Rank7BB = Rank1BB << (8 * 6);
constexpr Bitboard Rank8BB = Rank1BB << (8 * 7);

constexpr Bitboard LightSquares = 0x55AA55AA55AA55AA;
constexpr Bitboard DarkSquares = ~LightSquares;

constexpr Bitboard a1h8diagonal = 0x8040201008040201;
constexpr Bitboard h1a8antidiagonal = 0x0102040810204080;


/**
 * @brief Bitboard representing the square a1
 */
constexpr Bitboard oneBB = 1;

/**
 * @brief Everything true except A-file
 */
constexpr Bitboard notAFile = 0xfefefefefefefefe;
/**
 * @brief Everything true except H-file
 */
constexpr Bitboard notHFile = 0x7f7f7f7f7f7f7f7f;

/**
 * @brief Everything true except A-file and B-file
 */
constexpr Bitboard notABFile = 0xfcfcfcfcfcfcfcfc;

/**
 * @brief Everything true except H-file and G-file
 */
constexpr Bitboard notGHFile = 0x3f3f3f3f3f3f3f3f;

/**
 * @brief 2D array of bitboards representing rays, [Direction][SquareIndex] eg [NorthWest][e2]
 */
extern Bitboard rayAttacks[8][SquareCount];

/**
 * @brief Array of equivalent bitboard mappings of square indexes
 */
extern Bitboard SquareBB[SquareCount+1];

/**
 * @brief Array of attacks for white pawns (facing North) and black pawns (facing South)
 */
extern Bitboard PawnAttacks[PieceColorCount][SquareCount];

/**
 * @brief 2D array of attack bitboards, [PieceType][Square] eg [Rook][c3]
 */
extern Bitboard PieceAttacks[PieceTypeCount][SquareCount];

/**
 * @brief 2D array of pawn positions for a given Color and EnPassant Square.
 * Returned position of the piece corresponding to the file of the given EnPassant Square
 * I.e [White][e6] returns a bitboard corresponding to the white pawn on e4
 * I.e [Black][e6] returns a bitboard corresponding to the black pawn on e5
 */
extern Bitboard EnPassantPositions[PieceColorCount][SquareCount+1];