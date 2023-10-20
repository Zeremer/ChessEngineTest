#include <random>

//
// Created by paulgrahek on 1/28/19.
//

#include <vector>
#include "search.h"
#include "movegen.h"
#include "evaluation.h"
#include "thread.h"
#include "move.h"
#include "console.h"
#include "transpositiontable.h"

constexpr double npsOffset = 1000000000;

int Search::max(int a, int b){
    return a > b ? a : b;
}
int Search::min(int a, int b) {
    return a > b ? b : a;

}

SearchResult Search::workerSearch(SearchQuery query, Worker &worker) {
    SearchNode nd;
    nd.move = query.movePlayed;
    Position pos = *query.position;
    SearchResult result;
    result.position = query.position;
    result.eval = Search::MTDF(pos,query.firstguess,query.depth,nd);
//    result.eval = Search::negamax(pos,query.depth,query.alpha,query.beta,nd);
    result.node = nd;
    result.nullWindow = query.nullWindow;
    result.nodeIndex = query.nodeIndex;
//    if(query.nullWindow) syncCout << "NW" << syncEndIn;
    return result;
}

std::shared_ptr<SearchNode> Search::masterSearch(Position rootpos, int maxDepth, std::vector<Move> movesToSearch) {
    SearchPool.done = false;
    std::shared_ptr<SearchNode> rootNode = std::make_shared<SearchNode>(SearchNode());
    SearchOutput output;
    int score = Quiesce(rootpos, SEARCH_MIN, SEARCH_MAX);
    long long nodesPerSecond = 0;
    for(int depth = 1; depth <= maxDepth || SearchPool.endSearch; depth++){
        SearchPool.currDepth = depth;
        SearchPool.nodesSearched = 0;
        TTable.hashHits = 0;
//        rootNode->nextNodes.clear();
        auto tStart = std::chrono::high_resolution_clock::now();
//        output = MasterSearch(rootpos, score,depth);
//        score = output.node.eval * PieceColorWeight[output.position.getCurrentMoveColor()];
        score = MasterNegamax(rootpos,depth,SEARCH_MIN,SEARCH_MAX,*rootNode,movesToSearch);
        output.node = *rootNode;
//        score = MTDF(rootpos,score,depth,*rootNode);
        auto tEnd = std::chrono::high_resolution_clock::now();
        nodesPerSecond = (double)SearchPool.nodesSearched / (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(tEnd - tStart).count()) * npsOffset;
        syncCout << "depth " << depth
            << " score cp " << (double)score / 100//Evaluation::EvaluationOffset
            << " nodes " << SearchPool.nodesSearched
            << " nps " << nodesPerSecond
            << " hits " << TTable.hashHits
            << syncEndl;
        SearchPool.outputEval = score;
    }
    for (auto nextNode : output.node.nextNodes) {
        syncCout << Moves::createMoveString(nextNode.move) << ": " << nextNode.eval << syncEndl;
    }
//    Console::logNodeTree(output.node);

//    syncCout << (*rootNode).toString() << syncEndl;
    {
        std::unique_lock<std::mutex> lock(SearchPool.outputMutex);
        SearchPool.output = output;
        SearchPool.done = true;
    }

    return rootNode;

}

//SearchOutput Search::MasterSearch(Position pos,int firstguess, int depth){
//    SearchOutput output;
//    output.position = pos;
//
//    int bestscore = SEARCH_MIN;
//    size_t threadInd = 0;
//    size_t done = 0;
//    bool betaCutoff = false;
//    while(threadInd < SearchPool.threadCount){
//        SearchQuery query;
//        query.nodeIndex = threadInd;
//        query.position = std::make_shared<Position>(Position(pos));
//        query.depth = depth;// + threadInd % 2;
//        query.beta = SEARCH_MAX;
//        query.alpha = SEARCH_MIN;
//        query.firstguess = firstguess;
//        SearchPool.enqueue(query);
//        threadInd++;
//    }
//    while(done < SearchPool.threadCount){
//        SearchResult result = SearchPool.pop_result();
//        int score = result.eval;
//        if (score >= bestscore) {
//            bestscore = score;
//            output.node = result.node;
//        }
//        done += 1;
//    }
//    return output;
//}

