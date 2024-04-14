#include"head.h"


    

void flightdisplay()
{
printf("---------------------- 航班菜单 -------------------------\n");
printf("----1.增加航班----------------------2.浏览航班------------\n");
printf("----3.修改航班信息------------------4.删除航班信息--------\n");
printf("----5.查询航班路线------------------6.购票---------------\n");
printf("----7.退票--------------------------8.个人中心-----------\n");
printf("----0.返回上一级-----------------------------------------\n");
}

// 将用户信息链表写入文件
void writeUsersToFile(const char *filename, unode_t *head) 
{
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    unode_t *current = head;
    
        // 将用户信息写入文件中
        fprintf(file, "%s,%s,%s\n", current->userm->name, current->userm->passwd, current->userm->IDcard);
  
        for (int i = 0; i < 6; i++) 
        {
         
         
            fprintf(file, " %s", current->userm->ticket[i]);
    }
       fprintf(file, "\n");

    fclose(file);
}
// 从文件中读取用户信息并存储到链表中
void readUserFromFile(const char *filename, unode_t *userhead) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[255];
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%s %s %s", userhead->userm->name, userhead->userm->passwd, userhead->userm->IDcard);

        char *token = strtok(line, " ");
        int i = 0;
        while (token != NULL) {
            token = strtok(NULL, " ");
            if (token != NULL && i <6) {
                userhead->userm->ticket[i] = strdup(token);  // 使用 strdup 复制字符串到数组中
                i++;
            }
        }
    }

    fclose(file);
}

// 将航班信息写入文件
void writeFlightsToFile(const char *filename, node_t *flights) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) 
    {
        perror("Error opening file");
        return;
    }

    node_t *current = flights->after;

    while (current != flights)
    {
        flight_t *currentFlight = current->data;
        fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                currentFlight->flightNUM, currentFlight->flighttype,
                currentFlight->startcity, currentFlight->starttime,
                currentFlight->arrivecity, currentFlight->arrivetime,
                currentFlight->flytime, currentFlight->value,
                currentFlight->maxNUM, currentFlight->leftbuyersa,
                currentFlight->whetherfly);
        current = current->after;
    }

    fclose(file);
}
// 将航班信息清空写入文件
void ClearwriteFlightsToFile(const char *filename, node_t *flights) {
    // 打开文件以便写入航班信息
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // 遍历航班链表，将每个航班的信息写入文件
    node_t *current = flights->after;
    while (current != flights) {
        flight_t *currentFlight = current->data;
        fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                currentFlight->flightNUM, currentFlight->flighttype,
                currentFlight->startcity, currentFlight->starttime,
                currentFlight->arrivecity, currentFlight->arrivetime,
                currentFlight->flytime, currentFlight->value,
                currentFlight->maxNUM, currentFlight->leftbuyersa,
                currentFlight->whetherfly);
        current = current->after;
    }

    // 关闭文件
    fclose(file);
}
// 从文件中读取航班信息并构建双向循环链表
void readFlightsFromFile(const char *filename, node_t **flights) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    *flights = NULL; // 初始化链表为空

    char line[1000];
    while (fgets(line, sizeof(line), file) != NULL) {
        flight_t *newFlight = malloc(sizeof(flight_t));
        if (newFlight == NULL) {
            perror("Memory allocation error");
            fclose(file);
            return;
        }

        // 解析文件中的一行数据，并填充新的航班信息
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",
               newFlight->flightNUM, newFlight->flighttype,
               newFlight->startcity, newFlight->starttime,
               newFlight->arrivecity, newFlight->arrivetime,
               newFlight->flytime, newFlight->value,
               newFlight->maxNUM, newFlight->leftbuyersa,
               newFlight->whetherfly);

        // 创建新节点并插入链表
        node_t *newNode = malloc(sizeof(node_t));
        if (newNode == NULL) {
            perror("Memory allocation error");
            fclose(file);
            free(newFlight);
            return;
        }

        newNode->data = newFlight;

        if (*flights == NULL) {
            // 链表为空，插入第一个节点
            *flights = newNode;
            newNode->before = newNode;
            newNode->after = newNode;
        } else {
            // 链表非空，插入到链表尾部
            newNode->before = (*flights)->before;
            newNode->after = *flights;

            (*flights)->before->after = newNode;
            (*flights)->before = newNode;
        }
    }

    fclose(file);
}

