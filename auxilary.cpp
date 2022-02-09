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
    std::ifstream f;
    if(doBinary(f_name))
        f.open(f_name, std::ios::out | std::ios::binary);
    else
        f.open(f_name);
    std::stringstream ss;
    ss << f.rdbuf();
    std::string o = ss.str();
    f.close();
    return o;
}

std::string getMIME(std::string f_name){
    std::string ext;
    int iter = 0;
    for(auto &ch : f_name){
        if(ch == '.'){
            ext = f_name.substr(iter+1,(int)f_name.size()-iter);
            break;
        }
        iter++;
    }
    if(ext.empty()){return "text/plain";}
    else{
        if(doBinary(f_name))
            return "image/"+ext;
        return "text/"+ext;
    }
}
std::string getCode(int code){
    return code_desc[(code - (code%100))/100][code%100];
}

bool doBinary(std::string f_name){
    std::string ext;
    int iter = 0;
    for(auto &ch : f_name){
        if(ch == '.'){
            ext = f_name.substr(iter+1,(int)f_name.size()-iter);
            break;
        }
        iter++;
    }
    std::string exts[7] = {"jpg","jpeg", "bmp", "png","tif","tiff","webp"};
    for(auto &s : exts){
        if(ext == s){return true;}
    }
    return false;
}