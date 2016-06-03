/*
 *       'client.c'
 *    (C) 2014.4  GordonChen
 *
 *    文件传输客户端:可传输文件给服务器. 在'tf_config.h'中定义'SSL'可启用SSL协议的安全
 * 文件传输.
 *    Usage: ./client ip port
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "tf_socket.h"
#include "tf_trans.h"
#include "tf_config.h"

#ifdef SSL
#include "tf_ssl.h"
#endif

extern int  open_sock_cli(char *ip, int port);

int main(int argc, char *argv[])
{
    int port, sockfd;
    char pathname[MAXPATH];
    
    if(argc != 3)
    {
        printf("Usage: %s ip port\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[2]);
    
    //开启socket
    if((sockfd = open_sock_cli(argv[1], port)) < 0)
    {
        printf("open_sock_cli() failed!\n");
        exit(1);
    }
    
#ifdef SSL 
    //开启SSL协议
    if(open_ssl_cli(sockfd) == NULL)
    {
        printf("open_ssl_cli() failed!\n");
        exit(1);
    }
#endif 
   
    //发送文件
    bzero(pathname, MAXPATH);
    printf("Please input pathname:");  //很奇怪,这里并不支持相对路径！
    scanf("%s", pathname);
    if(trans_send(pathname))
    {
        printf("trans_send() failed!\n");
        exit(1);
    }
    
    //输出发送出文件名
    printf("Sendfile: %s successed!\n", pathname);
    
    //关闭客户端
    close_sock_cli();
    
#ifdef SSL
    //关闭SSL协议
    close_ssl();
#endif
    
    return 0;
}

