//
// Created by paulp on 2/21/2019.
//

#include "transpositiontable.h"
#include "coutsync.h"
#include <iterator>
#include <shared_mutex>

TranspositionTable TTable((1024)*(1024)*(1024)/sizeof(TTEntry)/2);

TranspositionTable::TranspositionTable(size_t tableSize) {
    maxSize = tableSize;
    hashTable = cuckoohash_map<Bitboard,TTEntry>();
//    syncCout << maxSize << syncEndl;
}

TTEntry TranspositionTable::Lookup(Position pos) {
    TTEntry entry;
    if(hashTable.find(pos.getHash(),entry)) hashHits++;
    return entry;
}

void TranspositionTable::Store(Position pos, TTEntry entry) {
    entry.timeUpdated = std::chrono::high_resolution_clock::now();
    entry.isValid = true;
    entry.bestMove.type = Hash;
    TTEntry hashEntry;
    hashTable.find(pos.getHash(),hashEntry);
    if(hashEntry.isValid){
        if(hashEntry.depth > entry.depth){
            hashEntry.timeUpdated = entry.timeUpdated;
            entry = hashEntry;
        }
        hashTable.insert_or_assign(pos.getHash(),entry);
    }
    // Replacement algorithm
    else if(size >= maxSize) {

        //Remove from list
        bool erasedEntry = false;
        {
//            std::unique_lock lock(hashMutex);
            while(!erasedEntry)
            {
                std::tuple<std::chrono::high_resolution_clock::time_point,Bitboard> expiration;
                {
//                    uint64_t ageInd = ageStart++;
//                    if(ageInd >= ageEnd) break;
                    if(!ageQueue.try_dequeue(expiration)) break;

                }
                TTEntry ttEntry;
                if(hashTable.find(std::get<1>(expiration),ttEntry)) {
                    if(ttEntry.timeUpdated <= std::get<0>(expiration)){
                        if(hashTable.erase(std::get<1>(expiration)))
                            erasedEntry = true;
                    }
                }
            }
        }
        if(erasedEntry) {
            hashTable.insert_or_assign(pos.getHash(),entry);
        }
    }
    else{
        size++;
        hashTable.insert_or_assign(pos.getHash(),entry);
    }
    ageQueue.enqueue(std::make_tuple(entry.timeUpdated,pos.getHash()));
}
