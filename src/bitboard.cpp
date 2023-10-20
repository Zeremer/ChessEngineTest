//
// Created by paulp on 1/16/2019.
//

#include "bitboard.h"
#include "console.h"


Bitboard rayAttacks[8][SquareCount];
Bitboard PawnAttacks[PieceColorCount][SquareCount];
Bitboard PieceAttacks[PieceTypeCount][SquareCount];
Bitboard SquareBB[SquareCount+1];
Bitboard EnPassantPositions[PieceColorCount][SquareCount+1];

void Bitboards::init(){
  compileAttacks();
  compileSquareBB();
  compileEnPassantBB();
}

void Bitboards::compileSquareBB(){
    Bitboard sqBB = 0x1;
    for(int sq = 0; sq < 64; sq++){
        SquareBB[sq] = sqBB << sq;
    }
    SquareBB[none] = 0;
}

void Bitboards::compileAttacks() {

    compileRays();

    compileSlidingAttacks();

    compileKnightAttacks();

    compilePawnAttacks();

    compileKingAttacks();
}



void Bitboards::compileSlidingAttacks() {
    // Fill attacks for sliding pieces (Rook, Bishop, Queen) for every piece on the board (8x8)
    for (int sq = 0; sq < 64; sq++) {
        PieceAttacks[Rook][sq] = rayAttacks[North][sq] | rayAttacks[South][sq] | rayAttacks[East][sq] | rayAttacks[West][sq];
        PieceAttacks[Bishop][sq] = rayAttacks[NorthEast][sq] | rayAttacks[SouthEast][sq] | rayAttacks[SouthWest][sq] | rayAttacks[NorthWest][sq];
        PieceAttacks[Queen][sq] = PieceAttacks[Rook][sq] | PieceAttacks[Bishop][sq];
    }
}


void Bitboards::compileKnightAttacks() {

    //          noNoWe    noNoEa
    //              +15  +17
    //               |     |
    //  noWeWe  +6 __|     |__+10  noEaEa
    //                \   /
    //                 >0<
    //             __ /   \ __
    //  soWeWe -10   |     |   -6  soEaEa
    //               |     |
    //              -17  -15
    //          soSoWe    soSoEa

    Bitboard position = oneBB;
    for (int sq = 0; sq < 64; sq++, position <<= 1) {  // shift east
        Bitboard attackBoard = 0;

        // Combine all directions a knight can attack
        attackBoard |= (position & notHFile) << 17;  // noNoEa
        attackBoard |= (position & notGHFile) << 10;  // noEaEa
        attackBoard |= (position & notGHFile) >> 6;  // soEaEa
        attackBoard |= (position & notHFile) >> 15;  // soSoEa
        attackBoard |= (position & notAFile) << 15;  // noNoWe
        attackBoard |= (position & notABFile) << 6;  // noWeWe
        attackBoard |= (position & notABFile) >> 10;  // soWeWe
        attackBoard |= (position & notAFile) >> 17;  // soSoWe

        PieceAttacks[Knight][sq] = attackBoard;
    }
}


void Bitboards::compilePawnAttacks() {
    // Note: en passant is not precompiled

    // Compile for white pawns (facing north)
    Bitboard position = oneBB;
    for (int sq = 0; sq < 64; sq++, position <<= 1) {  // shift east
        Bitboard attackBoard = 0;

        attackBoard |= northEastOne(position);
        attackBoard |= northWestOne(position);

        PawnAttacks[White][sq] = attackBoard;
    }

    // Compile for black pawns (facing south)
    position = oneBB;
    for (int sq = 0; sq < 64; sq++, position <<= 1) {  // shift east
        Bitboard attackBoard = 0;

        attackBoard |= southEastOne(position);
        attackBoard |= southWestOne(position);

        PawnAttacks[Black][sq] = attackBoard;
    }
}


void Bitboards::compileKingAttacks() {
    Bitboard position = oneBB;
    for (int sq = 0; sq < 64; sq++, position <<= 1) {  // shift east
        Bitboard attackBoard = 0;

        attackBoard |= northOne(position);
        attackBoard |= eastOne(position);
        attackBoard |= southOne(position);
        attackBoard |= westOne(position);

        attackBoard |= northEastOne(position);
        attackBoard |= southEastOne(position);
        attackBoard |= northWestOne(position);
        attackBoard |= southWestOne(position);

        PieceAttacks[King][sq] = attackBoard;
    }
}


