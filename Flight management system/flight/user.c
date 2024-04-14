#include"head.h"

user_t user[] = {
    {"lzf","123456","456789"}};


//用户菜单打印
void userdisplayMenu() 
{
printf("---------------------- 菜单 -----------------------\n");
printf("----1.用户登录----------------------0.退出-------------\n");
printf("\n-------------------------------------------------\n");
}

//创建用户链表
unode_t* create_userlist()
{
    unode_t *userList=malloc(sizeof(unode_t));
    if(userList==NULL)
    {
        perror("malloc is error\n");
    }
    userList->userm = malloc(sizeof(user_t));
    if (userList->userm == NULL)
    {
        perror("malloc is error\n");
        free(userList); // 释放之前分配的内存
        return NULL;
    }

    strcpy(userList->userm->name, "");
    strcpy(userList->userm->passwd, "");
   strcpy(userList->userm->IDcard, "");
    // 分配足够的内存来存储航班信息
    for (int i = 0; i < 10; i++)
    {
        userList->userm->ticket[i] = malloc(100 * sizeof(char));
        if (userList->userm->ticket[i] == NULL)
        {
            perror("malloc is error\n");
            // 释放之前分配的内存
            for (int j = 0; j < i; j++)
            {
                free(userList->userm->ticket[j]);
            }
            free(userList->userm);
            free(userList);
            return NULL;
        }
        strcpy(userList->userm->ticket[i], ""); // 初始化为空字符串
    }

    return userList;
}

//插入信息
int insert_hclinklist(unode_t *head, user_t *stus)
 {
    // 1. 判断参数
    if (head == NULL || stus == NULL)
     {
        perror("参数错误\n");
        return -1;
     }

    // 2. 申请节点空间 + 申请数据部分的空间 + 建立联系
    unode_t *newnode = malloc(sizeof(unode_t));
    if (newnode == NULL)
     {
        perror("newnode malloc 错误\n");
        return -1;
     }

    // 数据部分的空间
    newnode -> userm = malloc(sizeof(user_t));

    // 3. 填写数据
    
    // 或者使用结构体指针一次性赋值
     *newnode -> userm = *stus;

    // 4. 修改指针
    newnode -> next = head->next;
    head -> next = newnode;

    return 0;
}
//打印
void showall(user_t * stun)
{
    printf("%s ",stun->name);
    printf("%s ",stun->passwd);
    printf("%s ",stun->IDcard);
   
    printf("\n");
}

//链表 + 打印哪种数据
int showclinklist(unode_t*head,usershowfun_t showfun)
{   
    //循环链表
    if(showfun == NULL)
    {
        perror("parameters error\n");
        return -1;
    }
    if (head->next == head)
    {
        /* code */
        printf("链表已经为空！\n");
        return 0;
    }
    

    //遍历  并且输出内容
    unode_t * temp = head->next;
    while(temp != NULL)
    {
        showfun(temp->userm);
        temp = temp->next;
    }
    printf("\n");
    return 0;
}
int login(unode_t *userList) 
{
    char inputName[16];
    char inputPasswd[64];

    printf("请输入用户名：");
    scanf("%s", inputName);

    printf("请输入密码：");
    scanf("%s", inputPasswd);

    unode_t *current = userList->next;

    if(current != NULL) 
    {
        if (strcmp(current->userm->name, inputName) == 0 && strcmp(current->userm->passwd, inputPasswd) == 0) 
        {
            printf("登录成功！\n");
            flight_menu();
        }
        else
         printf("登录失败，用户名或密码错误\n");

    }
    return 0;

}
// 在用户信息链表中插入新用户信息
void insert_user(unode_t **head, const user_t *user)
 {
    unode_t *new_node = (unode_t *)malloc(sizeof(unode_t));
    if (new_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // 将用户信息复制到新节点中
    strcpy(new_node->userm->name, user->name);
    strcpy(new_node->userm->passwd, user->passwd);
    strcpy(new_node->userm->IDcard, user->IDcard);

    // 插入到链表头部
    new_node->next = (*head)->next;
    (*head)->next = new_node;
}


