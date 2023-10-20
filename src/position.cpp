//
// Created by paulp on 1/16/2019.
//

#include "position.h"
#include "bitboard.h"
#include "movegen.h"
#include "move.h"
#include "zhash.h"
#include "evaluation.h"
#include "console.h"
#include "coutsync.h"
#include <algorithm>
#include <string>

Position::Position() { initialize(defaultFEN); }
Position::Position(const std::string &fen) { initialize(fen); }
Position::Position(const Position &toCopy) {
    this->colorPositions = toCopy.getColorPositions();
    this->pieceBoards = toCopy.getPieceBoards();
    this->currentMoveColor = toCopy.getCurrentMoveColor();
    this->previousGameStates = toCopy.previousGameStates;
    this->previousCapturedPieces = toCopy.previousCapturedPieces;
    this->lastMoves = toCopy.lastMoves;
    this->previousEnPassantSquares = toCopy.previousEnPassantSquares;
//    this->lastMoveIndex = toCopy.lastMoveIndex;
    this->zHash = toCopy.getHash();
    this->halfmoveClock = toCopy.halfmoveClock;
    this->fullmoveNumber = toCopy.fullmoveNumber;
    this->previousCastlingBoards = toCopy.previousCastlingBoards;
}

Position Position::simpleCopy() const{
    Position copy;
    copy.colorPositions = this->getColorPositions();
    copy.pieceBoards = this->getPieceBoards();
    copy.currentMoveColor = this->getCurrentMoveColor();
    copy.zHash = getHash();
    copy.halfmoveClock = this->halfmoveClock;
    copy.fullmoveNumber = this->fullmoveNumber;
    return copy;
}

Position::Position(const Position &toCopy, struct Move mv) {
    this->colorPositions = toCopy.getColorPositions();
    this->pieceBoards = toCopy.getPieceBoards();
    this->currentMoveColor = toCopy.getCurrentMoveColor();
    this->previousGameStates = toCopy.previousGameStates;
    this->previousCapturedPieces = toCopy.previousCapturedPieces;
    this->lastMoves = toCopy.lastMoves;
    this->previousEnPassantSquares = toCopy.previousEnPassantSquares;
//    this->lastMoveIndex = toCopy.lastMoveIndex;
    this->zHash = toCopy.getHash();
    this->halfmoveClock = toCopy.halfmoveClock;
    this->fullmoveNumber = toCopy.fullmoveNumber;
    this->previousCastlingBoards = toCopy.previousCastlingBoards;
    move(mv);
}

void Position::initialize(const std::string &fen) {
    FENData data = parseFen(fen);

    initBitboards(data.rows);
    this->currentMoveColor = data.turnColor;
//    ++this->lastMoveIndex;

    // The 0th and 1st index of previousCastlingBoards needs to be set up to the same thing; just because
    // that is how it works with having three indexes instead of just two for undoing moves.
    this->previousCastlingBoards.push_back(parseCastlingRights(data.castlingRights));
    this->previousEnPassantSquares.push_back(data.enPassantTarget);
    this->previousGameStates.push_back(Normal);
    this->previousCapturedPieces.push_back(NoType);
    this->lastCaptureTypeWithoutCheck = NoType;
//    this-
    // TODO: We should be checking for the game state when initializing, if the FEN string is not the default.

    this->zHash = ZobristHash::hash(*this);
    this->halfmoveClock = data.halfmoveClock;
    this->fullmoveNumber = data.fullmoveNumber;
}

struct FENData Position::parseFen(const std::string &fen) {
    // fenPieces has this layout:
    // [0] Board Layout,
    // [1] Next color to play (rather, current player color),
    // [2] Castling Availability,
    // [3] En Passant Target Square,
    // [4] Halfmove Clock,
    // [5] Fullmove Number: incremented after Black's turn

    std::array<std::string, 6> fenPieces;
    int index = 0;
    FENData ret;

    std::string temp = fen;
    while(temp.find(' ') != std::string::npos) {
        fenPieces[index] = temp.substr(0, temp.find(' '));
        temp = temp.substr(temp.find(' ') + 1);
        index++;
    }
    fenPieces[index] = temp;

    temp = fenPieces[0];
    std::array<std::string, 8> rows;
    index = 7;
    while(temp.find('/') != std::string::npos) {
        rows[index] = temp.substr(0, temp.find('/'));
        temp = temp.substr(temp.find('/') + 1);
        index--;
    }
    rows[index] = temp;