int Search::MasterNegamax(Position pos, int depth, int alpha, int beta, SearchNode &node,
                             std::vector<Move> movesToSearch) {

    SearchPool.nodesSearched++;
    int alphaOrig = alpha;

    if(pos.getCurrentGameState() == Checkmate) {
        node.eval = SEARCH_MIN;
        return node.eval;
    }
    if(pos.getCurrentGameState() == Stalemate){
        node.eval = 0;
        return node.eval;
    }
    if (depth <= 0 || SearchPool.kill) {
        node.eval = Quiesce(pos,alpha,beta);
        return  node.eval;
    }

    TTEntry entry = TTable.Lookup(pos);
//    if(entry.isValid && entry.depth >= depth){
//        if(entry.flag == Exact){
//            node.eval = entry.value;
//            return node.eval;
//        }
//        else if(entry.flag == LowerBound)
//            alpha = std::max(alpha, entry.value);
//        else if(entry.flag == UpperBound)
//            beta = std::min(beta, entry.value);
//        if(alpha >= beta){
//            node.eval = entry.value;
//            return node.eval;
//        }
//    }
    Move bestMove = Move();//entry.bestMove;

    // Get possible moves for the current position
    std::vector<Move> moves;
    if(node.nextNodes.empty())
    {
        moves = MoveGen::getPossibleMoves(pos);
        sortMoves(moves,Move());
    }
    else {
        std::sort(node.nextNodes.begin(),node.nextNodes.end(),[&pos](const SearchNode& a, const SearchNode& b){
//            return (a.eval * PieceColorWeight[pos.getCurrentMoveColor()] < b.eval * PieceColorWeight[pos.getCurrentMoveColor()]);
            return (a.eval < b.eval);
        });
        for(const SearchNode &nd : node.nextNodes){
            moves.push_back(nd.move);
        }
//        moves = MoveGen::getPossibleMoves(pos);
//        sortMoves(moves,Move());
    }
    // Check if terminal node has no moves (Unable to be searched)
    if(moves.empty()) {
        node.eval =(pos.getCurrentGameState() == Check) ?
                   SEARCH_MIN : 0;
        return node.eval;
    }
    // If the movesToSearch list is not empty, parse the movelist down to the moves
    if(!movesToSearch.empty()) {
        moves.erase(std::remove_if(moves.begin(),moves.end(),[&movesToSearch](const Move& mv){
            for(auto it = movesToSearch.begin(); it != movesToSearch.end();++it){
                if(mv.endSquare == it->endSquare &&
                   mv.startSquare == it->startSquare &&
                   mv.pieceType == it->pieceType &&
                   mv.promotionType == it->promotionType) return false;
            }
            return true;
        }),moves.end());
    }

    //Reserve space for the next nodes
    node.nextNodes.reserve(moves.size());
    if(node.nextNodes.empty()){
        node.nextNodes.emplace_back(SearchNode(moves.front()));
    }

    int bestscore = SEARCH_MIN;
    size_t moveInd = 0;
    size_t done = moveInd;
    while(moveInd < moves.size()){
        if(node.nextNodes.size() < moves.size()){
            node.nextNodes.emplace_back(SearchNode(moves[moveInd]));
        }
        SearchQuery query;
        query.nodeIndex = moveInd;
        query.movePlayed = moves[moveInd];
        query.position = std::make_shared<Position>(Position(pos,moves[moveInd]));
        query.depth = depth-1;
        query.alpha = -beta;
        query.beta = -alpha;
        query.firstguess = -node.eval;
        SearchPool.enqueue(query);
        moveInd += 1;
    }
    bool betaCutoff = false;
    while(done < moves.size()){
        SearchResult result = SearchPool.pop_result();
//        result.node.eval = -result.eval;
        node.nextNodes[result.nodeIndex] = result.node;
        int score = -result.eval;
//        if(!betaCutoff) {
//        if(!SearchPool.betaCutoff) {
            if (score > alpha) {
                alpha = score;
            }
            if (score > bestscore) {
                if (score >= beta) {
//                    betaCutoff = true;
//                    SearchPool.betaCutoff = true;
                }
                bestscore = score;
                bestMove = moves[result.nodeIndex];
            }
//        }
        done += 1;
    }

    entry.value = bestscore;
    entry.bestMove = bestMove;
    if(bestscore <= alphaOrig)
        entry.flag = UpperBound;
    else if(bestscore >= beta)
        entry.flag = LowerBound;
    else
        entry.flag = Exact;
    entry.depth = depth;
    TTable.Store(pos,entry);

    SearchPool.betaCutoff = false;
    node.eval = bestscore;
    return node.eval;
}

