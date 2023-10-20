//
// Created by paulp on 1/16/2019.
//

#pragma once

#include <inttypes.h>
#include <assert.h>
#include <string>
#include <vector>
#include <algorithm>

#ifdef _MSC_VER
//not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif
typedef uint64_t Bitboard;
const std::string defaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
constexpr Bitboard one = 1;
enum EngineMode {
    EM_UCI,
    EM_CLI
};

enum SquareIndex {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    none = 64,
    SquareCount = 64
};

const std::string SquareIndexStrings[] {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "00",
    "00"
};

/**
 * @brief Converts a string, such as "a7" to a SquareIndex value, if possible.
 * @param toFind the string to convert to a SquareIndex
 * @return SquareIndex equivalent of the toFind, or SquareIndex.none.
 * @author Bryant Foster 3/6/2019
 */
static SquareIndex squareIndexStringToEnum(std::string toConvert) {
    auto sizeOfArray = sizeof(SquareIndexStrings)/sizeof(SquareIndexStrings[0]);
    long index = std::distance(SquareIndexStrings, std::find(SquareIndexStrings, SquareIndexStrings + sizeOfArray, toConvert));
    return index <= 63 ? (SquareIndex)index : none;
}


// Little Endian Rank-File(LERF) mapping
// Compass Rose
//  noWe         nort         noEa
//          +7    +8    +9
//              \  |  /
//  west    -1 <-  0 -> +1    east
//              /  |  \
//          -9    -8    -7
//  soWe         sout         soEa
enum Direction {
    North,
    South,
    East,
    West,
    NorthEast,
    NorthWest,
    SouthEast,
    SouthWest
};

enum File {
    FileA,FileB,FileC,FileD,FileE,FileF,FileG,FileH, FileCount
};

enum Rank {
    Rank1,Rank2,Rank3,Rank4,Rank5,Rank6,Rank7,Rank8, RankCount
};

enum PieceType{
    Pawn = 0,Bishop = 1,Knight = 2,Rook = 3,Queen = 4,King = 5, NoType = 6, PieceTypeCount = 6
};

const std::string PieceTypeStrings[] {"pawn", "bishop", "knight", "rook", "queen", "king", "00"};

const std::string PieceTypeShortStrings[]{"","B","N","R","Q","K","",""};
const std::string PieceTypeLowercaseShortStrings[]{"","b","n","r","q","k","",""};

enum PieceColor{
    Black = 0, White = 1, NoColor, PieceColorCount = 2
};

enum MoveType {
    Hash, Winning, Equal, Killer, LosingCapture,NonCapture, None
};

enum GameState {
    Check, Checkmate, Stalemate, Normal
};

//Increment Overload
#define AddIteratorOperators(T) \
inline T& operator++(T& obj) { return obj = T(int(obj) + 1); } \
inline T& operator--(T& obj) { return obj = T(int(obj) - 1); }

AddIteratorOperators(Rank);
AddIteratorOperators(File);
AddIteratorOperators(PieceColor);
AddIteratorOperators(PieceType);

struct Move {
    SquareIndex startSquare = none;
    SquareIndex endSquare = none;
    PieceType pieceType = NoType;
    PieceType promotionType = NoType;
    MoveType type = None;
    GameState stateChangedTo = Normal;

    SquareIndex castleRookStart = none;
    SquareIndex castleRookEnd = none;

    bool operator==(const Move item) {
      return startSquare == item.startSquare &&
             endSquare == item.endSquare &&
             pieceType == item.pieceType;
    }
};

/**
 * @brief Returns the number of ones on a given 64-bit bitboard
 * @param bb bitboard to calculate ones for
 * @return number of ones on a bitboard
 * @author Paul Grahek
 */
static int numOnes64(Bitboard bb) {
    bb = ((bb >> 1) & 0x5555555555555555) + (bb & 0x5555555555555555);
    bb = ((bb >> 2) & 0x3333333333333333) + (bb & 0x3333333333333333);
    Bitboard v = ((bb >> 32) + bb);
    v = ((v >> 4) & 0x0f0f0f0f) + (v & 0x0f0f0f0f);
    v = ((v >> 8) & 0x00ff00ff) + (v & 0x00ff00ff);
    return ((v >> 16) & 0x0000ffff) + (v & 0x0000ffff);
}

//static int numOnes32(Bitboard bb){
//    bb -= ((bb >> 1) & 0x55555555);
//    bb = (((bb >> 2) & 0x33333333) + (bb & 0x33333333));
//    bb = (((bb >> 4) + bb) & 0x0f0f0f0f);
//    bb += (bb >> 8);
//    bb += (bb >> 16);
//    return(bb & 0x0000003f);
//}

/**
 * @brief Gets the index of the MSB
 *
 * @param bb bitboard to smear
 * @return index of MSB
 * @author Paul Grahek
 */
static int bitScanReverse(Bitboard bb) {
    assert(bb != 0);
    bb |= (bb >>1);
    bb |= (bb >>2);
    bb |= (bb >>4);
    bb |= (bb >>8);
    bb |= (bb >>16);
    bb |= (bb >>32);
    return numOnes64(bb);
}

/**
 * @brief Gets the index of the LSB
 *
 * @param bb bitboard to smear
 * @return index of LSB
 * @author Paul Grahek
 */
static int bitScanForward(Bitboard bb) {
    assert(bb != 0);
    bb = (bb ^ (bb-1));
    return numOnes64(bb);
//    return (numOnes32(bb) + numOnes32(bb >> 32));
}

constexpr int PieceColorWeight[] {-1,1};

/**
 * @brief Flips the color given. Does not check if the color is not White or Black.
 *
 * @param color
 * @return Flipped color.
 * @author Paul Grahek
 */
static PieceColor FlipColor(PieceColor color) {
    return color==White?Black:White;
}

/**
 * @brief Inserts the elements of the second vector into the first.
 * @tparam T
 * @param vec1
 * @param vec2
 * @author Paul Grahek
 */
template<class T>
void combineVectors(std::vector<T>& vec1, std::vector<T> vec2){
    vec1.insert(vec1.end(),vec2.begin(),vec2.end());
}

/**
 * @brief Gets the square indexes for the bit marked on a bitboard
 * @param pieces
 * @return
 * @author Paul Grahek
 */
static std::vector<SquareIndex> getSquareIndexesForBitboard(Bitboard pieces){
    std::vector<SquareIndex> indexes;
    while(pieces){
        indexes.push_back((SquareIndex)(bitScanForward(pieces)-1));
        pieces &= pieces-1;
    }
    return indexes;
}

enum TTFlag { Exact, UpperBound, LowerBound};

#define SEARCH_MAX 1000000000
#define SEARCH_MIN -1000000000
#define SEARCH_MAX_DEPTH 100