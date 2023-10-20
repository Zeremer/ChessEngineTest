//
// Created by clint on 1/24/19.
//

#include <iostream>
#include "debug.h"
#include "types.h"

/*
 * Strings for labelling debug logs
 * [0] INFO
 * [1] WARNING
 */
const char* Debug::LogType[] = { "INFO", "WARNING" };
bool Debug::_isDebugLogEnabled = false;

void Debug::setIsDebugLogEnabled(bool value) {
    _isDebugLogEnabled = value;
}

bool Debug::getIsDebugLogEnabled(){
    return _isDebugLogEnabled;
}