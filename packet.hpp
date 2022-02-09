#ifndef PACKET_HPP
#define PACKET_HPP

#include <string>

enum pos {METHOD=11, HOST, AGENT, MIME, LANG, UNKNOWN};


class packet_util{
    private:
        std::string in_content;
        std::string out_content;
        void model();
        void fill_packet(std::string,std::string);
    public:
        packet_util(char* buffer);
        char* http_packet();
};

#endif