//
// Created by paulp on 1/19/2019.
//

#include <algorithm>
#include <string>
#include <vector>
#include "enginetools.h"
#include "types.h"

const std::string EngineTools::EngineName = "ISU Chess Engine";
const std::string EngineTools::AuthorList = "Clinton Brown, Bryant Foster, Porter Glines, Paul Grahek";

std::string EngineTools::vectorToString(std::vector<std::string> strings){
    std::string retString = "";
    for(auto it = strings.begin(); it != strings.end();it++){
        retString += *it + " ";
    }
    return retString;
}

std::string EngineTools::stringToLower(std::string str){
    std::string retString = "";
    std::transform(str.begin(),str.end(),retString.begin(), ::tolower);
    return retString;
}

std::string EngineTools::boolToString(bool b){
    return b ? "true":"false";
}

std::string EngineTools::EngineInfo(EngineMode mode){
    std::string InfoString = "";

    if(mode == EM_UCI){
        InfoString += "id name ";
        InfoString += EngineName;
        InfoString += "\n";
        InfoString += "id authors ";
        InfoString += AuthorList;
        InfoString += "\n";
    }
    else if (mode == EM_CLI){

    }
    return InfoString;
}

std::string EngineTools::getEngineName(){
    return EngineName;
}

std::string EngineTools::getAuthorList(){
    return AuthorList;
}