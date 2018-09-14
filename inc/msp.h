#include <stdarg.h>

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
#include  	<dirent.h>
#include  	<limits.h>
#include	<time.h>
#include	<sys/timeb.h>
#include	<netdb.h>

/**********************请求类型*******************************/
#define iATTACHNUM	10000	/*按邮箱号MSP_Attach请求	*/
#define iDETACH		20000	/*按邮箱号MSP_Detach请求	*/
#define iATTACHNAME	11000	/*按邮箱名MSP_Attach请求	*/
#define iATTACHREQ	12000	/*申请邮箱MSP_Attach请求	*/
#define iPUTMSG		30000	/*MSP_Put_Msg请求*/
#define iGETMSG		40000	/*MSP_Get_Msg请求*/
#define iSENDMSG	50000	/*发送邮件请求*/
#define iDEALSUCC	51000	/*邮件处理成功*/
#define iDEALFAIL	52000	/*邮件处理成功*/
#define iRESEND		53000	/*断点续传请求*/
#define iDELMAIL	54000   /*删除邮件请求*/
#define iLOGIN		60000	/*签到请求*/
#define iLOGOUT		61000	/*签退请求*/
#define iMSGSTATUS	64000	/*消息状态*/

/********************MSP_KNL返回码*************************/
#define	MSPSUCCESS		10000   /*成功*/
#define	MSPFAIL		20001   /*失败*/
#define ERR_FINDMB      20002	/*查找邮箱失败*/
#define NOMB		20003   /*无此邮箱*/
#define WPROCFAIL       20004   /*写进程空间失败*/
#define ERR_ENGROSS	20005   /*邮箱已被独占*/
#define ERR_FINDEPTMB   20006   /*查找空邮箱空间失败*/
#define DELPROCFAIL     20007   /*删除进程空间失败*/
#define NOTATTACH	20008   /*原邮箱没有attach*/
#define NOTXGROUP	20009   /*非交叉组*/
#define WMBFAIL		20010   /*写邮箱失败*/
#define RMBFAIL		20011   /*读邮箱失败*/
#define DELMAILFAIL	20012   /*删除邮件失败*/
#define NOMAIL		20013   /*邮箱中没有邮件*/
#define DELMBFAIL	20014   /*无此邮箱*/
#define NOSPACE		20015   /*共享内存中没有邮件头空间*/
#define LINKMBMEMERR	20016   /*连接邮箱共享内存失败*/
#define NOPROC		20017   /*无此进程*/
#define NAMEERR         20018   /*文件名错误*/
#define PARERR          20019   /*参数错误*/
#define NOTFILE          20021   /*文件不存在或没有权限*/
#define READQUEERR      20022   /*读消息队列失败*/
#define WRITEQUEERR     20023   /*读消息队列失败*/
#define SHORTSPACE      20024   /*用户缓存区太小*/
/*********************其它**************************/
#define COMPRESSSIZE	8192    /*需要压缩的报文大小*/

#define MAXMBNUM	1000         /*最大邮箱数*/
#define	LICENSESS	16			
#define MAXPROCNUM	65535        /*最大进程数*/
#define PROCTIMEOUT     3600	     /*用户进程的超时时间*/
#define SHARE           '0'	     /*邮箱共享*/
#define ENGROSS         '1'          /*邮箱独占*/
#define UNRESERVE       '0'	     /*邮件读后不保留*/
#define RESERVE         '1'	     /*邮件读后保留*/
#define INSHM		0    	     /*报文在共享内存中*/
#define INFILE		1	     /*报文在文件中*/
#define CHECKPROC	1	     /*是否检查进程Attach*/
#define NOCHECKPROC	0	     /*不需要进程check*/
#define FILENAMELEN      512         /*文件名长度*/
#define	REALTIME	1	     /*实时不可靠*/
#define	NOREALTIME	0	     /*可靠非实时*/
#define TSPREALTIME	2	     /*交换平台专用类型*/
#define TMPMB		0	     /*临时邮箱*/
#define LONGMB		1	     /*永久邮箱*/
#define SENDMAIL 	1
#define CHECKLINK  	2
#define LINKSUCC 	3
#define LINKFAIL 	4
#define KEYSIZE         8            /*密钥长度*/
#define MACSIZE         8            /*MAC长度*/
#define	MAXBUFLEN	8192
#define MSPERROR		-1

