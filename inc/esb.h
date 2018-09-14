#include	<assert.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<string.h>
#include	<arpa/inet.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<pwd.h>
#include	<grp.h>
#include	<sys/stat.h>
#include	<sys/ipc.h>
#include	<sys/sem.h>
#include	<sys/msg.h>
#include	<sys/shm.h>
#include 	<fcntl.h>
#include 	<sys/file.h>
#include 	<sys/time.h>
#include	<sys/signal.h>
#include	<signal.h>
#include	<setjmp.h>
#include  <dirent.h>
#include  <limits.h>

/* for linux
#include 	<stdarg.h>
#include <varargs.h> for AIX*/
#include <stdarg.h>
#include	<time.h>
#include	<sys/timeb.h>
#include	<netdb.h>

#include <ctype.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <libgen.h>
#include <sys/param.h>
#include <sys/sysmacros.h>
#include <sys/mman.h>

#include <sys/ioctl.h>
/*#include <sys/conf.h> fro AIX*/
#include <sys/wait.h>

#ifndef _ESB_H
#define _ESB_H 1
/********************ESB 常量定义**********************/
#define RESERVE       	'1'	     /*读后保留*/
#define UNRESERVE       '0'	     /*读后不保留*/
/********************ESB 常量定义**********************/
/********************ESB struct define**********************/
typedef struct{
	long mtype;
	unsigned char	mtext[512];
}EsbMsg;
typedef struct
{
	unsigned long iPackUnused;     /*未用空间首指针*/
	unsigned int iUnusedStore;     /*未用空间大小*/
	unsigned short iMailPackSize;  /*MailPack结构大小*/
	unsigned short iPackHeadSize;  /*PackHead结构大小*/
}PackHead;

typedef struct{
	unsigned int iLength;
	unsigned long lNextPosi;
	unsigned long lPrePosi;
	unsigned int iPackLength;
}MailPack;

/********************ESB struct define**********************/
/*
return code config
AXXXX
A:1--ESB;2--MSP;3--TSP;
*/
/********************ESB返回码*************************/
#define ESBTRUE		1	/*真*/
#define ESBFALSE	0       /*假*/
#define	ESBSUCCESS	10000   /*成功*/
#define	ESBFAIL		10001   /*失败*/
#define BUFSMALL        10002   /*存放报文的空间太小*/
#define STRNULL		10003   /*字符串为空*/
#define READTIMEOUT	10004	/*超时*/
/********************ESB返回码*************************/

#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__IBMC__)
#define __LIKE_PRINTF(fmt, arg) \
	__attribute__((__format__ (__printf__, fmt, arg)))
#define __LIKE_SCANF(fmt, arg) \
	__attribute__((__format__ (__scanf__, fmt, arg)))
#else
#define __LIKE_PRINTF(fmt, arg)
#define __LIKE_SCANF(fmt, arg)
#endif

/*define Process area */
int	InitPackMem(char *,int);
int 	WritePackMem ( char *, int , char *, unsigned int, unsigned long *);
int	ReadPackMem( char *,int, char *,char ,unsigned int *,unsigned long);
int	DelPackMem( char *, int	, unsigned long );
int	DisplUnusdPackMem( char *);
int	DisPackDetail( char *, unsigned long );
char	*CreateShm( key_t , int *, unsigned int );
char    *CreateMmapShm(const char *path, char *addr, size_t size);
unsigned short 	DeleteShm(int);
unsigned short 	CreateMsg(key_t , int *);
unsigned short 	DeleteMsg(int );
void		ClearMsg(int,long);
unsigned short	SendMsg(int,EsbMsg *,int);
unsigned short  RcvMsg(int,EsbMsg *,int *,int);
unsigned short 	CreateSem(key_t , int *);
unsigned short 	DeleteSem(int );
void 		*shmGet( key_t );
int		ESBP(int);
int		ESBV(int);
static	void	(*func)();
void		t_catch();
int esbDebug(char *frm, ...) __LIKE_PRINTF(1, 2);
int esbDebugerr(char *frm, ...) __LIKE_PRINTF(1, 2);
int     esbDebughex(char * ,short );
int     esbDebugStr(char * ,short );
int     esbDebugStrRaw(char * ,short );
int	StrReverse( char s[]);
int	StrLtoA(long,char s[]);
int	StrCmp(char *,char *);
int	StrReplace(char *,char *,char *,char *);
int	_Transfer(char *,short,char *);
int	_Untransfer(char *,char *,short *);
int	StrTrim(char *,char);
char*	StrTrim0(char *);
char* strcasestr(const char *s, const char *find);

void	 (*Signal(int sig, void (*func)(int)))(int);
char	*strftimeval(char *s, const char *fmt, const struct timeval *tp, int flag);
int		 DaemonInit(void);
int		 esbGetDateTime(char *date);
/*Process area sources*/


/********************ESB Global define**********************/
extern char	sgDebugFile[256];
extern EsbMsg	esbmsg;
extern long	igTrnid;
extern short	igLogLevel;

#if defined(_WITH_ORACLE_PROC_PARSE_NONE) || !defined(_WITH_ORACLE_PROC_PARSE)
#include "esb_log.h"
#endif

#endif /*_ESB_H*/
