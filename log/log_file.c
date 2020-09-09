//
// Created by 刘汉增 on 2020-09-09.
//
#include <stdio.h>
#include "log_file.h"

int log_write(char *file,char *data, int log_type, int *errNo)
{
    FILE *fptr;
    int wlen;

    fptr = fopen(file,"a+");

    if(file == NULL) {
        errNo = 1;  // 表示文件打开失败  file not exist
        return 0;
    }

    wlen = fputs(data,fptr);

    fclose(fptr);

    return wlen;
}