    ret.rows = rows;
    ret.turnColor = fenPieces[1] == "w" ? White : Black;
    ret.castlingRights = fenPieces[2];
    ret.enPassantTarget = fenPieces[3] == "-" ? none : squareIndexStringToEnum(fenPieces[3]);
    try {
        ret.halfmoveClock = !fenPieces[4].empty() ? std::stoi(fenPieces[4]) : 0;
        ret.fullmoveNumber = !fenPieces[5].empty() ? std::stoi(fenPieces[5]) : 1;
    } catch (std::invalid_argument& e) {
        // Do nothing for caught stoi exception
    }

    return ret;
}

Bitboard Position::parseCastlingRights(const std::string &rights) {
    Bitboard bbRights = 0;

    for(char c : rights) {
        auto color = getColor(c);
        auto side = getType(c);
        Bitboard coloredRooks = this->colorPositions[color] & this->pieceBoards[Rook];

        // Ensure the king is in the correct position
        if (FileEBB & (this->colorPositions[color] & this->pieceBoards[King])) {

            bbRights |= this->colorPositions[color] & this->pieceBoards[King];

            if (side == King) {  // kingside castle
                bbRights |= coloredRooks & FileHBB;
            } else if (side == Queen){  // queenside castle
                bbRights |= coloredRooks & FileABB;
            }
            // else do not update bbRights board
        }
    }

    return bbRights;
}

void Position::initBitboards(std::array<std::string, 8> boardRows) {
    for(int row = 0; row < 8; row++) {
        std::string s = boardRows[row];
        int space = 0;
        for (char curChar : s) {
            if(curChar >= 'A') { // This char is a letter, not a digit - init the piece.
                initPiece(getColor(curChar), getType(curChar), row, space);
                space++;
            } else { // This char is a digit, not a letter - deal with the blank space(s).
                int emptySpaces = (int) curChar - 48;
                while(emptySpaces > 0) {
                    space++;
                    emptySpaces--;
                }
            }
        }
    }
}

PieceColor Position::getColor(char piece) {
    return (piece >= 'A' && piece <= 'Z') ? White : Black;
}

PieceType Position::getType(char piece) {
    if(piece == 'K' || piece == 'k') { return King; }
    else if(piece == 'Q' || piece == 'q') { return Queen; }
    else if(piece == 'R' || piece == 'r') { return Rook; }
    else if(piece == 'B' || piece == 'b') { return Bishop; }
    else if(piece == 'N' || piece == 'n') { return Knight; }
    else if(piece == 'P' || piece == 'p') { return Pawn; }
    else { return NoType; }
}

void Position::initPiece(PieceColor color, PieceType type, int row, int space) {
    int bitToFlip = row * 8 + space;
    this->colorPositions[color] ^= (one << bitToFlip);
    this->pieceBoards[type] ^= (one << bitToFlip);
}

