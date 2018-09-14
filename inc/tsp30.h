#ifndef TSP30_H
#define TSP30_H


/* UNIX系统定义 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <regex.h> /*			for linux*/
/*#include <varargs.h>   for aix*/
#include <stdarg.h> /*   for linux*/

#include <dlfcn.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/sysmacros.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/select.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
/*
#include <sys/conf.h>
--del for SuSe Linux
#include <sys/conf.h>
*/

#include <sys/msg.h>
#include <sys/wait.h>
#include <setjmp.h>

#ifndef   COMPILE_FLAG
	#define   COMPILE_FLAG

	#define MAX_FILENAME_LEN  120
	#define NO_MORE_TOKEN     1
	#define COMMENT_TOKEN     2
	#define MAX_FUN_LEN       1024
	#define MAX_VANUM         100
	#define	MAX_TRANCODE      10
	#define SCRIPT_BEGIN      1
	#define SCRIPT_END        2
	#define PRECOMPILE        3
	#define TRANS_BEGIN       4
	#define TRANS_END         5
	#define PROCESS_BEGIN     8
	#define PROCESS_END       9
	#define NOTE              10
	#define DESC              14
	#define FLDALL            15
	#define ISO               1
	#define FML               0
	#define ZERO              0
	#define MAXFUNCNUM        500
	#define MAX_LINE_LEN      1024

	#define ADAPTENAME_LEN    32
	#define TRANS_CODE_LEN    10
	#define SYSID_LEN         5
	#define CONDITION_LEN     128
	#define MAX_TOKEN_LEN     128
	#define KEYWORD_LEN       32
	#define MAX_COMMAND_NUM   13
	#define MAX_MSG_LEN       4096
	#define FD_KEYWORD_LEN    256

	#define MAX_PARA_KEYWORD  15

	#define GET_TOKEN_OK      0
	#define NO_MORE_TOKEN     1
	#define COMMENT_TOKEN     2
	#define SET_VALUE         3

#endif


#ifdef AIX
	#define EOFSIGN 255
#endif
#ifdef LINUX
	#define EOFSIGN EOF
#endif



#ifndef  _TSPStruct_H_
#define  _TSPStruct_H_


/* 日志管理配置表（02T003）和 日志管理内存（02S003） */

typedef struct
{
	short ilPri;           /* 报文优先级 */
	short ilClass;         /* 报文所属组 */
	int 	ilType;          /* 报文索引,以平台流水号保证每笔报文的唯一性 */
	short	switch_mq;		 /* 交易对应的路由队列号 */
	short	iHostType;      /* 0 -单机版本，1-集群版本,2-集群模式2（AP和Out共同部署）由端口02T001获得*/
	short AP_node;         /* 节点号,在流量控制部分赋值 */
	short BAL_node;        /* 负载均衡节点号,在负载均衡服务器部分赋值 */
	short iLogLevel;       /* 日志级别,
	                        * 在负载均衡服务程序根据02T002配置赋予,
	                        * 并决定后续模块的日志方式,
	                        * 0-无交易日志；1-交易日志；2-跟踪分析日志 */
	short iLogFlag;        /* 日志标志,
	                        * 在负载均衡服务模块根据配02T002置赋值,
	                        * 决定日志文件是否单独记录还是各模块
	                        * 分开记录. 赋值范围参考01C008 */
	char sLogFileName[100];/* 日志文件名,
	                        * 在负载均衡服务模块根据配02T00
	                        * 2置赋值, 决定合并日志文件名,
	                        * 按照IP地址生成 */
	short iTrnBlockId;     /* 交易内存块位置,记录当前交易现场所在内存位置，由路由模块赋值*/
	int lTrnid;            /* 平台内部流水号,TSP平台内部流水号,
	                        * 由负载均衡服务程序赋值,BAL节点号 *
	                        * 1000000000  + 序号 */
	short iTrn_status;     /* 交易状态 01C009 */
	char Dev_IP[20];       /* 设备端IP地址由通讯负载均衡服务程序赋值 */
	short Dev_port;        /* 设备端异步应答端口如果是异步应答报文,
	                        * 由负载均衡程序根据02T002异步应答端口填
	                        * 写该值 */
	char  Dev_type;        /* 通讯类型 */
	short Back_port;       /* 设备端异步应答端口如果是异步应答报文，
	                        * 由负载均衡程序根据02T002异步应答端口填写该值*/
	long lBegintime;       /* 交易开始时间,由AP通讯程序根据
	                        * 系统时间赋值 */
	long lBalBegintime;    /* 交易在开始时间,由通讯负载均衡服务程序根据
	                        * 系统时间赋值 */
	short iTrnOverTime;    /* 交易超时时间,由负载均衡通讯服务程序根据0
	                        * 2T002配置赋值 */
	short iTrnRevmode;     /* 交易冲正模式,由接入渠道模块根据03T004
	                        * 配置赋值, 选择范围01C005 */
	short iTrnRevType;     /* 交易冲正风格,由接入渠道根据03T004配置赋值,
	                        * 分并冲和倒冲两种 */
	short iComMQ;          /* 通讯队列号,由通讯负载均衡服务程序案遭0
	                        * 2T002赋值 */
	short iOutMQ;           /*由接出渠道根据配置赋值*/
	short iBegin_MQ_id;    /* 接入渠道队列号,由负载均衡服务程序按照0
	                        * 2T002配置赋值 */
	short iProcChannel;    /* LINK BUSI POST 对方渠道号 */
	short iSrcMQ;          /* 报文发送源队列号 */
	short iDestMQ;         /* 报文发送目标队列号 */
	short iTraceFlag;      /* 交易模块执行效率标识,
	                        * 由负载均衡服务程序根据02T001配置赋值,
	                        * 在每个模块发送完报文后,
	                        * 将该报文发给模块效率分析记录模块。 */
	long iInModuleTime;    /* 报文进入模块,时间由libmspapi20.c根据i
	                        * TraceFlag读入时赋值 */
	long	iInModulemmTime;
	int iCostTime;         /* 本模块耗费时间 */
	char BusiNum[7];       /* 业务类型代码,由内外交易码转换表获得 */
	char aTrncode[11];     /* 由开发人员根据配置,
	                        * 然后在接入渠道模块根据内外交易码转换表03
	                        * T004赋值 */
	char aExtCode[11];     /* 外部交易码,由接入渠道模块,
	                        * 执行格式转换脚本获得 */
	short Proc_step;       /* 交易步骤号,由路由模块执行路由脚本赋值 */
	short iStepOverTime;   /* 交易步骤超时时间,由路由模块执行路由
	                        * 脚本赋值,决定本步骤超时时间 */
	long  Proc_begin;       /* 步骤开始时间 */
	short Proc_MQ;         /* 本步骤对应渠道 --del by hqz 20100104*/
	short Proc_RevMode;    /* 本步骤冲正模式 */
	short Proc_status;     /* 本步骤状态 */
	short iStepRevmode;    /* 交易步骤冲正模式,由路由模块执行
	                        * 路由脚本赋值,
	                        * 决定本步骤超时冲正模式01C005 */
	short iMsgClass;       /* 报文类别,初始由通讯服务赋值,
	                        * 后续根据格式转换执行情况改变值,
	                        * 参见01C006 */
	short iMsgFormat;      /* 报文类型,对应01C003,
	                        * 初始报文类型由通讯负载均衡服务程序
	                        * 根据02T001赋值, */
	short iFlowPosi;       /* 路由跳转位置,路由程序执行脚本后,
	                        * 记录下下次报文后返回后的执行位置 */
	short iBodylen;        /* 报文体长度,后续实际报文的长度 */
	short iEndflag;        /* 交易结束标志,决定是否可以清除现场 */
	short iSaf_num;        /* 冲正或者存储转发次数 */
	short iSwitch_num;     /* 路由参考步骤号  */
	short iSecu_num;       /* 安全参考步骤号  */
	char cBusiFlag;        /* 数据处理标志, 如果是数据处理模块,
	                        * 如果无冲正要求, 则不记录现场 */
	long lCode;            /* 错误或者命令信息码,执行过程中,
	                        * 错误或者信息记录标识参见01C007 */
	short iCommEndFlag;    /* 通讯结束标志?? */
	char aMemo1[30];       /* 预留字段, 在8583解包过程中作为
	                        * BITMAP临时保存空间用 */
	char aMemo2[768];       /* 预留字段,(见后说明) */
}MSGHEAD;
/*******************
MSGHEAD->aMemo2内容说明：

通用报文:
	位置            内容
	0-1		内部错误码
	2-4	
	5		DCC报文类型	
	7-8		DCC文件结构域个数	
	9-12	DCC明细笔数	
	13-22	DCC远程节点号存放位置
    26-34   一级分行行号
	48		Mac存放位置开始	
	100-	DCC文件结构指针开始位置

向C端下发命令报文：
	位置    内容
	0-1		内部错误码
	2-17	C端IP地址

****************/
/* add by wangbo 20100625 begin */
typedef struct
{
        short RequestFlag;/*0:发送返回包，1：发送请求包a  */
        short Fileflag;
        char  sFileName[60];
        char  sUrl[200];
}TSPHTTPHEAD;
/* add by wangbo 20100625 end   */

