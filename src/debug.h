//
// Created by paulp on 1/17/2019.
//

#pragma once

#include <iostream>
#include "types.h"

class Debug {
    private:
    static bool _isDebugLogEnabled;

    public:
    static const char* LogType[];
    static void setIsDebugLogEnabled(bool);
    static bool getIsDebugLogEnabled();

};