bool Position::updatePiecePosition(Move mv, PieceColor mvColor, bool isPlayerColor) {
    PieceColor color = mvColor;
    PieceColor otherColor = FlipColor(currentMoveColor);
    Bitboard notEnd = ~SquareBB[mv.endSquare];
    Bitboard notStart = ~SquareBB[mv.startSquare];
    PieceType captureType = NoType;
    PieceType destType = (mv.promotionType == NoType?mv.pieceType:mv.promotionType);
    SquareIndex captureSquare = (mv.pieceType == Pawn &&
        mv.endSquare == getEnPassantSquare())?
            (SquareIndex)bitScanForward(EnPassantPositions[otherColor][getEnPassantSquare()]) :
            mv.endSquare;
    Bitboard notCap = ~SquareBB[captureSquare];

    // Update the color board to reflect pieces moved/captured
    this->colorPositions[currentMoveColor] &= notStart;
    this->colorPositions[currentMoveColor] |= SquareBB[mv.endSquare];
    this->colorPositions[otherColor] &= notEnd;
    // Determine capture type
    uint64_t captureMask = 0;
    captureMask |= ((this->pieceBoards[King] != (this->pieceBoards[King] &= notCap)) << King);
    captureMask |= ((this->pieceBoards[Queen] != (this->pieceBoards[Queen] &= notCap)) << Queen);
    captureMask |= ((this->pieceBoards[Rook] != (this->pieceBoards[Rook] &= notCap)) << Rook);
    captureMask |= ((this->pieceBoards[Knight] != (this->pieceBoards[Knight] &= notCap)) << Knight);
    captureMask |= ((this->pieceBoards[Bishop] != (this->pieceBoards[Bishop] &= notCap)) << Bishop);
    captureMask |= ((this->pieceBoards[Pawn] != (this->pieceBoards[Pawn] &= notCap)) << Pawn);
    captureType = (PieceType)(captureMask?bitScanReverse(captureMask)-1:NoType);
    // Update the piece board of the piece moved
    this->pieceBoards[mv.pieceType] &= notStart;
    this->pieceBoards[destType] |= SquareBB[mv.endSquare];

    // Handle Updating Castling
    this->colorPositions[currentMoveColor] &= ~SquareBB[mv.castleRookStart]; // & not start
    this->colorPositions[currentMoveColor] |= SquareBB[mv.castleRookEnd]; // | end
    this->pieceBoards[Rook] &= ~SquareBB[mv.castleRookStart]; // & not start
    this->pieceBoards[Rook] |= SquareBB[mv.castleRookEnd]; // | end

    // Update the zhashve
    this->zHash ^= ZobristHash::zPieceTable[color][mv.pieceType][mv.startSquare];
    this->zHash ^= ZobristHash::zPieceTable[color][destType][mv.endSquare];
    this->zHash ^= ZobristHash::zPieceTable[otherColor][captureType][captureSquare];
    this->zHash ^= ZobristHash::zColorTable[color];
    // Castling zhash update
    this->zHash ^= ZobristHash::zPieceTable[color][Rook][mv.castleRookEnd];
    this->zHash ^= ZobristHash::zPieceTable[color][Rook][mv.castleRookStart];

    this->previousCapturedPieces.push_back(captureType);
    this->lastMoves.push_back(mv);

    // Pawn advance or capture resets halfmoveClock to 0; otherwise, it is incremented.
    if(mv.pieceType == Pawn || captureType != NoType) { this->halfmoveClock = 0; }
    else { this->halfmoveClock++; }

    return true;
}

bool Position::validMove(PieceType type, SquareIndex start, SquareIndex end) {
    auto allPossibleMoves = getAllPossibleMoves(type, start, getFriendlyBlockers(), getEnemyBlockers(type));
    auto allEndSquareIndexes = getSquareIndexesForBitboard(allPossibleMoves);
    for(SquareIndex si : allEndSquareIndexes) {
        if(end == si) return true;
    }
    return false;
}

Bitboard Position::getFriendlyBlockers() {
    return this->colorPositions[currentMoveColor];
}

Bitboard Position::getEnemyBlockers(PieceType type) {
    return this->colorPositions[FlipColor(currentMoveColor)]; //TODO: Does this need to be more complicated? Paul - No
}

Bitboard Position::getAllPossibleMoves(PieceType type, SquareIndex start, Bitboard friendlyBlockers,
                                       Bitboard enemyBlockers) {
    if(type == Rook) { return MoveGen::getPossibleMovesForRook(start, friendlyBlockers, enemyBlockers); }
    else if(type == Knight) { return MoveGen::getPossibleMovesForKnight(start, friendlyBlockers, enemyBlockers); }
    else if(type == Bishop) { return MoveGen::getPossibleMovesForBishop(start, friendlyBlockers, enemyBlockers); }
    else if(type == King) { return MoveGen::getPossibleMovesForKing(start, friendlyBlockers, enemyBlockers); }
    else if(type == Queen) { return MoveGen::getPossibleMovesForQueen(start, friendlyBlockers, enemyBlockers); }
    else if(type == Pawn) { return MoveGen::getPossibleMovesForPawn(start, this->currentMoveColor, friendlyBlockers, enemyBlockers, this->previousEnPassantSquares.back());}
    return 0;
}

bool Position::pawnTwoSpaces(SquareIndex start, SquareIndex end) {
    return abs(end - start) == 16;
}

SquareIndex Position::enPassant(SquareIndex start) {
    if(this->currentMoveColor == White) { return static_cast<SquareIndex>(start + 8); }
    else if(this->currentMoveColor == Black) { return static_cast<SquareIndex>(start - 8); }
    else { return none; }
}

void Position::clearBit(Bitboard &b, int index) { b &= ~(one << index); }

void Position::setBit(Bitboard &b, int index) { b |= (one << index); }

