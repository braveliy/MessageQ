#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_linkedlist.h"

link_user head = NULL;

/**************************************************************
��������mystrlen
�������ܣ������ԡ�\n����'\-'��'-'�����ַ����ĳ���
����������������ַ������׵�ַ
����ֵ�������Եĳ���
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
��������make_node
�������ܣ�����һ������ڵ�
���������buf,Ϊ�û�ID�ź�������ַ���
          user_status,Ϊ�û���״̬����ʼ��Ϊ����
����ֵ�������ɹ��ڵ���׵�ַ
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
    p->connfd = -1;         //���û������������׽��ָ�ֵΪ-1(��Ч)
    p->silence = -1;        //��ʼ�������û�����-1������1
    p->kicked_out = 0;     //��ʼ�������û����߳���־Ϊ0
        
    /* �����⵽Ϊ����ID���ʼ��Ϊ����ԱȨ�� */
    if (strncmp(p->user_ID, "pyfadmin", 8) == 0)    
        p->administrator = 1;   //��ʼ�������û�Ϊ����Ա�û�
    else 
        p->administrator = -1;   //��ʼ�����������û�Ϊ��ͨ�û�
        
    p->next = NULL;
    return p;
}

/**************************************************************
��������free_node
�������ܣ��ͷ�һ������ڵ�
���������p��Ϊ���ͷŽڵ���׵�ַ
����ֵ����
**************************************************************/
void free_node(link_user p)
{
    free(p);
}


/**************************************************************
��������insert
�������ܣ�����һ���ڵ�
���������p��Ϊ������ڵ���׵�ַ
����ֵ����
**************************************************************/
void insert(link_user p)
{
    p->next = head;
    head = p;
}

/**************************************************************
��������traverse
�������ܣ����������е�ID�ź�������Ϣ
�����������
����ֵ����
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
��������turn_node
�������ܣ��������е����нڵ�ߵ�һ��˳��
�����������
����ֵ����
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
��������search_connfd
�������ܣ�����������ͨ����������ͬ�Ľڵ�
���������sockfd��Ϊ��ͻ��˽���ͨ�ŵ��׽���������
����ֵ�����ҵ��Ľڵ���׵�ַ
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
��������search_ID
�������ܣ������������û�ID��ͬ�Ľڵ�
���������msg��Ϊ��Ҫ�����û���ID
����ֵ�����ҵ��Ľڵ���׵�ַ
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
/* �ļ�����server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define SERV_PORT 8000       //�˿ں�

int main(int argc, char **argv)
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;        //socket������
	int nready, client[FD_SETSIZE];      //client�洢�ͷ��������������� 
	ssize_t n; 
	fd_set temp_set, allset;      //���弯��  fd_set���崢���ļ���������
	char str[INET_ADDRSTRLEN];   //ipv4��ַ�ַ����ĳ���   
	socklen_t cliaddr_len;       //�ͷ���ip����
	struct sockaddr_in	cliaddr, servaddr;  //���IP�Ľṹ��

	//link_user p;
    link_msg from_client_message;         //�������Կͻ��˵���Ϣ�ṹ��
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
	FD_SET(listenfd, &allset);       //��listenfd����allset������

	for ( ; ; ) 
	{
		temp_set = allset;	/* structure assignment �ṹ����*/
        /*temp_set�ļ����仯��temp_setΪ�������������nread�仯������*/
		nready = select(maxfd+1, &temp_set, NULL, NULL, NULL);  //select�����У�time��NULL
		if (nready < 0)
			perr_exit("select error");
        //printf("nready : %d\n",nready);

		if (FD_ISSET(listenfd, &temp_set)) 
		{ /* new client connection */
			cliaddr_len = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

			if (MONITORING_CLIENT_OPEN)
                printf("IPΪ %s �˿ں�Ϊ %d �Ŀͻ����ѿ���\n",
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
			            printf("ID��ַΪ %s �˿ں�Ϊ %d �Ŀͻ����ѹر�\n",
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


