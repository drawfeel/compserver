#include <stdio.h>
#include "utils.h"

void str2Argv(char** argv, const char* str, int maxLines) {
    int cursor = 0, line = 0, j = 0;
    bool isStr = false;
    while ('\0' != str[cursor]) {
        if ('"' == str[cursor]) {
            isStr = !isStr;
        } else  if (!isStr && ' ' == str[cursor]) {
            if (0 != j) {
                argv[line][j] = '\0';
                if (line < maxLines - 1) {
                    line++;
                }
                else {
                    break;
                }
                j = 0;
            }
        } else {
            *((unsigned char *)(&argv[line][j])) = *((unsigned char *)(&str[cursor]));
            j++;
        }
        cursor++;
    }
    argv[line][j] = '\0';

    //for(int i = 0; i < maxLines; i ++)NERC_LOGI("nerccmd str2Argv(), arg[%d]=%s",i, argv[i]);
}