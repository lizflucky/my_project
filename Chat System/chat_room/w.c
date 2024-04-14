#include "head.h"
time_t tm = 0;

pthread_key_t tls_key; // 定义线程局部存储键

void destructor_function(void *value)
{
    // 在线程结束时调用的析构函数
    printf("Thread-specific data %s is being destroyed\n", (char *)value);
    free(value); // 释放内存
}
node_t *head;
// cidnum： 代表的是 当前系统有多少个人。

int cidnum = 0;
// 静态 cid ;
char bufs[128] = "";
char bufv[128] = "";
char buftime[128] = " ";
char *bufname = NULL;

int len = 0;
void *pthread_fun(void *arg)
{
    bzero(bufv, sizeof(bufv));
    bzero(bufs, sizeof(bufs));
    bzero(buftime, sizeof(buftime));
    // cid是从主线程传过来的。
    int pcid = *(int *)arg;
    char *name = find_vlinklist(head, pcid);
    FILE *fp = fopen("info.txt", "a+");
    int i = 0;

    // 在线程开始时将线程名保存到线程局部存储中
    char *thread_specific_data = strdup(name);          // 复制线程名
    pthread_setspecific(tls_key, thread_specific_data); // 将线程名关联到线程局部存储
    name = (char *)pthread_getspecific(tls_key);

    // 发送登录信息
    strncpy(bufv, name, strlen(name));
    strcat(bufv, " is online!");
    for (i = 1; i <= cidnum; i++)
    {

        int scid = find_ilinklist(head, i);
        send(scid, bufv, strlen(bufv), 0);
    }
    // 清空bufs
    bzero(bufv, sizeof(bufv));
    bzero(bufs, sizeof(bufs));
    bzero(buftime, sizeof(buftime));

    while (1)
    {
        // 找出当前时间
        time_t tm2 = time(&tm);
        char *time = asctime(localtime(&tm));
        // 清空
        bzero(bufv, sizeof(bufv));
        bzero(bufs, sizeof(bufs));
        bzero(buftime, sizeof(buftime));
        // 接收消息
        len = recv(pcid, bufs, sizeof(bufs), 0);
        // strncpy(bufv,bufs,strlen(bufs)-2);
        printf("cid=%d的客户端发送过来的数据是:%s len = %d\n", pcid, bufs, len);

        // 做好格式 名字:消息
        strncpy(bufv, name, strlen(name));
        strcat(bufv, ":");
        strncat(bufv, bufs, strlen(bufs));
        // 做好格式 当前时间->名字:消息
        strncpy(buftime, time, strlen(time) - 1);
        strcat(buftime, "->");
        strncat(buftime, bufv, strlen(bufv));
        // 将做好的格式写入文件中
        fwrite(buftime, strlen(buftime), 1, fp);
        fflush(fp);

        // 在这里进行转发。

        for (i = 1; i <= cidnum; i++)
        {
            int scid = find_ilinklist(head, i);
            if (!strncmp(bufs, "@ ", 2))
            {
                char *name1 = find_vlinklist(head, scid);
                printf("%s\n",name1);
                if (strstr(bufs, name1) != NULL)
                {
                    printf("%s\n",name1);
                    // 私发
                    printf("1\n");
                    send(scid, bufv, strlen(bufv), 0);
                    break;
                }
            }
            else
                send(scid, bufv, strlen(bufv), 0);
        }
        // 如果接收到"quit"，证明有一个用户退出，将其节点删除
        if (!strncmp(bufs, "quit", 4))
        {
            delete_vlinklist(head, pcid);
            printf("%s正在退出....\n", name);
            cidnum--;
            break;
        }
    }
    // 线程退出
    fclose(fp);
    shutdown(pcid, SHUT_RDWR);
    // 线程退出
    pthread_exit((void *)0);
}

// 多用户登录 - 并发服务器 - 线程方法
int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        perror("usage:./server ip port\n");
        return -1;
    }

    //.建立套接字
    int sid = socket(AF_INET, SOCK_STREAM, 0);
    printf("sid = %d\n", sid);
    // 创建线程局部存储键
    pthread_key_create(&tls_key, destructor_function);
    // 实际地址结构体
    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    saddr.sin_port = htons(atoi(argv[2]));
    if (bind(sid, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        perror("bind error\n");
        return -2;
    }
    printf("bind successful\n");

    // 3.监听
    listen(sid, 10);

    // 4.循环连接
    char bufs[128] = "";
    int len = 0;
    pthread_t tid = 0;
    struct sockaddr_in caddr = {0};
    socklen_t addr_len = sizeof(saddr);
    // 只要初始化一次，在全局数据区
    static int cid = -1;
    bufname = (char *)malloc(50); // 分配内存给 bufname

    head = create_Linklist();
    if (head == NULL)
    {
        perror("create_linklist error\n");
        return -1;
    }
    while (1)
    { // 主线程。  链接。
        cid = accept(sid, (struct sockaddr *)&saddr, &addr_len );
        memset(bufname, 0, sizeof(bufname));
        recv(cid, bufname, 50, 0);
        bufname[50 - 1] = '\0'; // 添加 NULL 终止符
        insert_hlinklist(head, cid, bufname);
        cidnum = showlinklistnode(head);

        if (cid != -1)
        {
            // 把线程设定成分离属性 - 线程执行完毕以后，自动释放占用的空间。
            pthread_create(&tid, NULL, pthread_fun, &cid);
            pthread_detach(tid);
            printf("%s登录 cid = %d\n", bufname, cid);
        }
    }
    free(bufname);
    // 销毁线程局部存储键
    pthread_key_delete(tls_key);
    destroy_linklist(&head);
    return 0;
}

