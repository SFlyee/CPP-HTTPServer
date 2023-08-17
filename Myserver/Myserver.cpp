#include <iostream>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "Socket/Socket.hpp"
#include "HTTPServer/HTTPServer.hpp"
#define MAX_LISTEN 9999 // 设置最大监听个数
int main()
{
	Myserver::Socket sock_server("", 6379);
	Myserver::HTTPserver http_server("./html_project","Zhao's httpserver");
	Myserver::Epoll ep(MAX_LISTEN);
	sock_server.listen(MAX_LISTEN);
	ep.e_ctl(EPOLL_CTL_ADD, sock_server.fd(), EPOLLIN);
	int numEvents=0;
	while(1){
		// 等待事件发生
		numEvents = ep.e_wait(-1);
		for(int i=0;i<numEvents;i++){
			if(ep.events(i).data.fd==sock_server.fd()){
				int clnt_sock = sock_server.accept();
				// 将客户端套接字添加到 epoll 实例中
				ep.e_ctl(EPOLL_CTL_ADD, clnt_sock, EPOLLIN | EPOLLET);
			}else{
				int clnt_sock = ep.events(i).data.fd;
				Myserver::HTTPserver::content_info info = http_server.resolver(http_server.receive(clnt_sock));
				std::cout << "Request content: "<< "method:" << info.method << " path:" << info.path << " type:" << info.type << std::endl;
				if (info.status)
				{
					int size = sock_server.file_to_string(info.path).size();
					http_server.response(clnt_sock, info.type, size, sock_server.file_to_string(info.path));
					sock_server.transfer(clnt_sock, "\r\n\r\n");
				}
				if(info.method=="GET")close(clnt_sock);
			}
		}
	}	
	return 0;
}