#ifndef MSPTRUE
	#define MSPTRUE        1
#endif
#ifndef MSPFALSE
	#define MSPFALSE       0
#endif


#ifndef strmove
#define strmove( s1, s2, nmove ) ( (strncpy(s1,s2,nmove) ), (s1[nmove] = '\0') )
#endif
/************************Msg_Send错误码*************************/
#define MSND_FILE_ERR		30001   /*  文件读写出错 */
#define MSND_SHM_ERR 		30002   /*  共享内存出错 */
#define MSND_MSGQ_ERR 		30003   /* 消息队列出错 */
#define MSND_SOCK_ERR 		30004   /*  Socket出错 */
#define MSND_TIMEOUT 		30005   /* 超时出错    */
#define MSND_OTHER_ERR 		30006   /* 其它错误 */

/************************Msg_RCV错误码**********************/
#define MSRV_FILE_ERR 		40001   /*  文件读写出错 */
#define MSRV_SHM_ERR 		40002   /*  共享内存出错 */
#define MSRV_MSGQ_ERR 		40003   /* 消息队列出错 */
#define MSRV_SOCK_ERR 		40004   /*  Socket出错 */
#define MSRV_TIMEOUT 		40005   /* 超时出错 */
#define MSRV_OTHER_ERR 		40006   /* 其它错误 */

/****************************客户端配置结构****************************/
typedef struct
{
	char	HostIp[20];		/*邮箱服务器Tcp地址*/
	char	HostPort[5+1];		/*邮箱服务器Port口*/
	char	ErrLogPath[40];		/*错误记录目录*/
	char	SysLogPath[40];		/*系统日志目录*/
	char	LogNum[5+1];		/*Log最大行数*/
	char	FrmSize[8];		/*每帧报文的大小(不包含数据报文头)*/
}CLIENTCFG ;

/****************************公共信息配置结构****************************/
typedef struct {
	char	MaxMail[10];		/*MSP管理最大邮件数*/
	char	MaxApp[5+1];  		/*最大管理进程数*/
	char	TmpMbNum[5+1];		/*最大可申请临时邮箱数*/
	char    MailShmNum[8];		/*整个Mail存放共享内存大小块数*/
	char	LogNum[5];		/*Log最大行数*/
	char	BlockSize[8];		/*块的大小*/
}SHARECFG;

/****************************节点配置结构****************************/
typedef  struct {
	char	NodeName[10];           /*节点名*/
	short	iNodeId;		/*节点号*/
	char	NodeIp[20];		/*节点ip 地址*/
	short	iNodePort;		/*节点Port口*/
	short	iFrmSize;		/*每帧报文的大小(不包含数据报文头)*/
    	short   iTimeOut;             	/*超时时间*/
    	char    NodeList[185];          /*转发节点id列表*/
	int	iSocketFd;
}NODECFG;

/****************************邮箱配置结构****************************/
struct MAILBOXCFG{
	char	MailBoxId[5+1];		/*箱号*/
	char	MailBoxName[10];	/*邮箱名*/
	char	MbGrpId[5+1];		/*所属组号*/
	char	MaxMail[5+1];		/*邮箱最大邮件数*/
	char	CpsFlag[1+1];		/*是否支持压缩*/
	char	CryptFlag[1+1];		/*是否支持加密*/
	char	ShareFlag[1+1];		/*是否独占标志*/
	char	MailTimeOut[6+1];	/*报文最大存放时间 2010-11-4 add by hqz*/
	char	DieMqId[6+1];		/*死信队列号 2010-11-4 add by hqz*/
	struct MAILBOXCFG	*pre;
	struct MAILBOXCFG	*next;
};

