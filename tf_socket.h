/*
 *       'tf_socket.h'
 *    (C) 2014.4  GordonChen
 *
 */
#ifndef _TF_SOCKET_H
#define _TF_SOCKET_H

#include "tf_config.h"

#define LISNUM 4

//开启服务器并等待客户端连接成功;成功返回sockfd,失败返回-1.
int  open_sock_ser(char *ip, int port);
//开启客户端并连接服务器;成功返回sockfd,失败返回-1.
int  open_sock_cli(char *ip, int port);
//TCP发送数据
int  tcp_send(const void *buf, int len);
//TCP接收数据
int  tcp_recv(const void *buf, int len);
//关闭服务器
void close_sock_ser(void);
//关闭客户端
void close_sock_cli(void);

#endif

