#include"head.h"


char bufn[128] = "";
static sem_t sem;
int value=0;
int cid=-1;
char b[12];



void display(char chat[][256],char* name)//聊天界面
{

    time_t tm1 = 0;
    char * timstr =NULL;
    tm1 = time(NULL);
    timstr = asctime(localtime(&tm1));
    system("clear");
    printf("------------------------聊天室------------------------------\n");
    printf("           %s\n",timstr);
    for(int i = 0;i<20;i++)
    {   
        if(strlen(chat[i])!=0)
        {
                printf("%s",chat[i]);
        }
        
        else
            printf("\n");
    }
    printf("---------------------------------------------------------\n");
    printf("私信：@+空格+名字+空格+内容\n");
    printf("上传文件：^+空格+文件名\n");
    printf("%s%s:",b,name);
    fflush(stdout);
}
void  sigfun(int signo)
{   
   

    if(signo==SIGINT)
    {   
        system("sl");
    }
  
}
int main(int argc, char const *argv[])
{
     if(argc <3 )
    {
        perror("使用: ./a.out + 服务器IP地址 + 服务器端口号\n");
        return -1;
    }
    int ret = 0;
    //1建立套接字：客户端只有一个套接字。就是cid，把客户端cid与对面的绑定在一起。
    cid = socket(AF_INET,SOCK_STREAM,0);
    if (cid < 0)
    {
        perror("socket error\n");
        return -1;
    }
    sem_init(&sem,0,1);

    
    struct sockaddr_in caddr={0};
    caddr.sin_addr.s_addr=inet_addr(argv[1]);
    caddr.sin_family=AF_INET;
    caddr.sin_port=htons(atoi(argv[2]));
    ret = connect(cid,(struct sockaddr * )&caddr,sizeof(caddr));
    if(ret < 0 )
    {
        printf("连接失败..\n");

    }
    char bufs[128] = "";
    char bufn1[128] = "";
    char bufm[128] = "";
    char chat[20][256] = {0};
    int i,j=0;
    int len = 0 ;
    fd_set rest={0};
    struct timeval tm ={0};
    int count=0;
    char name[16] = "";   
  
    printf("请输入你在聊天室中的名字：");
    scanf("%s",name);
    send(cid,name,strlen(name),0);
    printf("请输入你的标识符");
    scanf("%s",b);
    send(cid,b,strlen(b),0);
    printf("欢迎加入聊天室\n");
    while(1)
    {
        signal(SIGINT,sigfun);
        signal(SIGTSTP,sigfun);
        FD_SET(STDIN_FILENO,&rest);
        FD_SET(cid,&rest);
        tm.tv_sec=3;
        count=select(cid+1,&rest,NULL,NULL,&tm);
        if(FD_ISSET(STDIN_FILENO,&rest))
        {
            bzero(bufs,sizeof(bufs));
            read(STDIN_FILENO,bufn,sizeof(bufn));

            strncat(bufs,bufn,strlen(bufn));
            if(!strncmp(bufn,"quit",4))
            {
            send(cid,bufn,strlen(bufn),0);    
            printf("客户端正在退出...\n");
           break;
            }
            if(!strncmp(bufn,"^",1))
                {
                    char * str=bufn;
                    char buf[12]="";
                    char buf1[16]="";
                    char bufw[1024]="";
                    bzero(bufw,sizeof(bufw));
                    sscanf(str,"%s %s",buf,buf1);
                    send(cid,buf,strlen(buf),0);
                    bzero(buf,sizeof(buf));
                    int fd=open(buf1,O_RDWR);
                    if(fd<0)
                        {
                            printf("没有该文件\n");
                             send(cid,"123",strlen("123"),0);
                        }
                    read(fd,bufw,sizeof(bufw));
                    send(cid,bufw,strlen(bufw),0);
                    close(fd);
                }
                else
                    send(cid,bufs,strlen(bufs),0);
              bzero(bufn,sizeof(bufn));
        }
        if(FD_ISSET(cid,&rest))
        {
            bzero(bufn1,sizeof(bufn1));
            len=recv(cid,bufn1,sizeof(bufn1),0);  
           
            strcat(bufn1,"\n");
              if(i<19)
            {
                strcpy(chat[i],bufn1);
                i++;
            }
            else
            {
                for(j = 0;j < 19;j++)
                {
                    strcpy(chat[j],chat[j+1]);
                }
                strcpy(chat[j],bufn1);
            }
            display(chat,name); 
        }
    }
    signal(SIGQUIT,SIG_DFL);
    close(cid);
    return 0;
}