typedef struct{
	int iShareSemid;      /*本共享内存PV操作信号量*/
	int iTrnSemid;        /*交易存储空间控制信号量*/
	int iPackSemid;       /*报文体存储空间控制信号量*/
}TSPShare;


typedef struct{
	short iFlagUsed;    /*是否可用0-空闲，1-正在使用*/
	short InChnl_MQ;    /*接入渠道号*/
	char BusiNum[7];    /*业务类型代码,由内外交易码转换表获得*/
	char aTrncode[11];  /* 由开发人员根据配置，然后在接入渠道模块根据内外交易码转换表03T004赋值*/
	char aExtCode[11];  /*外部交易码*/
	int lTrnid;         /*平台内部流水号,TSP平台内部流水号，由负载均衡服务程序赋值,BAL节点号 * 1000000000  + 序号*/
	long lBegintime;    /*交易开始时间,由通讯负载均衡服务程序根据系统时间赋值*/
	short iTrn_ovtm;    /*交易超时时间*/
	short iBlockId;     /*当前现场内存中序号*/
	unsigned long iPackPosi; /*交易现场空间*/
}TSPTrnLog;


typedef struct {
  char sDevIP[  33 + 1 ];
  char sDevID[  16 + 1 ];
  char sPinKey[ 16 + 1 ];
  char sMacKey[ 16 + 1 ];
  char sWorkKey[16 + 1 ];
  char sBakWorkKey[16 + 1];
} _DevSecuInfo;

_DevSecuInfo          *pgDevSecuInfo;


#define   MAINKEY      "11118888"
#define	  SEPFILE	1
#define	  ANSERSVR	'1'

#endif

#ifndef  _TSP_Constant
#define  _TSP_Constant
/****************************************************************/
/*                          TSP平台常量定义                     */
/****************************************************************/
/*共享内存全局变量*/
#define SHM_EXITED      0 /*该共享内存已经存在*/
#define SHM_NDINIT      1 /*该共享内存需要初始化*/
#define AGENTMAXCLI     4 /*agent SERVER 最大连接数*/

/*01C006报文类型*/
#define iMSGAPP        10 /*应用报文*/
#define iMSGUNPACK     11 /*应用报文解包报文*/
#define iMSGPACK       12 /*应用报文打包报文*/
#define iMSGREVUNPACK  21 /*冲正报文解包报文*/
#define iMSGREVPACK    22 /*冲正报文打包报文*/
#define iMSGMESSAGE    40 /*信息报文*/
#define iMSGORDER      50 /*命令报文*/
#define iMSGREVATONCE  60 /*及时冲正报文*/
#define iMSGENDTRN     70 /*正常结束通知报文*/
#define iMSGERRTRN     80 /*失败发送通知报文*/
#define iMSGAPLOGOUT	90
#define iMSGAPLOGIN		91
#define	iMSGPOSTSUC		92
#define	iMSGPOSTFAIL	93
#define	iMSGMODCOST		94
#define	iMSGSYSMON		95
#define iMSGTASKLOGIN	96
#define iMSGTASKLOGOUT	97

#define PACK      0
#define UNPACK    1
#define UNUSED    0
#define USED      1


/*C002编码字符集*/
#define cASCII   '0' /*ASCII*/
#define cEBCDIC  '1' /*EBCDIC*/


/*C003交易流水处理状态*/
#define iTRNING     0  /*正在处理*/
#define iTRNSUC     1  /*交易成功*/
#define iREVWAIT    3  /*冲正等待*/
#define iREVING     4  /*正在冲正*/
#define iREVSUC     5  /*冲正成功*/
#define iREVPARFAIL 6  /*冲正部分失败*/
#define iREVALLFAIL 7  /*冲正完全失败*/
#define iSAFING     9  /*步骤SAF，交易处理*/
#define iSAFFAIL    10 /*SAF 失败*/
#define iSAFSUC     11 /*SAF 成功*/
#define iREVFAIL    12 /*冲正失败*/



/*01C003报文格式类型*/
#define iFMTFML     0	/*内部标准报文格式*/
#define iFMTXML     1	/*XML格式*/
#define iFMTNSP     2	/*无分隔符定长格式*/
#define iFMTSEP     3	/*有分隔符格式*/
#define iFMT8583    4	/*ISO8583格式*/
#define iFMTUNKNOW  5	/*未知*/


/*C005内部标准域的域类型*/
#define cTYPESTRING 1	/*字符*/
#define cTYPEINT    2	/*整数*/
#define cTYPEFLOAT  3	/*浮点数*/
#define cTYPEDATE   4	/*日期*/


/*01C004格式转换类型代码*/
#define iFMLFML   0   /*FML报文格式→FML报文格式*/
#define iXMLFML   10  /*XML报文格式→FML报文格式*/
#define iNOSEPFML 20  /*无分隔符定长格式→FML报文格式*/
#define iSEPFML   30  /*有分隔符格式→FML格式*/
#define i8583FML  40  /*ISO8583格式→FML格式*/
#define iFMLXML   1   /*FML格式→XML格式*/
#define iFMLNOSEP 2   /*FML格式→无分隔符定长格式*/
#define iFMLSEP   3   /*FML格式→有分隔符格式*/
#define iFML8583  4   /*FML格式→ISO8583格式*/


/*C007逻辑条件*/
#define TRUE  1 /*真*/
#define FALSE 0 /*假*/


/*C008冲正方式*/
#define cOTREVYES '1' /*超时冲正*/
#define cOTREVNO  '2' /*超时不冲正*/


/*C009任务检测方式*/
#define cDOWN   '1' /*进程Down*/
#define cNORESP '2' /*进程对回声测试不响应*/


/*C010任务状态标识*/
#define cRUNING   '1' /*正在启动*/
#define cRUNOVER  '2' /*已启动*/
#define cDOWNING  '3' /*任务正在DOWN*/
#define cDOWNOVER '4' /*任务Down*/
#define cRUNERROR '5' /*任务无法启动*/


