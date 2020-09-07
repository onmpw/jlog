//
// Created by 迹忆 on 2020/9/7.
//

#ifndef JLOG_JLOG_STORAGE_H
#define JLOG_JLOG_STORAGE_H

typedef struct _log_storage_mm {
    unsigned long mm_size;
    void *p;
} log_storage_mm;

typedef struct _log_segment_val {
    unsigned int len;
    unsigned int size;
    char data[1];
} log_segment_val;

typedef struct _log_segment{
    unsigned int log_type;
    log_segment_val val;
} log_segment;

typedef struct _queue_list {
    unsigned int used;
    unsigned int front;
    unsigned int slots;
    unsigned int tail;
    log_storage_mm **segments;
    log_storage_mm first_segment;

} queue_list;

#endif //JLOG_JLOG_STORAGE_H
