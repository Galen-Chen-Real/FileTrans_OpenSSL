/*
 *       'tf_trans.h'
 *    (C) 2014.4  GordonChen
 *
 */
#ifndef _TF_TRANS_H
#define _TF_TRANS_H

#include "tf_config.h"

#define MAXBUF 1024
#define MAXPATH 64
#define MAXNAME 32

//用于fedbck
#define RECVED 01
#define NOREVD 02
#define SUCCES 04

#ifdef THREAD
//使用多线程传输的单元
typedef struct
{
    //线程编号
    int thread_no;
    //数据
    char buf[MAXBUF];
}TRANS;
#endif

//发送文件;成功返回0,失败返回-1.
int trans_send(char *pathname);
//接收文件;成功返回接收文件名,失败返回NULL.
char *trans_recv(char *path);

#endif