int Search::negamax(Position& pos, int depth, int alpha, int beta,SearchNode& node) {
    SearchPool.nodesSearched++;
    int alphaOrig = alpha;

    if(pos.getCurrentGameState() == Checkmate) {
        node.eval = SEARCH_MIN;
        return node.eval;
    }
    if(pos.getCurrentGameState() == Stalemate){
        node.eval = 0;
        return node.eval;
    }
    if (depth <= 0 || SearchPool.betaCutoff || SearchPool.kill) {
        node.eval = Quiesce(pos,alpha,beta);
        return  node.eval;
    }
    TTEntry entry = TTable.Lookup(pos);
    if(depth < SearchPool.currDepth && entry.isValid && entry.depth >= depth){
        if(entry.flag == Exact){
            node.eval = entry.value;
            return node.eval;
        }
        else if(entry.flag == LowerBound)
            alpha = std::max(alpha, entry.value);
        else if(entry.flag == UpperBound)
            beta = std::min(beta, entry.value);
        if(alpha >= beta){
            node.eval = entry.value;
            return node.eval;
        }
    }
    Move bestMove = entry.bestMove;

    std::vector<Move> possibleMoves = MoveGen::getPossibleMoves(pos);
    node.nextNodes.reserve(possibleMoves.size());
    if(possibleMoves.empty()) {
        node.eval = (pos.getCurrentGameState() == Check) ?
               SEARCH_MIN : 0;
        return node.eval;
    }
    sortMoves(possibleMoves, bestMove);

    int newDepth = depth-1;
    int bestValue = SEARCH_MIN;
    for(auto nextMove = possibleMoves.begin(); nextMove != possibleMoves.end(); ++nextMove){
        bool nullMovePruning = false;
        // Null Move Reduction
        if(pos.getCurrentGameState() == Normal && nextMove != possibleMoves.begin()) {
            int R = depth > 6 ? 4 : 3;
            pos.MakeNullMove();
            SearchNode nullNode;
            int nullScore = -negamax(pos,newDepth-R,-beta,1-beta,nullNode);
            pos.UnMakeNullMove();
            if(nullScore >= beta){
                newDepth -= 4;
                nullMovePruning = true;
                if(newDepth <= 0){
                    bestMove = Move();
                    bestValue = Quiesce(pos,alpha,beta);
                    break;
                }
            }
        }
        int value;
//        {
//            pos.move(*nextMove);
//            node.nextNodes.emplace_back(SearchNode(*nextMove));
//            value = std::max(value,-negamax(pos,depth-1,-beta,-alpha,node.nextNodes.back()));
//            pos.undoLastMove();
//        }

        Position nextPos = pos.simpleCopy();
        nextPos.move(*nextMove);
        node.nextNodes.emplace_back(SearchNode(*nextMove));
        int doFullDepthSearch = 0;
        if(depth >= 3
            && nextMove != possibleMoves.begin()
            && (!(nextMove->type == Winning ||
                  nextMove->type == Equal ||
                  nextMove->type == LosingCapture ||
                  nextMove->promotionType != NoType) ||
                nullMovePruning)){
            int reduction = (int)(sqrt(double(newDepth)) + sqrt(double(std::distance(possibleMoves.begin(),nextMove)-1)));
            int searchDepth = std::max((newDepth)- std::max(reduction,0),1);
            value = -negamax(nextPos,searchDepth,-(alpha+1),-alpha,node.nextNodes.back());
            doFullDepthSearch = (value > alpha && searchDepth != (depth-1));
        }
        else{
            value = -negamax(nextPos,newDepth,-beta,-alpha,node.nextNodes.back());
        }
        if(doFullDepthSearch){
            value = -negamax(nextPos,newDepth,-(alpha+1),-alpha,node.nextNodes.back());
        }
        nextPos.undoLastMove();
//        assert(nextPos.getHash() == pos.getHash());
//        assert(nextPos.getPieceBoards() == pos.getPieceBoards());
//        assert(nextPos.getColorPositions() == pos.getColorPositions());
        if(value > bestValue ) {
            bestValue = value;
            bestMove = *nextMove;
            if(value > alpha){
                alpha = value;
            }
        }
        if(alpha >= beta) break;

    }
    entry.value = bestValue;
    entry.bestMove = bestMove;
    if(bestValue <= alphaOrig)
        entry.flag = UpperBound;
    else if(bestValue >= beta)
        entry.flag = LowerBound;
    else
        entry.flag = Exact;
    entry.depth = depth;
    TTable.Store(pos,entry);

    if(SearchPool.currDepth-depth > 6) { node.nextNodes = std::vector<SearchNode>();}
//    node.nextNodes = std::vector<SearchNode>();
    node.eval = bestValue;
    return node.eval;
}

