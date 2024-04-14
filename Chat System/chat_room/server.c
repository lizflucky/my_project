#include"head.h"

char name1[128]="";
 char name[128]="";
int cid=-1;
typedef struct user
{
   char *name;
   int  cidnum;
   struct user *next;

}user_t;

user_t*head;

user_t * creat_list()
{
    user_t *head1=(user_t*)malloc(sizeof(user_t));
    if(head1==NULL)
        {
            perror("list error\n");
            return (user_t*)-1;
        }
    head1->next=NULL;
    head1->name=NULL;
    head1->cidnum=0;
    return head1;
}
int w_list(user_t *head,char *namedata,int cidnum)
{   
    if(head==NULL)
        {
            perror("head error\n");
            return -1;
        }
    user_t *newhead=(user_t*)malloc(sizeof(user_t));
    newhead->name = (char*)malloc(strlen(namedata) + 1);
    strcpy(newhead->name, namedata);
    newhead->cidnum=cidnum;
    user_t*tail=head;
    while (tail->next!=NULL) 
    {
        tail=tail->next;    
    }
    newhead->next=NULL;
    tail->next=newhead;
    
}
void show(user_t*head)
{
    user_t *temp=head->next;
    while(temp != NULL)
    {
        printf("%s\t  %d\t",temp->name,temp->cidnum);
        temp = temp->next;
    }
    printf("\n");
}
int delete_list(user_t*head,int cidnum1)
{
    user_t*temp=head->next,*before=head;
    while (temp!=NULL&&temp->cidnum!=cidnum1)
    {   
       before=temp;
       temp=temp->next;
    }
    if(temp==NULL)
        {
            perror("error\n");
            return -1;
        }
    before->next=temp->next;
    free(temp);
    temp=NULL;
}
int zhaocid(user_t *head,char *name)
{   
    if(head==NULL)
        {
            perror("123\n");
            return -1;
        }
    user_t*temp=head->next;
    while(temp!=NULL&&strcmp(temp->name,name)!=0)
    {
        temp=temp->next;
    }
    if(temp==NULL)
        {
            printf("没有改该用户\n");
              return -1;
        }
    return temp->cidnum;


}
char *zhaoname(user_t * head, int cid)
{

    if(head==NULL)
        {
            perror("123\n");
            return (char*)(-1);
        }
    user_t*temp=head->next;
    while(temp!=NULL&&temp->cidnum!=cid)
    {
        temp=temp->next;
    }
    if(temp==NULL)
        {
            char *name="没有改该用户";
            printf("没有改该用户\n");
            return name;
        }
    return temp->name;
}
void qufa(user_t *head,char *bufs1)
{
    user_t* temp=head->next;
     while (temp!=NULL)
        {  
            
        send(temp->cidnum,bufs1,strlen(bufs1),0);
        temp=temp->next;
        }

}

void *pthread_fun(void *arg)
{   
    int pcid=*(int*)arg; 
    char biaoshi[5]="";
    recv(pcid,biaoshi,sizeof(biaoshi),0);
    int flg=0;
    
    char bufs[128]="";
    char bufs1[128]="";
    char name3[128]="";
    char *name1=name3;
    name1=zhaoname(head,pcid);
    time_t tm=0;
    char *time1="";
    int fd1=open("liaotan.txt",O_APPEND|O_RDWR|O_CREAT,0666);
    while(1)
    {    
        time(&tm);
        time1=ctime(&tm);
        bzero(bufs1,sizeof(bufs1));
        strcat(bufs1,biaoshi);
        strcat(bufs1,name1);
        strcat(bufs1,":");
        bzero(bufs,sizeof(bufs));
        recv(pcid,bufs,sizeof(bufs),0);
        strncat(bufs1,bufs,strlen(bufs)-1);
        write(fd1,time1,strlen(time1));
        write(fd1,bufs1,strlen(bufs1));
         write(fd1,"\n",strlen("\n"));
        printf("%s\n",bufs1); 


      //退出
        if(!strncmp(bufs,"quit",4))
        {
            strcat(name1,"退出了聊天室\n");
            printf("%s\n",name1);
            delete_list(head,pcid);
            qufa(head,name1);
            show(head);
            break;
        }
        //发文件
          else if(!strncmp(bufs,"^",1))
            {   
                char bufw[512]="";
                bzero(bufw,sizeof(bufw));
                 printf("%s\n",bufw);
                int fd=open("shuju.txt",O_APPEND|O_RDWR); 
                read(pcid,bufw,sizeof(bufw));
                printf("%s\n",bufw);
                write(fd,bufw,strlen(bufw));
                write(fd,"\n",strlen("\n"));
                close(fd);
                if(!strcmp(bufw,"123"))
                {
                     send(pcid,"发送失败，你没有该文件",strlen("发送失败，你没有该文件"),0);
                
                }
                else 
              send(pcid,"发送成功",sizeof("发送成功"),0);
                bzero(bufs,sizeof(bufs));
            }
            //私发

        else if(!strncmp(bufs,"@",1))
            {   
                
                char *str=bufs;
                char buf1[16] = "";//@
                char buf2[16] = "";//私聊的名字
                char buf3[128] = "";//私聊的信息
                char buf[128]="";
                char bus[128]="";
                char buf4[16]="没有该用户";
                sscanf(str, "%s %s %s", buf1, buf2,buf3);
                strcat(buf,name1);
                strcat(buf,"私信给你:");
                strcat(buf,buf3);

                strcat(bus,"你私信给");
                strcat(bus,buf2);
                strcat(bus,":");
                strcat(bus,buf3);


                char *str1=buf2;
                int ppcid= zhaocid(head,str1);
                if(ppcid>0)
                {
                send(ppcid,buf,strlen(buf),0);
                send(pcid,bus,strlen(bus),0);
                }
                else
                    send(pcid,buf4,strlen(buf4),0);
                bzero(buf,sizeof(buf));
                bzero(bufs1,sizeof(bufs1));
            }
            //群发
        else if(strncmp(bufs,"quit",4))
            {
                qufa(head,bufs1);
                bzero(bufs1,sizeof(bufs1));
            }
            
    }
    shutdown(pcid,SHUT_RDWR);
    close(fd1);
    pthread_exit((void *)0);
}


int main(int argc,char ** argv)
{
    system("clear");
    if(argc <3 )
    {
        perror("使用: ./a.out + iP地址 + 端口号\n");
        return -1;
    }
    int ret = 0;
    /****  1创建套接字 ****/
    int sid = socket(AF_INET,SOCK_STREAM,0);

    /***** 2绑定自己ip和端口   *****/
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
   saddr.sin_port = htons(atoi(argv[2]));
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    ret = bind(sid,(struct sockaddr *)&saddr,sizeof(saddr));
    if( ret < 0 )
    {
        perror("绑定失败..\n");
        return -1;
    }
    listen(sid,10);
    struct sockaddr_in caddr;
    int size = sizeof(caddr);
    pthread_t tid = 0;
    head=creat_list();
    printf("等待用户连接..........\n");
    while (1)
    {   
        cid = accept(sid,(struct sockaddr*)&caddr,&size);
        recv(cid,name,sizeof(name),0);
        char *name2=name;
        w_list(head,name2,cid);
        show(head);
        if(cid>0)
        {
        pthread_create(&tid,NULL,pthread_fun,&cid);
        pthread_detach(tid);
        }  
        strcat(name,"加入聊天室\n");
        qufa(head,name);
        bzero(name,sizeof(name));
        
    }      
    return 0;
}