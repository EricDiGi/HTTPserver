#ifndef AUXILARY_H
#define AUXILARY_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string getCode(int);
bool doBinary(std::string);

//requires lock
bool fileExists(std::string);
bool pathAuthorized(std::string);
std::string getContent(std::string);
std::string getMIME(std::string);
#endif