/*C012 ISO8583域格式定义*/
enum ISO8583FieldType
{
	Efb=1,      /*定长，长度被8整除*/
	Efebc,      /*定长，EBCDIC码*/
	Efbcd,      /*定长，BCD码*/
	Efasc,      /*定长，ASCI码*/
	Elebc,      /*变长，第一位是域的长度，EBCDIC码*/
	Ellebc,     /*变长，头两位是域的长度，是EBCDIC码*/
	Elllebc,    /*变长，头三位是域的长度，是EBCDIC码*/
	Elasc,      /*变长，第一位是域的长度，是ASC码*/
	Ellasc,     /*变长，头两位是域的长度，是ASC码*/
	Elllasc,    /*变长，头三位是域的长度，是ASC码*/
	Elbcd,      /*变长，头一位是域的长度，是BCD码*/
	Ellbcd,     /*变长，头两位是域的长度，是BCD码*/
	Elllbcd,    /*变长，头三位是域的长度，是BCD码*/
	Elbcdasc,   /*变长，头一位是域的长度BCD码，值为ASC*/
	Ellbcdasc,  /*变长，头两位是域的长度是BCD码，值为ASC*/
	Elllbcdasc, /*变长，头三位是域的长度是BCD码，值为ASC*/
	Elllbcdasc2 /*变长，头三位是域的长度是BCD码，值为ASC,不可见字符*/
};


/*C012 冲正组包方式定义*/
#define cREVREQ   '1' /*根据请求报文组包*/
#define cREVRESP  '2' /*根据响应报文组包*/

/*C013 交易交换平台内部邮箱******/
#define iMBWORKREV  11  /*事务处理*/
/*#define iMBSWITCH   12  交易路由控制*/
#define iMBSWITCH   igSwitch_mq		/*交易路由队列 add  by hqz 2010-11-4*/
#define iMBTASK     14  /*任务管理*/
/*#define iMBSECU     15  安全邮箱*/
#define iMBSECU     igSecu_mq /*安全队列 add by hqz 2010-11-4*/
#define iMBMONSYS   16  /*监控邮箱*/
#define iMBOUTNODE	17  /*接出通讯模块长连接对应节点队列*/
#define iMBOUTCOMM  18  /*接出通讯模块长连接对应渠道队列*/
#define iMBINCOMM   19  /*接入通讯模块长连接队列*/
#define iMBCLEAR    10  /*透传邮箱*/

/*C014 系统其它约定参数*/
/* end changed*/
#define iMSGMAXLEN	32767	/*报文最大长度*/
#define iFLDNAMELEN	20	/*域名最大长度*/
#define iFLDVALUELEN	32767	/*域值最大长度*/
#define aDBGPATH		/*调试文件所在路径*/
#define aINIPATH		/*配置文件所在路径*/
#define aTRNPATH		/*交易报文现场路径*/
#define LOGIDBUFLEN	10	/*流水号共享内存大小*/
#define SEMPERM		0666	/*信号量权限*/
#define iMAXSERNO	100000000L	/*最大流水号*/
#define LOGDIRNUM	99
#define GETMODCOST		1
#define TRNENDFLAG		1
#define TRNENDATONCE	2


/*C016  ISO8583域属性标识*/
#define		LEFTZERO	1	/*左加0*/
#define		RIGHTZERO	2	/*右加0*/
#define		LEFTSPACE	3	/*左加空格*/
#define		RIGHTSPACE	4	/*右加空格*/


/*C017 信息代码事件标识*/
#define		iMSGTRNREV	1	/*冲正/停止冲正*/
#define		iMSGTRNSMIT	2	/*交易转发*/


/*C020 使用和未用*/
#define		CUSED		'1'
#define		CNOUSED		'0'

/*系统配置共享内存大小*/
#define		SHMSIZE		512

#define		SUCCESS		0
#define		FAIL		-1

#define  	MAXTRNLOG  	30000      /*Modi by ltj for test old 500*/
#define  	PACKSIZE   	30000*8192 /* update by hqz 20050413*/

#define		NOTREV		0	/*不需要冲正*/
#define		REQREV		1	/*请求冲正*/
#define		RSPREV		2	/*响应冲正*/
#define		REQSND		3	/*请求通知*/
#define		RSPSND		4	/*响应通知*/
#define		NOWSND		5

#define		BUSITRUE		'0'
#define		LINKTRUE		'1'
#define		POSTTRUE		'2'
#define		ENDTRUE			'3'
#define		BEGINTRUE		'4'
#define 	BUSIFALSE		'5'

/*服务程序通讯类型*/

#define	COMSRV_SS	'1'
#define COMSRV_SL	'2'
#define	COMSRV_AS	'3'
#define	COMSRV_AL	'4'
#define	COMSRV_ADL	'5'
#define	COMSRV_SUN	'S'        /*add by ly For Sunlink*/

#define SECUNOT        0       /* 不送安全*/
#define SECUAFT        1       /* 打解包后送安全*/
#define SECUBFE        2       /* 打解包前送安全*/
#define SECUALL        3       /* 打解包前后都送安全*/

#define         NOCRYPT         0        /*不加密*/
#define         PARTCRYPT       1        /*部分加密*/
#define         ALLCRYPT        2        /*全部加密*/


#define		iMSGHEADLEN   sizeof(MSGHEAD) /* 报文头长度*/
#define 	iSHARESIZE    sizeof(TSPShare) /**/
#define		iTRNSIZE      sizeof(TSPTrnLog)/**/
#define		iHEADLEN      sizeof(_HeadPack)
#define         SQLCODE       sqlca.sqlcode
#define         SQLDESP       sqlca.sqlerrm.sqlerrmc /* Oracle   错误信息*/
#define	        SQLTEXT       sqlca.sqlerrm.sqlerrmc /* Oracle   错误信息*/
#define	        SQLMSG        sqlca.sqlerrm          /* Informix 错误信息*/
#endif

#ifndef  _TSP_Global
#define  _TSP_Global
/* changed by ly 2003-10-29*/
#define GETDESC        1
#define ETOI           0
#define ITOE           1
#define MIDDLETRANCODE 1
#define MSGCODE        2
#define AUTHCODE       3
#define CURRCODE       4
#define PARTMSGCODE    5
#define TRANCODE_T       	"T"
#define MIDDLETRANCODE_T 	"P"
#define MSGCODE_T        	"E"
#define AUTHCODE_T       	"A"
#define CURRCODE_T       	"C"
#define PARTMSGCODE_T    	"U"
#define   cSIXCHARLEN         'D'
/*end changed */
/* 记调试日志标志级别 */
extern short igLogLevel;
extern short igLogLevel_src;
extern char sgDebugFile[256];
extern char sgDebugFile_src[256];
extern char	pgCommArea[iMSGMAXLEN];
extern char	pgNewCommArea[iMSGMAXLEN];
extern char	pgTmpStr[iMSGMAXLEN];
extern int  igPosition;

extern short	igClass,igPri;
extern int		igType;  /*mody by hrn 20090810*/

extern char 	*ptrFile;
extern char		*ptrTrnStart;
extern char 	*ptrPackStart;

extern TSPShare	*pgShare;

/*move from TSPComm.h by hqz 20040225*/
extern int    igSockfd;
extern struct sockaddr_in cli_addr;
extern struct sockaddr_in serv_addr;

extern char     *pgShmPtr;
extern char			sgTspPath[256];
extern short		igChnlattr;
extern short 		igunpack_sectype;
extern short		igpack_sectype;
extern short		igComm_mb;
extern short		igComm_port;
extern short    igFmtsptid;
extern short    igMsg_format;
#endif



#define CURRENCY_TYPE_NUM  2