void Search::sortMoves(std::vector<Move>& moves, Move ttMove) {
    if(ttMove.type != None){
        auto ttIt = moves.begin();
        while(ttIt != moves.end()){
            if(ttIt->pieceType == ttMove.pieceType &&
               ttIt->promotionType == ttMove.promotionType &&
               ttIt->startSquare == ttMove.startSquare &&
               ttIt->endSquare == ttMove.endSquare) {
                ttIt->type = Hash;
                break;
            }
            ++ttIt;
        }
    }

    std::sort(moves.begin(), moves.end(), [](Move a, Move b) { return a.type < b.type;});
}

int Search::Quiesce(Position pos, int alpha, int beta) {
    int bestScore = Evaluation::evalPosition(pos);
//    return bestScore; // Disables Quiesce Search
    if(SearchPool.kill || SearchPool.betaCutoff) return bestScore;
    alpha = std::max(alpha, bestScore);
    if(alpha>=beta) return bestScore;

    std::vector<Move> possibleMoves = MoveGen::getPossibleMoves(pos,true);
    if(possibleMoves.empty()) return bestScore;
    sortMoves(possibleMoves, Move());
    for(auto nextMove = possibleMoves.begin(); nextMove != possibleMoves.end(); ++nextMove){
        SearchPool.nodesSearched++;
        Position nextPos(pos);
        nextPos.move(*nextMove);
        int score = -Quiesce(nextPos,-beta,-alpha);
        bestScore = std::max(bestScore,score);
        alpha = std::max(alpha, bestScore);
        if(alpha>=beta) break;
    }
    return bestScore;
}

int Search::MTDF(Position pos, int firstguess, int depth, SearchNode& node) {

    int bound[2] = {SEARCH_MIN,SEARCH_MAX};
    do{
        int beta = firstguess + (firstguess == bound[0]);
        node.nextNodes.clear();
        firstguess = negamax(pos, depth, beta - 1, beta, node);
        bound[firstguess < beta] = firstguess;
    }while(bound[0] < bound[1]);
    return firstguess;
}

