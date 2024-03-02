//
// Created by Filipe Correia on 02/03/2024.
//

#ifndef WM_PARSER_H
#define WM_PARSER_H
#include <string>

class Graph;
class Parser {
Graph* parse(const std::string& reservoirFile, const std::string& stationFile,
const std::string& cityFile, const std::string& pipeFile);
};


#endif //WM_PARSER_H