int   AtoE( );
int   EtoA( );
char ASCII2EBCDIC(char);
char EBCDIC2ASCII(char);
char *_AtoEA();
char *_EtoAA();
char *_AtoEP();
char *_EtoAP();

char *_NAtoEP();
void _NEtoAP();


#ifndef  _TSPPubfun_H_
#define  _TSPPubfun_H_

/*交易处理状态标志*/
#define TRN_REQ_FLAG                    '1'            /*交易请求*/
#define TRN_RESP_FLAG                   '2'            /*交易响应*/




extern int Dateymd( char *, int* , int* , int* );
/**************************************************************
[语  法]  int  Dateymd(char *s,int *y,int *m,int *d)
[说  明]  根据字符串日期 s (格式: "yyyy/mm/dd"), 生成其对应的
          年, 月, 日, 存放在地址 y, m, d 中.
[返回值] 无
***************************************************************/

extern int Daysof( char* , char* );
/**************************************************************
[语  法]  int  Daysof(char *smallday,char *bigday)
[说  明]  计算两个日期的实际天数(算头[bigday]不算尾[smallday])
[返回值]  天数
***************************************************************/

extern int Trim( char* );
/**************************************************************
[语  法]  int  Trim(char *s)
[说  明]  该函数将字符串 s 的前后空格及尾回车去掉
[返回值]  无
***************************************************************/

extern int itoa( int , char* );
/**************************************************************
[语  法]  itoa(int n,char s[])
[说  明]  把整型n转换为字符串s
[返回值]  无
***************************************************************/

extern int udf_ltoa( long, char * );
/**************************************************************
[语  法]  int udf_ltoa(long n, char s[])
[说  明]  long integer to strings
[返回值]  无
***************************************************************/

extern int StrToDate( char*, char* );
/**************************************************************
[语  法]  int  StrToDate(char *date1,char *date2)
[说  明]  八位日期date1(YYYYMMDD)转化为十位日期date2(YYYY/MM/DD)
[返回值]  -1:年的输入不对 | -2:月的输入不对 | -3:日的输入不对 |
           0:转换成功
***************************************************************/

extern int DateToStr( char* , char* );
/**************************************************************
[语  法]  int  DateToStr(char *date1,char *date2)
[说  明]  十位日期date1(YYYY/MM/DD)转化为八位日期date2(YYYYMMDD)
[返回值]  -1:年的输入不对 | -2:月的输入不对 | -3:日的输入不对 |
           0:转换成功
***************************************************************/


extern int TSPDberror(char *);
/**************************************************************
[语  法]  int TSPDberror(char *msgpack);
[说  明]  数据库错误处理.把错误信息记入debug文件,同时向监控发报文
          (msgpack). 若没有错误,则不记debug文件,不发监控报文.
          msgpack:报文(若不传递报文,输入(char *) 0 )
[返回值]  操作数据库的返回值
***************************************************************/

extern int tspDebugStr(char *,short);
extern int tspDebugmsghead(char *);
/**************************************************************
[语  法]  int TSPDebug(char *frm,va_alist)
[说  明]  按照frm的格式把va_alist的内容记跟踪日志.
[返回值]  -1:操作日志文件出错  | 0:成功
***************************************************************/

extern int TSPDbopen(void);
/**************************************************************
[语  法]  int TSPDbopen()
[说  明]  本函数用于打开数据库TSP
[返回值]  成功返回 0  | 失败返回 SQLCODE.
***************************************************************/

extern int TSPDbclose(void);
/**************************************************************
[语  法]  int TSPDbclose()
[说  明]  本函数用于关闭数据库TSP
[返回值]  成功返回 0  | 失败返回 SQLCODE.
***************************************************************/

extern int TSPGettranlog(long ,short ,char * );
/**************************************************************
[语  法]  int TSPGettranlog(long logoffset,short loglen,char* msgpack)
[说  明]  根据报文在交易日志文件中的起始偏移量logoffset和报文的
          长度loglen, 得到交易流水日志中的报文msgpack
[返回值]  SUCCESS:成功  |  FAIL:失败
***************************************************************/

extern int TSPPuttranlog(char * ,long * );
/**************************************************************
[语  法]  int TSPPuttranlog(char* msgpack,long* logoffset)
[说  明]  将传入的报文msgpack内容存放到交易日志文件中,并返回
          报文存放的起始偏移位置logoffset.
[返回值]  SUCCESS:成功  | FAIL:失败
***************************************************************/

extern int TSPGetrevlog(long ,short ,char * );
/**************************************************************
[语  法]  int TSPGetrevlog(long logoffset,short loglen,char* msgpack)
[说  明]  根据报文在冲正响应日志文件中的起始偏移量logoffset和
          报文的长度loglen,得到冲正响应流水日志中的报文msgpack
[返回值]  SUCCESS:成功 |  FAIL:失败
***************************************************************/

extern int TSPPutrevlog(char * ,long * );
/**************************************************************
[语  法]  int TSPPutrevlog(char* msgpack,long* logoffset)
[说  明]  将传入的报文msgpack内容存放到冲正响应日志文件中,并返回
          报文存放的起始偏移位置logoffset
[返回值]  SUCCESS:成功  |  FAIL:失败
***************************************************************/


/*extern int TSPFmlunion(char *);*/
/**************************************************************
[语  法]  int TSPFmlunion(char *)
[说  明]
[返回值]  成功:0  |  失败(长度超长):-1
***************************************************************/

extern int TSPDebugfml(char *, short);
/**************************************************************
[语  法]  int  TSPDebugfml(char *msgbody, short msglen)
[说  明]  在跟踪文件中打印FML报文的内容,msgbody传入的报文
[返回值]  SUCCESS |  FAIL
***************************************************************/

int AtoE(char *,char *,int);
/**************************************************************
[语  法]  AtoE(char *d,char *s,int len)
[说  明]  ascii(s) 转换到 ebcdic码(d)   len:ebcdic[d]的长度。
[返回值]
***************************************************************/

int EtoA(char *,char *,int);
/**************************************************************
[语  法] EtoA(char *d,char *s,int len)
[说  明] ascii(d) 转换到 ebcdic码(s)   len:ascii[d]的长度。
[返回值]
***************************************************************/

int TSPTimeLongToChs( long , char * ,char * );
/*******************************************************************
[语  法]    TSPTimeLongToChs(lDateTime,pcDateTime,pcFormat)
[说  明]    将长整型的时间转换为字符串型
              iDateTime 长整型的时间
              pcFormat 时间的格式  pcDateTime 字符串型的时间
[返回值]
*******************************************************************/

/**************************************************************
 ** 函数名      ： catch_alarm
 ** 功  能      ：
 ** 返回值      ：
***************************************************************/
extern void catch_alarm();

/**************************************************************
 ** 函数名      ： TCPSelect
 ** 功  能      ：
 ** 返回值      ：
***************************************************************/
extern int TCPSelect(int, fd_set *, int);

/**************************************************************
 ** 函数名      ： TCPAccept
 ** 功  能      ： 接受网络连接
 ** 返回值      ：
***************************************************************/
extern int TCPAccept(int,char *);
extern int TCPAccept_16(int sockfd, char *client_ip, char *proxy_ip);

/**************************************************************
 ** 函数名      ： TCPRead
 ** 功  能      ：
 ** 返回值      ：
***************************************************************/
extern short TCPRead(char ,short ,int ,short *,char *,short );

/**************************************************************
 ** 函数名      ： TCPWrite
 ** 功  能      ：
 ** 返回值      ：
***************************************************************/
extern int TCPWrite( char ,int , char *, short );

