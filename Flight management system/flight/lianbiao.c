#include"head.h"




//1.创建一个双向循环链表
node_t * create_flightlinklist()
{
    node_t* head = (node_t*)malloc(sizeof(node_t));
    if(head == NULL)
    {
      perror("create error\n");
      return NULL;
    }
    head->after = head->before = head;
    head->data = NULL;
    return head;
}

//2.头插法
//链表头 - 数据起始地址 - 长度
int insert_hdclinklist(node_t*head,void *newdata,int size)
{
    if (head==NULL || newdata == NULL || size <=0)
    {
      /* code */
      perror("error\n");
      return -1;
    }
    //申请节点空间 - 申请数据空间 - 填写数据 - 建立联系
    node_t* newnode = malloc(sizeof(node_t));
    if(newnode == NULL)
    {
      perror("newnode malloc error\n");
      return -2;
    }
    //size本身大小
    newnode->data = malloc(size);
    //填写数据
    memcpy(newnode->data,newdata,size);
    //修改指针
    //先处理新节点的指针
    newnode->before = head;
    newnode->after = head->after;
    //
    head->after->before = newnode;
    head->after = newnode;

    return 0;
}

void showflightall(void *data)
{
   flight_t* newdata = (flight_t*)data;
   printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\n",newdata->flightNUM,newdata->flighttype,newdata->startcity,newdata->starttime,newdata->arrivecity,
   newdata->arrivetime,newdata->flytime,newdata->value,newdata->maxNUM,newdata->leftbuyersa,newdata->whetherfly);
}

//插入航班数据
int show_dclinklist(node_t*head,showfun_t showfun)
{
    if(head == NULL || head->before ==  head)
    {
      perror("head is illegal or empty!\n");
      return -1;
    }
    node_t * temp = head->after;
    while (temp!=head)
    {
      /* code */
      showfun(temp->data);
      temp = temp->after;
    }


}
#if 0
//7.修改第几个数据
int update_idclinklist(node_t*head,int index,void *data,int size)
{
    //1.
    if (head==NULL||head->after==head||index<=0||data==NULL||size<=0)
    {
      /* code */
      perror("error\n");
      return -1;
    }
    //修改bug
    index = index % (shownodenumber(head) + 1);
    if(!index)
    {
      perror("index error\n");
      return -1;
    }

    //2.找到第index个数据
    int i =  1; 
    node_t* temp = head->after;
    while(i < index)
    {
        temp = temp->after;
        i++;
    }
    //i = index 
    memcpy(temp->data,data,size);
    
    return 0;
}

//3.链表打印
typedef void (*showfun_t)(void *data);
void showall(void *data)
{
   flight_t* newdata = (flight_t*)data;
   printf("%s %d\n",newdata->name,newdata->price);
}
//显示名字
void showflightname(void *data)
{
   flight_t* newdata = (flight_t*)data;
   printf("%s ",newdata->name);
}
//显示票价
void showflightprice(void *data)
{
  flight_t* newdata = (flight_t*)data;
  printf("%d ",newdata->price);
}
//链表头 - 显示函数
int show_dclinklist(node_t*head,showfun_t showfun)
{
    if(head == NULL || head->before ==  head)
    {
      perror("head is illegal or empty!\n");
      return -1;
    }
    node_t * temp = head->after;
    while (temp!=head)
    {
      /* code */
      showfun(temp->data);
      temp = temp->after;
    }
}

//4尾插法
int insert_tdclinklist(node_t*head,void *data,int size)
{
    if(head==NULL || data==NULL || size <= 0 )
    {
      perror("parameters error\n");
      return -1;
    }
    //申请空间
    node_t* newnode = malloc(sizeof(node_t));
    newnode->data = malloc(size);
    //填写数据
    memcpy(newnode->data,data,size);

    //修改指针
    node_t * tail = head->before;
    newnode->before = tail;
    newnode->after = head;

    tail->after = newnode;
    head->before=newnode;
    return 0;
}
//5在任意位置插入数据节点
int insert_idclinklist(node_t*head,int index,void*data,int size)
{
    //1.判断参数
    if (head == NULL || data == NULL || size <= 0 || index <= 0 )
    {
      /* code */
      perror("parameters error\n");
      return -1;
    }

    //2 找到index -1 指针6
    int i = 0 ;
    node_t * temp = head;
    while(i < index - 1)
    {
      temp = temp->after;
      i++;
    }
    //3.申请节点空间 + 申请数据空间 + 建立联系 + 填写数据
    node_t*newnode = malloc(sizeof(node_t));
    newnode->data = malloc(size);
    memcpy(newnode->data,data,size);

    //4.修改指针
    newnode->after = temp->after;
    newnode->before = temp;

    temp->after->before = newnode;
    temp->after = newnode;  
    return 0;
}

