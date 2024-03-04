//
// Created by Filipe Correia on 02/03/2024.
//

#ifndef WM_PARSER_H
#define WM_PARSER_H
#include <string>

class Graph;
class Parser {
public:
Graph* parse(const std::string& reservoirFilePath, const std::string& stationFilePath,
const std::string& cityFilePath, const std::string& pipeFilePath);
};


#endif //WM_PARSER_H