/**************************************************************
 ** 函数名      ： TCPServer
 ** 功  能      ：
 ** 返回值      ：
***************************************************************/
extern int TCPServer( int );

/**************************************************************
 ** 函数名      ： TCPConnect
 ** 功  能      ：
 ** 返回值      ：
***************************************************************/
extern int TCPConnect(char *,int);

/**************************************************************
 ** 函数名      ： _FunSGT
 ** 功  能      :  判断num1是否大于num2
 ** 返回值      ： 大于则返回1,否则返回0
***************************************************************/
extern short _FunSGT(long ,long );

/**************************************************************
 ** 函数名      ： _FunSGE
 ** 功  能      :  判断num1是否大于等于num2
 ** 返回值      ： 大于则返回1,否则返回0
***************************************************************/
extern short _FunSGE(long ,long );

/**************************************************************
 ** 函数名      ： _FunSLT
 ** 功  能      :  判断num1是否小于num2
 ** 返回值      ： 大于则返回1,否则返回0
***************************************************************/
extern short _FunSLT(long ,long );

/**************************************************************
 ** 函数名      ： _FunSLE
 ** 功  能      :  判断num1是否小于等于num2
 ** 返回值      ： 大于则返回1,否则返回0
***************************************************************/
extern short _FunSLE(long ,long );

/**************************************************************
 ** 函数名      ： _FunSNE
 ** 功  能      :  判断num1是否不等于num2
 ** 返回值      ： 大于则返回1,否则返回0
***************************************************************/
extern short _FunSNE(long ,long );

/**************************************************************
 ** 函数名      ： _FunSNOT
 ** 功  能      :  逻辑非
 ** 返回值      ： if真：0; if假：1
***************************************************************/
extern short _FunSNOT(short );

/**************************************************************
 ** 函数名      ： _FunSADD
 ** 功  能      :  将两个数相加
 ** 返回值      ： sum
***************************************************************/
extern float _FunSADD(float , float );

/**************************************************************
 ** 函数名      ： _FunSSUB
 ** 功  能      :  将两个数相减
 ** 返回值      ： 差
***************************************************************/
extern float _FunSSUB(float , float );

/**************************************************************
 ** 函数名      ： _FunSMUL
 ** 功  能      :  将两个数相乘
 ** 返回值      ： 积
***************************************************************/
extern float _FunSMUL(float , float );

/**************************************************************
 ** 函数名      ： _FunSMOD
 ** 功  能      :  求余
 ** 返回值      ： 余数
***************************************************************/
extern long _FunSMOD(long , long);

/**************************************************************
 ** 函数名      ： _FunSEQ
 ** 功  能      :  比较两个数据串是否相等
 ** 返回值      ： 1：相等；0：不等
***************************************************************/
extern int _FunSEQ(char *,char *);

/**************************************************************
 ** 函数名      ： _FunDSEQ
 ** 功  能      :  比较两个整数是否相等
 ** 返回值      ： 1：相等；0：不等
***************************************************************/
extern int _FunDSEQ( int ,int );

/**************************************************************
 ** 函数名      ： TSPMsgsend
 ** 功  能      :  本函数用于向监控模块发信息报文
 ** 返回值      ： SUCCESS,FAIL
***************************************************************/
extern int  TSPMsgsend(long ,char *);

/**************************************************************
 ** 函数名      ： TSPOrdsend
 ** 功  能      :  本函数用于向各相关模块发命令报文
 ** 返回值      ： SUCCESS,FAIL
***************************************************************/
extern int  TSPOrdsend(long ,char *,short );

/**************************************************************
 ** 函数名      ： TSPCheckHead
 ** 功  能      :  报文头检查
 ** 返回值      ： SUCCESS,FAIL
***************************************************************/
extern  int TSPCheckHead(char *);

/**************************************************************
 ** 函数名      ： FMLDelZERO
 ** 功  能      :  将监控的FML报文转换为内部FML报文
 ** 返回值      ： SUCCESS,FAIL
***************************************************************/
extern  int FMLDelZERO();

/**************************************************************
 ** 函数名      ： FMLAddZERO
 ** 功  能      :  将内部FML报文转换为监控的FML报文
 ** 返回值      ： SUCCESS,FAIL
***************************************************************/
extern  int FMLAddZERO();

/**************************************************************
 ** 函数名      ： StrItoA
 ** 功  能      :  将短整型转化为字符串
 ** 返回值      ： SUCCESS,FAIL
***************************************************************/
extern  char* StrItoA(short );

/**************************************************************
 ** 函数名      ： StrReverse
 ** 功  能      :  将字符串反转
 ** 返回值      ： SUCCESS,FAIL
***************************************************************/
extern  int StrReverse(char *);

/**************************************************************
 ** 函数名      ： StrReverse
 ** 功  能      :  将长整型转化为字符串
 ** 返回值      ： SUCCESS,FAIL
***************************************************************/
extern  int StrLtoA(long ,char *);

/**************************************************************
 ** 函数名      ： StrCmp
 ** 功  能      :  在字符串s1中查找s2
 ** 返回值      ： -1,0:字符串s1中没有s2
                  成功则返回s2在s1中的个数.
***************************************************************/
extern  int StrCmp(char *,char *);

/**************************************************************
 ** 函数名      ： StrReplace
 ** 功  能      :  将s1中的s2替换成指定的s3.,拷贝到字符串s4
 ** 返回值      ： 成功则返回替换的个数
                  -1: s1中没有s2
                  -2: strlen(s2) != strlen(s3)
***************************************************************/
extern  int  StrReplace(char *,char *,char *,char *);

/**************************************************************
 ** 函数名      ： GetProfileString
 ** 功  能      :  从文件 psPfn 中的指定区域Stanza_flag,取回由
**                psChar 项定义的值,
**                返回 psRetStr
 ** 返回值      ： 0  -- 成功
**               -1 -- 文件读写失败
**               -2 -- 没有镇定的关键字
**               -3 -- 输入串长度不合法
**修改时间：20170525
**修改人：  ly
**修改内容：将函数名GetProfileString修改为GetProfileStr
**修改原因：与Sunlink静态库中的函数名重复
***************************************************************/
extern  int GetProfileStr(char* ,char *,char *,char *);

/**************************************************************
 ** 函数名      ： _Transfer
 ** 功  能      :  将未经转义的字符串进行转义
 ** 返回值      ：
***************************************************************/
extern  int _Transfer(char *,short ,char *);

/**************************************************************
 ** 函数名      ： _Untransfer
 ** 功  能      :  将已经转义的字符串进行反转义
 ** 返回值      ：
***************************************************************/
extern  int _Untransfer(char *,char *,short *);

/**************************************************************
 ** 函数名      ： _TransferUNC
 ** 功  能      :  山东联通将未经转义的字符串进行转义
 ** 返回值      ：
***************************************************************/
extern  int _TransferUNC(char *,short ,char *);

/**************************************************************
 ** 函数名      ： _UntransferUNC
 ** 功  能      :  山东联通将已经转义的字符串进行反转义
 ** 返回值      ：
***************************************************************/
extern  int _UntransferUNC(char *,char *,short *);

/**************************************************************
 ** 函数名      ： _FunUntUNC
 ** 功  能      :  山东联通将已经转义的字符串进行反转义
 ** 返回值      ：
***************************************************************/
extern  char *_FunUntUNC(char *);

/**************************************************************
 ** 函数名      ： _CheckSubFun
 ** 功  能      :  检查alStr串中有没有子函数
 ** 返回值      ：TRUE:有 FALSE:无
***************************************************************/
extern  int _CheckSubFun(char *);