/****************************公共描述区****************************/
typedef	struct{
	unsigned int   iMaxStoreNum;       /*邮件存储区共享内存块数*/
	unsigned int   iBlockSize;
/*	unsigned int   iCurUsdNum;-del by hqz	邮件存储区当前占用内存空间*/
	unsigned int   iMaxMbNum;	    /*最大管理邮箱数*/
	unsigned int   iMaxMailNum;	    /*最大管理邮件数*/
	unsigned short iMaxAttachNum;	    /*最大连接进程数*/
	unsigned short iCurMbNum;	    /*当前邮箱数*/
	unsigned int   iCurMailNum;	    /*当前管理邮件数*/
	unsigned short iCurAttachNum;	    /*当前联接进程数*/
	unsigned short iInQueFlag;          /*入口消息队列是否有报文标志*/
	int            iPubShmId;           /*公共描述区共享内存Id*/
	int            iPubSemId;           /*公共描述区信号灯Id*/
	int            iMailShmId;          /*邮箱和节点共享内存ID*/
	int	       iMailSemId;	    /*邮箱描述区信号灯Id*/
	int            iBodySemId;          /*邮件信号灯Id*/
	int            iToProc;             /*进程描述区偏移*/
	int            iToMailHead;         /*邮件头描述区偏移*/
	int            iToMailBody;         /*邮件体描述区偏移*/
	int 	       iInQueId;	    /*核心入口消息队列Id*/
	int	       iOutQueId;	    /*核心出口消息队列Id*/
	unsigned int   iReqFileSer;         /*文件序号*/
	unsigned int   iFstEmptyMH;	    /*空邮件头首位置*/
	/* -del by hqz 20060703
	unsigned int   iFstEmptyMB;	    空邮件体首位置*/
	unsigned int   iFstEmptyProc;	    /*空进程链首位置*/
	unsigned int   iFstUsdProc;	    /*已用进程首位置*/
	long	       iLogNum;             /*Log最大行数*/
}Msp_Pub_Tab;
/****************************邮箱描述区****************************/
typedef struct{
	char	       cMbDscFlag;      /*邮箱使用标志*/
	char	       cPermFlag;	/*邮箱是否永久邮箱:1:永久；0:临时*/
	unsigned short iMbId;	        /*邮箱号*/
	char	       sMbName[10];     /*邮箱名	*/
	unsigned short iMbGrpId;	/*邮箱的组号*/
	signed long    iMailTimeOut;	/*报文最大存放时间 2010-11-4 add by hqz*/
	unsigned short iDieMqId;	/*死信队列号 2010-11-4 add by hqz*/	
	unsigned int   iFstMail;	/*本邮箱第一封邮件位置*/
	unsigned short iProcNum;        /*本邮箱联接进程数*/
	unsigned int   iMaxMailNum;	/*邮箱最大邮件数*/
	unsigned int   iMailNum;	/*邮箱当前邮件数*/
	unsigned int   iMailSize;	/*邮箱当前邮件占用空间*/
	unsigned int   iRcvMailNum;	/*邮箱接收邮件数*/
	unsigned int   iRcvMailSize;	/*邮箱接收数据大小*/
	unsigned int   iSndMailNum;	/*邮箱发送邮件数*/
	unsigned int   iSndMailSize;	/*邮箱发送数据大小*/
	char           cMbOpenMode;     /*邮箱打开模式（共享与独占)*/
	char	       cCompressFlag;	/*邮件存储数据是否压缩*/
	char	       cRecryptFlag;	/*邮件存储数据是否加密*/
}Msp_Mb_Tab;
/****************************邮件描述区****************************/
typedef struct {
	unsigned short iSrcMbId;	/*源邮箱号*/
	unsigned short iSrcGrpId;	/*源邮箱的组号*/
	unsigned short iDstMbId;	/*目标邮箱号*/
	unsigned short iDstMbGrpId;	/*目标邮箱的组号*/
	unsigned int   iMailNum;        /*邮件号*/
	unsigned short ipriority;	/*邮件Priority*/
	unsigned short iclass;		/*邮件Class*/
	int	       iType;		/*邮件Type*/
	char	       cStorageFlag;	/*邮件存放位置*/
	unsigned int   iMailSize;	/*邮件大小*/
	signed long    iMailInTime;	/*邮件进入时间 2010-11-4 add by hqz*/
	signed long    iMailTimeOut;	/*邮件超时时间 2010-11-4 add by hqz*/
	unsigned short iDieMqId;	/*死信队列号 2010-11-4 add by hqz*/
	unsigned long   iMailBeginPosi;	/*邮件存放起始位置*/
	unsigned int   iMailPosi;	/*本邮件位置*/
	unsigned int   iNextMail;	/*下一邮件位置*/
}Msp_Mail_Tab;

