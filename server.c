/*
 *       'server.c'
 *    (C) 2014.4  GordonChen
 *
 *    文件传输服务器:可接受多个用户同时传输文件给服务器. 在'tf_config.h'中定义'SSL'可启用
 * SSL协议的安全文件传输.
 *    Usage: ./server ip port
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "tf_socket.h"
#include "tf_trans.h"
#include "tf_config.h"

#ifdef SSH
#include "tf_ssl.h"
#endif

//接收到的文件存储路径
const char dpath[] = "./";

int main(int argc, char *argv[])
{
    int port;
    int sockfd;
    char path[MAXPATH], *recvf;
    
    if(argc != 3)
    {
        printf("Usage: %s ip port\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[2]);

    //开启socket
    if((sockfd = open_sock_ser(argv[1], port)) < 0)
    {
        printf("open_sock_ser() failed!\n");
        exit(1);
    }
    
#ifdef SSL
    //开启SSL协议
    if(open_ssl_ser(sockfd) == NULL)
    {
        printf("open_ssl_ser() failed!\n");
        exit(1);
    }
#endif
    
    //接收文件
    bzero(path, MAXPATH);
    strcpy(path, dpath);
    if((recvf = trans_recv(path)) == NULL)
    {
        printf("trans_recv() failed!\n");
        exit(1);
    }
    
    //输出接收到文件名
    printf("Recvfile: %s successed!\n", recvf);
    
    //关闭服务器
    close_sock_ser();
    
#ifdef SSL
    //关闭SSL协议
    close_ssl();
#endif
    
    return 0;
}