/**************************************************************
 ** 函数名      ： getsepstr
 ** 功  能      :  从由分隔符隔开的字符串srcstr[]中取给定域的子串
 ** 返回值      ：子串
***************************************************************/
extern  char *getsepstr(char , int , char *);

/**************************************************************
 ** 函数名      ： _FunSTRIM
 ** 功  能      :  该函数将字符串头部或尾部的空格去掉
 ** 返回值      :SUCCESS(成功),FAIL(失败)
***************************************************************/
extern  char* _FunSTRIM(char , char* );

/**************************************************************
 ** 函数名      ： _FunSRM
 ** 功  能      :  删除字符串中出现的子串
 ** 返回值      :已处理的字符串
***************************************************************/
extern  char *_FunSRM(char* , char* );

/**************************************************************
 ** 函数名      ： _FunSFLD
 ** 功  能      :  从字符串中的指定位置开始取给定长度的子串
 ** 返回值      :已处理的字符串
***************************************************************/
extern  char *_FunSFLD( char* ,  short ,  short );

/**************************************************************
 ** 函数名      ： _FunSGETR
 ** 功  能      :  取右子串
 ** 返回值      :子串
***************************************************************/
extern  char *_FunSGETR( char *,  short );

/**************************************************************
 ** 函数名      ： _FunSRMZERO
 ** 功  能      :  除去整数字符串前的0
 ** 返回值      :去掉零的字符串
***************************************************************/
extern  char *_FunSRMZERO( char *);

/**************************************************************
 ** 函数名      ： _FunSLEN
 ** 功  能      :  得出域的字符串的长度
 ** 返回值      :长度值
***************************************************************/
extern  long _FunSLEN( char *);

/**************************************************************
 ** 函数名      ： _FunSFILL
 ** 功  能      :  从参数1中左边取参数3个长度的字符串,不足根据参数4(R或L)
                  用参数2字符在前或后补充。
 ** 返回值      :取到的字符串
***************************************************************/
extern  char *_FunSFILL(char* , char , long , char );

/**************************************************************
 ** 函数名      ： _FunSNULL
 ** 功  能      :  判断字符串是否为空
 ** 返回值      :为空返回1, 否则返回0
***************************************************************/
extern  short _FunSNULL(char *);

/**************************************************************
 ** 函数名      ： _FunSSPACE
 ** 功  能      :  取指定长度的空格字符串
 ** 返回值      :
***************************************************************/
extern  char *_FunSSPACE(long );

/**************************************************************
 ** 函数名      ： _FunSN2T
 ** 功  能      :  将未经转义的字符串进行转义
 ** 返回值      :转义字符串
***************************************************************/
extern  char *_FunSN2T(char *);

/**************************************************************
 ** 函数名      ： _FunST2N
 ** 功  能      :  将字符串进行反转义
 ** 返回值      :反转义字符串
***************************************************************/
extern  char *_FunST2N(char *);

/**************************************************************
 ** 函数名      ： _FunSTOTAL
 ** 功  能      :  将字符串进行反转义
 ** 返回值      :反转义字符串
***************************************************************/
extern  long _FunSTOTAL(char *);

/**************************************************************
 ** 函数名      ： _FunSD2STR1
 ** 功  能      :  将双精度浮点型转换成字符型
 ** 返回值      :转换后的字符串
***************************************************************/
extern  char* _FunSD2STR1(char *);

/**************************************************************
 ** 函数名      ： _FunSD2STR2
 ** 功  能      :  将双精度浮点型转换成字符型
 ** 返回值      :转换后的字符串
***************************************************************/
extern  char* _FunSD2STR2(char *,char *,char *);

/**************************************************************
 ** 函数名      ： _FunSI2STR
 ** 功  能      :  将短整型,整型,长整型转换成14个字节的字符型
 ** 返回值      :转换后的字符串
***************************************************************/
extern  char* _FunSI2STR(char *,char );

/**************************************************************
 ** 函数名      ： _FunSL2HL
 ** 功  能      :  long 型的高低字节转换
 ** 返回值      :转换后的字符串
***************************************************************/
extern  char *_FunSL2HL(char *);

/**************************************************************
 ** 函数名      ： _FunSL2HS
 ** 功  能      :  short 型的高低字节转换
 ** 返回值      :转换后的字符串
***************************************************************/
extern  char *_FunSL2HS(char *);

/**************************************************************
 ** 函数名      ： _FunSCHAR
 ** 功  能      :  取得数字的字符表示
 ** 返回值      :转换后的字符
***************************************************************/
extern  char* _FunSCHAR(char *);

/**************************************************************
 ** 函数名      ： _FunSSHORT
 ** 功  能      :  将字符串转换为短整数
 ** 返回值      :所求的整数   如"12"转换为0x00,0x0c
***************************************************************/
extern  char* _FunSSHORT(char *);

/**************************************************************
 ** 函数名      ： _FunSINT
 ** 功  能      :  将字符串转换为整数
 ** 返回值      :所求的整数   如"12"转换为0x00,0x00,0x00,0x0c
***************************************************************/
extern  char* _FunSINT(char *);

/**************************************************************
 ** 函数名      ： _FunSLONG
 ** 功  能      :
 ** 返回值      :
***************************************************************/
extern  char* _FunSLONG(char *);

/**************************************************************
 ** 函数名      ： _FunSDOUBLE
 ** 功  能      :  将字符串转换为双精度浮点型
 ** 返回值      :所求的双精度浮点数 一个双精度浮点型占八个字节
***************************************************************/
extern  char* _FunSDOUBLE(char *);

/**************************************************************
 ** 函数名      ： seperatestr
 ** 功  能      :  分割字符串到字符串数组
 ** 返回值      :
***************************************************************/
extern  void seperatestr(char strlSep[][iFLDVALUELEN],const char *s );

/**************************************************************
 ** 函数名      ： addbuf
 ** 功  能      :  分割字符串到字符串数组
 ** 返回值      :
***************************************************************/
extern  void addbuf(char *, char *);

/**************************************************************
 ** 函数名      ： SetDefault
 ** 功  能      :  将字段设置成缺省值
 ** 返回值      :
***************************************************************/
extern  int SetDefault(char *, short, char );

/**************************************************************
 ** 函数名      ： GetOneField
 ** 功  能      :  从 Buffer 中取第n个字段放入Field中。
                  Buffer中的每个字段以Seperator 为分隔苻。
                  n 从 1 开始。
 ** 返回值      :  SUCCESS/FAIL
***************************************************************/
extern  int GetOneField(char *,int , char *,char );

/**************************************************************
 ** 函数名      ： Get_Field
 ** 功  能      :  本函数用于在文件 fname 中寻找第 key_n 个字段为
*			key 的记录,    把该记录的第 field_n 个字段赋给
*			field, 字段以 sep 为分隔符.
 ** 返回值      :  成功返回 0, 失败返回 -1.
***************************************************************/
extern  short Get_Field(char *,char *,char *,int ,int , char );

/**************************************************************
 ** 函数名      ： _FunSep
 ** 功  能      :  区有分隔符报文的域值；
 ** 返回值      :  域值.
***************************************************************/
extern  char * _FunSep(char *, short );

/**************************************************************
 ** 函数名      ： _FunSepUNCOM
 ** 功  能      :  区有分隔符报文的域值(针对山东联通特殊处理)；
 ** 返回值      :  域值.
***************************************************************/
extern  char * _FunSepUNCOM(char *, short );

/**************************************************************
 ** 函数名      ： _FunSCAT
 ** 功  能      :  连接多个字符串
 ** 返回值      :  连接后的字符串.
***************************************************************/
extern  char* _FunSCAT(char *,char *);

