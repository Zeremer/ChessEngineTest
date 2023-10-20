//
// Created by paulp on 1/17/2019.
//

#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>
#include "uci.h"
#include "enginetools.h"
#include "coutsync.h"
#include "position.h"
#include "console.h"
#include "window.h"
#include "debug.h"

void UCI::loop(int argc, char **argv) {
//    Position pos;
    uci();
    bool quit = false;

    do{
        std::string input;
        getline(std::cin,input);
        if(std::cin.eof()) break;

        std::vector<std::string> tokens = getValidInputTokens(input);
        std::string cmd = tokens.size() == 0? "":tokens[0];
        auto argIt = tokens.size()==1?tokens.end():tokens.begin()+1;
        std::vector<std::string> cmdArgs(tokens.end(),tokens.end());

//        for(auto it = tokens.begin(); it != tokens.end(); ++it){
//            std::cout << *it << " ";
//        }
//        std::cout << std::endl;

        if(cmd == "uci") uci();
        else if(cmd == "debug") debug(cmdArgs);
        else if(cmd == "isready") isready();
        else if(cmd == "setoption") setoption(cmdArgs);
//        else if(cmd == "register") registerEngine(cmdArgs);
        else if(cmd == "ucinewgame") ucinewgame();
        else if(cmd == "Position") position(cmdArgs);
        else if(cmd == "go")  go(cmdArgs);
        else if(cmd == "kill") stop();
        else if(cmd == "ponderhit") ponderhit();
        else if(cmd == "quit") quit=true;
        else {
            std::cout << "Unknown Command: " << input << std::endl;
        }
    } while(!quit);
}

void UCI::init() {

}

std::vector<std::string> UCI::getValidInputTokens(std::string input) {
    const std::vector<std::string> validCommands = {
            "uci","debug","isready","setoption","ucinewgame",
            "Position","go","kill","ponderhit","quit"
    };
    std::istringstream buffer(input);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(buffer),
            std::istream_iterator<std::string>(),
            std::back_inserter(tokens));
    for(auto it = tokens.begin(); it != tokens.end();){
        if(std::find(validCommands.begin(), validCommands.end(), *it) != validCommands.end()){
            break;
        }
        else{
            it = tokens.erase(it);
        }
    }
    return tokens;
}


void UCI::uci() {
    std::cout << EngineTools::EngineInfo(EM_UCI) << std::endl
              << "uciok" << std::endl;
}

void UCI::debug(std::vector<std::string> args) {
    bool invalidArgs = false;
    if(args.empty()) invalidArgs = true;
    else if(args[0] == "on") {}
    else if(args[0] == "off") {}
    else {
        invalidArgs=true;
    }
    if(invalidArgs){
        sendInvalidArguments(args);
    }
}

void UCI::isready() {

}

void UCI::setoption(std::vector<std::string> args) {

}

void UCI::ucinewgame() {
    // set up initial board
    std::shared_ptr<Position> boards(new Position());

    // in debug mode print the bitboard layout to the console
    if (Debug::getIsDebugLogEnabled()) {
        syncCout << Debug::LogType[0] << ": Printing initial board layout" << syncEndl;
        Console::printCombinedBitboards(boards);
    }
}

void UCI::position(std::vector<std::string> args) {

}

void UCI::go(std::vector<std::string> args) {
    if(args.empty()) sendInvalidArguments(args);
    else {

//        std::string goCmd = args[0];
//        if(goCmd == "searchmoves"){
//
//        }
//        else if(goCmd = ""){}
//        else if(goCmd = ""){}
//        else if(goCmd = ""){}
//        else if(goCmd = ""){}
//        else if(goCmd = ""){}
//        else if(goCmd = ""){}
//        else if(goCmd = ""){}
//        else if(goCmd = ""){}
//        else if(goCmd = ""){}
//        else if(goCmd = ""){}
//        else if(goCmd = "infinite"){}
//        else sendInvalidArguments(args);
    }
}

void UCI::stop() {

}

void UCI::ponderhit() {

}

void UCI::sendDebugInfo(std::string message) {
    std::cout << "info debug " << message << std::endl;
}

void UCI::sendDebugInfo(std::vector<std::string> messages) {
    for(auto it = messages.begin(); it != messages.end(); ++it){
        sendDebugInfo(*it);
    }
}

void UCI::sendInvalidArguments(std::vector<std::string> args) {
    std::cout << "Invalid Arguments: ";
    if(args.empty()){
        std::cout << "None Provided";
    }
    else{
        std::cout << EngineTools::vectorToString(args);
    }
    std::cout << std::endl;
}
