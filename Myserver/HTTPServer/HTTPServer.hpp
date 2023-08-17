#include <string>
#include <map>
namespace Myserver{
class HTTPserver{
public:
    struct content_info{
        std::string type;
        std::string path;
        std::string method;
        bool status;
    };
    HTTPserver(std::string,std::string);//设置html所在文件夹路径,定义服务器名称
    void response(int,std::string,int,std::string="");
    std::string receive(int);
    content_info resolver(std::string);
    bool set_html_path(std::string);
protected:
    std::string html_path; 
    std::string server_name;
    std::map<std::string,std::string> mtype;
};



}