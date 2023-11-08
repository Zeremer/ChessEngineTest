#include <iostream>
#include <string.h>
#include <SDL2/SDL_timer.h>
#include "bitboard.h"
#include "uci.h"
#include "coutsync.h"
#include "console.h"
#include "thread.h"
#include "debug.h"
#include "zhash.h"
#include "evaluation.h"
#include "debugging.h"

int main(int argc, char* argv[]) {
    Bitboards::init();
    Evaluation::init();
    ZobristHash::init();
    //TODO: Make testing grounds
//    testUndoMove();
//    SearchPool.startSearch(Position(),std::vector<Move>{test});
//    Position pos;
//    SearchPool.maxDepth = 12;
//    pos.MakeNullMove();
//    SearchPool.startSearch(pos,std::vector<Move>());
//    SearchPool.startSearch(Position("4k3/8/8/3p4/1b6/2N5/8/4K3 w - -"),std::vector<Move>());
//    SearchPool.startSearch(Position("1k1r4/1pp4p/p7/4p3/8/P5P1/1PP4P/2K1R3 w - -"),std::vector<Move>());
//    SearchPool.startSearch(Position("rnbqkbnr/8/8/8/8/8/8/RNBQKBNR w KQkq - 0 1"),std::vector<Move>());
//    Position pos("8/K6r/8/8/8/k7/8/8 w - - 0 1");
//    Position pos("K2k4/8/8/8/3Pp3/8/8/8 b - d3 0 1");
//    Position pos("rn2kbnr/ppp1pp1p/3q4/5bp1/4p3/6P1/PPPP1PBP/RNBQK2R b KQkq - 0 1");
//    SearchPool.maxDepth = 1;
//    SearchPool.startSearch(pos,std::vector<Move>());
    while(true){
        // to enable debug mode pass it as 1st command line arg
        if (argc > 1 && (strcasecmp(argv[1], "debug") == 0)) Debug::setIsDebugLogEnabled(true);

        //Console::printStartMsg();

        //std::string interfaceType;
        //getline(std::cin,interfaceType);

        // run using uci commands directly on a command line
        //if(interfaceType == "uci"){
            //UCI::loop(argc,argv);
            //break;
        //}
        // run from an GUI window powered by SDL2
        //else if(interfaceType == "gui"){
            // initial setup for SDL window for GUI
            std::shared_ptr<Window> gameWindow(new Window());
            gameWindow->gameLoop();
            break;
        }
        //else if(interfaceType == "quit")break;
        //else {
            //syncCout << "Invalid Interface. Please use uci or cli (I found typing gui brings up the gui)" << syncEndl;
        //}
    //}

    return 0;
}