/**************************************************************
 ** 函数名      ： match
 ** 功  能      :  进行正则表达式的匹配校验
 ** 返回值      :  TRUE      :匹配
		  FALSE     :不匹配.
***************************************************************/
extern  int match(char *, char *);

/**************************************************************
 ** 函数名      ： _FunHexStr
 ** 功  能      :
 ** 返回值      :
***************************************************************/
extern  char * _FunHexStr(short );

/**************************************************************
 ** 函数名      ： StrFieldMask(char *name, char *value)
 ** 功  能      : 用星号代替2/3磁等敏感数据
 ** 返回值      : 
***************************************************************/
extern char * StrFieldMask(char *name, char *value);

/**************************************************************
 ** 函数名      ： TSPByteOrder
 ** 功  能      :  判断本机高低位字节存储的先后顺序
 ** 返回值      :  1：高位字节放在前面;
 **                0：低位字节放在前面;
***************************************************************/
extern  int TSPByteOrder();

/**************************************************************
 ** 函数名      ： TSPStoreOrder
 ** 功  能      :  判断本机高低位字节存储的先后顺序
 ** 返回值      :  1：高位字节放在前面;
 **                0：低位字节放在前面;
***************************************************************/
extern  int TSPStoreOrder();

/**************************************************************
 ** 函数名      ： Pkg_ntoh
 ** 功  能      :  进行网络字节转换  ntoh
 ** 返回值      :  SUCCESS,FAIL
***************************************************************/
extern  int  Pkg_ntoh(char * );

/**************************************************************
 ** 函数名      ： Fml_ntoh
 ** 功  能      :  前台监控网络字节转换
 ** 返回值      :  SUCCESS,FAIL
***************************************************************/
extern  int  Fml_ntoh(char  *, short );

/**************************************************************
 ** 函数名      ： Fml_hton
 ** 功  能      :  前台监控网络字节转换
 ** 返回值      :  SUCCESS,FAIL
***************************************************************/
extern  int  Fml_hton(char  *, short );

/**************************************************************
 ** 函数名      ： _FunCpu
 ** 功  能      :  获取当前cpu使用情况
 ** 返回值      :  SUCCESS,FAIL
***************************************************************/
/*
extern  int _FunCpu(char *,char *,char *,char *,short );
*/

/**************************************************************
 ** 函数名      ： FmlGet
 ** 功  能      :  从FML存放池中获取域值
 ** 返回值      :  成功返回FMLOK，失败返回FMLOVERFLOW
***************************************************************/
extern  int FmlGet( char *,char *,char *);

/**************************************************************
 ** 函数名      ： FmlDel
 ** 功  能      :  从FML存放池删除FML域
 ** 返回值      :  成功返回FMLOK，失败返回FMLOVERFLOW
***************************************************************/
extern  int FmlDel( char *,char *);

/**************************************************************
 ** 函数名      ： _TSPFmlSet
 ** 功  能      :  设置FML域名,标准域值
 ** 返回值      :  标准域值
***************************************************************/
extern  int _TSPFmlSet( char *, char* );

/**************************************************************
 ** 函数名      ： _TSPFmlSetInt
 ** 功  能      :  设置FML域名,标准域值
 ** 返回值      :  标准域值
***************************************************************/
extern  int _TSPFmlSetInt( char *, int );

/**************************************************************
 ** 函数名      ： _TSPFmlSetShort
 ** 功  能      :  设置FML域名,标准域值
 ** 返回值      :  标准域值
***************************************************************/
extern  int _TSPFmlSetShort( char *, short );

/**************************************************************
 ** 函数名      ： _TSPFmlSetFloat
 ** 功  能      :  设置FML域名,标准域值
 ** 返回值      :  标准域值
***************************************************************/
extern  int _TSPFmlSetFloat( char *, float );

/**************************************************************
 ** 函数名      ： _TSPFmlSetDouble
 ** 功  能      :  设置FML域名,标准域值
 ** 返回值      :  标准域值
***************************************************************/
extern  int _TSPFmlSetDouble( char *, double );

/**************************************************************
 ** 函数名      ： FmlSet
 ** 功  能      :  设置FML存放池中域名的域值
 ** 返回值      :  成功返回FMLOK，失败返回FMLOVERFLOW
***************************************************************/
extern  int FmlSet( char *,char *,char * );

/**************************************************************
 ** 函数名      ： FmlCat
 ** 功  能      :  增加新的报文
 ** 返回值      :  成功返回FMLOK，失败返回FMLOVERFLOW
***************************************************************/
extern  int FmlCat( char *,char *,char *,short );

/**************************************************************
 ** 函数名      ： TSPFmlunion
 ** 功  能      :  对2个FML预解包取并集,并以第2个预解包为主
 ** 返回值      :  SUCCESS,FAIL
***************************************************************/
extern  long TSPFmlunion(char *,char *);

/**************************************************************
 ** 函数名      ： _TSPGERNAME
 ** 功  能      :
 ** 返回值      :
***************************************************************/
extern  char* _TSPGERNAME(char *,short );

/**************************************************************
 ** 函数名      ： _FunFmlGet
 ** 功  能      :   根据标准域名取得标准域值
 ** 返回值      :  标准域值
***************************************************************/
extern  char *_FunFmlGet(char* );
extern  char *_TSPFmlGet(char* );
extern  char *_NtFmlGet(char* );
/**************************************************************
 ** 函数名      ： _FunNewFmlGet
 ** 功  能      :   根据标准域名取得标准域值
 ** 返回值      :  标准域值
***************************************************************/
extern  char *_FunNewFmlGet(char* );

/**************************************************************
 ** 函数名      ： _FunNSP
 ** 功  能      :   取得定长格式报文的域值
 ** 返回值      :  域值
***************************************************************/
extern  char *_FunNSP(short ,short );
extern  char *_FunNSP1(short ,short );
extern char *_FunNSPEnd(short);

/**************************************************************
 ** 函数名      ： _FunCNSPSET
 ** 功  能      :   取得当前定长格式报文的域值
 ** 返回值      :
***************************************************************/
extern  int _FunCNSPSET(char* ,char *,short  ,short );

/**************************************************************
 ** 函数名      ： _FunGetMSGHEAD
 ** 功  能      :   取当前报文头的值
 ** 返回值      :
***************************************************************/
extern  char* _FunGetMSGHEAD(char *);

/**************************************************************
 ** 函数名      ： _Fun_MONFML
 ** 功  能      :
 ** 返回值      :
***************************************************************/
extern  char *_Fun_MONFML( char *);

/**************************************************************
 ** 函数名      ： _FunGetStatus
 ** 功  能      :   根据指定域值返回状态
 ** 返回值      :  状态值
***************************************************************/
extern  char *_FunGetStatus(char *,short ,short );

/**************************************************************
 ** 函数名      ： _FunGetStatusDt
 ** 功  能      :    根据指定域值返回状态(迪宝）
 ** 返回值      :  状态值
***************************************************************/
extern  char *_FunGetStatusDt(char *,short ,short );

/**************************************************************
 ** 函数名      ： TSPFmlCat
 ** 功  能      :    增加新的报文
 ** 返回值      :
***************************************************************/
extern  int TSPFmlCat( char *,char *,char *,short );


/**************************************************************
 ** 函数名      ： TSPQuit
 ** 功  能      :
 ** 返回值      :
***************************************************************/
extern  void    TSPQuit(int );
extern  char *  _FunSDATE(char *);
extern  char *  _FunFmlGetNSP(char * ,short ,short);
extern  char *  _FunFmlGetNSPFromNew(char * ,short ,short);
extern  char *  _FunIsoGet(int);
extern  char *  _FunCodeChange(char *,short,short,short);
extern  unsigned char * _AtoEOCUT(char *, short );

