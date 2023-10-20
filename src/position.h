//
// Created by paulp on 1/16/2019.
//

#pragma once
#include "types.h"
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <shared_mutex>
#include <deque>

class Position {
private:
    std::array<Bitboard, 2> colorPositions {0, 0};
    std::array<Bitboard, 6> pieceBoards {0, 0, 0, 0, 0, 0};
    PieceColor currentMoveColor = White;
    std::vector<GameState> previousGameStates {};// {Normal, Normal, Normal};
    std::vector<PieceType> previousCapturedPieces {};// {NoType, NoType, NoType};
    PieceType lastCaptureTypeWithoutCheck;
    std::vector<Move> lastMoves {};
    std::vector<SquareIndex> previousEnPassantSquares {};// {none, none, none};
    /// Stores the rook bits for which side the King is able to castle on as well as the King bits themselves
    std::vector<Bitboard> previousCastlingBoards {};// {0, 0, 0};
//    int lastMoveIndex = -1;
    Bitboard zHash = 0;
    int halfmoveClock = 0; // Check if this reaches 50; if it does, then a draw needs to be called.
    int fullmoveNumber = 0;

    /**
     * @brief Initializes Position by calling functions and setting values.
     * @param fen the starting FEN string
     * @author Bryant Foster 3/6/2019
     */
    void initialize(const std::string &fen);

    /**
     * @brief Initializes each bitboard.
     *
     * From the given array of board row strings, initializes the two color bitboards and
     * the six piece boards.
     *
     * @param boardRows array of strings representing how each row is set
     * @author Bryant Foster 2/2/2019; updated 3/6/2019
     */
    void initBitboards(std::array<std::string, 8> boardRows);

    /**
     * @brief Parses a given FEN string into a FENData struct object.
     *
     * @param fen the FEN string representation of the game
     * @return a FENData struct object representing the game state
     * @author Bryant Foster 2/2/2019; updated 3/6/2019
     */
    struct FENData parseFen(const std::string &fen);

    /**
     * @brief Parses a string that contains the castling rights to a Bitboard.
     * @param rights the string that contains the castling rights
     * @return the bitboard containing the rook bits that the king can castle on
     * @author Bryant Foster 3/10/2019
     */
    Bitboard parseCastlingRights(const std::string &rights);

    /**
     * @brief Returns the color a given FEN string char.
     *
     * FEN strings use upper-case and lower-case letters to denote the piece's color.
     * This returns the color based on the following criteria:
     * Upper-Case: White
     * Lower-Case: Black
     *
     * @param piece the FEN string char
     * @return a PieceColor
     * @author Bryant Foster 2/2/2019
     */
    PieceColor getColor(char piece);

    /**
     * @brief Returns the piece type of a given FEN string char.
     *
     * FEN strings use the first letter of a piece type to denote that piece type.
     * NOTE: The only difference is with Knights, which are denoted by an "n".
     *
     * @param piece the FEN string char
     * @return a PieceType
     * @author Bryant Foster 2/2/2019
     */
    PieceType getType(char piece);

    /**
     * @brief Initializes the given piece in the bitboards.
     *
     * Flips the bit to on in the proper colorPositions bitboard and the proper
     * pieceBoards bitboard (based on the row and space).
     *
     * @param color the piece's color
     * @param type the piece's type
     * @param row the row it is located in
     * @param space the space it is located on
     * @author Bryant Foster 2/2/2019
     */
    void initPiece(PieceColor color, PieceType type, int row, int space);

    /**
     * @brief Updates the specified piece's position.
     * @param color the piece color
     * @param type the piece type
     * @param start the starting index
     * @param end the ending index
     * @param playerMove is player move
     * @return true/false if the pieces could be updated
     * @author Bryant Foster/Paul
     */
    bool updatePiecePosition(Move mv, PieceColor mvColor, bool playerMove);

    /**
     * @brief Checks if the move is valid or not.
     * @param type the piece type
     * @param start the starting index
     * @param end the ending index
     * @return true/false; valid/invalid move
     * @author Bryant Foster 2/12/2019
     */
    bool validMove(PieceType type, SquareIndex start, SquareIndex end);

    /**
     * @brief Gets a bitboard representation of all friendly blockers for a given piece.     *
     * @return a bitboard of all friendly blockers
     * @author Bryant Foster 2/12/2019
     */
    Bitboard getFriendlyBlockers();

    /**
     * @brief Gets a bitboard representation of all enemy blockers for a given piece.     *
     * @param type the piece type
     * @return a bitboard of all enemy blockers
     * @author Bryant Foster 2/12/2019
     */
    Bitboard getEnemyBlockers(PieceType type);

