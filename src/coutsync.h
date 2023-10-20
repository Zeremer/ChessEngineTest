//
// Created by clint on 1/27/19.
//

#pragma once

#include <iosfwd>
#include <mutex>
#include <iostream>



/**
 * @brief Enumeration for controlling the synchronous output mutex
 */
enum CoutSync { Lock, Unlock };

/**
 * @brief Overloaded operator for managing synchronous output
 * @param os output stream
 * @param sc Sync lock action
 * @return
 */
inline std::ostream& operator<<(std::ostream& os, CoutSync sc) {

    static std::mutex m;

    if (sc == Lock)
        m.lock();

    if (sc == Unlock)
        m.unlock();

    return os;
}
#define syncCout std::cout << Lock
#define syncEndl std::endl << Unlock
#define syncEndIn "" << Unlock