/****************************进程描述区****************************/
typedef struct  {
	int	       iSeriNo;		/*序号*/
	int	       iUserPid;	/*进程号*/
	char           sUserName[15+1];	/*用户名*/
	unsigned short iProcBlockFlag;	/*本进程阻塞标志*/
	unsigned short iMbIdPosi;      	/*邮箱位置*/
	long	       lBeginTime;	/*进程上次操作时间*/
	unsigned short iNextPosi;	/*下一进程位置*/
}Msp_Proc_Tab;

/****************************节点表****************************/
typedef struct {
	char	       sNodeName[7+1];	/*节点名*/
	unsigned short iNodeId;	 	/*节点号*/
	char	       sNodeTcpip[16];	/*节点IP Address*/
	char	       sNodePort[5+1];	/*节点TCP port*/
	unsigned short iCurLinkNum;	/*链路数*/
	short          iTimeOut;        /*超时时间*/
	short	       iFrmNum;		/*窗口包含帧数*/
	short	       iFrmSize;	/*每帧报文的大小(不包含数据报文头)*/
    	int            iNodePid;        /*进程号1*/
}Msp_Node_Tab;

/****************************Saf与Send的接口****************************/
typedef struct{
	unsigned short iReqType;	     /*请求类型*/
	unsigned int   iMailPosi;	     /*邮件位置*/
	unsigned int   iMailNo;		     /*邮件序号*/
}Msp_Saf_Tab;

/****************************消息传递控制头****************************/
typedef struct{
	int		iType;		     /*邮件Type*/
	int		iResp_code;          /* 返回码*/
	unsigned short	iReqType;	     /*请求类型*/
	unsigned short	iSrcMbId;	     /*源邮箱号*/
    	unsigned short  iSrcGrpId;           /*原邮箱组号*/
    	unsigned short	iSrcBusId;	     /*源邮箱bus号*/
	unsigned short	iDstMbId;	     /*目的邮箱号*/
	unsigned short	iDstMbGrpId;	     /*目的邮箱组号*/
	unsigned short	iPriority;	     /*邮件优先级别*/
	unsigned short	iClass;		     /*邮件Class*/
	unsigned short  iTimeOut;            /*超时时间*/
	char		sSrcMbName[20];	     /*源邮箱名*/
	unsigned char	sMacCheck[KEYSIZE];  /*Mac校验*/
	char            cFlag;               /*是否独占、实时、读后保留*/
}Msp_Msg_Head;
/****************************消息体****************************/
typedef struct {
	unsigned char FileName[10];
	unsigned int  iMsgLength;
}Msg_Request;
typedef struct {
	unsigned int	iCurDataPosi;	     /*当前邮件位置*/
	unsigned int    iCurSize;            /*本帧数据的大小*/
	char		cEndFlag;	     /*邮件传递标志*/
	unsigned char	sMacCheck[MACSIZE];  /*Mac校验*/
}Msg_Body;
typedef struct {
	unsigned int  iRetPosi;
}Msg_Return;

/*****************************全局变量*****************************/
extern Msp_Pub_Tab  		*pgPubDsc;	            /*公共描述区结构变量指针*/
extern unsigned short		igBusId;	            /*本Bus号*/
extern unsigned char 		*pgMspmsg;	            /*传递报文结构指针*/
extern unsigned char 		*pgMbHead;	            /*邮箱起始地址*/
extern unsigned char 		*pgMailHead;	            /*邮件头起始地址*/
extern unsigned char 		*pgProcHead;	            /*进程起始地址*/
extern unsigned char 		*pgMailBody;	            /*邮箱体起始地址*/
extern unsigned char 		*pgNodeHead;	            /*结点起始地址*/
extern char    		cgShutdownFlag;               /*关闭标志*/

extern SHARECFG        	shacfg;
extern struct MAILBOXCFG 	mailboxcfg;

extern unsigned int		igBlockSize;
extern unsigned short		igNodeId;
extern unsigned short		igSrcMbId,igSrcGrpId,igSrcMailBoxPosi,igProcPosi;
extern pid_t			igUsrPid;
extern unsigned int		igSendMsgLen;
extern char			sgMspPath[256];

/*****************************函数声明*****************************/
int Msp_Cls_Child
(
	int			/*  套接字描述符		*/
);