void Position::updateMembersFromMove(Move mv) {
    this->previousEnPassantSquares.push_back((mv.pieceType == Pawn && pawnTwoSpaces(mv.startSquare, mv.endSquare) ? enPassant(mv.startSquare) : none));

    auto tempCastlingBoard = previousCastlingBoards.back();
    tempCastlingBoard &= (this->pieceBoards[Rook] | this->pieceBoards[King]);
    previousCastlingBoards.push_back(tempCastlingBoard);

    if(this->currentMoveColor == Black) { this->fullmoveNumber++; }
    this->currentMoveColor = FlipColor(this->currentMoveColor);
    this->previousGameStates.push_back(mv.stateChangedTo);
//        updateHash(mv);
}

Bitboard Position::getPiecePositions(PieceColor color, PieceType type) const {
    if(color == NoColor || type == NoType) { return 0; }
    return this->colorPositions[color] & this->pieceBoards[type];
}

std::array<Bitboard, 2> Position::getColorPositions() const {
    return this->colorPositions;
}

Bitboard Position::getColorPosition(PieceColor color) const {
    return this->colorPositions[color];
}

Bitboard Position::getCurrentPlayerPiecePosition() const {
    return this->colorPositions[currentMoveColor];
}

Bitboard Position::getOtherPlayerPiecePosition() const {
    return this->colorPositions[FlipColor(currentMoveColor)];
}

std::array<Bitboard, 6> Position::getPieceBoards() const {
    return this->pieceBoards;
}

Bitboard Position::getPieceBoard(PieceType type) const {
    return this->pieceBoards[type];
}

PieceColor Position::getCurrentMoveColor() const {
    return this->currentMoveColor;
}

GameState Position::getCurrentGameState() const {
    return this->previousGameStates.back();
}

void Position::setCurrentGameState(GameState state) {
    this->previousGameStates.push_back(state);
}

void Position::move(struct Move mv) {
    if(updatePiecePosition(mv,this->currentMoveColor, false)) {
        updateMembersFromMove(mv);

        // Check Checkmate/stalemate for the next color (updateMembersFromMove has flipped color)
        bool areMovesAvailable = MoveGen::areAnyMovesPossible(*this, this->currentMoveColor);

        if (mv.stateChangedTo == Check && !areMovesAvailable) {  // No moves available and in check
            this->previousGameStates.back() = Checkmate;
        } else if (!areMovesAvailable){  // No available moves
            this->previousGameStates.back() = Stalemate;
        }
    }
}

bool Position::playerMove(Move mv) {

    // This is slow, but it doesn't need to be fast for the player
    auto possibleMoves = MoveGen::getPossibleMoves(*this);
    auto foundMoveItr = std::find(possibleMoves.begin(), possibleMoves.end(), mv);
    if (foundMoveItr != possibleMoves.end()) {  // if move is found

        // Use the foundMove to allow the gui to not
        // have to set any move properties other than
        // Type, startSquare, and endSquare
        auto foundMove = (*foundMoveItr);//.base();

        if (updatePiecePosition(foundMove, this->currentMoveColor, true)) {
            updateMembersFromMove(foundMove);

            // Check Checkmate/stalemate for the next color (updateMembersFromMove has flipped color)
            bool areMovesAvailable = MoveGen::areAnyMovesPossible(*this, this->currentMoveColor);

            if (foundMove.stateChangedTo == Check && !areMovesAvailable) {  // No moves available and in check
                this->previousGameStates.back()= Checkmate;
            } else if (!areMovesAvailable) {  // No available moves
                this->previousGameStates.back() = Stalemate;
            }
        }
        return true;
    } else {
        return false;
    }
}