//打印航班信息
void printFlights(node_t *flights) {
    node_t *current = flights;

    do {
        flight_t *currentFlight = current->data;
        printf(" %s\t  %s\t  %s\t  %s\t %s\t  %s\t %s\t  %s\t  %s\t  %s\t %s\t\n",
               currentFlight->flightNUM, currentFlight->flighttype, currentFlight->startcity, currentFlight->starttime,
                currentFlight->arrivecity, currentFlight->arrivetime, currentFlight->flytime, currentFlight->value,
               currentFlight->maxNUM, currentFlight->leftbuyersa,currentFlight->whetherfly);
        current = current->after;
    } while (current != flights);
}

// 释放链表内存
void freeFlightList(node_t *flights)
 {
    if (flights == NULL) {
        return;
    }

    node_t *current = flights;

    do {
        node_t *next = current->after;
        free(current->data);
        free(current);
        current = next;
    } while (current != flights);
}
//从键盘输入新的航班信息
void inputNewFlight(node_t* newFlight)
 {
    flight_t* newFlight1=(flight_t*)(newFlight->data);
    printf("请输入航班号:\n");
    scanf("%s", newFlight1->flightNUM);
    printf("请输入机型:\n");
    scanf("%s", newFlight1->flighttype);
    printf("请输入起点站:\n");
     scanf("%s", newFlight1->startcity);
    printf("请输入起飞时间:\n");
    scanf("%s", newFlight1->starttime);
    printf("请输入终点站:\n");
    scanf("%s",  newFlight1->arrivecity);
    printf("请输入到达时间:\n");
    scanf("%s", newFlight1->arrivetime);
    printf("请输入飞行时间:\n");
    scanf("%s", newFlight1->flytime);
    printf("请输入票价:\n");
    scanf("%s", newFlight1->value);
    printf("请输入载客量:\n");
    scanf("%s", newFlight1->maxNUM);
    printf("请输入余票:\n");
    scanf("%s",newFlight1->leftbuyersa);
    printf("请输入是否起飞:\n");
    scanf("%s", newFlight1->whetherfly);
       
}
//增加航班
void add_flight(const char *filename1)
{
    node_t * flighthead1=create_flightlinklist();
  node_t* newFlight = malloc(sizeof(node_t));
    if (newFlight == NULL) 
    {
        perror("Memory allocation error");
        // 处理内存分配失败的情况
    } 
    else 
    {
        // 为 newFlight->data 分配内存
        newFlight->data = malloc(sizeof(flight_t));
        if (newFlight->data == NULL)
        {
            perror("Memory allocation error");
            return;
        }

        // 调用函数
        inputNewFlight(newFlight);

    }
    insert_hdclinklist(flighthead1,newFlight->data,sizeof(flight_t));
    writeFlightsToFile(filename1,flighthead1);
}

