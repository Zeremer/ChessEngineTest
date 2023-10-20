//
// Created by clint on 1/24/19.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "console.h"
#include "types.h"
#include "enginetools.h"
#include "coutsync.h"
#include "debug.h"
#include "position.h"
#include "move.h"
#include "graphics.h"

void Console::printBitboard(Bitboard bb) {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            std::cout << " " << ((bb >> (8 * row + col)) & 1) << "";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Console::printCombinedBitboards(std::shared_ptr<Position> board){
    char displayBoard[8][8];
    Graphics::createDisplayBitboard(board, displayBoard);

    for (int row = 7; row >= 0; row--) {
        std::stringbuf rowChars;
        for (int col = 0; col < 8; col++) {
            if (displayBoard[row][col] != 0) {
                rowChars.sputc(' ');
                rowChars.sputc(displayBoard[row][col]);
            } else {
                rowChars.sputc(' ');
                rowChars.sputc('.');
            }
        }
        syncCout << rowChars.str() << syncEndl;
    }
    syncCout << syncEndl;
}

void Console::printStartMsg(){
    syncCout << EngineTools::getEngineName() << " by " << EngineTools::getAuthorList() << syncEndl;
    std::string mode = Debug::getIsDebugLogEnabled() ? "DEBUG" : "STANDARD";

    // print mode message, label it as a warning if the engine is in Debug mode
    if (Debug::getIsDebugLogEnabled()) std::cout << Debug::LogType[1] << ": ";
    syncCout << "Current play mode is " << mode << syncEndl;
}

void Console::logNodeTree(SearchNode &rootNode) {
    std::string fileName = "nodetree";
    std::string fileExt = ".txt";
    int treeFileNum = 0;
    std::stringstream fullFileName;
    fullFileName << fileName << treeFileNum << fileExt;
    while(std::ifstream(fullFileName.str().c_str())){
        fullFileName = std::stringstream();
        treeFileNum++;
        fullFileName << fileName << treeFileNum << fileExt;
//        syncCout << fullFileName.str() << syncEndl;
    }
    syncCout << fullFileName.str() << syncEndl;
    std::ofstream out(fullFileName.str().c_str());
    writeNodeTree(rootNode,out);
    out.close();
    syncCout << "Done" << syncEndl;
}

void Console::writeNodeTree(SearchNode & node, std::ofstream& out, int nodeLevel) {
    for(int i = 0; i < nodeLevel; i++) {
        out << ("-");
    }
    out << ("- ");
    out << (Moves::createMoveString(node.move));
    out << (" Eval: ");
    out << (node.eval);
    out << (std::endl);
    for(auto nextNode : node.nextNodes){
        writeNodeTree(nextNode,out,nodeLevel+1);
    }
}