void Position::undoLastMove() {
    // If there are no moves to undo, return
    if(this->lastMoves.empty()) { return; }
    // Pop the last move made off the stack
    Move toUndo = this->lastMoves.back();
    lastMoves.pop_back();

    PieceColor undoColor = FlipColor(currentMoveColor);
    PieceColor captureColor = currentMoveColor;
    PieceType destType = (toUndo.promotionType == NoType?toUndo.pieceType:toUndo.promotionType);
    // Pop the enpassant square for the position being rolled back first
//    this->previousEnPassantSquares.pop_back();
//    SquareIndex captureSquare = (toUndo.pieceType == Pawn &&
//         toUndo.endSquare == previousEnPassantSquares.back())?
//        (SquareIndex)bitScanForward(EnPassantPositions[captureColor][getEnPassantSquare()]) :
//        toUndo.endSquare;
    // Undo last move
    // Set bitboards for moved piece back
    colorPositions[undoColor] &= ~SquareBB[toUndo.endSquare];
    colorPositions[undoColor] |= SquareBB[toUndo.startSquare];
    pieceBoards[destType] &= ~SquareBB[toUndo.endSquare];
    pieceBoards[toUndo.pieceType] |= SquareBB[toUndo.startSquare];
    // Set bitboards for piece captured, if applicable
    if(previousCapturedPieces.back() != NoType) {
        colorPositions[captureColor] |= SquareBB[toUndo.endSquare];
        pieceBoards[previousCapturedPieces.back()] |= SquareBB[toUndo.endSquare];
    }

    // Handle Updating Castling
    this->colorPositions[undoColor] &= ~SquareBB[toUndo.castleRookEnd];
    this->colorPositions[undoColor] |= SquareBB[toUndo.castleRookStart];
    this->pieceBoards[Rook] &= ~SquareBB[toUndo.castleRookEnd];
    this->pieceBoards[Rook] |= SquareBB[toUndo.castleRookStart];

    PieceType captureType = this->previousCapturedPieces.back();
    PieceType type = toUndo.pieceType;
    SquareIndex start = toUndo.startSquare;
    SquareIndex end = toUndo.endSquare;
    // Castling zhash update
    this->zHash ^= ZobristHash::zPieceTable[undoColor][Rook][toUndo.castleRookEnd];
    this->zHash ^= ZobristHash::zPieceTable[undoColor][Rook][toUndo.castleRookStart];
    this->zHash ^= ZobristHash::zColorTable[undoColor];
    this->zHash ^= ZobristHash::zPieceTable[captureColor][captureType][end];
    this->zHash ^= ZobristHash::zPieceTable[undoColor][destType][end];
    this->zHash ^= ZobristHash::zPieceTable[undoColor][type][start];
//        if(captureType != NoType)

    this->currentMoveColor = FlipColor(this->currentMoveColor);
    this->previousCastlingBoards.pop_back();
    this->previousCapturedPieces.pop_back();
    this->previousGameStates.pop_back();

    // Pawn advance or capture resets halfmoveClock to 0; otherwise, it is incremented.
//    if(type == Pawn || captureType != NoType) { this->halfmoveClock = 0; }
//    else { this->halfmoveClock++; }
    if(undoColor == Black) fullmoveNumber--;
}

void Position::undoLastTwoMoves() {
    if(this->lastMoves.size() < 2) { return; }
    undoLastMove();
    undoLastMove();
}

SquareIndex Position::getEnPassantSquare() const {
    return this->previousEnPassantSquares.back();
}

Bitboard Position::getHash() const {
    return this->zHash;
}

Bitboard Position::AttacksToSquare(SquareIndex sq, Bitboard allBlockers) const {
    Bitboard bishopQueens, rooksQueens;
    if(!(~allBlockers)) allBlockers = (colorPositions[Black] | colorPositions[White]);
    rooksQueens = (pieceBoards[Queen] | pieceBoards[Rook]);
    bishopQueens = (pieceBoards[Queen] | pieceBoards[Bishop]);
    return ((PawnAttacks[White][sq] & pieceBoards[Pawn] & colorPositions[Black])
            |   (PawnAttacks[Black][sq] & pieceBoards[Pawn] & colorPositions[White])
            |   (PieceAttacks[Knight][sq] & pieceBoards[Knight])
            |   (PieceAttacks[King][sq] & pieceBoards[King])
            |   (MoveGen::getPossibleMovesForBishop(sq,0,allBlockers) & bishopQueens)
            |   (MoveGen::getPossibleMovesForRook(sq,0,allBlockers) & rooksQueens));
}

PieceType Position::getPieceTypeOfSquare(SquareIndex sq) const {
    Bitboard pieceBoard = SquareBB[sq];
    if((pieceBoard & getPieceBoard(Pawn))) return Pawn;
    else if((pieceBoard & getPieceBoard(Knight))) return Knight;
    else if((pieceBoard & getPieceBoard(Bishop))) return Bishop;
    else if((pieceBoard & getPieceBoard(Rook))) return Rook;
    else if((pieceBoard & getPieceBoard(Queen))) return Queen;
    else if((pieceBoard & getPieceBoard(King))) return King;
    return NoType;
}

