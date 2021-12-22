#include "CmdSocket.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <thread>

#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_NERCCMD_LISTEN_CLIENT_Fd_NUM (5)

extern int isExiting;

CmdSocket::CmdSocket()
    : serverFd(-1) {NERC_LOGI("nerccmd CmdSocket Constructor");}

CmdSocket::~CmdSocket() {
    NERC_LOGI("nerccmd ~CmdSocket");
    if (serverFd > 0) {
        close(serverFd);
        serverFd = -1;
    }
}

CmdSocketServer::~CmdSocketServer() {}

int CmdSocketServer::service_listen() {
    int ret = 0;
#ifdef USING_UNIX_DOMAIN_SOCKET
    const char* path = NERC_CMD_SOCKET_PATH;
    unlink(path);  //remove old file
    serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un local;
    memset(&local, 0, sizeof(struct sockaddr_un));
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path,path);

#elif defined(USING_INET_SOCKET)
    serverFd = socket(AF_INET,SOCK_STREAM, 0);
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(NERCCMD_INET_SOCKET_PORT);
#endif
    int reuse = 1;
    ret = setsockopt( serverFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse) );

    int flags = 0;
    fcntl(serverFd, F_GETFL, &flags);
    fcntl(serverFd, F_SETFL, /*O_NONBLOCK |*/ flags);

    ret = bind(serverFd,(struct sockaddr*)&local, sizeof(local));
    if (ret < 0)  {
        NERC_LOGE("nerccmd bind() failed, serverFd=%d", serverFd);
        return ret;
    }

#ifdef USING_UNIX_DOMAIN_SOCKET
    ret = chmod(path, NERC_CMD_SOCKET_MODE);
#endif

    ret = listen(serverFd, MAX_NERCCMD_LISTEN_CLIENT_Fd_NUM);
    NERC_LOGD("nerccmd listen() Fd>> %d <<<  status=%d", serverFd, ret);
    if (ret < 0) {
        return ret;
    }
    return ret;
}

static int execOnce(int clientFd, void* handler) {
    int ret = 0, size = -1;
    static char recvBuf[MAX_CMD_LEN + 1];
    char* respBuf;
    memset(recvBuf, 0, sizeof(recvBuf));
    size = CmdSocket::receive_data(clientFd, recvBuf, MAX_CMD_LEN);
    if(size >= 0) {
        respBuf = (char*)malloc(MAX_BUFF_SIZE + 1);
        if(NULL == respBuf) return -3;
        memset(respBuf, 0, MAX_BUFF_SIZE + 1);
        ret = ((CommandProcessorFuncType)handler)(recvBuf, respBuf, MAX_BUFF_SIZE);
        if(strlen(respBuf) == 0){
            sprintf(respBuf,"%d", ret);  //use function ret as response data
        }
        if (strlen(respBuf) > 0 && strlen(respBuf) < 255) {
            NERC_LOGI("nerccmd response to FD:%d, str=%s", clientFd, respBuf);
        }
        CmdSocket::send_data(clientFd, respBuf, strlen(respBuf) );
        free(respBuf);
    } else if(size == -1) {
        NERC_LOGE("nerccmd Error[%d] when recieving Data:%s.\n", errno, strerror(errno));
    }
    close(clientFd);
    return ret;
}

int CmdSocketServer::processing(int flag) {
    int clientFd = -1;
    int res = 1;
    while (res && !isExiting) {
        clientFd = accept(get_service_fd(), NULL, NULL);
        if (clientFd > 0 && NULL != mhandler) {
            NERC_LOGD("nerccmd accept() fd:%d ,flag=%d", clientFd, flag);
            execOnce(clientFd, (void*)mhandler);
        }
        usleep(10*1024);
    }
    return (clientFd);
}

int CmdSocket::send_data(int fd, const char* buffer, int len) {
    int ret;
    ret = send(fd, buffer, len, 0);
    if (ret < 0) {
        NERC_LOGE("nerccmd send() failed");
        close(fd);
    }
    return ret;
}

int CmdSocket::receive_data(int fd, const char * buffer, int max_len) {
    return recv(fd, (void*)buffer, max_len, 0);
}