// 修改航班信息
void modifyFlight(node_t *flights, const char *flightNumber, const char *newValue,const char *newmaxNUM,const char *newleftbuyersa,const char *newwhetherfly) 
{
    node_t *current = flights;

    do {
        flight_t *currentFlight = current->data;
        if (strcmp(currentFlight->flightNUM, flightNumber) == 0) {
            // 找到指定航班，修改票价信息
            strcpy(currentFlight->value, newValue);
            printf("航班 %s 的票价已修改为 %s\n", flightNumber, newValue);
            strcpy(currentFlight->maxNUM, newmaxNUM);
            printf("航班 %s 的额定载客量已修改为 %s\n", flightNumber, newmaxNUM);
            strcpy(currentFlight->leftbuyersa, newleftbuyersa);
            printf("航班 %s 的剩余座位数量已修改为 %s\n", flightNumber, newleftbuyersa);
            strcpy(currentFlight-> whetherfly, newwhetherfly);
            printf("航班 %s 的起飞状态已修改为 %s\n", flightNumber, newwhetherfly);
            return;
        }

        current = current->after;
    } while (current != flights);

    // 没有找到指定航班
    printf("未找到航班 %s\n", flightNumber);
}
void swapNodes(node_t *a, node_t *b)
 {
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// 按照票价升序排序
void sortByValue(node_t *flights) 
{
    if (flights == NULL) 
    {
        return;
    }

    int swapped;
    node_t *ptr1;
    node_t *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = flights->after;

        while (ptr1->after != lptr)
         {
            flight_t *flight1 = (flight_t *)ptr1->data;
            flight_t *flight2 = (flight_t *)ptr1->after->data;

            // 比较票价并交换节点
            if (atoi(flight1->value) > atoi(flight2->value)) 
            {
                swapNodes(ptr1, ptr1->after);
                swapped = 1;
            }

            ptr1 = ptr1->after;
        }
        lptr = ptr1;
    } while (swapped);
}

//查询航班路线
void printFlightsByArriveCity(node_t *flights, const char *arriveCity) 
{
    node_t *current = flights;
    int found = 0;

    printf("航班终点站为 %s 的航班信息:\n", arriveCity);

    do {
        flight_t *currentFlight = current->data;
        if (strcmp(currentFlight->arrivecity, arriveCity) == 0) {
            printf("航班号: %s, 机型: %s, 起点站: %s, 起飞时间: %s, 终点站: %s, 到达时间: %s, 飞行时间: %s, 票价: %s, 载客量: %s, 余票: %s, 是否起飞: %s\n",
                   currentFlight->flightNUM, currentFlight->flighttype, currentFlight->startcity,
                   currentFlight->starttime, currentFlight->arrivecity, currentFlight->arrivetime,
                   currentFlight->flytime, currentFlight->value, currentFlight->maxNUM,
                   currentFlight->leftbuyersa, currentFlight->whetherfly);
            found = 1;
        }

        current = current->after;
    } while (current != flights);

    if (!found) {
        printf("未找到终点站为 %s 的航班\n", arriveCity);
    }
}
//删除航班
void deleteFlightByNumberFromFile( node_t **flights,const char *flightNumber)
 {
     if (*flights == NULL) {
        printf("航班链表为空，无法删除航班。\n");
        return;
    }

    node_t *current = *flights;
    node_t *toDelete = NULL;

    do {
        flight_t *currentFlight = current->data;
        if (strcmp(currentFlight->flightNUM, flightNumber) == 0) {
            // 找到要删除的航班
            toDelete = current;
            break;
        }

        current = current->after;
    } while (current != *flights);

    if (toDelete != NULL) {
        // 找到了要删除的航班节点
        if (toDelete->before == toDelete && toDelete->after == toDelete) {
            // 链表中只有一个节点
            free(toDelete->data);
            free(toDelete);
            *flights = NULL;
        } else {
            toDelete->before->after = toDelete->after;
            toDelete->after->before = toDelete->before;

            if (toDelete == *flights) {
                // 要删除的节点是链表头
                *flights = toDelete->after;
            }

            free(toDelete->data);
            free(toDelete);
        }
        

        printf("航班 %s 已删除。\n", flightNumber);
    } else {
        printf("未找到航班 %s无法删除。\n", flightNumber);
    }
}

