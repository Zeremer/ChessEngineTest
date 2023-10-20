//
// Created by clint on 1/30/19.
//

#include <typeinfo>
#include <iostream>
#include "option.h"

template <class T>
Option<T>::Option() = default;

/*
 * Creates an Option with the default set as the value passed in
 */
template <class T>
Option<T>::Option(T val){
    defaultVal = val;
    currVal = val;
}

template <class T>
void Option<T>::resetToDefault(){
    currVal = defaultVal;
}

/*
 * Returns the type of the current value stored in this option
 * NOTE: This is sometimes not in a user friendly format
 */
template <class T>
std::string Option<T>::getOptionType(){
    return typeid(this->currVal).name();
}

//*********************************************//
// ANY OPTION TYPES WE NEED MUST BE ADDED HERE //
//*********************************************//

template class Option<bool>;
template class Option<int>;