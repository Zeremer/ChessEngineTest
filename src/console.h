//
// Created by clint on 1/24/19.
//

#pragma once

#include "types.h"
#include "position.h"

class Console {
    public:
    /**
     * @brief Prints the contents of a single bitboard
     * @param bb bitboard to print
     * @author Clinton Brown
     * @return
     */
    static void printBitboard(Bitboard bb);

    /**
     * @brief Prints the combined content of bitboards for display
     *
     * Prints all the pieces on the pieces on the bitboard by cross
     * referencing the position bitboards.
     *
     * @author Clinton Brown
     * @return
     */
    static void printCombinedBitboards(std::shared_ptr<Position>);


    /**
     * @brief Prints the start message to show when the program launches
     * @author Clinton Brown
     * @return
     */
    static void printStartMsg();

    /**
     * @brief logs a given rootNode's tree to a file.
     * @param rootNode
     * @author Paul Grahek
     */
    static void logNodeTree(SearchNode& rootNode);

    /**
     * @brief Writes a node tree to a file given a root node. Recursive function
     * @param node Node to Print
     * @param out Out Stream
     * @param nodeLevel recursive level
     * @author Paul Grahek
     */
    static void writeNodeTree(SearchNode& node, std::ofstream& out,int nodeLevel=0);

};