/**************************************************************
 ** 函数名      ： XmlPackPar
 ** 功  能      :  青岛财税XML报文打包
 ** 返回值      :
***************************************************************/
extern  char *XmlPackPar(char *,char *,char *);

/**************************************************************
 ** 函数名      ： XmlUnpackPar
 ** 功  能      :  青岛财税XML报文解包
 ** 返回值      :
***************************************************************/
extern  char *XmlUnpackPar(char *,char *,char *);

/**************************************************************
 ** 函数名      ： XmlUnpk
 ** 功  能      :  SCM项目认证系统XML报文解包
 ** 入  参	    :  XML域
 ** 返回值      :  XML域值
***************************************************************/
extern  char *XmlUnpk(char *);

/**************************************************************
 ** 函数名      ： XmlUnpkSeg
 ** 功  能      :  SCM项目认证系统XML报文解包
 ** 入  参	    :  XML域,XML子域
 ** 返回值      :  XML子域值
***************************************************************/
extern  char *XmlUnpkSeg(char *,char *);

/**************************************************************
 ** 函数名      ： XmlUnpkDtl
 ** 功  能      :  SCM项目认证系统XML报文解明细信息
 ** 入  参	    :  XML域,XML域标号
 ** 返回值      :  XML明细域
***************************************************************/
extern  char *XmlUnpkDtl(char *,int);

extern	int TSPDbclose();
extern  int _TSPFmlSet(char *,char *);


/**************************************************************
 ** 函数名      ：GetSep2
 ** 功  能      ：取有分隔符报文的域值；
 ** 作  者      ：王波
 ** 建立日期    ：
 ** 最后修改日期：
 ** 调用其它函数：
 ** 参数含义    ：
 ** 返回值      ：域值
***************************************************************/
//extern char * GetSep2(char *Sepsign,int *ilen, char **msg, char* tmpstr);
extern char * GetSep2(char *Sepsign, char **msg);
/**************************************************************
 ** 函数名      SetSep2
 ** 功  能      ：赋值分隔符报文的域值；
 ** 作  者      ：王波
 ** 建立日期    ：
 ** 最后修改日期：
 ** 调用其它函数：
 ** 参数含义    ：
 ** 返回值      ：域值
***************************************************************/
extern int SetSep2(char *Sepsign,char * fldval,char **msg);

/*
  List Function is coded by hqz 2006-7
  these Function is used for memory managerment of the tsp system
*/


extern int  TSPTrnPackWrite(char *,short);
extern int  TSPTrnRollBack(char *);
extern int  TSPFmlUnion(char *);
extern int  TSPEndTran(char *);
extern void TSPQuit(int );
extern int  TSPTrnBegin();
extern int  TSPTrnBusi(short iDestMbId,short iTimeOut,short iRevMode,short iSwitch_num);
extern int  TSPTrnLink(short iDestMbId,short iTimeOut,short iRevMode,short iSwitch_num);
extern int  TSPTrnPOST(short iDestMbId,short iPostNum,short iEndFlag,short iSwitch_num);
extern int  _FunGetTspDate(char *);
extern int  _FunGetTspTime(char *);
extern int  _FunGetTspTraceNum(char *);
extern char *_FunDateAdd(char *,int);
extern char* _nAutoRevSetMsgValue(void);

extern int  sendFile( char *,char *,char *,char *,char * );

/*
 * @fn			int mkdir_p(char *path)
 * @brief		新建目录
 * @param[in]	path	目录名
 * @retval		0		成功
 * @retval		-1		失败
 * 
 * 新建目录和中间目录，完成mkdir -p命令的功能
 */
int mkdir_p(char *path);

extern int TSPMBattach(short qid);
extern int TSPMBdetach(void);
extern int TSPMBwrite (char *msg_area, unsigned int  msg_size, short  qid,
		      short mpriority,         short  mclass, int mtype);
extern int TSPMBread  (char *msg_area, unsigned int *msg_size, short *qid,
	         short *mpriority,         short *mclass, int *mtype);
extern int TSPMBreadnw(char *msg_area, unsigned int *msg_size, short *qid,
             short *mpriority,         short *mclass, int *mtype, short wtime);

extern int  TSPDbopen(void);
extern int  TSPDbopenbusi(void);
int open_database(
		const char *_username,
		const char *_password,
		const char *_servname,
		const char *_connname);
int close_database(void);
int begin_work(void);
int rollback_work(void);
int commit_work(void);

int TrnEndSend( short iMsgClass,short iDestMQ,short iSrcMQ);
int GetTimess(long *Sec,long *mmSec);

int Init_Tsp(void);
int Get_Tsp(void);
int Del_Tsp(void);

char * _FunSTRPACK(const char *s);
int NlvGet(const char *name, char *value, const char *buf);
int NlvSet(const char *name, const char *value, char *buf);
int NlvAdd(const char *name, const char *value, char *buf);
int NlvDel(const char *name, char *buf);

char *_FunVarGet(int n);
char *_FunVarSet(int n, const char *s);
char *_FunVarCat(int d, const char *s, int n, ...);

char *_FunCQGetField(const char *name);

int _FunFmlGetInt(char* alParam);
int _FunFmlGetInt(char* alParam);
float _FunFmlGetFloat(char* alParam);

/*
 * libudp.c
 */
#define MAXLINE	4096
int udp_connect(const char *host, int port);
int udp_server(int port);
ssize_t readline(int fd, void *vptr, size_t maxlen);

#endif

/*
 * Migrate to oracle
 */

/*
 * Informix
 */
#if defined(IFXCMP) || defined(IFX_CMP)
#ifndef _WITH_INFORMIX
#define _WITH_INFORMIX
#endif
#endif

#ifdef _WITH_INFORMIX
/* #define SQLNOTFOUND */
#define SQLCOLNULL				-1405
#define SQLCOLNOTFOUND			-217
#define SQLNOTCONNECTED			-349
#define SQLKEYDUP				(SQLCODE == -268 ? -268 : -239)
#endif /* _WITH_INFORMIX */


/*
 * Oracle
 */
#if defined(ORACMP) || defined(ORA_CMP)
#ifndef _WITH_ORACLE
#define _WITH_ORACLE
#endif
#endif

#ifndef SQLCA_STORAGE_CLASS
#define SQLCA_STORAGE_CLASS extern
#endif

#ifdef _WITH_ORACLE
#define SQLNOTFOUND				1403
#define SQLCOLNULL				-1405
#define SQLINVLID				-904
#define SQLCOLNOTFOUND			SQLINVLID
#define SQLNOTCONNECTED			-1012
#define SQLKEYDUP				-1	
#endif /* _WITH_ORACLE */


#define SQLNOTFOUND2 \
	(SQLCODE == SQLNOTFOUND || (SQLCODE == 0 && sqlca.sqlerrd[2] == 0))

/**
 * @struct	TABLE_SWAP
 * @brief   A-B表控制结构
 */
typedef struct _TABLE_SWAP {
	char  swap_name[255 + 1];	/**< 表名 */
	short swap_step;			/**< A-B切换步骤 */
	short swap_flag;			/**< A-B表切换标志 0-不用A-B表, 1-启用A */
	short swap_stat;			/**< A-B表切换状态 0-单表运行, 1-双表并行
								                   2-切换失败(单表) */
	char  name_curr[1 + 1];		/**< 运行表: 'A'-A表 'B'-B表 */
	char  name_prev[1 + 1];		/**< 后备表: 'A'-A表 'B'-B表 */
} TABLE_SWAP;


#include "globle_tsp30.h"

#endif /* TSP30_H */
