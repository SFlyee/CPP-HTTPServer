#include <string>
#include <sys/types.h>
#include <sys/epoll.h>
namespace Myserver{
class Socket{
public:
    Socket(std::string,int,bool=true);
	~Socket();
	bool bind(std::string,int);
	bool listen(int);
	int accept();
    int file_size(std::string);
    bool transfer(int,std::string);
    std::string file_to_string(std::string);
    inline int fd(){return m_socketfd;};
protected:
    std::string m_ip;
    int m_port;
    int m_socketfd;
};

class Epoll{
public:
    Epoll(int);
    ~Epoll();
    int e_ctl(int,int,uint32_t);
    int e_wait(int);
    inline struct epoll_event &events(int i){return ep_events[i];}; 
protected:
    int epfd;
    int event_num;
    struct epoll_event ep_event;
    struct epoll_event *ep_events;
};

}
