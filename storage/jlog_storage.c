//
// Created by 迹忆 on 2020/9/7.
//
#include <sys/mman.h>
#include "jlog_storage.h"

queue_list *jlog_queue;
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
    ++node_num;  // 预留一个空节点

    // 申请存放数据的内存
    first_node.p = mmap(0,storage_size,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    if(first_node.p == MAP_FAILED) {
        return 0;
    }

    jlog_queue = (queue_list *)calloc(1,sizeof(queue_list)+sizeof(log_storage_node)*node_num);

    JLOG_VSG(first_node).p = first_node.p;
    JLOG_VSG(used) = 0;
    JLOG_VSG(front) = JLOG_VSG(tail) = 0;
    JLOG_VSG(slots) = node_num;

    JLOG_VSG(nodes) = (log_storage_node **)((char *)jlog_queue + sizeof(queue_list));

    // todo 这里会有一个问题，就是内存中的每一个节点头部都会被占用 sizeof(unsigned long) 大小的空间 用来存储当前节点中的内容的大小
    for(i = 0; i < node_num; i++) {
        JLOG_VSG(nodes)[i] = (log_storage_node *)((char *)(JLOG_VSG(first_node).p) + node_size*i);
        JLOG_VSG(nodes)[i]->p = (char *)(JLOG_VSG(nodes)[i]) + sizeof(unsigned long);
        JLOG_VSG(nodes)[i]->mm_size = 0;
    }

    return 1;
}

