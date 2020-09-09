//
// Created by 迹忆 on 2020/9/7.
//

#ifndef JLOG_JLOG_STORAGE_H
#define JLOG_JLOG_STORAGE_H

#define NODE_NUM         1024   // 元素个数
#define NODE_MIN_SIZE    (10*1024)  // 节点最小内存  10K
#define STORAGE_SIZE        (32*1024*1024)  // 分配的内存  默认 32MB
#define FILE_NAME_LEN     128


#define PHP_USER_ALLOC      emalloc
#define PHP_USER_FREE       efree

typedef struct _log_storage_mm {
    unsigned long mm_size;
    char *p;
} log_storage_node;

typedef struct _log_node_val {
    unsigned int len;
    unsigned int size;
    unsigned int fsize;
    char fname[FILE_NAME_LEN];
    char data[1];
} log_node_val;

typedef struct _log_node{
    unsigned int log_type;
    log_node_val val;
} log_node;

typedef struct _queue_list {
    unsigned int used;
    unsigned int read;
    unsigned int slots;
    unsigned int write;
    unsigned int node_size;
    log_storage_node *nodes;
    log_storage_node first_node;
} queue_list;

extern queue_list *jlog_queue;

extern pthread_mutex_t mutex;

int queue_init(void);
void outNode(log_node **data);
int putNode(char * data, char *file_name,unsigned int size, unsigned int fsize, unsigned int log_type);
int checkQueueIsEmpty();




#define JLOG_VSG(element) (jlog_queue->element)
#endif //JLOG_JLOG_STORAGE_H
