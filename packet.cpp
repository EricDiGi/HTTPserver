#include "packet.hpp"
#include "auxilary.hpp"

packet_util::packet_util(char* buffer){
    this->in_buffer = (std::string)buffer;
    puff();
}

void packet_util::puff(){
    struct http_request packet;

    std::stringstream ss(this->in_buffer);

    std::string item;
    ss >> item;
    
    if(item == "GET"){ packet.method = GET; }
    else if(item == "POST"){ packet.method = POST; }
    else if(item == "HEAD"){ packet.method = HEAD; }
    else{ packet.method = UNKNOWN; }

    // "web_content" is root folder for files
    ss >> item;
    //if just url still send home page
    if(item == "/"){item = "/index.html";}
    packet.uri = "web_content"+item;

    ss >> item;
    packet.http_v = item;

    if(packet.method == POST){
        while(!item.empty()){ss >> item;}
        while(getline(ss,item)){
            packet.content.append("\n"+item);
        }
    }
    print_request(packet);
    packet_builder(packet);
}

void packet_util::packet_builder(struct http_request packet){
    if(!fileExists(packet.uri)){
        std::string cont = "<html><body><h1>404 Page does not Exist</h1></body></html>";
        fill("",NOT_FOUND,cont);
    }
    else{
        if(pathAuthorized(packet.uri)){
            fill(packet.uri, OK, getContent(packet.uri));
        }
        else{
            std::string cont = "<html><body><h1>401 Page is not Authorized</h1></body></html>";
            fill("", UNAUTH, "");
        }
    }
}

void packet_util::fill(std::string uri, int code, std::string content){
    this->pkg.response_code = code;
    this->pkg.http_v = "HTTP/1.1";
    this->pkg.response_def = getCode(code);
    this->pkg.content_length = (int)content.size();
    this->pkg.content_type = getMIME(uri);
    this->pkg.content = content;
}

void packet_util::print_request(struct http_request req){
    printf("\n>>> REQUEST PACKET\nMethod: %d\nURI: %s\nHTTP version: %s\nContent: %s\n\n",req.method, (char*)req.uri.c_str(), (char*)req.http_v.c_str(), (char*)req.content.c_str());
}

std::string packet_util::http_response(){
    std::stringstream ss;
    if(pkg.content_type.substr(0,5)=="image"){
        std::cout << ">>> RESPONSE PACKET\n";
        std::cout << pkg.http_v << " " << pkg.response_code << " " << pkg.response_def << "\nContent-Type: " << pkg.content_type << "\nContent-Length: " << pkg.content_length << "\nContent:\n\nIMAGE DATA\n";
    }else{
        std::cout << ">>> RESPONSE PACKET\n";
        std::cout << pkg.http_v << " " << pkg.response_code << " " << pkg.response_def << "\nContent-Type: " << pkg.content_type << "\nContent-Length: " << pkg.content_length << "\nContent:\n\n" << pkg.content << std::endl;
    }
    ss << pkg.http_v << " " << pkg.response_code << " " << pkg.response_def << "\nContent-Type: " << pkg.content_type << "\nContent-Length: " << pkg.content_length << "\nContent:\n\n" << pkg.content;
    return ss.str();
}