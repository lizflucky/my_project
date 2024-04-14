#ifndef ___HEAD_H_
#define ___HEAD_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <syslog.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/poll.h>

// 节点类型
typedef struct stu
{
    int pcid;
    char *name;
    struct stu *next;
} node_t;

//服务端
int server();

//创建链表存储用户信息
node_t *create_Linklist();
// 2.头插法：新来的数据节点连接在链表头后面  第一个数据节点 数据头前面
int insert_hlinklist(node_t *head, int newdata,char*name);
//删除用户
int delete_vlinklist(node_t*head,int value);
//用户个数
int showlinklistnode(node_t*head);
//查找地i个数据
int find_ilinklist(node_t *head, int index);
//销毁链表
int destroy_linklist(node_t**head);

char * find_vlinklist(node_t *head, int value);












#endif