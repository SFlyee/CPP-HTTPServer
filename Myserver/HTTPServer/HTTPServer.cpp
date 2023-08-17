#include "HTTPServer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>

using namespace Myserver;
HTTPserver::HTTPserver(std::string pth,std::string name):html_path(pth),server_name(name){
    mtype["html"]="text/html";
    mtype["css"]="text/css";
    mtype["jpg"]="image/jpeg";
    mtype["png"]="image/png";
    mtype["gif"]="image/gif";
    mtype["json"]="application/json";
    mtype[""]="text/html";
}
void HTTPserver::response(int clnt_sock,std::string content_type,int content_size,std::string file){
    std::string buff;
	buff = "HTTP/1.1 200 OK\r\n"
           "Server: "+server_name+"\r\n"
           "Content-Size:"+std::to_string(content_size)+"\r\n"
           "Content-Type: "+content_type+"\r\n"
           "\r\n"+file;
	send(clnt_sock,buff.c_str(),buff.size(),0);
    send(clnt_sock,"\r\n",3,0);
}

std::string HTTPserver::receive(int clnt_sock){
    char buff[1024]={0};
    recv(clnt_sock,buff,sizeof(buff),0);
    if(buff==""){
        std::cout<<"receive error!"<<std::endl;
        std::cout<<"errpo sock is:"<<clnt_sock<<std::endl;
    }
    return buff;
}

HTTPserver::content_info HTTPserver::resolver(std::string request){
    content_info info;
    size_t start = request.find(' ') + 1;
    info.method = request.substr(0,start-1);
	size_t end = request.find(' ', start);
	info.path = html_path+request.substr(start, end - start);
    size_t tsta = info.path.find('.',1)+1;
    info.type=mtype[info.path.substr(tsta,info.path.size()-tsta)];
    if(info.method == "GET" && info.path==html_path+"/"){
        info.path=html_path+"/test.html";
        info.type=mtype[""];
    }
    if(info.path.size()==1)
        info.status=false;
    else 
        info.status=true;
    return info;
}
bool HTTPserver::set_html_path(std::string tph){
    html_path=tph;
    return true;
}