// 购票函数
void bookTicket(node_t *flights, const char *arriveCity,unode_t *userhead) 
{
    node_t *current = flights->after;

    while (current != flights)
    {
        flight_t *currentFlight = current->data;
        if (strcmp(currentFlight->arrivecity,arriveCity) == 0) 
        {
            // 找到指定航班
            if (atoi(currentFlight->leftbuyersa) > 0) 
            {
                printf("该趟航班还有%d张票\n",atoi(currentFlight->leftbuyersa));
                // 有余票
                
                snprintf(currentFlight->leftbuyersa, sizeof(currentFlight->leftbuyersa), "%d", atoi(currentFlight->leftbuyersa) - 1);
                // 创建购票记录并存储
               strcpy(userhead->userm->ticket[0], currentFlight->flightNUM);
                strcpy(userhead->userm->ticket[1], currentFlight->flighttype);
                strcpy(userhead->userm->ticket[2], currentFlight->startcity);
                strcpy(userhead->userm->ticket[3],currentFlight->starttime);
                strcpy(userhead->userm->ticket[4], currentFlight->arrivecity);
                strcpy(userhead->userm->ticket[5],currentFlight->arrivetime);
                printf("购票成功！\n");
            } else {
                printf("航班 %s 余票不足，无法购票。\n", currentFlight->flightNUM);
            }
            return;
        }

        current = current->after;
    } 

    printf("未找到航班,无法购票。\n");
}
//退票函数
void delete_tick(node_t *flights,unode_t *userhead,const char *arriveCity)
{
     node_t *current = flights->after;

    do {
        flight_t *currentFlight = current->data;
        if (strcmp(currentFlight->arrivecity,arriveCity) == 0) 
        {
           
            for (int i = 0; i < 6; i++) 
            {

                        strcpy(userhead->userm->ticket[i], "");
             }

            snprintf(currentFlight->leftbuyersa, sizeof(currentFlight->leftbuyersa), "%d", atoi(currentFlight->leftbuyersa) + 1);  
            printf("退票成功\n");
            return;
        }

        current = current->after;
    } while (current != flights);
}

