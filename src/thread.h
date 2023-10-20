//
// Created by paulp on 1/20/2019.
// Worker pool derived from http://progsch.net/wordpress/?p=81
//

#pragma once

#include <vector>
//#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <atomic>
#include <queue>
#include "position.h"
#include <functional>
#include "thread_win32.h"
//#include <atomic>

class Worker;
class WorkerPool;


/**
 * @brief Worker
 *
 * A Worker processes search queries sent to it from the Master Worker.
 * Object is aware of its parent pool for synchronization purposes.
 *
 * @author Paul Grahek 2/4/19
 */
class Worker{
public:
    /**
     * @brief Instantiates Worker with link to its owner Threadpool
     * @param p
     * @author Paul Grahek
     */
    Worker(WorkerPool& p);

    /**
     * @brief Primary loop for worker to wait for SearchQueries
     * @author Paul Grahek
     */
    virtual void operator()();
    WorkerPool& pool;
};

/**
 * @brief MasterWorker
 *
 * A Master Thread takes in search queries from the command interface and performs the tree search.
 * Delegates other branch searching to Worker Threads
 *
 * @author Paul Grahek 2/4/19
 */
class MasterWorker : public Worker{
public:
    /**
     * @brief Instiantiates MasterWorker with link to its owner Threadpool
     * @param p
     * @author Paul Grahek
     */
    MasterWorker(WorkerPool& p);

    /**
     * @brief Primary loop for Master Worker to wait for Search Requests
     * @author Paul Grahek
     */
    void operator()() override;
private:

};



/**
 * @brief WorkerPool
 *
 * A Worker Pool processes search queries and controls the threads for the search.
 *
 * @author Paul Grahek 2/4/19
 */
class WorkerPool {
public:

    WorkerPool(size_t);

    /**
     * @brief Adds a SearchQuery to in InQueue
     * @param query
     * @author Paul Grahek
     */
    void enqueue(SearchQuery query);

    /**
     * @brief
     * @param startpos
     * @param movesToSearch
     * @author Paul Grahek
     */
    void startSearch(Position startpos, std::vector<Move> movesToSearch);

    /**
     * @brief Stops the search for the Threadpool
     * @author Paul Grahek
     */
    void stopSearch();

    /**
     * @brief Gets the Output of the Threadpool
     * @author Paul Grahek
     */
    SearchOutput getOutput();

    /**
     * @brief pops a SearchResult off of the OutQueue
     * @author Paul Grahek
     */
    SearchResult pop_result();

    ~WorkerPool();

    std::atomic_bool done;
    std::atomic<int> maxDepth{10};
private:
    std::atomic<int> currDepth{1};
    std::atomic<double> outputEval{0};
    std::atomic<int>  threadCount {8};
    std::atomic<unsigned long> nodesSearched {0};
    std::atomic_bool kill, endSearch, betaCutoff;


    friend class Worker;
    friend class MasterWorker;
    friend class Search;
    std::vector<std::thread> workers;
    std::thread masterThread;

    std::deque<SearchQuery> tasks;
    std::deque<SearchResult> returnVals;

    std::mutex inQueueMutex;
    std::condition_variable inQueueNotEmpty;

    std::mutex outQueueMutex;
    std::condition_variable outQueueNotEmpty;

    std::mutex searchMutex;
    std::condition_variable go;
    std::vector<Move> movesToSearch;

    std::mutex outputMutex;
    SearchOutput output;

    Position rootPos;// = Position("4k3/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    SearchNode rootNode;
};

extern WorkerPool SearchPool;