#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<iostream>

using namespace std;

int main()
{
    //创建监听的套接字
    int fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(fd==-1)
    {
        perror("socket");
        return -1;
    }

    //绑定本地的IP port
    sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(8888);
    saddr.sin_addr.s_addr=INADDR_ANY;    //0=0.0.0.0
    int ret=bind(fd,(sockaddr*)&saddr,sizeof(saddr));
    if(ret==-1)
    {
        perror("bind");
        return -1;
    }

    //设置监听
    ret=listen(fd,128);
    if(ret==-1)
    {
        perror("listen");
        return -1;
    }

    //阻塞并等待客户端的连接
    sockaddr_in caddr;
    int addrlen=sizeof(sockaddr_in);
    int cfd=accept(fd,(sockaddr*)&caddr,(socklen_t*)&addrlen);
    if(cfd==-1)
    {
        perror("accept");
        return -1;
    }

    //连接建立成功，打印客户端的IP和端口信息
    char ip[32];
    cout<<"客户端的IP "<<inet_ntop(AF_INET,&caddr.sin_addr.s_addr,ip,sizeof(ip))<<" 端口: "<<ntohs(caddr.sin_port)<<endl;

    //通信

    while(1)
    {
        char buff[1024];
        int len=recv(cfd,buff,sizeof(buff),0);
        if(len>0)
        {
            cout<<"client say: "<<buff;
            send(cfd,buff,len,0);
        }
        else if(len==0)
        {
            cout<<"客户端已经断开了连接..."<<endl;
            break;
        }
        else
        {
            perror("recv");
            break;
        }
    }

    //关闭文件描述符
    close(fd);
    close(cfd);
    return 0;
}