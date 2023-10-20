//
// Created by paulp on 2/21/2019.
//

#pragma once

#include "types.h"
#include "position.h"
#include <map>
#include "thread_win32.h"
#include <atomic>
#include <shared_mutex>
#include "libcuckoo/cuckoohash_map.hh"
#include "moodycamel-concurrentqueue/concurrentqueue.h"
#include <queue>

struct TTEntry {
    int depth;
    TTFlag flag;
    int value;
    Move bestMove;
    bool isValid = false;
    std::chrono::high_resolution_clock::time_point timeUpdated;
};

typedef cuckoohash_map<Bitboard,TTEntry> ConcurrentMap;

class TranspositionTable {
    ConcurrentMap hashTable;

    moodycamel::ConcurrentQueue<std::tuple<std::chrono::high_resolution_clock::time_point,Bitboard>> ageQueue;
    std::atomic<uint64_t> ageStart;
    std::atomic<uint64_t> ageEnd;
    std::mutex hashMutex;
    std::atomic<size_t> maxSize {0};
    std::atomic<long> hashHits {0};
    std::atomic<size_t> size {0};
    friend class Search;
public:
    TranspositionTable(size_t tableSize);

    /**
     * @brief look up a position to see if there is an entry for it in the table
     * @param pos
     * @return
     * @author Paul Grahek
     */
    TTEntry Lookup(Position pos);

    /**
     * @brief Stores a given TTEntry in the map
     * @param pos
     * @param entry
     * @author Paul Grahek
     */
    void Store(Position pos, TTEntry entry);
};

extern TranspositionTable TTable;