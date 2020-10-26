//
// Created by 迹忆 on 2020-09-09.
//
#include <stdio.h>
#include <time.h>
#include "log_file.h"

static void get_current_date_time(char *date_time)
{
    time_t time_ptr;
    struct tm *tmp_ptr = NULL;
    int year,month,day,hour,minute,sec;
    // 获取当前时间
    time(&time_ptr);
    tmp_ptr = localtime(&time_ptr);

    year = tmp_ptr->tm_year + 1900;
    month = tmp_ptr->tm_mon + 1;
    day = tmp_ptr->tm_mday;

    hour = tmp_ptr->tm_hour;
    minute = tmp_ptr->tm_min;
    sec = tmp_ptr->tm_sec;

//    sprintf(date_time,"[%d-%d%d-%d%d %d%d:%d%d:%d%d] ",year,month/10,month%10,day/10,day%10,hour/10,hour%10,minute/10,minute%10,sec/10,sec%10);
    sprintf(date_time,"[%d-%d%d-%d%d %d%d:%d%d:%d%d] ",year,month/10,month%10,day/10,day%10,hour/10,hour%10,minute/10,minute%10,sec/10,sec%10);
}

int log_write(char *file,char *data, int log_type, int *errNo)
{
    FILE *fptr;
    char formatDateTime[23];  // 存储时间 格式 [YYYY-mm-dd HH:ii:ss] 然后后面跟一个空格
    int wlen;

    fptr = fopen(file,"a+");

    if(fptr == NULL) {
        *errNo = 1;  // 表示文件打开失败  file not exist
        return 0;
    }

    get_current_date_time(formatDateTime);

    fputs(formatDateTime,fptr);
    wlen = fputs(data,fptr);
    fputc('\n',fptr);

    fclose(fptr);

    return wlen;
}

