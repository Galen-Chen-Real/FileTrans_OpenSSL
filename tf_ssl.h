/*
 *       'tf_ssl.h'
 *    (C) 2014.4  GordonChen
 *
 */
#ifndef _TF_SSL_H
#define _TF_SSL_H

#include "tf_config.h"

//开启SSL协议服务器;成功返回非NULL,失败返回NULL.
void *open_ssl_ser(int sockfd);
//开启SSL协议客户端;成功返回非NULL,失败返回NULL.
void *open_ssl_cli(int sockfd);
//关闭SSL协议
void close_ssl(void);
//使用SSL协议发送数据;成功返回发送的字符数,失败返回-1.
int ssl_send(const void *buf, int len);
//使用SSL协议接收数据;成功返回接收的字符数,失败返回-1.
int ssl_recv(void *buf, int len);

#endif