void Bitboards::compileRays() {
     // Compile Positive Rays

    // North
    Bitboard nort = 0x101010101010100;  // File A filled starting at bottom left corner
    for (int sq = 0; sq < 64; sq++, nort <<= 1) {  // shift east, discarding bits as it goes
        rayAttacks[North][sq] = nort;
    }

    // East
    Bitboard east = 0xfe;  // bottom row (0 1 1 1 1 1 1 1)
    for (int file = 0; file < 8; file++, east = eastOne(east)) {
        Bitboard ea = east;
        for (int r8 = 0; r8 < 8*8; r8 += 8, ea <<= 8) {  // move up one rank
            rayAttacks[East][r8+file] = ea;
        }
    }

    // North East
    Bitboard noea = 0x8040201008040200;  // Diagonal filled starting at lower left corner
    for (int file = 0; file < 8; file++, noea = eastOne(noea)) {
        Bitboard ne = noea;
        for (int r8 = 0; r8 < 8*8; r8 += 8, ne <<= 8) {  // move up one rank
            rayAttacks[NorthEast][r8+file] = ne;
        }
    }

    // North West
    Bitboard nowe = 0x102040810204000;  // Diagonal filled starting at lower right corner
    for (int file = 7; file >= 0; file--, nowe = westOne(nowe)) {
        Bitboard nw = nowe;
        for (int r8 = 0; r8 < 8*8; r8 += 8, nw <<= 8) {  // move up one rank
            rayAttacks[NorthWest][r8+file] = nw;
        }
    }


    // Compile Negative Rays

    // South
    Bitboard sout = 0x0080808080808080;  // File 8 filled starting at top right corner
    for (int sq = 63; sq >= 0; sq--, sout >>= 1) {  // shift west, discarding bits as it goes
        rayAttacks[South][sq] = sout;
    }

    // West
    Bitboard west = 0x7f;  // bottom row (1 1 1 1 1 1 1 0)
    for (int file = 7; file >= 0; file--, west = westOne(west)) {
        Bitboard we = west;
        for (int r8 = 0; r8 < 8*8; r8 += 8, we <<= 8) {  // move up one rank
            rayAttacks[West][r8+file] = we;
        }
    }

    // South East
    Bitboard soea = 0x2040810204080;  // Diagonal filled starting at top left corner
    for (int file = 0; file < 8; file++, soea = eastOne(soea)) {
        Bitboard se = soea;
        for (int r8 = 8*8-8; r8 >= 0; r8 -= 8, se >>= 8) {  // move down one rank
            rayAttacks[SouthEast][r8+file] = se;
        }
    }

    // South West
    Bitboard sowe = 0x40201008040201;  // Diagonal filled starting at top right corner
    for (int file = 7; file >= 0; file--, sowe = westOne(sowe)) {
        Bitboard sw = sowe;
        for (int r8 = 8*8-8; r8 >= 0; r8 -= 8, sw >>= 8) {  // move down one rank
            rayAttacks[SouthWest][r8+file] = sw;
        }
    }
}


Bitboard Bitboards::northOne(Bitboard bb) {
    return bb<< 8;
}

Bitboard Bitboards::eastOne(Bitboard bb) {
    return (bb & notHFile) << 1;
}

Bitboard Bitboards::southOne(Bitboard bb){
    return bb >> 8;
}

Bitboard Bitboards::westOne(Bitboard bb) {
    return (bb & notAFile) >> 1;
}


Bitboard Bitboards::northEastOne(Bitboard bb) {
    return northOne(eastOne(bb));
}

Bitboard Bitboards::southEastOne(Bitboard bb) {
    return southOne(eastOne(bb));
}

Bitboard Bitboards::southWestOne(Bitboard bb) {
    return southOne(westOne(bb));
}

Bitboard Bitboards::northWestOne(Bitboard bb){
    return northOne(westOne(bb));
}

void Bitboards::compileEnPassantBB() {
    Bitboard whiteSqBB = one << 23;
    Bitboard blackSqBB = one << 31;
    for(int sq = 0; sq < 64; sq++){
        EnPassantPositions[White][sq] = whiteSqBB << (sq%8);
        EnPassantPositions[Black][sq] = blackSqBB << (sq%8);
    }
    EnPassantPositions[Black][none] = 0;
    EnPassantPositions[White][none] = 0;
}
