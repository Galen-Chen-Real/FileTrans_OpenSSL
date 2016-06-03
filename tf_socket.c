/*
 *       'tf_socket.c'
 *    (C) 2014.4  GordonChen
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "tf_socket.h"

static int sockfd;

int open_sock_ser(char *ip, int port)
{
    int fd;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int lisnum = LISNUM;
  
    /* 开启一个 socket 监听 */
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return -1;
    }

    bzero((void *)&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = port;
    my_addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(fd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        return -1;
    }

    if (listen(fd, lisnum) == -1)
    {
        perror("listen");
        return -1;
    }

    while(1)
    {
        len = sizeof(struct sockaddr);
        /* 等待客户端连上来 */
        if ((sockfd = accept(fd, (struct sockaddr *) &their_addr, &len)) == -1)
        {
            perror("accept");
            continue;
        }
        
        if(fork() == 0)
        {
            return sockfd;
        }
    }
}

int open_sock_cli(char *ip, int port)
{
    int len, fd, size;
    struct sockaddr_in dest;

    /* 创建一个 socket 用于 tcp 通信 */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket");
        return -1;
    }

    /* 初始化服务器端（对方）的地址和端口信息 */
    bzero((void *)&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = port;
    if (inet_aton(ip, (struct in_addr *) &dest.sin_addr.s_addr) == 0)
    {
        perror(ip);
        return -1;
    }

     /* 连接服务器 */
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0)
    {
        perror("Connect ");
        return -1;
    }
    
    return sockfd;
}

int  tcp_send(const void *buf, int len)
{
    return send(sockfd, buf, len, 0);
}

int  tcp_recv(void *buf, int len)
{
    return recv(sockfd, buf, len, 0);
}

void close_sock_ser(void)
{
    /* 关闭监听的 socket */
    close(sockfd);
}

void close_sock_cli(void)
{
    /* 关闭监听的 socket */
    close(sockfd);
}