/* add by yl 20060704 */
short Msp_attach(
  unsigned short *iSrcMbId, 
  char 		*MailBoxName, 
  unsigned short iSrcGrpId,
  unsigned short iBusId,
  unsigned short iAttachType
);

short Msp_Put_Msg(
char 		*sMsg, 
unsigned int		iMsgSize,
char            cPutType,	
unsigned short  iDstMbId, 
unsigned short  iDstMbGrpId, 
unsigned short	iPriority,
unsigned short	iClass,
int		iType,
char            cStoreFlag
);

short Msp_Get_Gate(
char 		*sMsg, 
unsigned int	*iUsrLen,	
unsigned short  *iSrcMbId, 
unsigned short  *iSrcGrpId, 
unsigned short	*iPriority,
unsigned short	*iClass,
int		*iType,
char		*cStoreFlag,
char            cReserve,
short  		iTimeOut,
Msp_Msg_Head	*stpHead
);

short Msp_Get_Msg
(
char 		*sMsg, 
unsigned int	*iUsrLen,	
unsigned short  *iSrcMbId, 
unsigned short  *iSrcGrpId, 
unsigned short	*iPriority,
unsigned short	*iClass,
int		*iType,
char		*cStoreFlag,
char            cReserve,
short  		iTimeOut
);

short Msp_Detach();
short Msp_Detach_Proc(pid_t ilPid);
short Msp_Detach_Async(void);

unsigned short ReadMailHead
(
	unsigned int 	*ilFstMail, 
	Msp_Msg_Head	*stpHead,
	int 		*iUsrLen,
	unsigned int	*ilFstBkId,
	char		*slFileName,
	char		*cStorageFlag,
	char		cReserve,
	unsigned short iDieMq
);

unsigned short DelMailHead(unsigned int iPosi);

unsigned short ReadMemList
( 
	char 		*msg,
	int 		msgsize, 
	unsigned int 	iFstbkid,
	char		cStorageFlag
);

unsigned short ReadFileList
( 
	 char 	*msg,
	int 	msgsize, 
	char	*sFileName,
	char	cStorageFlag
);

unsigned short WriteMemList
( 
	char *msg, 
	int msgsize, 
	unsigned int *iFstbkid
);

unsigned short DeleteMemList( unsigned int iFstbkid);
unsigned short BlockRead(unsigned short ilProcPosi, short iTimeOut, int *iTimeUsed);

short Msp_attach_Gate(
	int		*iSocketFd,
	short		iFrmSize,
	char		*NodeIp,
	short		iNodePort,
	unsigned short *iSrcMbId, 
	unsigned short iSrcGrpId,
	unsigned short iBusId
);

short Msp_Put_Gate(
int		iSocketFd,
short		iFrmSize,
Msp_Msg_Head	*stpHead,
char 		*sMsg, 
int		iMsgSize,
char            cStoreFlag
);

void Msp_Init(SHARECFG *shacfg, struct MAILBOXCFG *mailboxcfg);

unsigned short CreateMbShm(key_t key, int iMbShmSize,int iMailNum);

unsigned short  CreateIPC( );

unsigned short DeleteMbSpace();

unsigned short DeleteIPC();

void LoadOldMail();

unsigned short 	ReadFileSpace(char *sFilename, Msp_Mail_Tab *pTmp);

int MspCheckSocket(int sock, int tosec, int tousec); 

int SendSocketReturn( int sockfd, Msg_Return *plReturn);

int RecvSocketReturn( int sockfd, Msg_Return *plReturn);

int RecvHead
(
        int             sockfd,         /*  套接字描述符        */
        Msp_Msg_Head    *plMsgHead
);

int RecvSocketRequest(int sockfd,Msg_Request *plRequest);

int SendSocketRequest(int sockfd,Msg_Request *plRequest);

int RecvSocketBuf(int sockfd,unsigned char *strbuf,Msg_Body *plbody);

int SendSocketBuf(int sockfd,unsigned char *strbuf);

int SendHead(int sockfd,Msp_Msg_Head *plMsgHead);

short Msg_Put(
int		ilsocketfd,
char 		*sMsg, 
int		iMsgSize,
unsigned short	ilFrmSize,
char            cStoreFlag
);

short	Msg_Recv
(
	int		ilsocketfd,
	unsigned char	*plCommArea,
	unsigned char 	*cStorageFlag,
	unsigned int	*iMsgSize
);

void t_catch();

