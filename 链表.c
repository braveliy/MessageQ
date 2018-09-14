#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_linkedlist.h"

link_user head = NULL;

/**************************************************************
函数名：mystrlen
函数功能：计算以‘\n’或'\-'或'-'结束字符串的长度
输入参数：被测试字符串的首地址
返回值：被测试的长度
**************************************************************/
int mystrlen(const char *src)
{
    int len = 0;
    if (src == NULL)
    {
        return 0;
    }
    //while ((*s != '\n') && (*s != '\0') && (*s != ' ') && (*s != ':'))
    while((*src != '-') && (*src != '\n') && (*src != '\0'))
    {
        src++;
        len++;
    }
    return len;
}


/**************************************************************
函数名：make_node
函数功能：创建一个链表节点
输入参数：buf,为用户ID号和密码的字符串
          user_status,为用户的状态，初始化为下线
返回值：创建成功节点的首地址
**************************************************************/
link_user make_node(char *buf, int user_status)
{
    link_user p;
    int len_ID = 0,len_password;
    p = (link_user)malloc(sizeof(*p));
    if (p == NULL)
    {
        perror("out of memory.\n");
        return NULL;
    }
    memset(p,0,sizeof(*p));
    len_ID = mystrlen(buf);
    strncpy(p->user_ID, buf, len_ID);
    *((p->user_ID) + len_ID) = '\0';
                                            
    len_password = mystrlen(buf + len_ID + 1);
    strncpy(p->user_password, (buf + len_ID + 1), len_password);
    *((p->user_password) + len_password) = '\0';

    p->user_status = user_status;
    p->connfd = -1;         //将用户与服务器间的套接字赋值为-1(无效)
    p->silence = -1;        //初始化所有用户正常-1，禁言1
    p->kicked_out = 0;     //初始化所有用户被踢出标志为0
        
    /* 如果监测到为作者ID则初始化为管理员权限 */
    if (strncmp(p->user_ID, "pyfadmin", 8) == 0)    
        p->administrator = 1;   //初始化作者用户为管理员用户
    else 
        p->administrator = -1;   //初始化其他所有用户为普通用户
        
    p->next = NULL;
    return p;
}

/**************************************************************
函数名：free_node
函数功能：释放一个链表节点
输入参数：p，为被释放节点的首地址
返回值：空
**************************************************************/
void free_node(link_user p)
{
    free(p);
}


/**************************************************************
函数名：insert
函数功能：插入一个节点
输入参数：p，为被插入节点的首地址
返回值：空
**************************************************************/
void insert(link_user p)
{
    p->next = head;
    head = p;
}

/**************************************************************
函数名：traverse
函数功能：遍历链表中的ID号和密码信息
输入参数：空
返回值：空
**************************************************************/
void traverse(void)
{
    link_user p;
    turn_node();
    printf("\n");
    int len_ID;
    for (p = head; p; p = p->next)
    {
        len_ID = strlen(p->user_ID);
        if (len_ID <= 12)
            printf("ID:%s\t\tpassword:%s\n", p->user_ID, p->user_password);
        else
            printf("ID:%s\tpassword:%s\n", p->user_ID, p->user_password);
    }
    printf("\n");
    turn_node();
}

/**************************************************************
函数名：turn_node
函数功能：将链表中的所有节点颠倒一次顺序
输入参数：空
返回值：空
**************************************************************/
void turn_node(void)
{
    if((head == NULL) ||(head->next == NULL))
        return;
    link_user p1,p2,p3;
    p1 = head;
    p3 = p1->next;
    p1->next = NULL;
    while(p3->next != NULL)
    {
        p2 = p3;
        p3 = p3->next;
        p2->next = p1;
        p1 = p2;
    }
    p3->next = p1;
    head = p3;
}

int search_status(link_user p)
{
    printf("***user status : %d\n",p->user_status);
    return p->user_status;
}

