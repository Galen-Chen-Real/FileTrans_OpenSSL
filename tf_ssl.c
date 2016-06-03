/*
 *       'tf_ssl.c'
 *    (C) 2014.4  GordonChen
 *
 */
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "tf_ssl.h"

static SSL_CTX *ctx;
static SSL *ssl;

static void *open_ssl_init(void)
{
    /* SSL 库初始化 */
    SSL_library_init();
    /* 载入所有 SSL 算法 */
    OpenSSL_add_all_algorithms();
    /* 载入所有 SSL 错误消息 */
    SSL_load_error_strings();
    /* 以 SSL V2 和 V3 标准兼容方式产生一个 SSL_CTX ，即 SSL Content Text */
    ctx = SSL_CTX_new(SSLv23_server_method());
    /* 也可以用 SSLv2_server_method() 或 SSLv3_server_method() 单独表示 V2 或 V3标准 */
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    return ctx;
}

void *open_ssl_ser(int sockfd)
{
    char pwd[100];
    char *temp;

    /* SSL 库初始化 */
    if(!open_ssl_init())
    {
        return NULL;
    }
    /* 载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
    getcwd(pwd,100);
    if(strlen(pwd)==1)
    pwd[0]='\0';
    if (SSL_CTX_use_certificate_file(ctx, temp=strcat(pwd,"/cacert.pem"), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    /* 载入用户私钥 */
    getcwd(pwd,100);
    if(strlen(pwd)==1)
    pwd[0]='\0';
    if (SSL_CTX_use_PrivateKey_file(ctx, temp=strcat(pwd,"/privkey.pem"), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    /* 检查用户私钥是否正确 */
    if (!SSL_CTX_check_private_key(ctx))
    {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    /* 基于 ctx 产生一个新的 SSL */
    ssl = SSL_new(ctx);
    /* 将连接用户的 socket 加入到 SSL */
    SSL_set_fd(ssl, sockfd);
    /* 建立 SSL 连接 */
    if (SSL_accept(ssl) == -1)
    {
        perror("accept");
        return NULL;
    }

    return ssl;
}

void *open_ssl_cli(int sockfd)
{
    /* SSL 库初始化 */
    if(!open_ssl_init())
    {
        return NULL;
    }
    
    /* 基于 ctx 产生一个新的 SSL */
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    
    /* 建立 SSL 连接 */
    if (SSL_connect(ssl) == -1)
    {
        ERR_print_errors_fp(stderr);
        return NULL;
    }

    return ssl;
}

void close_ssl(void)
{
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
}

int ssl_send(const void *buf, int len)
{
    return SSL_write(ssl, buf, len);
}

int ssl_recv(void *buf, int len)
{
    return SSL_read(ssl, buf, len);
}


