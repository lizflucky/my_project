#include"head.h"
extern user_t user[];
extern flight_t flm[20];


int main()
{
 unode_t *userhead1= create_userlist();//存储用户信息的链表
 insert_hclinklist(userhead1, user);//插入用户信息
node_t * flighthead=create_flightlinklist();//创建航班链表
 
  int choice;
    do {
        
        userdisplayMenu();//用户菜单
        printf("请输入选择: ");
        scanf("%d", &choice);
        switch (choice) 
        {
            case 1:
            login(userhead1);//用户登录
                break;
            case 0:
                printf("退出登录\n");
                break;
            default:
                printf("无效选择，请重试。\n");
        }
    } while (choice != 0);
 
return 0;
}