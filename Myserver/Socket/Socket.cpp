#include "Socket.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
using namespace Myserver;
Socket::Socket(std::string ip,int port,bool auto_bind):m_ip(ip),m_port(port){
    m_socketfd = ::socket(PF_INET, SOCK_STREAM, 0);
    if(m_socketfd==-1){
        std::cout << "socket error!" << std::endl;
        exit(0);
    }
    if(auto_bind)
        Socket::bind(m_ip.c_str(),m_port);
}
Socket::~Socket(){
    ::close(m_socketfd);
}
bool Socket::bind(std::string ip,int port){
    struct sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
    if(ip=="")
	    addr_in.sin_addr.s_addr = htons(INADDR_ANY);
	else 
        addr_in.sin_addr.s_addr = htons(inet_addr(ip.c_str()));
    if (::bind(m_socketfd, (struct sockaddr *)&addr_in, sizeof(addr_in)) == -1){
		std::cout << "addr error" << std::endl;
        exit(0);
    }
    return true;
}
bool Socket::listen(int lnum){
    if (::listen(m_socketfd,lnum) == -1){
	    std::cout << "listen error" << std::endl;
        exit(0);
    }
	std::cout << "Listening in progress ..." << std::endl;
    return true;
}
int Socket::accept(){
    int clnt_sock;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_len = sizeof(clnt_addr);
    clnt_sock = ::accept(m_socketfd, (struct sockaddr *)&clnt_addr, &clnt_len);
    if(clnt_sock==-1){
        std::cout << "accept error" << std::endl;
        return -1;
    }
    printf("%s正在请求服务!\n",inet_ntoa(clnt_addr.sin_addr));
    return clnt_sock;
}
int Socket::file_size(std::string strFile){
    std::ifstream in(strFile, std::ios::in | std::ios::binary | std::ios::ate);  // 将本地图片读取到输入流
    in.seekg(0, in.end);
    unsigned int size = in.tellg();  // 计算读取的图片大小
    in.close();
    std::cout<<"Size is :"<<size<<std::endl;
    return size;
}

std::string Socket::file_to_string(std::string path){
    std::ifstream file(path, std::ios::binary);
    std::ostringstream bytesStream;
    bytesStream << file.rdbuf();
    std::string s = bytesStream.str();
    if(s == ""){
        std::cout<<"file_to_string error!"<<std::endl;
        //exit(0);
    }
    return s;
}

bool Socket::transfer(int clnt_sock,std::string s){
    send(clnt_sock,s.c_str(),s.size(),0);
    return true;
}


Epoll::Epoll(int num):event_num(num){
    epfd = epoll_create(10);
    if(epfd==-1){
        std::cerr<<"Failed to create epoll instance"<<std::endl;
        exit(0);
    }
    
    ep_events=new epoll_event[num];
}
Epoll::~Epoll(){
    close(epfd);
}
int Epoll::e_ctl(int op,int fd,uint32_t events){
    ep_event.events = events;
    ep_event.data.fd = fd;
    int status = epoll_ctl(epfd,op,fd,&ep_event);
    if (status == -1) {
        std::cerr << "Failed to add server socket to epoll" << std::endl;
        exit(0);
    }
    return status;
}
int Epoll::e_wait(int timeout){
    int num = epoll_wait(epfd,ep_events,event_num,timeout);
    if (num == -1)
	{
		std::cerr << "Failed to wait for events" << std::endl;
		return -1;
	}
    return num;
}