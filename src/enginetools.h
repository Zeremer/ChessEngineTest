//
// Created by paulp on 1/19/2019.
//

#pragma once

#include <vector>
#include <string>
#include "types.h"

class EngineTools {
    private:
    const static std::string EngineName;
    const static std::string AuthorList;

    public:
    /**
     * @brief Returns a vector of strings as a single string with space-separated entries
     * @return
     * @author Paul Grahek
     */
    static std::string vectorToString(std::vector<std::string>);

    /**
     * @brief Returns a string with all characters lowercase
     * @return
     * @author Paul Grahek
     */
    static std::string stringToLower(std::string);

    /**
     * @brief Returns the string equivalent of a boolean
     * @return
     * @author Paul Grahek
     */
    static std::string boolToString(bool);

    /**
     * @brief Returns the engine info string for UCI
     * @return
     * @author Paul Grahek
     */
    static std::string EngineInfo(EngineMode);

    /**
     * @brief Returns the name of the Chess Engine
     * @return
     * @author Paul Grahek
     */
    static std::string getEngineName();

    /**
     * @brief Returns the list of Authors of the engine
     * @return
     * @author Paul Grahek
     */
    static std::string getAuthorList();
};
