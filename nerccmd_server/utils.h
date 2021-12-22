#ifndef NERCCMD_UTILS_H
#define NERCCMD_UTILS_H
#ifdef __ANDROID__
#include <utils/Log.h>
#endif

#ifdef __ANDROID__
#define NERC_LOGI ALOGI
#define NERC_LOGD ALOGD
#define NERC_LOGE ALOGE
#define NERC_LOGV ALOGV
#define NERC_LOGW ALOGW
#else
#define NERC_LOGI(str, ...) do{printf(str,##__VA_ARGS__);printf("\n");}while(0)
#define NERC_LOGD(str, ...) do{printf(str,##__VA_ARGS__);printf("\n");}while(0)
#define NERC_LOGV(str, ...) do{printf(str,##__VA_ARGS__);printf("\n");}while(0)
#define NERC_LOGE(str, ...) do{printf(str,##__VA_ARGS__);printf("\n");}while(0)
#define NERC_LOGW(str, ...) do{printf(str,##__VA_ARGS__);printf("\n");}while(0)
#endif

#define MAX_CMD_LEN (1024)

void str2Argv(char** argv, const char* str, int maxLines);

#endif