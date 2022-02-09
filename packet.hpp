#ifndef PACKET_HPP
#define PACKET_HPP
#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>

enum pos {METHOD=11, HOST, AGENT, MIME, LANG, UNKNOWN=-1};
enum methods {GET=1000, HEAD, POST};
enum codes {OK=200, BADREQ=400, UNAUTH,PAYREQ, FORBIDDEN,NOT_FOUND};

struct http_packet{
    //Status line
    std::string http_v;
    int response_code;
    std::string reponsed_def;
    //Header
    std::string content_type;
    int content_length;
    //Buffer
    std::string content;
};

struct http_request{
    //Status Line
    int method;
    std::string uri;
    std::string http_v;
    //For Post Only
    std::string content;
};


class packet_util{
    private:
        struct http_packet pkg;
        std::string in_buffer;

        void puff();
        void packet_builder(struct http_request);
        void fill(std::string, int, std::string);
        void print_request(struct http_request);
    public:
        packet_util(char* buffer);
        //char* http_response();
};

#endif