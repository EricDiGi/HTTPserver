#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include "packet.hpp"

packet_util::packet_util(char* buffer){
    this->in_content = (std::string)buffer;
    model();
}

void packet_util::model(){
    std::stringstream ss(this->in_content);
    std::string item;

    std::vector<int> tokens;
    std::vector<std::string> words;

    while(ss >> item){
        if(item == "GET"){
            tokens.insert(tokens.begin(),METHOD);
            words.insert(words.begin(), item);
        }
        else if (item == "Accept:"){
            tokens.insert(tokens.begin(),MIME);
            words.insert(words.begin(), item);
        }
        else{
            tokens.insert(tokens.begin(),UNKNOWN);
            words.insert(words.begin(), item);
        }
    }

    std::string req_file;
    std::string mime_type;
    int iter = 0;
    for(auto &tok : tokens){
        printf("%d\n", tok);
        if(tok == METHOD){
            req_file = words.at(iter-1);
            //printf("%s\n",(char*)req_file.c_str());
        }
        else if(tok == MIME){
            mime_type = words.at(iter-1);
            printf("%s\n",(char*)mime_type.c_str());
        }
        iter++;
    }
    req_file.erase(0,1);
    fill_packet(req_file, mime_type);
}

void packet_util::fill_packet(std::string file_n, std::string mime){
    std::ifstream in_f;
    if(doBinary(file_n))
        in_f.open(file_n, std::ios::out | std::ios::binary);
    else
        in_f.open(file_n);
    std::stringstream buff;
    buff << in_f.rdbuf();

    std::stringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html" /*<< mime*/ << "\nContent-Length: " << (int)buff.str().length() << "\n\n" << buff.str();

    this->out_content = ss.str();
    
    in_f.close();
}

char* packet_util::http_packet(){
    //std::cout << this->out_content << std::endl;
    return (char*)this->out_content.c_str();
}

bool packet_util::doBinary(std::string file_name){
    std::string exts[4] = {"jpg", "jpeg","png","ico"};
    std::string ext;
    int iter = 0;
    for(auto &ch : file_name){
        if(ch == '.')
            ext = file_name.substr(iter,(int)file_name.size()-iter);
        iter++;
    }
    for(auto &s : exts){
        if(strcmp((char*)s.c_str(),(char*)ext.c_str()) == 0)
            return true;
    }
    return false;
}