#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <nerccmd.h>

#include <signal.h>
#include <sys/time.h>
#include <CmdSocket.h>


int isExiting = 0;
CmdSocketServer * server {NULL};


static void nerccmd_signal_handler(int signal) {
    switch(signal) {
        case SIGPIPE:   //important, ignore broken pipe
            NERC_LOGE("nerccmd Received signal SIGPIPE\n");
            break;
        default:        //other singals
            NERC_LOGE("nerccmd Received signal 0x%X, exiting...\n", signal);
            break;      //exit
    }
    if (NULL != server) {
        delete server;
    }
    exit(0);
}

static int commandProcessor(const char * str, char* const outData, int maxBuffSize) {
    NERC_LOGD("nerccmd process command >>> %s", str);
    pid_t status;
    int ret;

    {
        //TODO check str
        status = system(str);
        if (-1 == status) {
            //NERC_LOGD("nerccmd system() error <<< %d", status);
            ret = status;
        } else {
            //NERC_LOGD("nerccmd system() exit value=[0x%x]", status);
            ret = WEXITSTATUS(status);
            if (0 != ret){
                NERC_LOGD("nerccmd run script fail, script exit code: %d", ret);
            }
        }
    }
    return ret;
}

static int startCmdServer() {
    server = new CmdSocketServer();
    server->set_handler(commandProcessor);
    if (server->service_listen() < 0) {
        NERC_LOGE("nerccmd service listen() failed");
        return -1;
    }

    server->processing(0);
    return 0;
}

int main(int argc, char * argv[]) {
    if (argc >= 2) {
        NERC_LOGI("nerccmd cmd: %s", argv[1]);
    }
    NERC_LOGD("nerccmd enter");
    if(system("fuser -v -n tcp 9777 -k")){
        NERC_LOGW("kill port 9777 failed.");
    }
    usleep(512*1024);
    signal(SIGPIPE, nerccmd_signal_handler);

    startCmdServer();

    NERC_LOGD("nerccmd exit");
    return 0;
}