    /**
     * @brief Gets all possible moves from movegen.h for a given piece.
     * @param type the piece type
     * @param start the starting index
     * @param friendlyBlockers bitboard of the friendly blockers
     * @param enemyBlockers bitboard of the enemy blockers
     * @return a vector of all possible moves
     * @author Bryant Foster 2/23/2019
     */
    Bitboard getAllPossibleMoves(PieceType type, SquareIndex start, Bitboard friendlyBlockers, Bitboard enemyBlockers);

    /**
     * @brief Returns whether or not a pawn moved two spaces.
     * @param start the starting index
     * @param end the ending index
     * @return true/false pawn moved two spaces or not
     * @author Bryant Foster 2/18/2019
     */
    bool pawnTwoSpaces(SquareIndex start, SquareIndex end);

    /**
     * @brief Calculates the en passant square index.
     * @param start the starting index
     * @return the index that represents the en passant square
     * @author Bryant Foster 2/18/2019
     */
    SquareIndex enPassant(SquareIndex start);

    /**
     * @brief Clears the nth bit.
     * @param b the bitboard
     * @param index the bit to be cleared
     * @author Bryant Foster 2/26/2019
     */
    void clearBit(Bitboard &b, int index);

    /**
     * @brief Sets the nth bit.
     * @param b the bitboard
     * @param index the bit to be set
     * @author Bryant Foster 2/26/2019
     */
    void setBit(Bitboard &b, int index);

    /**
     * @brief Updates the data members based on a successful move.
     * @param mv the move struct object
     * @author Bryant Foster 3/8/2019
     */
    void updateMembersFromMove(Move mv);

public:
    /**
     * @brief Default constructor; uses the defaultFEN to initialize.
     * @author Bryant Foster 2/4/2019
     */
    Position();

    /**
     * @brief Overloaded constructor; uses a given FEN string to initialize.
     * @param fen the FEN string representation of the game/board
     * @author Bryant Foster 2/2/2019
     */
    explicit Position(const std::string &fen);

    /**
     * @brief Copy constructor; uses an instance of the Position class to initialize.
     * @param toCopy the Position object to copy
     * @author Bryant Foster 2/5/2019
     */
    Position(const Position &toCopy);

    /**
     * @brief Copies a position and makes a move on the copied board. (NOT IN USE)
     * @param toCopy position to copy
     * @param mv move to run after copy
     * @author Paul 2/11/2019
     */
    Position(const Position &toCopy, Move mv);

    /**
     * @brief Gives a bitboard representing the required piece(s)'s position(s).
     * @param color the color of the piece
     * @param type the piece type
     * @return a bitboard that is either empty, or shows the position of the piece
     * @author Bryant Foster 1/31/2019
     */
    Bitboard getPiecePositions(PieceColor color, PieceType type) const;

    /**
     * @brief The getter for the colorPositions array.
     * @return the colorPositions array
     * @author Bryant Foster 2/5/2019
     */
    std::array<Bitboard, 2> getColorPositions() const;

    /**
     * @brief gets the bitboard for a given PieceColor
     * @param color color piece
     * @return
     * @author Paul 2/11/2019
     */
    Bitboard getColorPosition(PieceColor color) const;

    /**
     * @brief gets the position bitboard for the current player's PieceColor
     * @return
     * @author Paul 2/11/2019
     */
    Bitboard getCurrentPlayerPiecePosition() const;

    /**
     * @brief gets the position bitboard for the other player's PieceColor
     * @return
     * @author Paul 2/11/2019
     */
    Bitboard getOtherPlayerPiecePosition() const;

    /**
     * @brief The getter for the pieceBoards array.
     * @return the pieceBoards array
     * @author Bryant Foster 2/5/2019
     */
    std::array<Bitboard, 6> getPieceBoards() const;

    /**
     * @brief Gets the pieceboard for a given PieceType
     * @return
     * @param type
     * @author Paul 2/11/2019
     */
    Bitboard getPieceBoard(PieceType type) const;

    /**
     * @brief Gets castling bitboard
     * @return bitboard of initial Rook and King locations
     * @author Porter Glines 2/28/2019
     */
    Bitboard getCastlingBoard() const;

    /**
     * @brief Gets the PieceColor of the current player
     * @return
     * @author Paul 2/11/2019
     */
    PieceColor getCurrentMoveColor() const;

    /**
     * @brief The getter for the Current Game State.
     * @return the Current Game State
     * @author Bryant Foster 2/23/2019
     */
    GameState getCurrentGameState() const;

    /**
     * @brief Setter for CurrentGameState
     * @param state GameState to set to
     * @authorPorter Glines 3/10/19
     */
    void setCurrentGameState(GameState state);

    /**
     * @brief Plays a move on a position
     * @param mv
     * @author Paul 2/11/2019
     */
    void move(Move mv);

    /**
     * @brief Plays a move for the human player on a position
     * @param mv the Move struct object
     * @return true if the move was valid and took place
     * @author Bryant Foster 3/8/2019
     */
    bool playerMove(Move mv);