void *getshm(void);

int  MSPGetTime(char *date);

int MSPP(int ilsemid);

int MSPV(int ilsemid);

void Init();

int TakeCount(char Flag);

int Msp_CliCfg_Load(char *filename, CLIENTCFG  *clientcfg);

int Msp_SrvCfg_Load(
char       *filename,
SHARECFG   *sharecfg,
struct MAILBOXCFG *mailboxcfg
);

int Msp_Item_Get(FILE *plFile, char *strlItemName, char *strlItemVar);

int Msp_Str_Trim(char *pstrlBuf);

long FileLen
(
char		*ptrFile		/*  文件全路径名	*/
);

int FileExist
(
char		*ptrFile		/*  文件全路径名	*/
);

short CompressData(unsigned char *InBuf,
unsigned char *OutBuf, short InBufLength);

void pack_init();

short lookup_ct( short code, unsigned char thischar);

short putcode( short code,unsigned char * buf, short bufPosition);

short UnCompressData(unsigned char *InBuf, 
unsigned char *OutBuf,short InBufLength);

void unpack_init();

void insert_ct( short code, short oldcode);

short putx(short code,unsigned char *buf,short bufPosition);


void CreatMAC
(
void	*pData,
int	DataSize,
void	*pKey,
int	KeySize,
void	*pMAC,
int	MACSize
);

int CheckMAC
(
void	*pData,
int	DataSize,
void	*pKey,
int	KeySize,
void	*pMAC,
int	MACSize
);

void * CreatKey
(
void	*pKey,
int		KeySize	
);

int BlockAllSig
(
sigset_t	*pOldSet	/*  原有信号屏蔽状态	*/
);

int UnblockAllSig
(
sigset_t	*pOldSet	/*  原有信号屏蔽状态	*/
);

int BlockSig
(
int			Sig			/*  信号	*/
);

int UnblockSig
(
int			Sig			/*  信号	*/
);

int ProcSig
(
int			Sig			/*  信号	*/
);

int SigDirect
(
int			Sig,		/*  信号			*/
void(* func)(int),		/*  信号处理函数	*/
struct sigaction	*poldact
);

int SigDefault
(
int		Sig				/*  信号	*/
);

int TestSig
(
int		Sig		/*  信号	*/
);

int ClearTimer(void);

int SetTimer
(
long	sec,			/*  秒			*/
long	usec,			/*  微秒		*/
char	cCycFlag		/*  循环标志	*/
);

int ReadMailDsc
(
char		*ptrFile,		/*  文件全路径名	*/
Msp_Mail_Tab	*pMailDsc		/*  邮件描述地址	*/
);

void ClearMsg
(
int		MsgQueID,	/*  消息队列标识符			*/
long	lType		/*  所要清空消息的Type值	*/
);

unsigned short ReqFileSer(unsigned int *iFileNum);


int  daemon_init(void);

void AddSubProc(unsigned short ilPosi,unsigned short ilFlag);

unsigned short CreatePubDscSpace(key_t iShmKey);

unsigned short DeletePubDscSpace();

unsigned short FindMbBynum(
	 unsigned short mbid,
	unsigned short mbgrpid,
	 unsigned short *mbdsc_id,
	char	cPutType
) ;

unsigned short FindMbByname(
	char * mbname, 
	unsigned short *mbdsc_id ,
	char cPutType
);

unsigned short FindEmptyMb(unsigned short *mbdsc_id);

unsigned short WriteMbDsc(struct MAILBOXCFG * mailbox,unsigned short *mbdscid);

unsigned short WriteProcdscSpace
(
	char *pidname, 
	int pid,
	unsigned short ilMbPosi,
	unsigned short *ProcPosi
);

unsigned short UpdateProc
(
	unsigned short iProcPosi, 
	int pid
);

unsigned short DeleteProcdsc(pid_t pid,unsigned short *iMbPosi);
short Msp_Get_MqInfo(
unsigned short	 iSrcMbId, 	/* 队列编号 */
unsigned short	 iSrcGrpId,	/* 队列组号 */
unsigned int	*MailNum,	/* 队列当前消息数量 */
unsigned int	*RcvMailNum,/* 队列接收消息数量 */
unsigned int	*SndMailNum	/* 队列发送消息数量 */
);

#include "globle_msp.h"
