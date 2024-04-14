#ifndef ___HEAD_H_
#define ___HEAD_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//购票
// 结构体定义
typedef struct booking 
{
    char my_flightNUM[100];//航班代号
    char my_flighttype[100] ;//飞机机型
    char my_startcity[100] ; //航班起点站
    char my_starttime[100];  //起飞时间 时:分
    char my_arrivecity[100] ; //航班终点站
    char my_arrivetime[100] ; //到达时间 时:分

} booking_t;

//用户信息
// 用户数据结构体
typedef struct user {
    char name[16];       // 用户名
    char passwd[64];     // 用户密码
    char IDcard[20];          // 用户身份证信息
    char *ticket[10];    // 指针数组: 存储航班信息。
} user_t;

// 用户数据节点 - 单链表
typedef struct unode {
    user_t *userm;
    struct unode *next;
} unode_t;

typedef void (*usershowfun_t)(user_t* stun); 
//用户函数声明
void userdisplayMenu();//用户菜单打印函数
unode_t* create_userlist();
int insert_hclinklist(unode_t *head,user_t * stus);
void showall(user_t * stun);
int showclinklist(unode_t*head,usershowfun_t showfun);
int login(unode_t *userList);
void writeUsersToFile(const char *filename, unode_t *head);
void readUserFromFile(const char *filename, unode_t *userhead);
void insert_user(unode_t **head, const user_t *user);
//航班函数声明



//航班数据结构体
typedef struct flight
{
        char flightNUM[100];//航班代号
        char flighttype[100] ;//飞机机型
        char startcity[100] ; //航班起点站
        char starttime[100];  //起飞时间 时:分
        char arrivecity[100] ; //航班终点站
        char arrivetime[100] ; //到达时间 时:分
        char flytime[100] ;//飞行时间
        char value[100] ;  //票价
        char maxNUM[100] ; //额定载客量，
        char leftbuyersa[100] ; //剩余座位数量
        char whetherfly[100]  ;//是否起飞 ‘y’ ‘n’
}flight_t;
//航班信息节点 - 双向循环链表
typedef struct fnode
{
    struct fnode * before;
    void * data;
    struct fnode * after;
}node_t;

typedef void (*showfun_t)(void *data);
void flightdisplay();
void flight_menu();
node_t * create_flightlinklist();
int insert_hdclinklist(node_t*head,void *newdata,int size);
void showflightall(void *data);
int show_dclinklist(node_t*head,showfun_t showfun);
void printFlights(node_t *flights);
void readFlightsFromFile(const char *filename, node_t **flights);
void writeFlightsToFile(const char *filename, node_t *flights);
void freeFlightList(node_t *flights);
void ClearwriteFlightsToFile(const char *filename, node_t *flights);
void add_flight(const char *filename1);
void inputNewFlight(node_t* newFlight);
void modifyFlight(node_t *flights, const char *flightNumber, const char *newValue,const char *maxNUM,const char *leftbuyersa,const char *whetherfly);
void deleteFlightByNumberFromFile( node_t **flights,const char *flightNumber);

void sortByValue(node_t *flights);
void swapNodes(node_t *a, node_t *b);
void printFlightsByArriveCity(node_t *flights, const char *arriveCity);
void bookTicket(node_t *flights, const char *arriveCity,unode_t *userhead);
void printBooking(const unode_t *userhead);
void delete_tick(node_t *flights,unode_t *userhead,const char *arriveCity);
#endif