//创建链表存储用户信息
node_t *create_Linklist()
{
    // 1 申请空间
    node_t *head = (node_t *)malloc(sizeof(node_t));
    if (head == NULL)
    {
        perror("malloc error\n");
        return (node_t *)-1;
    }
    // 2 数据填写
    head->pcid = 0; // 单链表的头结点是链表的标志：不存放具体数据，通常存放链表号码。
    head->name = NULL;
    // 3 指针赋值
    head->next = NULL;
    return head;
}
// 2.头插法：新来的数据节点连接在链表头后面  第一个数据节点 数据头前面
int insert_hlinklist(node_t *head, int newdata,char *newname)
{
    // 1.参数判断
    if (head == NULL)
    {
        perror("head is null\n");
        return -1;
    }
    // 2.申请空间
    node_t *newnode = (node_t *)malloc(sizeof(node_t));
    if (newnode == NULL)
    {
        perror("malloc error\n");
        return -1;
    }
    // 3.填写数据
    newnode->pcid = newdata;
    newnode->name= newname;
    // 4.修改指针
    newnode->next = head->next;
    head->next = newnode;
    return 0;
}
//按值删除节点
int delete_vlinklist(node_t*head,int value)
{
    //1.参数判断
    if (head == NULL || head->next==NULL)
    {
        /* 用perror也可以*/
        printf("head is null or linklist is illegal\n");
        return -1;
    }
    //2.遍历
    node_t*temp = head->next,*before = head;
    while (temp != NULL && temp->pcid != value)
    {
        /* code */
        //before先移动
        before = temp;
        //temp再移动
        temp = temp->next;
    }
    //出界判读
    if (temp == NULL)
    {
        perror("this value is not in this linklist\n");
        return -1;
    }
    //3.修改指针
    before->next  = temp->next;
    free(temp);
    temp = NULL;
    return 0;
}
//元素个数
int showlinklistnode(node_t*head)
{
    //1.结构不存在
    if(head == NULL)
    {
        perror("this linklist is not exits\n");
        return -1;
    }
    //2 元素为 0个元素
    if(head->next == NULL)
    {
        printf("there is no elemet in this linklist\n");
        return 0;
    }

    //3 遍历
    int i = 1 ;
    node_t * temp = head->next;
    while(temp ->next != NULL)
    {
        i++;
        temp = temp->next;
    }
    //i就是元素个数
    return i;
}
//查找地i个数据
int find_ilinklist(node_t *head, int index)
{
    // 1.参数判断
    // index 过大不在这里讨论
    if (head == NULL || head->next == NULL || index <= 0)
    {
        perror("parameters error\n");
        return -1;
    }
    // 2.开始查找
    int i = 1;
    node_t *temp = head->next;
    // 在不出界的情况下  i < index
    while (temp != NULL && i < index)
    {
        i++;
        temp = temp->next;
    }
    // 3立即判断 index太大
    if (temp == NULL)
    {
        printf("index is bigger than linklist length\n");
        return -1;
    }
    // 4.真正找到了
    int res = temp->pcid;
    return res;
}

//销毁链表
int destroy_linklist(node_t**head)
{
    if(*head == NULL)
    {
        perror("linklist is not exits\n");
        return -1;
    }
    //先取出元素 再取出成员
    node_t*temp =(*head)->next ,*before = NULL;  
    (*head) ->next = NULL;
    while( temp != NULL)
    {
        before = temp;
        temp = temp->next;
        free(before);
    }
    free(*head);
    *head = NULL;
    return 0 ;
}
char * find_vlinklist(node_t *head, int value)
{
    
    // 1.参数判断
    // 链表不存在 + 链表没数据
    if (head == NULL || head->next == NULL )
    {
        perror("parameters error\n");
        return (char*)-1;
    }
    // 2.遍历
    int i = 0;
    node_t *temp = head;
    while (temp != NULL && temp->pcid != value)
    {
        i++;
        temp = temp->next;
    }
    // 3.跳出循环的分析
    if (temp == NULL)
    {
        perror("value is not int this linklist\n");
        // 系统提示:本程序没有违规操作。
        return (char*)-1;
    }
    char * ires= temp->name;
    // 返回值要写好
    return ires;
}