    /**
     * @brief Resets the bitboards back one turn, if possible.
     * @author Bryant Foster 2/26/2019
     */
    void undoLastMove();

    /**
     * @brief Resets the bitboards back two turns, if possible.
     * @author Bryant Foster 2/28/2019
     */
    void undoLastTwoMoves();

    /**
     * @brief En Passant Square Index getter.
     * @return the En Passant Square Index
     * @author Paul 2/12/2019
     */
    SquareIndex getEnPassantSquare() const;

    /**
     * @brief gets the Zobrist Hash for a position
     * @return
     * @author Paul
     */
    Bitboard getHash() const;

    /**
     * @brief returns a bitboard of pieces that are attacking a given square in the position
     * @param sq
     * @param allBlockers If not set will use the blockers of the positions itself
     * @return
     * @author Paul
     */
    Bitboard AttacksToSquare(SquareIndex sq, Bitboard allBlockers = ~0ULL) const;

//    /**
//     * @brief returns bitboard of pieces that X-Ray a square
//     * @param sq
//     * @return
//     */
//    Bitboard XRayAttacksToSquare(SquareIndex sq) const;

    /**
     * @brief Returns a tuple with the PieceType and Square Index of the smallest attacker
     * @param pos
     * @param pieces
     * @return
     * @author Paul Grahek
     */
    std::tuple<PieceType,SquareIndex> getSmallestAttacker(SquareIndex sq, Bitboard pieces = ~0ULL) const;

    /**
     * @brief Calculates the Static Exchange Evaluation of a given move
     * @param mv
     * @return
     * @author Paul Grahek
     */
    int SEE(Move mv) const;

    /**
     * @brief returns the piecetype of the entity at the SquareIndex
     * @param sq
     * @return
     */
    PieceType getPieceTypeOfSquare(SquareIndex sq) const;

    /**
     * @brief Makes a "passing" move to skip the current position. Used for Null Move Reductions
     * @author Paul Grahek
     */
    void MakeNullMove();

    /**
     * @brief undo a "passing" move
     * @author Paul Grahek
     */
    void UnMakeNullMove();

    /**
     * @brief Simple, faster copy
     * @return
     * @author Paul Grahek
     */
    Position simpleCopy() const;

    /**
     * @brief Getter for a reference to the last element of the lastMoves vector.
     * @return A constant reference to the last element of the vector
     * @author Bryant Foster
     */
    Move getLastMove() const;
};

/**
  * @brief SearchNode
  *
  * A Search Node contains data about the move made, its best evaluation,
  * and its child nodes during the search
  *
  * @author Paul Grahek 2/21/2019
  */
struct SearchNode {
    SearchNode();
    explicit SearchNode(Move mv);
//    static std::shared_mutex sMutex;
    Move move;
    int eval = 0;
    std::vector<SearchNode> nextNodes;
    
    /**
     * @brief Overloaded comparison operator, to compare two SearchNodes' evals
     * @param b The SearchNode to compare to
     * @return True/False this SearchNode has an eval less than b's
     * @author Bryant Foster
     */
    bool operator<(const SearchNode& b) const;
};

/**
 * @brief SearchQuery
 *
 * Struct used to transfer data from the master to a
 * slave thread to process a particular search from a node in the tree
 *
 * @author Paul Grahek 2/4/19
 */
struct SearchQuery {
    SearchQuery();
    SearchQuery(int depth);
//    SearchQuery(const SearchQuery& sq);
    std::shared_ptr<Position> position;
    int depth = 0;
    int alpha = 0;
    int beta = 0;
    int firstguess = 0;
    Move movePlayed;
    size_t nodeIndex = 0;
    bool nullWindow;
};

/**
 * @brief SearchResult
 *
 * Struct used to transfer data from a slave to the
 * master thread as a result from a particular search from a node in the tree.
 *
 * @author Paul Grahek 2/4/19
 */
struct SearchResult{
    SearchResult();
//    SearchResult(const SearchResult& sq);
    std::shared_ptr<Position> position;
    int eval = 0;
    size_t nodeIndex = 0;
    SearchNode node;
    bool nullWindow;
};

/**
 * @brief A search output contains a position and the corresponding search tree for that position
 * @author Paul
 */
struct SearchOutput {
    Position position;
    SearchNode node;
};

struct SearchStack {
    std::array<Move,SEARCH_MAX_DEPTH> pvmoves;
    int stackDepth = 0;
    Move currentMove;
    Move excludedMove;
    int evaluation = 0;
    int statScore;
    int moveCount;

};
/**
 * @brief Contains data obtained from FEN Strings.
 * @author Bryant Foster 3/6/2019
 */
struct FENData {
    std::array<std::string, 8> rows;
    PieceColor turnColor;
    std::string castlingRights;
    SquareIndex enPassantTarget;
    int halfmoveClock;
    int fullmoveNumber;
};