std::tuple<PieceType, SquareIndex> Position::getSmallestAttacker(SquareIndex sq, Bitboard pieces) const{
    if(!(~pieces)) pieces = AttacksToSquare(sq) & getCurrentPlayerPiecePosition();
    std::tuple<PieceType,SquareIndex> smallestPiece = std::make_tuple(NoType, none);
    Bitboard pieceBoard = 0;
    if((pieceBoard = pieces & getPieceBoard(Pawn)) != 0) smallestPiece = std::make_tuple(Pawn, (SquareIndex)(bitScanForward(pieceBoard)-1));
    else if((pieceBoard = pieces & getPieceBoard(Knight)) != 0) smallestPiece = std::make_tuple(Knight, (SquareIndex)(bitScanForward(pieceBoard)-1));
    else if((pieceBoard = pieces & getPieceBoard(Bishop)) != 0) smallestPiece = std::make_tuple(Bishop, (SquareIndex)(bitScanForward(pieceBoard)-1));
    else if((pieceBoard = pieces & getPieceBoard(Rook)) != 0) smallestPiece = std::make_tuple(Rook, (SquareIndex)(bitScanForward(pieceBoard)-1));
    else if((pieceBoard = pieces & getPieceBoard(Queen)) != 0) smallestPiece = std::make_tuple(Queen, (SquareIndex)(bitScanForward(pieceBoard)-1));
    else if((pieceBoard = pieces & getPieceBoard(King)) != 0) smallestPiece = std::make_tuple(King, (SquareIndex)(bitScanForward(pieceBoard)-1));
    return smallestPiece;
}

int Position::SEE(Move mv) const{
//    PieceType attackPiece = mv.pieceType;
    int gain[32];
    int depth = 0;
    Bitboard mayXRay = pieceBoards[Pawn] | pieceBoards[Bishop] | pieceBoards[Rook] | pieceBoards[Queen];
    Bitboard fromSet = SquareBB[mv.startSquare];
    Bitboard occupied = colorPositions[White] | colorPositions[Black];
    Bitboard attadef = AttacksToSquare(mv.endSquare);
    PieceColor currColor = currentMoveColor;
    std::tuple<PieceType, SquareIndex> smallestAttacker = std::make_tuple(mv.pieceType,mv.startSquare);

    gain[depth] = Evaluation::WeightArray[getPieceTypeOfSquare(mv.endSquare)];
    do{
        depth++;
        gain[depth] = Evaluation::WeightArray[std::get<0>(smallestAttacker)] - gain[depth-1];
        if(std::max(-gain[depth-1], gain[depth]) < 0) break;
        attadef ^= fromSet;
        occupied ^= fromSet;
//        if(fromSet & mayXRay)
        attadef |= AttacksToSquare(mv.endSquare,occupied);
        currColor = FlipColor(currColor);
        smallestAttacker = getSmallestAttacker(mv.endSquare, attadef & (colorPositions[currColor] & occupied));
        fromSet = SquareBB[std::get<1>(smallestAttacker)];
    } while(fromSet);
    while(--depth){
        gain[depth-1] = -std::max(-gain[depth-1],gain[depth]);
    }
    return gain[0];
}

Bitboard Position::getCastlingBoard() const {
    return this->previousCastlingBoards.back();
}

void Position::MakeNullMove() {
    zHash ^= ZobristHash::zColorTable[currentMoveColor];
    currentMoveColor = FlipColor(currentMoveColor);
}

void Position::UnMakeNullMove() {
    currentMoveColor = FlipColor(currentMoveColor);
    zHash ^= ZobristHash::zColorTable[currentMoveColor];
}

Move Position::getLastMove() const {
    if(lastMoves.empty()) { return Move{};}
    else { return lastMoves.back(); }
}

SearchNode::SearchNode() {}

SearchNode::SearchNode(Move mv) {
    this->move = mv;
}

bool SearchNode::operator<(const SearchNode& b) const {
    return eval < b.eval;
}

SearchQuery::SearchQuery() {
    this->depth = 0;
}

SearchQuery::SearchQuery( int depth){
    this->depth = depth;
}

SearchResult::SearchResult() { }

//SearchResult::SearchResult(const SearchResult &sr){
//    this->eval = sr.eval;
//    this->position = sr.position;
//    this->node = sr.node;
//    this->nullWindow = sr.nullWindow;
//}
//
//SearchResult SearchResult::operator=(const SearchResult &nd) {
//    if(this == &nd) return *this;
//    SearchResult newRes(nd.node);
//    newRes.node = nd.node;
//    newRes.eval = nd.eval;
//    newRes.position = nd.position;
//    return newRes;
//}
