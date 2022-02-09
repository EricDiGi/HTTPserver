#include "auxilary.hpp"

std::string code_desc[5][5] = {
    {""},{""},
    {"OK"}, {""},
    {"BAD REQUEST", "UNAUTHORIZED","PAYMENT REQUIRED" "FORBIDDEN","NOT FOUND"}
};

bool fileExists(std::string f_name){
    std::ifstream f(f_name);
    if(f.good()){
        f.close();
        return true;
    }
    f.close();
    return false;
}

bool pathAuthorized(std::string path){
    if(path == "../"){return false;}
    //if path not in ignore list return true;
    return true;
}

std::string getContent(std::string f_name){
    std::ifstream f(f_name);
    std::stringstream ss;
    ss << f.rdbuf();
    std::string o = ss.str();
    f.close();
    return o;
}

std::string getMIME(std::string f_name){
    return "text/html";
}
std::string getCode(int code){
    return code_desc[(code - (code%100))/100][code%100];
}