//打印购票信息
void printBooking(const unode_t *userhead) 
{
    if(userhead->userm->ticket[0]==NULL)
    {
        printf("您还没有购票\n");
    }
    else
    {
    printf("航班代号: %s\n", userhead->userm->ticket[0]);
    printf("飞机机型: %s\n", userhead->userm->ticket[1]);
    printf("航班起点站: %s\n", userhead->userm->ticket[2]);
    printf("起飞时间: %s\n", userhead->userm->ticket[3]);
    printf("航班终点站: %s\n", userhead->userm->ticket[4]);
    printf("到达时间: %s\n", userhead->userm->ticket[5]);
    }
}
void flight_menu()
{
    user_t user1[]= {"lzf","123456","456789"};
   
    unode_t *userhead2= create_userlist();//存储用户信息的链表
 insert_hclinklist(userhead2, user1);//插入用户信息

    char  arriveCity1[100];
    char  arriveCity[100];
    char deleteflightNUM[100];//航班代号
    char findarrivecity[100];//查询航班终点站
    char nowflightNUM[100];//航班代号
    char newvalue[100] ;  //新票价
    char newmaxNUM[100] ; //新额定载客量，
    char newleftbuyersa[100] ; //新剩余座位数量
    char newwhetherfly[100]  ;//新是否起飞 ‘y’ ‘n’
    const char *filename1 = "flights.txt";
    const char *filename2 = "user.txt";
    node_t * flighthead=create_flightlinklist();//创建航班链表
   flight_t flm[] = {
      {"东航MU2774","320","长沙黄花机场T2","2024/2/11/16:30","南京禄口机场T2"," 17:50","   约1小时      ","￥229","150","   80","    Y"},
      {"海航HU7374","737","长沙黄花机场T2","2024/2/11/19:30","首都国际机场T2"," 22:00","   约2小时30分钟","￥281","162","   63","    Y"},
      {"东方CZ2447","320","长沙黄花机场T2","2024/2/11/21:40","白云国际机场T2"," 23:10","   约1小时30分钟","￥770","150","   86","    Y"},
      {"东航MU9300","737","长沙黄花机场T2","2024/2/11/20:00","虹桥国际机场T2"," 21:30","   约1小时30分钟","￥450","162","   50","    Y"},
      {"川航3U1909","737","长沙黄花机场T2","2024/1/11/13:35","保安国际机场T3"," 15:20","   约1小时45分钟","￥980","162","   20","    Y"},
      {"  航班号  ","机型","  起点站 ","        起飞时间","        终点站 "," 到达时间"," 飞行时间","     票价","载客量","余票","是否启"}
    };
#if 0
     //插入航班
    int i = 0 ;
    int flg = 0 ;
    for(i = 0; i < sizeof(flm)/sizeof(flm[0]) ; i++)
    {
      insert_hdclinklist(flighthead,flm+i,sizeof(flight_t));

    }
#endif
    //将航班写入文件中
    writeFlightsToFile(filename1,flighthead);
    //读取文件并
  
    readFlightsFromFile(filename1, &flighthead);
    //将用户信息写入文件中

     writeUsersToFile(filename2, userhead2);
    //读取用户
   readUserFromFile(filename2, userhead2);
 int c;
    do {
        
        flightdisplay();
        printf("请输入您要查询的信息选择: ");
        scanf("%d", &c);
        switch (c) 
        {
            case 1:
                printf("增加航班\n");
                add_flight(filename1);
                readFlightsFromFile(filename1, &flighthead);
                printFlights(flighthead);
            break;
            case 2:
                printf("浏览航班\n");
                readFlightsFromFile(filename1, &flighthead);
               printFlights(flighthead);
           //show_dclinklist(flighthead,showflightall);
            break;
            case 3:
                printf("修改航班信息\n");
                readFlightsFromFile(filename1, &flighthead);
                printf("航班信息（修改前）：\n");
                printFlights(flighthead);
                printf("请输入要修改的航班号：\n");
                scanf("%s",nowflightNUM);
                printf("请输入新的票价信息：\n");
                scanf("%s",newvalue);
                printf("请输入新的额定载客量信息：\n");
                scanf("%s",newmaxNUM);
                printf("请输入新的剩余座位数量信息：\n");
                scanf("%s",newleftbuyersa);
                printf("请输入新的起飞状态信息：\n");
                scanf("%s",newwhetherfly);
                modifyFlight(flighthead, nowflightNUM, newvalue,newmaxNUM,newleftbuyersa,newwhetherfly);
                writeFlightsToFile(filename1,flighthead);
                printf("航班信息（修改后）：\n");
                printFlights(flighthead);
            break;
            case 4:
                printf("删除航班信息\n");
                readFlightsFromFile(filename1, &flighthead);
                printf("请输入要删除航班号信息：\n");
                scanf("%s",deleteflightNUM);
                deleteFlightByNumberFromFile(&flighthead,deleteflightNUM);
                ClearwriteFlightsToFile(filename1,flighthead);
                printFlights(flighthead);
            break;
            case 5:
                printf("请输入要查询的终点站信息：\n");
                scanf("%s",findarrivecity);
                printFlightsByArriveCity(flighthead, findarrivecity);
            break;
            case 6:
                printf("购票\n");
                readUserFromFile(filename2, userhead2);
                printf("请输入到达的机场名字\n");
                scanf("%s",arriveCity);
                bookTicket(flighthead,arriveCity,userhead2);
                printFlights(flighthead);
                 
            break;
            case 7:
                printf("退票\n");
              readUserFromFile(filename2, userhead2);
                printf("请输入要退票的终点站\n");
                scanf("%s",arriveCity1);
                delete_tick(flighthead,userhead2,arriveCity1);
                printFlights(flighthead);
                printf("退票成功\n");
            break;
            case 8:
                printf("购买的票\n");
                readUserFromFile(filename2, userhead2);
                printBooking(userhead2);
                break;
            case 0:
                printf("返回上一级\n");
                break;
            default:
                printf("无效选择，请重试。\n");
        }
    } while (c!= 0);
}
