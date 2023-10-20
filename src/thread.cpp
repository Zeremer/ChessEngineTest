//
// Created by paulp on 1/20/2019.
//

//#include "thread.h"
//#include <sleep>
#include "thread.h"
#include "search.h"
#include <chrono>


WorkerPool SearchPool(4);

WorkerPool::WorkerPool(size_t threadCount) {
    this->threadCount = threadCount;
    kill = false;
    endSearch = false;
    for(size_t i = 0; i < threadCount; i++){
        workers.emplace_back(std::thread(Worker(*this)));
    }
    masterThread = std::thread(MasterWorker(*this));
}

WorkerPool::~WorkerPool() {
    kill = true;
    inQueueNotEmpty.notify_all();
    go.notify_all();
    for(auto it = workers.begin(); it != workers.end(); ++it){
        it->join();
    }
    masterThread.join();

}

void WorkerPool::enqueue(SearchQuery query) {
    {
        std::unique_lock<std::mutex> lock(inQueueMutex);
        tasks.push_back(query);

    }
    inQueueNotEmpty.notify_one();

}

SearchResult WorkerPool::pop_result() {
    SearchResult retVal;
    {
        std::unique_lock<std::mutex> lock(outQueueMutex);
        while(!kill && returnVals.empty())
            outQueueNotEmpty.wait(lock);
        if(kill) return retVal;

        retVal = returnVals.front();
        returnVals.pop_front();
    }
    return retVal;
}

void Worker::operator()() {
    SearchQuery task;
    for(;;){
        {
            std::unique_lock<std::mutex> lock(pool.inQueueMutex);

            while(!pool.kill && pool.tasks.empty()) {
                pool.inQueueNotEmpty.wait(lock);
            }

            if(pool.kill) return;
//            pool.idleWorkers-=1;
            task = pool.tasks.front();
            pool.tasks.pop_front();
        }
        SearchResult retVal = Search::workerSearch(task, *this);
        {
            std::unique_lock<std::mutex> lock(pool.outQueueMutex);
            pool.returnVals.emplace_back(retVal);
        }
        pool.outQueueNotEmpty.notify_one();
//        pool.idleWorkers+=1;
    }
}

Worker::Worker(WorkerPool &p) : pool(p){}

void MasterWorker::operator()() {
    for(;;){
        {
            std::unique_lock<std::mutex> lock(pool.searchMutex);
            pool.go.wait(lock);
            if(pool.kill) return;
            Position posToSearch = pool.rootPos;
            std::vector<Move> movesToSearch = pool.movesToSearch;
            auto rootNode = Search::masterSearch(posToSearch, pool.maxDepth,pool.movesToSearch);
        }
    }
}


MasterWorker::MasterWorker(WorkerPool &p) : Worker(p){}

void WorkerPool::startSearch(Position startpos, std::vector<Move> movesToSearch){
    endSearch = false;
    {
        std::unique_lock<std::mutex> lock(searchMutex);
        this->rootPos = startpos;
        this->movesToSearch = std::move(movesToSearch);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    go.notify_one();
}

void WorkerPool::stopSearch() {
    endSearch = true;
}

SearchOutput WorkerPool::getOutput() {
    {
        std::unique_lock<std::mutex> lock(SearchPool.outputMutex);
        return output;
    }
}


