#ifndef _CmdSOCKET_H_
#define _CmdSOCKET_H_

#include <limits.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <nerccmd.h>

#define USING_INET_SOCKET

#ifdef USING_UNIX_DOMAIN_SOCKET
#define NERC_CMD_SOCKET_PATH "/dev/nerccmd_socket"
#define NERC_CMD_SOCKET_MODE (0666)
#elif defined(USING_INET_SOCKET)
#define NERCCMD_INET_SOCKET_PORT (9777)
#endif


#define MAX_BUFF_SIZE (1024)

class CmdSocket {
public: 
    CmdSocket();
    ~CmdSocket();

public:
    //get socket fd
    int get_service_fd() { return serverFd;}

static int send_data(int fd, const char * buffer, int max_len);
static int receive_data(int fd, const char * buffer, int max_len);

protected:
    int serverFd;  //server socket FD
    char serverPath[NAME_MAX] = {0,};
};

class CmdSocketServer : public CmdSocket {
public:
    CmdSocketServer(){serverFd = -1;}
    ~CmdSocketServer();

public:
    int service_listen();

    int processing(int flag);
    void set_handler(CommandProcessorFuncType func) {mhandler = func;}

protected:
    CommandProcessorFuncType mhandler;
};

class CmdSocketClient : public CmdSocket {
public:
    //connect to server
    int connect_server(const char * path);

protected:
    int clientFd;

};
#endif
