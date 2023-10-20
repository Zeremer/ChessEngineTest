//
// Created by paulgrahek on 2/6/19.
//

#pragma once

#include <string>
#include "types.h"
#include "position.h"

namespace Moves {
    /**
     * @brief Creates a move from a given UCI string
     * @param strMove
     * @return
     * @author Paul Grahek
     */
    Move parseMoveString(std::string strMove);

    /**
     * @brief Creates a string representation of a move (UCI compatible)
     * @param move
     * @return
     * @author Paul Grahek
     */
    std::string createMoveString(Move move);
};
