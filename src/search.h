//
// Created by paulgrahek on 1/28/19.
//

#pragma once

#include "position.h"
#include "types.h"
#include "thread.h"
#include "coutsync.h"
#include <future>

class Search {

    /**
     * @brief Negamax for the Master Thread. Delegates sub-searches to Workers
     * @param pos
     * @param depth
     * @param alpha
     * @param beta
     * @param node
     * @param master
     * @return
     * @author Paul Grahek
     */
    static int MasterNegamax(Position pos, int depth, int alpha, int beta, SearchNode &node,
                                std::vector<Move> movesToSearch = std::vector<Move>());

    /**
     *
     * @param pos
     * @param depth
     * @return
     */
    static SearchOutput MasterSearch(Position pos, int firstguess, int depth);

    /**
     * @brief Memory Test Driver (MTD(f)) search. Delegates alpha-beta to workers
     * @param pos
     * @param firstguess
     * @param depth
     * @return
     */
    static int MTDF(Position pos, int firstguess, int depth, SearchNode& node);

    /**
     * @brief Takes a search query from the master thread and searches that tree until completion
     * @param query query to search
     * @param worker reference to the worker thread itself
     * @return SearchResult of the query
     * @author Paul Grahek
     */
    static SearchResult workerSearch(SearchQuery query, Worker &worker);

    /**
     * @brief Root call for master thread to execute search.
     * @param rootpos
     * @param maxDepth
     * @param master
     * @return
     * @author Paul Grahek
     */
    static std::shared_ptr<SearchNode> masterSearch(Position rootpos, int maxDepth, std::vector<Move> movesToSearch);

    /**
     * @brief Standard negamax tree search for workers
     * @param pos
     * @param depth
     * @param alpha
     * @param beta
     * @param node
     * @param worker
     * @return evaluation of the node
     * @author Paul Grahek
     */
    static int negamax(Position& pos, int depth, int alpha, int beta, SearchNode& node);

    /**
     * @brief compares two doubles for the largest value
     *      Not used right now but may need to for VS C++ compatiblity
     * @param a first number to compare
     * @param b second number to compare
     * @return largest value between the comparison
     * @author Paul Grahek
     */
    static int max(int a, int b);

    /**
     * @brief compares two doubles for the smaller value
     *      Not used right now but may need to for VS C++ compatiblity
     * @param a first number to compare
     * @param b second number to compare
     * @return smallest value between the comparison
     * @author Paul Grahek
     */
    static int min(int a, int b);

    /**
     * @brief Sorts a vector of moves, ordering the given Transposition Table move at the top.
     * @param moves
     * @author Paul Grahek
     */
    static void sortMoves(std::vector<Move>& moves, Move ttMove);

    /**
     * @brief Search that looks to reduce the horizon effect at nodes with depth of 0
     * @param pos
     * @param alpha
     * @param beta
     * @return
     * @author Paul Grahek
     */
    static int Quiesce(Position pos, int alpha, int beta);

//    /**
//     * @brief
//     * @param pos
//     * @param depth
//     * @param alpha
//     * @param beta
//     * @param node
//     * @param worker
//     * @return evaluation of the node
//     * @author Paul Grahek
//     */
//     static double negascout(Position pos, int depth, double alpha, double beta,SearchNode& node);

    friend class Worker;
    friend class MasterWorker;

};
