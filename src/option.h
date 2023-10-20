//
// Created by paulp on 1/19/2019.
//

#pragma once

#include <string>
#include <sstream>
#include "enginetools.h"

/**
 * @brief TODO: Add Brief
 * @tparam T
 * @author Clinton Brown
 */
template <class T>
class Option{
public:
    Option();
    explicit Option(T);

    /**
     * @brief TODO: Add Brief
     * @author Clinton Brown
     */
    void resetToDefault();

    /**
     * @brief TODO: Add Brief
     * @return
     * @author Clinton Brown
     */
    std::string getOptionType();

private:
    T defaultVal, currVal;
};