//6.显示链表中节点个数
int shownodenumber(node_t*head)
{
    if(head == NULL)
    {
        perror("head is null\n");
        return -1;
    }
    if(head->after == head)
    {
      printf("dclinklist is empty\n");
      return 0;
    }

    int i = 1;
    node_t * temp = head->after;
    //temp不要求指向头结点
    while(temp->after != head)
    {
      temp  = temp ->after ;
      i++;
    }
    return i;
}
//7.修改第几个数据
int update_idclinklist(node_t*head,int index,void *data,int size)
{
    //1.
    if (head==NULL||head->after==head||index<=0||data==NULL||size<=0)
    {
      /* code */
      perror("error\n");
      return -1;
    }
    //修改bug
    index = index % (shownodenumber(head) + 1);
    if(!index)
    {
      perror("index error\n");
      return -1;
    }

    //2.找到第index个数据
    int i =  1; 
    node_t* temp = head->after;
    while(i < index)
    {
        temp = temp->after;
        i++;
    }
    //i = index 
    memcpy(temp->data,data,size);
    
    return 0;
}
//8根据部分信息输出全部信息。
typedef int (*cmpfun_t)(void *data1,void *data2);
//1.比对函数
int cmpname(void *data1,void*data2)
{
  flight_t* newdata1=(flight_t*)data1;
  flight_t* newdata2=(flight_t*)data2;
  return strcmp(newdata1->name,newdata2->name);
}
//2.比对价格
int cmprice(void *data1,void*data2)
{ 
  flight_t* newdata1=(flight_t*)data1;
  flight_t* newdata2=(flight_t*)data2;
  return newdata1->price - newdata2->price;
}
//比对成功返回指针。
node_t * Search_dclinklist(node_t*head,void *value,cmpfun_t cmpfun)
{ 
  //1.参数判断
  if (head == NULL || head->after == head || value == NULL || cmpfun == NULL)
  {
    /* code */
    perror("ERROR!\n");
    return NULL;
  }
  //2.遍历
  node_t * temp = head->after;
  while (temp != head)
  {
    /* code */
    if (!cmpfun(temp->data,value))
    {
      /* code */
      return temp;
    }
    temp  = temp->after;
  }
  return NULL;
}

//9.删除头节点
int delete_hdclinklist(node_t*head,void *data,int size)
{
    if (head == NULL ||head->after==head)
    {
      /* code */
      perror("error\n");
      return -1;
    }
    node_t * temp = head->after;
    
    head->after = temp->after;
    temp->after->before = head;

    //把数据传递出去
    memcpy(data,temp->data,size);
    free(temp->data);
    free(temp);
    return 0;
}

//10.删除尾部节点
int delete_tdclinklist(node_t*head,void*data,int size)
{
  if (head==NULL||head->after == head||data == NULL || size <=0)
  {
    /* code */
    perror("parameters error\n");
    return -1;
  }
  node_t * tail = head->before;
  node_t* temps= tail->before;

  //修改指针
  temps->after = head;
  head->before = temps;

  //传输数据
  memcpy(data,tail->data,size);
  //释放空间
  free(tail->data);
  free(tail); 
  return 0;
}

//11.删除第index个节点  ： 找到第index -1 这个节点的指针。
int delete_idclinklist(node_t*head,int index,void *data,int size)
{
    //1.参数判断
    if(head == NULL || head->after == head || index <= 0 || data == NULL)
    {
      perror("ERROR!\n");
      return -1 ;
    }
    //2.
    index = index % (shownodenumber(head) + 1);
    if(index == 0)
    {
      perror("index error\n");
      return -1;
    }
    //3.找到第index -1的指针
    int i = 0;  
    node_t * temp = head->after;
    while(i < index -1)
    {
      temp = temp->after;
      i++;
    }
    node_t * temps = temp->after;
    //4.输出数据
    memcpy(data,temps->data,size);
    //5.修改指针
    temps->after->before = temp;
    temp->after = temps->after;
    //6.释放空间
    free(temps->data);
    free(temps);
    return 0;
}
//销毁链表
int destroy_dclinklist(node_t**head)
{
    if (*head== NULL || (*head)->after == *head)
    {
      /* code */
      perror("链表已经位空 或者不存在\n");
    }
    //便利
    //before指向第一个节点
    node_t * left = (*head)->after; //left在左边
    node_t* right= left;   //right在右边

    //分离头结点
    (*head)->after = *head;

    while(right != *head )
    {   
        //释放左边的节点空间
        right = left->after;
        free(left->data);
        free(left);
        left = right;
    }

    free(*head);
    *head = NULL;
    return 0;
}
#endif