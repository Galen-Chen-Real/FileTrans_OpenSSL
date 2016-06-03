/*
 *       'tf_trans.h'
 *    (C) 2014.4  GordonChen
 *
 */
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "tf_trans.h"

#ifdef SSL
#include "tf_ssl.h"
#else
#include "tf_socket.h"
#endif

int tf_send(const void *buf, int len)
{
#ifdef SSL
    return ssl_send(buf, len);
#else
    return tcp_send(buf, len);
#endif
}

int tf_recv(void *buf, int len)
{
#ifdef SSL
    return ssl_recv(buf, len);
#else
    return tcp_recv(buf, len);
#endif 
}

int trans_send(char *pathname)
{
    int i, j, len, fd, size;
    char fileName[MAXPATH], sendFN[MAXNAME];
    char buffer[MAXBUF + 1];
    int fedbck;
    
    strcpy(fileName, pathname);

    /* 接收用户输入的文件名，并打开文件 */
    if((fd = open(fileName,O_RDONLY))<0)
    {
        printf("fileName: %s\n", fileName);
        perror("open:");
        return -1;
    }

    /* 将用户输入的文件名，去掉路径信息后，发给对方 */
    for(i = 0; i <= strlen(fileName); i++)
    {
        if(fileName[i] == '/')
        {
            j = 0;
            continue;
        }
        else 
        {
            sendFN[j] = fileName[i];
            ++j;
        }
    }
    len = tf_send(sendFN, strlen(sendFN));
    if (len < 0)
        printf("'%s'message Send failure ！Error code is %d，Error messages are '%s'\n", 
            buffer, errno, strerror(errno));
    /* 接收对方回复 */
    len = tf_recv(&fedbck, sizeof(fedbck));
    if(len < 0 || fedbck != RECVED)
    {
        printf("Something error!\n");
        return -1;
    }

    /* 循环发送文件内容到服务器 */
    bzero(buffer, MAXBUF + 1); 
    while((size=read(fd,buffer,1024)))
    {
        if(size<0)
        {
            perror("read:");
            return -1;
        }
        else
        {
            len = tf_send(buffer, size);
            if (len < 0)
                printf("'%s'message Send failure ！Error code is %d，Error messages are '%s'\n", 
                    buffer, errno, strerror(errno));
        }
        bzero(buffer, MAXBUF + 1);
        /* 接收对方回复 */
        len = tf_recv(&fedbck, sizeof(fedbck));
        if(len < 0 || fedbck != RECVED)
        {
            printf("Something error!\n");
            return -1;
        }
    }
    printf("Send complete !\n");

    /* 关闭连接 */
    close(fd);
    return 0;
}

char *trans_recv(char *path)
{
    int fd, len;
    char buf[MAXBUF + 1];
    static char new_fileName[MAXPATH];
    mode_t mode;
    char pwd[100];
    int fedbck;

    bzero(new_fileName, MAXPATH);
    strcpy(new_fileName, path);

    /* 接受客户端所传文件的文件名并在特定目录创建空文件 */
    bzero(buf, MAXBUF + 1);
    len = tf_recv(buf, MAXBUF);
    if(len < 0)
    {
        printf("Failure to receive message ! Error code is %d，Error messages are '%s'\n", 
	        errno, strerror(errno));
	    return NULL;
	}
    if((fd = open(strcat(new_fileName, buf), O_CREAT | O_TRUNC | O_RDWR, 0666)) < 0)
    {
        printf("new_fileName: %s\n", new_fileName);
        perror("open:");
        return NULL;
    }
    /* 告诉对方已收到 */
    fedbck = RECVED;
    len = tf_send(&fedbck, sizeof(fedbck));

    /* 接收客户端的数据并写入文件 */
    while(1)
    {
        bzero(buf, MAXBUF + 1);
        len = tf_recv(buf, MAXBUF);
        if(len == 0)
        {
            printf("Receive Complete !\n");
            break;
        }
        else if(len < 0)
        {
            printf("Failure to receive message ! Error code is %d，Error messages are '%s'\n", 
                errno, strerror(errno));
            return NULL;
        }
        if(write(fd,buf,len)<0)
        {
            perror("write:");
            return NULL;
        }
        /* 告诉对方已收到 */
        fedbck = RECVED;
        len = tf_send(&fedbck, sizeof(fedbck));
    }

    /* 关闭文件 */
    close(fd);

    return new_fileName;
}

