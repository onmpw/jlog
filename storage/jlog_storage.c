//
// Created by 迹忆 on 2020/9/7.
//
#include <sys/mman.h>
#include <pthread.h>
#include "php.h"
#include "jlog_storage.h"

queue_list *jlog_queue;
pthread_mutex_t mutex;
int queue_init(void)
{
    log_storage_node first_node;
    unsigned int node_num = NODE_NUM, node_size;
    unsigned long storage_size = STORAGE_SIZE;
    int i;

    while((storage_size/node_num) < NODE_MIN_SIZE) {
        node_num >>= 1;
    }

    node_size = storage_size/node_num;

    // 申请存放数据的内存
    first_node.p = mmap(0,storage_size,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    if(first_node.p == MAP_FAILED) {
        return 0;
    }

    jlog_queue = (queue_list *)calloc(1,sizeof(queue_list)+sizeof(log_storage_node)*node_num);

    JLOG_VSG(first_node).p = first_node.p;
    JLOG_VSG(used) = 0;
    JLOG_VSG(read) = JLOG_VSG(write) = 0;
    JLOG_VSG(slots) = node_num;
    JLOG_VSG(node_size) = node_size - sizeof(unsigned long);

    JLOG_VSG(nodes) = (log_storage_node *)((char *)jlog_queue + sizeof(queue_list));

    // todo 这里会有一个问题，就是内存中的每一个节点头部都会被占用 sizeof(unsigned long) + sizeof(unsigned int) 大小的空间 用来存储当前节点中的内容的大小
    for(i = 0; i < node_num; i++) {
        JLOG_VSG(nodes)[i].p = (void *)((char *)(JLOG_VSG(first_node).p) + node_size*i);
        JLOG_VSG(nodes)[i].mm_size = 0;
    }

    return 1;
}

//检测队列是否为空
int checkQueueIsEmpty()
{
    return JLOG_VSG(used) == 0 ? 1 : 0;
}

void outNode(log_node **data)
{
    log_node *node;
    log_storage_node *sn;

    if(JLOG_VSG(read) == JLOG_VSG(slots)) {
        JLOG_VSG(read) = 0;
    }

    sn = &(JLOG_VSG(nodes)[JLOG_VSG(read)]);

    node = (log_node *) sn->p;

    memcpy((char *)*data,(char *)node,sn->mm_size);

    JLOG_VSG(read) +=1;
    pthread_mutex_lock(&mutex);
    JLOG_VSG(used) -= 1;
    pthread_mutex_unlock(&mutex);
}

int putNode(char * data, char *file_name, unsigned int size, unsigned int fsize, unsigned int log_type)
{
    log_node *val;
    log_storage_node *node;
    int timer = 0;

    while(1) {
        if (JLOG_VSG(used) >= JLOG_VSG(slots)) {
            sleep(1);
            timer++;
            if(timer == 5) {
                return 0;
            }
            continue;
        }

        if (size > JLOG_VSG(node_size)- sizeof(unsigned int)*4 - FILE_NAME_LEN) {
            return 0;
        }

        val = PHP_USER_ALLOC(sizeof(log_node) - 4 + size);

        val->log_type = log_type;
        val->val.len = strlen(data);
        val->val.size = size;
        val->val.fsize = fsize;
        memcpy(val->val.fname,file_name,fsize);
        val->val.fname[fsize] = '\0'; // 增加结束符号
        memcpy(val->val.data, data, size);

        // 加入队列
        // 如果队列空了， 则读的位置和写的位置必然重合
        // 但是 由于是多线程兵法，used 等于0 并不一定是队列空了，所以还要加上 读位置与写位置重合
        if (JLOG_VSG(used) == 0 && JLOG_VSG(write) == JLOG_VSG(read)) {
            JLOG_VSG(write) = 0;
            JLOG_VSG(read) = 0;
        }

        if (JLOG_VSG(write) == JLOG_VSG(slots)) {
            JLOG_VSG(write) = 0;
        }

        node = &(JLOG_VSG(nodes)[JLOG_VSG(write)]);
        node->mm_size = sizeof(log_node) - 4 +val->val.size;

        memcpy((char *)node->p,(char *)val,sizeof(log_node) -4 + size);

        PHP_USER_FREE(val);
        JLOG_VSG(write) += 1;
        pthread_mutex_lock(&mutex);
        JLOG_VSG(used) += 1;
        pthread_mutex_unlock(&mutex);
        return 1;
    }
}

