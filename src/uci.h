//
// Created by paulp on 1/17/2019.
//

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "window.h"

namespace UCI {

    constexpr char DefaultFEN[]= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    /**
     * @brief Initializes the UCI part of the engine (UNUSED)
     *
     * @author Paul Grahek
     */
    void init();

    /**
     * @brief Primary interface loop of the UCI interface
     *
     * @param argc
     * @param argv
     * @author Paul Grahek
     */
    void loop(int argc, char* argv[]);

    /**
     * @brief Parses input for individual tokens. Removes invalid input at front of command
     *
     * Inspiration: https://stackoverflow.com/questions/2275135/splitting-a-string-by-whitespace-in-c
     *
     * @param input
     * @return
     * @author Paul Grahek
     */
    std::vector<std::string> getValidInputTokens(std::string input);

    /**
     * @brief Tells engine to use UCI interface
     *
     * @author Paul Grahek
     */
    void uci();

    /**
     * @brief Enters the engine into debug mode
     *
     * @param args
     * @author Paul Grahek
     */
    void debug(std::vector<std::string> args);

    /**
     * @brief Synchronizes the engine with the GUI
     *
     * @author Paul Grahek
     */
    void isready();

    /**
     * @brief sets a parameter for an option in the engine
     *
     * @param args
     * @author Paul Grahek
     */
    void setoption(std::vector<std::string> args);

    /**
     * @brief sent to the engine when the next search will be from a different game
     *
     * @param: Window*: pointer to the SDL window for the GUI
     * @author Paul Grahek, Clinton Brown
     */
    void ucinewgame();

    /**
     * @brief sets up the position described in the FEN string on the internal board
     *       and plays the moves given on it
     *
     * @param args
     * @author Paul Grahek
     */
    void position(std::vector<std::string> args);

    /**
     * @brief start calculating on the current position
     *
     * From UCI:
     * There are a number of commands that can follow this command, all will be sent in the same string.
     * If one command is not sent its value should be interpreted as it would not influence the search.
	 * searchmoves <move1> .... <movei>
     *  restrict search to this moves only
     *  Example: After "position startpos" and "go infinite searchmoves e2e4 d2d4"
     *  the engine should only search the two moves e2e4 and d2d4 in the initial position.
	 * ponder
	 *	start searching in pondering mode.
	 *	Do not exit the search in ponder mode, even if it's mate!
	 *	This means that the last move sent in in the position string is the ponder move.
     *  The engine can do what it wants to do, but after a "ponderhit" command
	 *	it should execute the suggested move to ponder on. This means that the ponder move sent by
	 *	the GUI can be interpreted as a recommendation about which move to ponder. However, if the
	 *	engine decides to ponder on a different move, it should not display any mainlines as they are
	 *	likely to be misinterpreted by the GUI because the GUI expects the engine to ponder
	 *  on the suggested move.
	 * wtime <x>
	 *	white has x msec left on the clock
	 * btime <x>
	 *	black has x msec left on the clock
	 * winc <x>
	 *	white increment per move in mseconds if x > 0
	 * binc <x>
	 *	black increment per move in mseconds if x > 0
	 * movestogo <x>
     *  there are x moves to the next time control,
	 *	this will only be sent if x > 0,
	 *	if you don't get this and get the wtime and btime it's sudden death
	 * depth <x>
	 *	search x plies only.
     * nodes <x>
	 *  search x nodes only,
	 * mate <x>
	 *	search for a mate in x moves
	 * movetime <x>
	 *	search exactly x mseconds
	 * infinite
	 *	search until the "stop" command. Do not exit the search without being told so in this mode!
     *
     * @param args
     * @author Paul Grahek
     */
    void go(std::vector<std::string> args);

    /**
     * @brief Stop calculating as soon as possible
     * From UCI:
     *  stop calculating as soon as possible,
	 *  don't forget the "bestmove" and possibly the "ponder" token when finishing the search
     * @author Paul Grahek
     */
    void stop();

    /**
     * @brief
     * From UCI:
     *  The user has played the expected move. This will be sent if the engine was told to ponder on the same move
	 *   the user has played. The engine should continue searching but switch from pondering to normal search.
     * @author Paul Grahek
     */
    void ponderhit();

    /**
     * @brief Sends debug information to the CLI interface
     *
     * @param message
     * @author Paul Grahek
     */
    void sendDebugInfo(std::string message);

    /**
     * @brief Send multiple debug messages to the CLI interface
     *
     * @param messages
     * @author Paul Grahek
     */
    void sendDebugInfo(std::vector<std::string> messages);

    /**
     * @brief send the invalid arguments to the command line
     *
     * @param args
     * @author Paul Grahek
     */
    void sendInvalidArguments(std::vector<std::string> args);

    std::string getAnswer();
};