/**************************************************************
函数名：search_connfd
函数功能：查找与所给通信描述符相同的节点
输入参数：sockfd，为与客户端进行通信的套接字描述符
返回值：查找到的节点的首地址
**************************************************************/
link_user search_connfd(int sockfd)
{
    link_user p;
    for (p = head; p; p = p->next)
    {
        if (p->connfd == sockfd)
        {
            return p;
        }
    }
    return NULL;
}

/**************************************************************
函数名：search_ID
函数功能：查找与所给用户ID相同的节点
输入参数：msg，为需要查找用户的ID
返回值：查找到的节点的首地址
**************************************************************/
link_user search_ID(char *msg)
{
    link_user p;
    int len = 0;
    len = mystrlen(msg);
    for (p = head; p; p = p->next)
    {
        if ((strncmp(p->user_ID, msg, len) == 0) && (strlen(p->user_ID) == len))
        {
            return p;
        }
    }
    return NULL;
}

/*************************************************************/
/* 文件名：server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define SERV_PORT 8000       //端口号

int main(int argc, char **argv)
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;        //socket描述符
	int nready, client[FD_SETSIZE];      //client存储客服端描述符的数组 
	ssize_t n; 
	fd_set temp_set, allset;      //定义集合  fd_set定义储存文件描述符的
	char str[INET_ADDRSTRLEN];   //ipv4地址字符串的长度   
	socklen_t cliaddr_len;       //客服端ip长度
	struct sockaddr_in	cliaddr, servaddr;  //存放IP的结构体

	//link_user p;
    link_msg from_client_message;         //定义来自客户端的消息结构体
    from_client_message = (link_msg)malloc(sizeof(*from_client_message));
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	Listen(listenfd, 20);
    
    maxfd = listenfd;		/* initialize */
	maxi = -1;			/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;	/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);       //将listenfd放入allset集合中

	for ( ; ; ) 
	{
		temp_set = allset;	/* structure assignment 结构分配*/
        /*temp_set哪几个变化，temp_set为输入输出参数。nread变化的数量*/
		nready = select(maxfd+1, &temp_set, NULL, NULL, NULL);  //select阻塞中，time置NULL
		if (nready < 0)
			perr_exit("select error");
        //printf("nready : %d\n",nready);

		if (FD_ISSET(listenfd, &temp_set)) 
		{ /* new client connection */
			cliaddr_len = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

			if (MONITORING_CLIENT_OPEN)
                printf("IP为 %s 端口号为 %d 的客户端已开启\n",
			       inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			       ntohs(cliaddr.sin_port));
            from_client_message->feedback = MONITORING_CLIENT_HEALTH_TIPS;
            Write(connfd, from_client_message, sizeof(*from_client_message));
            memset(from_client_message, 0, sizeof(*from_client_message));

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) 
                {
					client[i] = connfd; /* save descriptor */
					break;
				}
            //printf("client number : %d\n",i);
			if (i == FD_SETSIZE) {
				fputs("too many clients\n", stderr);
				exit(1);
			}

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd; /* for select */
			if (i > maxi)
				maxi = i;	/* max index in client[] array */
            
			if (--nready == 0)
				continue;	/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) 
        {	/* check all clients for data */
            if ( (sockfd = client[i]) < 0)
				continue;
			//printf("maxi : %d\n",maxi);
			if (FD_ISSET(sockfd, &temp_set)) 
            {
				if ( (n = Read(sockfd,from_client_message, sizeof(*from_client_message))) == 0) 
                {
			        if (MONITORING_CLIENT_CLOSE)
			            printf("ID地址为 %s 端口号为 %d 的客户端已关闭\n",
			                inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			                ntohs(cliaddr.sin_port));
					/* connection closed by client */
					FD_CLR(sockfd, &allset);
					client[i] = -1;
					Close(sockfd);
				} else 
                {
                    message_processing(sockfd, from_client_message, fd);
				}

				if (--nready == 0)
					break;	/* no more readable descriptors */
			} //end if
		}  //end for
	} //end for(;;)
} //end main


