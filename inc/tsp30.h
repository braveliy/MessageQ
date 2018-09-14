#ifndef TSP30_H
#define TSP30_H


/* UNIXϵͳ���� */
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


/* ��־�������ñ�02T003���� ��־�����ڴ棨02S003�� */

typedef struct
{
	short ilPri;           /* �������ȼ� */
	short ilClass;         /* ���������� */
	int 	ilType;          /* ��������,��ƽ̨��ˮ�ű�֤ÿ�ʱ��ĵ�Ψһ�� */
	short	switch_mq;		 /* ���׶�Ӧ��·�ɶ��к� */
	short	iHostType;      /* 0 -�����汾��1-��Ⱥ�汾,2-��Ⱥģʽ2��AP��Out��ͬ�����ɶ˿�02T001���*/
	short AP_node;         /* �ڵ��,���������Ʋ��ָ�ֵ */
	short BAL_node;        /* ���ؾ���ڵ��,�ڸ��ؾ�����������ָ�ֵ */
	short iLogLevel;       /* ��־����,
	                        * �ڸ��ؾ������������02T002���ø���,
	                        * ����������ģ�����־��ʽ,
	                        * 0-�޽�����־��1-������־��2-���ٷ�����־ */
	short iLogFlag;        /* ��־��־,
	                        * �ڸ��ؾ������ģ�������02T002�ø�ֵ,
	                        * ������־�ļ��Ƿ񵥶���¼���Ǹ�ģ��
	                        * �ֿ���¼. ��ֵ��Χ�ο�01C008 */
	char sLogFileName[100];/* ��־�ļ���,
	                        * �ڸ��ؾ������ģ�������02T00
	                        * 2�ø�ֵ, �����ϲ���־�ļ���,
	                        * ����IP��ַ���� */
	short iTrnBlockId;     /* �����ڴ��λ��,��¼��ǰ�����ֳ������ڴ�λ�ã���·��ģ�鸳ֵ*/
	int lTrnid;            /* ƽ̨�ڲ���ˮ��,TSPƽ̨�ڲ���ˮ��,
	                        * �ɸ��ؾ���������ֵ,BAL�ڵ�� *
	                        * 1000000000  + ��� */
	short iTrn_status;     /* ����״̬ 01C009 */
	char Dev_IP[20];       /* �豸��IP��ַ��ͨѶ���ؾ���������ֵ */
	short Dev_port;        /* �豸���첽Ӧ��˿�������첽Ӧ����,
	                        * �ɸ��ؾ���������02T002�첽Ӧ��˿���
	                        * д��ֵ */
	char  Dev_type;        /* ͨѶ���� */
	short Back_port;       /* �豸���첽Ӧ��˿�������첽Ӧ���ģ�
	                        * �ɸ��ؾ���������02T002�첽Ӧ��˿���д��ֵ*/
	long lBegintime;       /* ���׿�ʼʱ��,��APͨѶ�������
	                        * ϵͳʱ�丳ֵ */
	long lBalBegintime;    /* �����ڿ�ʼʱ��,��ͨѶ���ؾ������������
	                        * ϵͳʱ�丳ֵ */
	short iTrnOverTime;    /* ���׳�ʱʱ��,�ɸ��ؾ���ͨѶ����������0
	                        * 2T002���ø�ֵ */
	short iTrnRevmode;     /* ���׳���ģʽ,�ɽ�������ģ�����03T004
	                        * ���ø�ֵ, ѡ��Χ01C005 */
	short iTrnRevType;     /* ���׳������,�ɽ�����������03T004���ø�ֵ,
	                        * �ֲ���͵������� */
	short iComMQ;          /* ͨѶ���к�,��ͨѶ���ؾ�����������0
	                        * 2T002��ֵ */
	short iOutMQ;           /*�ɽӳ������������ø�ֵ*/
	short iBegin_MQ_id;    /* �����������к�,�ɸ��ؾ�����������0
	                        * 2T002���ø�ֵ */
	short iProcChannel;    /* LINK BUSI POST �Է������� */
	short iSrcMQ;          /* ���ķ���Դ���к� */
	short iDestMQ;         /* ���ķ���Ŀ����к� */
	short iTraceFlag;      /* ����ģ��ִ��Ч�ʱ�ʶ,
	                        * �ɸ��ؾ������������02T001���ø�ֵ,
	                        * ��ÿ��ģ�鷢���걨�ĺ�,
	                        * ���ñ��ķ���ģ��Ч�ʷ�����¼ģ�顣 */
	long iInModuleTime;    /* ���Ľ���ģ��,ʱ����libmspapi20.c����i
	                        * TraceFlag����ʱ��ֵ */
	long	iInModulemmTime;
	int iCostTime;         /* ��ģ��ķ�ʱ�� */
	char BusiNum[7];       /* ҵ�����ʹ���,�����⽻����ת������ */
	char aTrncode[11];     /* �ɿ�����Ա��������,
	                        * Ȼ���ڽ�������ģ��������⽻����ת����03
	                        * T004��ֵ */
	char aExtCode[11];     /* �ⲿ������,�ɽ�������ģ��,
	                        * ִ�и�ʽת���ű���� */
	short Proc_step;       /* ���ײ����,��·��ģ��ִ��·�ɽű���ֵ */
	short iStepOverTime;   /* ���ײ��賬ʱʱ��,��·��ģ��ִ��·��
	                        * �ű���ֵ,���������賬ʱʱ�� */
	long  Proc_begin;       /* ���迪ʼʱ�� */
	short Proc_MQ;         /* �������Ӧ���� --del by hqz 20100104*/
	short Proc_RevMode;    /* ���������ģʽ */
	short Proc_status;     /* ������״̬ */
	short iStepRevmode;    /* ���ײ������ģʽ,��·��ģ��ִ��
	                        * ·�ɽű���ֵ,
	                        * ���������賬ʱ����ģʽ01C005 */
	short iMsgClass;       /* �������,��ʼ��ͨѶ����ֵ,
	                        * �������ݸ�ʽת��ִ������ı�ֵ,
	                        * �μ�01C006 */
	short iMsgFormat;      /* ��������,��Ӧ01C003,
	                        * ��ʼ����������ͨѶ���ؾ���������
	                        * ����02T001��ֵ, */
	short iFlowPosi;       /* ·����תλ��,·�ɳ���ִ�нű���,
	                        * ��¼���´α��ĺ󷵻غ��ִ��λ�� */
	short iBodylen;        /* �����峤��,����ʵ�ʱ��ĵĳ��� */
	short iEndflag;        /* ���׽�����־,�����Ƿ��������ֳ� */
	short iSaf_num;        /* �������ߴ洢ת������ */
	short iSwitch_num;     /* ·�ɲο������  */
	short iSecu_num;       /* ��ȫ�ο������  */
	char cBusiFlag;        /* ���ݴ����־, ��������ݴ���ģ��,
	                        * ����޳���Ҫ��, �򲻼�¼�ֳ� */
	long lCode;            /* �������������Ϣ��,ִ�й�����,
	                        * ���������Ϣ��¼��ʶ�μ�01C007 */
	short iCommEndFlag;    /* ͨѶ������־?? */
	char aMemo1[30];       /* Ԥ���ֶ�, ��8583�����������Ϊ
	                        * BITMAP��ʱ����ռ��� */
	char aMemo2[768];       /* Ԥ���ֶ�,(����˵��) */
}MSGHEAD;
/*******************
MSGHEAD->aMemo2����˵����

ͨ�ñ���:
	λ��            ����
	0-1		�ڲ�������
	2-4	
	5		DCC��������	
	7-8		DCC�ļ��ṹ�����	
	9-12	DCC��ϸ����	
	13-22	DCCԶ�̽ڵ�Ŵ��λ��
    26-34   һ�������к�
	48		Mac���λ�ÿ�ʼ	
	100-	DCC�ļ��ṹָ�뿪ʼλ��

��C���·�����ģ�
	λ��    ����
	0-1		�ڲ�������
	2-17	C��IP��ַ

****************/
/* add by wangbo 20100625 begin */
typedef struct
{
        short RequestFlag;/*0:���ͷ��ذ���1�����������a  */
        short Fileflag;
        char  sFileName[60];
        char  sUrl[200];
}TSPHTTPHEAD;
/* add by wangbo 20100625 end   */

typedef struct{
	int iShareSemid;      /*�������ڴ�PV�����ź���*/
	int iTrnSemid;        /*���״洢�ռ�����ź���*/
	int iPackSemid;       /*������洢�ռ�����ź���*/
}TSPShare;


typedef struct{
	short iFlagUsed;    /*�Ƿ����0-���У�1-����ʹ��*/
	short InChnl_MQ;    /*����������*/
	char BusiNum[7];    /*ҵ�����ʹ���,�����⽻����ת������*/
	char aTrncode[11];  /* �ɿ�����Ա�������ã�Ȼ���ڽ�������ģ��������⽻����ת����03T004��ֵ*/
	char aExtCode[11];  /*�ⲿ������*/
	int lTrnid;         /*ƽ̨�ڲ���ˮ��,TSPƽ̨�ڲ���ˮ�ţ��ɸ��ؾ���������ֵ,BAL�ڵ�� * 1000000000  + ���*/
	long lBegintime;    /*���׿�ʼʱ��,��ͨѶ���ؾ������������ϵͳʱ�丳ֵ*/
	short iTrn_ovtm;    /*���׳�ʱʱ��*/
	short iBlockId;     /*��ǰ�ֳ��ڴ������*/
	unsigned long iPackPosi; /*�����ֳ��ռ�*/
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
/*                          TSPƽ̨��������                     */
/****************************************************************/
/*�����ڴ�ȫ�ֱ���*/
#define SHM_EXITED      0 /*�ù����ڴ��Ѿ�����*/
#define SHM_NDINIT      1 /*�ù����ڴ���Ҫ��ʼ��*/
#define AGENTMAXCLI     4 /*agent SERVER ���������*/

/*01C006��������*/
#define iMSGAPP        10 /*Ӧ�ñ���*/
#define iMSGUNPACK     11 /*Ӧ�ñ��Ľ������*/
#define iMSGPACK       12 /*Ӧ�ñ��Ĵ������*/
#define iMSGREVUNPACK  21 /*�������Ľ������*/
#define iMSGREVPACK    22 /*�������Ĵ������*/
#define iMSGMESSAGE    40 /*��Ϣ����*/
#define iMSGORDER      50 /*�����*/
#define iMSGREVATONCE  60 /*��ʱ��������*/
#define iMSGENDTRN     70 /*��������֪ͨ����*/
#define iMSGERRTRN     80 /*ʧ�ܷ���֪ͨ����*/
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


/*C002�����ַ���*/
#define cASCII   '0' /*ASCII*/
#define cEBCDIC  '1' /*EBCDIC*/


/*C003������ˮ����״̬*/
#define iTRNING     0  /*���ڴ���*/
#define iTRNSUC     1  /*���׳ɹ�*/
#define iREVWAIT    3  /*�����ȴ�*/
#define iREVING     4  /*���ڳ���*/
#define iREVSUC     5  /*�����ɹ�*/
#define iREVPARFAIL 6  /*��������ʧ��*/
#define iREVALLFAIL 7  /*������ȫʧ��*/
#define iSAFING     9  /*����SAF�����״���*/
#define iSAFFAIL    10 /*SAF ʧ��*/
#define iSAFSUC     11 /*SAF �ɹ�*/
#define iREVFAIL    12 /*����ʧ��*/



/*01C003���ĸ�ʽ����*/
#define iFMTFML     0	/*�ڲ���׼���ĸ�ʽ*/
#define iFMTXML     1	/*XML��ʽ*/
#define iFMTNSP     2	/*�޷ָ���������ʽ*/
#define iFMTSEP     3	/*�зָ�����ʽ*/
#define iFMT8583    4	/*ISO8583��ʽ*/
#define iFMTUNKNOW  5	/*δ֪*/


/*C005�ڲ���׼���������*/
#define cTYPESTRING 1	/*�ַ�*/
#define cTYPEINT    2	/*����*/
#define cTYPEFLOAT  3	/*������*/
#define cTYPEDATE   4	/*����*/


/*01C004��ʽת�����ʹ���*/
#define iFMLFML   0   /*FML���ĸ�ʽ��FML���ĸ�ʽ*/
#define iXMLFML   10  /*XML���ĸ�ʽ��FML���ĸ�ʽ*/
#define iNOSEPFML 20  /*�޷ָ���������ʽ��FML���ĸ�ʽ*/
#define iSEPFML   30  /*�зָ�����ʽ��FML��ʽ*/
#define i8583FML  40  /*ISO8583��ʽ��FML��ʽ*/
#define iFMLXML   1   /*FML��ʽ��XML��ʽ*/
#define iFMLNOSEP 2   /*FML��ʽ���޷ָ���������ʽ*/
#define iFMLSEP   3   /*FML��ʽ���зָ�����ʽ*/
#define iFML8583  4   /*FML��ʽ��ISO8583��ʽ*/


/*C007�߼�����*/
#define TRUE  1 /*��*/
#define FALSE 0 /*��*/


/*C008������ʽ*/
#define cOTREVYES '1' /*��ʱ����*/
#define cOTREVNO  '2' /*��ʱ������*/


/*C009�����ⷽʽ*/
#define cDOWN   '1' /*����Down*/
#define cNORESP '2' /*���̶Ի������Բ���Ӧ*/


/*C010����״̬��ʶ*/
#define cRUNING   '1' /*��������*/
#define cRUNOVER  '2' /*������*/
#define cDOWNING  '3' /*��������DOWN*/
#define cDOWNOVER '4' /*����Down*/
#define cRUNERROR '5' /*�����޷�����*/


/*C012 ISO8583���ʽ����*/
enum ISO8583FieldType
{
	Efb=1,      /*���������ȱ�8����*/
	Efebc,      /*������EBCDIC��*/
	Efbcd,      /*������BCD��*/
	Efasc,      /*������ASCI��*/
	Elebc,      /*�䳤����һλ����ĳ��ȣ�EBCDIC��*/
	Ellebc,     /*�䳤��ͷ��λ����ĳ��ȣ���EBCDIC��*/
	Elllebc,    /*�䳤��ͷ��λ����ĳ��ȣ���EBCDIC��*/
	Elasc,      /*�䳤����һλ����ĳ��ȣ���ASC��*/
	Ellasc,     /*�䳤��ͷ��λ����ĳ��ȣ���ASC��*/
	Elllasc,    /*�䳤��ͷ��λ����ĳ��ȣ���ASC��*/
	Elbcd,      /*�䳤��ͷһλ����ĳ��ȣ���BCD��*/
	Ellbcd,     /*�䳤��ͷ��λ����ĳ��ȣ���BCD��*/
	Elllbcd,    /*�䳤��ͷ��λ����ĳ��ȣ���BCD��*/
	Elbcdasc,   /*�䳤��ͷһλ����ĳ���BCD�룬ֵΪASC*/
	Ellbcdasc,  /*�䳤��ͷ��λ����ĳ�����BCD�룬ֵΪASC*/
	Elllbcdasc, /*�䳤��ͷ��λ����ĳ�����BCD�룬ֵΪASC*/
	Elllbcdasc2 /*�䳤��ͷ��λ����ĳ�����BCD�룬ֵΪASC,���ɼ��ַ�*/
};


/*C012 ���������ʽ����*/
#define cREVREQ   '1' /*�������������*/
#define cREVRESP  '2' /*������Ӧ�������*/

/*C013 ���׽���ƽ̨�ڲ�����******/
#define iMBWORKREV  11  /*������*/
/*#define iMBSWITCH   12  ����·�ɿ���*/
#define iMBSWITCH   igSwitch_mq		/*����·�ɶ��� add  by hqz 2010-11-4*/
#define iMBTASK     14  /*�������*/
/*#define iMBSECU     15  ��ȫ����*/
#define iMBSECU     igSecu_mq /*��ȫ���� add by hqz 2010-11-4*/
#define iMBMONSYS   16  /*�������*/
#define iMBOUTNODE	17  /*�ӳ�ͨѶģ�鳤���Ӷ�Ӧ�ڵ����*/
#define iMBOUTCOMM  18  /*�ӳ�ͨѶģ�鳤���Ӷ�Ӧ��������*/
#define iMBINCOMM   19  /*����ͨѶģ�鳤���Ӷ���*/
#define iMBCLEAR    10  /*͸������*/

/*C014 ϵͳ����Լ������*/
/* end changed*/
#define iMSGMAXLEN	32767	/*������󳤶�*/
#define iFLDNAMELEN	20	/*������󳤶�*/
#define iFLDVALUELEN	32767	/*��ֵ��󳤶�*/
#define aDBGPATH		/*�����ļ�����·��*/
#define aINIPATH		/*�����ļ�����·��*/
#define aTRNPATH		/*���ױ����ֳ�·��*/
#define LOGIDBUFLEN	10	/*��ˮ�Ź����ڴ��С*/
#define SEMPERM		0666	/*�ź���Ȩ��*/
#define iMAXSERNO	100000000L	/*�����ˮ��*/
#define LOGDIRNUM	99
#define GETMODCOST		1
#define TRNENDFLAG		1
#define TRNENDATONCE	2


/*C016  ISO8583�����Ա�ʶ*/
#define		LEFTZERO	1	/*���0*/
#define		RIGHTZERO	2	/*�Ҽ�0*/
#define		LEFTSPACE	3	/*��ӿո�*/
#define		RIGHTSPACE	4	/*�Ҽӿո�*/


/*C017 ��Ϣ�����¼���ʶ*/
#define		iMSGTRNREV	1	/*����/ֹͣ����*/
#define		iMSGTRNSMIT	2	/*����ת��*/


/*C020 ʹ�ú�δ��*/
#define		CUSED		'1'
#define		CNOUSED		'0'

/*ϵͳ���ù����ڴ��С*/
#define		SHMSIZE		512

#define		SUCCESS		0
#define		FAIL		-1

#define  	MAXTRNLOG  	30000      /*Modi by ltj for test old 500*/
#define  	PACKSIZE   	30000*8192 /* update by hqz 20050413*/

#define		NOTREV		0	/*����Ҫ����*/
#define		REQREV		1	/*�������*/
#define		RSPREV		2	/*��Ӧ����*/
#define		REQSND		3	/*����֪ͨ*/
#define		RSPSND		4	/*��Ӧ֪ͨ*/
#define		NOWSND		5

#define		BUSITRUE		'0'
#define		LINKTRUE		'1'
#define		POSTTRUE		'2'
#define		ENDTRUE			'3'
#define		BEGINTRUE		'4'
#define 	BUSIFALSE		'5'

/*�������ͨѶ����*/

#define	COMSRV_SS	'1'
#define COMSRV_SL	'2'
#define	COMSRV_AS	'3'
#define	COMSRV_AL	'4'
#define	COMSRV_ADL	'5'
#define	COMSRV_SUN	'S'        /*add by ly For Sunlink*/

#define SECUNOT        0       /* ���Ͱ�ȫ*/
#define SECUAFT        1       /* �������Ͱ�ȫ*/
#define SECUBFE        2       /* ����ǰ�Ͱ�ȫ*/
#define SECUALL        3       /* ����ǰ���Ͱ�ȫ*/

#define         NOCRYPT         0        /*������*/
#define         PARTCRYPT       1        /*���ּ���*/
#define         ALLCRYPT        2        /*ȫ������*/


#define		iMSGHEADLEN   sizeof(MSGHEAD) /* ����ͷ����*/
#define 	iSHARESIZE    sizeof(TSPShare) /**/
#define		iTRNSIZE      sizeof(TSPTrnLog)/**/
#define		iHEADLEN      sizeof(_HeadPack)
#define         SQLCODE       sqlca.sqlcode
#define         SQLDESP       sqlca.sqlerrm.sqlerrmc /* Oracle   ������Ϣ*/
#define	        SQLTEXT       sqlca.sqlerrm.sqlerrmc /* Oracle   ������Ϣ*/
#define	        SQLMSG        sqlca.sqlerrm          /* Informix ������Ϣ*/
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
/* �ǵ�����־��־���� */
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

/*���״���״̬��־*/
#define TRN_REQ_FLAG                    '1'            /*��������*/
#define TRN_RESP_FLAG                   '2'            /*������Ӧ*/




extern int Dateymd( char *, int* , int* , int* );
/**************************************************************
[��  ��]  int  Dateymd(char *s,int *y,int *m,int *d)
[˵  ��]  �����ַ������� s (��ʽ: "yyyy/mm/dd"), �������Ӧ��
          ��, ��, ��, ����ڵ�ַ y, m, d ��.
[����ֵ] ��
***************************************************************/

extern int Daysof( char* , char* );
/**************************************************************
[��  ��]  int  Daysof(char *smallday,char *bigday)
[˵  ��]  �����������ڵ�ʵ������(��ͷ[bigday]����β[smallday])
[����ֵ]  ����
***************************************************************/

extern int Trim( char* );
/**************************************************************
[��  ��]  int  Trim(char *s)
[˵  ��]  �ú������ַ��� s ��ǰ��ո�β�س�ȥ��
[����ֵ]  ��
***************************************************************/

extern int itoa( int , char* );
/**************************************************************
[��  ��]  itoa(int n,char s[])
[˵  ��]  ������nת��Ϊ�ַ���s
[����ֵ]  ��
***************************************************************/

extern int udf_ltoa( long, char * );
/**************************************************************
[��  ��]  int udf_ltoa(long n, char s[])
[˵  ��]  long integer to strings
[����ֵ]  ��
***************************************************************/

extern int StrToDate( char*, char* );
/**************************************************************
[��  ��]  int  StrToDate(char *date1,char *date2)
[˵  ��]  ��λ����date1(YYYYMMDD)ת��Ϊʮλ����date2(YYYY/MM/DD)
[����ֵ]  -1:������벻�� | -2:�µ����벻�� | -3:�յ����벻�� |
           0:ת���ɹ�
***************************************************************/

extern int DateToStr( char* , char* );
/**************************************************************
[��  ��]  int  DateToStr(char *date1,char *date2)
[˵  ��]  ʮλ����date1(YYYY/MM/DD)ת��Ϊ��λ����date2(YYYYMMDD)
[����ֵ]  -1:������벻�� | -2:�µ����벻�� | -3:�յ����벻�� |
           0:ת���ɹ�
***************************************************************/


extern int TSPDberror(char *);
/**************************************************************
[��  ��]  int TSPDberror(char *msgpack);
[˵  ��]  ���ݿ������.�Ѵ�����Ϣ����debug�ļ�,ͬʱ���ط�����
          (msgpack). ��û�д���,�򲻼�debug�ļ�,������ر���.
          msgpack:����(�������ݱ���,����(char *) 0 )
[����ֵ]  �������ݿ�ķ���ֵ
***************************************************************/

extern int tspDebugStr(char *,short);
extern int tspDebugmsghead(char *);
/**************************************************************
[��  ��]  int TSPDebug(char *frm,va_alist)
[˵  ��]  ����frm�ĸ�ʽ��va_alist�����ݼǸ�����־.
[����ֵ]  -1:������־�ļ�����  | 0:�ɹ�
***************************************************************/

extern int TSPDbopen(void);
/**************************************************************
[��  ��]  int TSPDbopen()
[˵  ��]  ���������ڴ����ݿ�TSP
[����ֵ]  �ɹ����� 0  | ʧ�ܷ��� SQLCODE.
***************************************************************/

extern int TSPDbclose(void);
/**************************************************************
[��  ��]  int TSPDbclose()
[˵  ��]  ���������ڹر����ݿ�TSP
[����ֵ]  �ɹ����� 0  | ʧ�ܷ��� SQLCODE.
***************************************************************/

extern int TSPGettranlog(long ,short ,char * );
/**************************************************************
[��  ��]  int TSPGettranlog(long logoffset,short loglen,char* msgpack)
[˵  ��]  ���ݱ����ڽ�����־�ļ��е���ʼƫ����logoffset�ͱ��ĵ�
          ����loglen, �õ�������ˮ��־�еı���msgpack
[����ֵ]  SUCCESS:�ɹ�  |  FAIL:ʧ��
***************************************************************/

extern int TSPPuttranlog(char * ,long * );
/**************************************************************
[��  ��]  int TSPPuttranlog(char* msgpack,long* logoffset)
[˵  ��]  ������ı���msgpack���ݴ�ŵ�������־�ļ���,������
          ���Ĵ�ŵ���ʼƫ��λ��logoffset.
[����ֵ]  SUCCESS:�ɹ�  | FAIL:ʧ��
***************************************************************/

extern int TSPGetrevlog(long ,short ,char * );
/**************************************************************
[��  ��]  int TSPGetrevlog(long logoffset,short loglen,char* msgpack)
[˵  ��]  ���ݱ����ڳ�����Ӧ��־�ļ��е���ʼƫ����logoffset��
          ���ĵĳ���loglen,�õ�������Ӧ��ˮ��־�еı���msgpack
[����ֵ]  SUCCESS:�ɹ� |  FAIL:ʧ��
***************************************************************/

extern int TSPPutrevlog(char * ,long * );
/**************************************************************
[��  ��]  int TSPPutrevlog(char* msgpack,long* logoffset)
[˵  ��]  ������ı���msgpack���ݴ�ŵ�������Ӧ��־�ļ���,������
          ���Ĵ�ŵ���ʼƫ��λ��logoffset
[����ֵ]  SUCCESS:�ɹ�  |  FAIL:ʧ��
***************************************************************/


/*extern int TSPFmlunion(char *);*/
/**************************************************************
[��  ��]  int TSPFmlunion(char *)
[˵  ��]
[����ֵ]  �ɹ�:0  |  ʧ��(���ȳ���):-1
***************************************************************/

extern int TSPDebugfml(char *, short);
/**************************************************************
[��  ��]  int  TSPDebugfml(char *msgbody, short msglen)
[˵  ��]  �ڸ����ļ��д�ӡFML���ĵ�����,msgbody����ı���
[����ֵ]  SUCCESS |  FAIL
***************************************************************/

int AtoE(char *,char *,int);
/**************************************************************
[��  ��]  AtoE(char *d,char *s,int len)
[˵  ��]  ascii(s) ת���� ebcdic��(d)   len:ebcdic[d]�ĳ��ȡ�
[����ֵ]
***************************************************************/

int EtoA(char *,char *,int);
/**************************************************************
[��  ��] EtoA(char *d,char *s,int len)
[˵  ��] ascii(d) ת���� ebcdic��(s)   len:ascii[d]�ĳ��ȡ�
[����ֵ]
***************************************************************/

int TSPTimeLongToChs( long , char * ,char * );
/*******************************************************************
[��  ��]    TSPTimeLongToChs(lDateTime,pcDateTime,pcFormat)
[˵  ��]    �������͵�ʱ��ת��Ϊ�ַ�����
              iDateTime �����͵�ʱ��
              pcFormat ʱ��ĸ�ʽ  pcDateTime �ַ����͵�ʱ��
[����ֵ]
*******************************************************************/

/**************************************************************
 ** ������      �� catch_alarm
 ** ��  ��      ��
 ** ����ֵ      ��
***************************************************************/
extern void catch_alarm();

/**************************************************************
 ** ������      �� TCPSelect
 ** ��  ��      ��
 ** ����ֵ      ��
***************************************************************/
extern int TCPSelect(int, fd_set *, int);

/**************************************************************
 ** ������      �� TCPAccept
 ** ��  ��      �� ������������
 ** ����ֵ      ��
***************************************************************/
extern int TCPAccept(int,char *);
extern int TCPAccept_16(int sockfd, char *client_ip, char *proxy_ip);

/**************************************************************
 ** ������      �� TCPRead
 ** ��  ��      ��
 ** ����ֵ      ��
***************************************************************/
extern short TCPRead(char ,short ,int ,short *,char *,short );

/**************************************************************
 ** ������      �� TCPWrite
 ** ��  ��      ��
 ** ����ֵ      ��
***************************************************************/
extern int TCPWrite( char ,int , char *, short );

/**************************************************************
 ** ������      �� TCPServer
 ** ��  ��      ��
 ** ����ֵ      ��
***************************************************************/
extern int TCPServer( int );

/**************************************************************
 ** ������      �� TCPConnect
 ** ��  ��      ��
 ** ����ֵ      ��
***************************************************************/
extern int TCPConnect(char *,int);

/**************************************************************
 ** ������      �� _FunSGT
 ** ��  ��      :  �ж�num1�Ƿ����num2
 ** ����ֵ      �� �����򷵻�1,���򷵻�0
***************************************************************/
extern short _FunSGT(long ,long );

/**************************************************************
 ** ������      �� _FunSGE
 ** ��  ��      :  �ж�num1�Ƿ���ڵ���num2
 ** ����ֵ      �� �����򷵻�1,���򷵻�0
***************************************************************/
extern short _FunSGE(long ,long );

/**************************************************************
 ** ������      �� _FunSLT
 ** ��  ��      :  �ж�num1�Ƿ�С��num2
 ** ����ֵ      �� �����򷵻�1,���򷵻�0
***************************************************************/
extern short _FunSLT(long ,long );

/**************************************************************
 ** ������      �� _FunSLE
 ** ��  ��      :  �ж�num1�Ƿ�С�ڵ���num2
 ** ����ֵ      �� �����򷵻�1,���򷵻�0
***************************************************************/
extern short _FunSLE(long ,long );

/**************************************************************
 ** ������      �� _FunSNE
 ** ��  ��      :  �ж�num1�Ƿ񲻵���num2
 ** ����ֵ      �� �����򷵻�1,���򷵻�0
***************************************************************/
extern short _FunSNE(long ,long );

/**************************************************************
 ** ������      �� _FunSNOT
 ** ��  ��      :  �߼���
 ** ����ֵ      �� if�棺0; if�٣�1
***************************************************************/
extern short _FunSNOT(short );

/**************************************************************
 ** ������      �� _FunSADD
 ** ��  ��      :  �����������
 ** ����ֵ      �� sum
***************************************************************/
extern float _FunSADD(float , float );

/**************************************************************
 ** ������      �� _FunSSUB
 ** ��  ��      :  �����������
 ** ����ֵ      �� ��
***************************************************************/
extern float _FunSSUB(float , float );

/**************************************************************
 ** ������      �� _FunSMUL
 ** ��  ��      :  �����������
 ** ����ֵ      �� ��
***************************************************************/
extern float _FunSMUL(float , float );

/**************************************************************
 ** ������      �� _FunSMOD
 ** ��  ��      :  ����
 ** ����ֵ      �� ����
***************************************************************/
extern long _FunSMOD(long , long);

/**************************************************************
 ** ������      �� _FunSEQ
 ** ��  ��      :  �Ƚ��������ݴ��Ƿ����
 ** ����ֵ      �� 1����ȣ�0������
***************************************************************/
extern int _FunSEQ(char *,char *);

/**************************************************************
 ** ������      �� _FunDSEQ
 ** ��  ��      :  �Ƚ����������Ƿ����
 ** ����ֵ      �� 1����ȣ�0������
***************************************************************/
extern int _FunDSEQ( int ,int );

/**************************************************************
 ** ������      �� TSPMsgsend
 ** ��  ��      :  ��������������ģ�鷢��Ϣ����
 ** ����ֵ      �� SUCCESS,FAIL
***************************************************************/
extern int  TSPMsgsend(long ,char *);

/**************************************************************
 ** ������      �� TSPOrdsend
 ** ��  ��      :  ����������������ģ�鷢�����
 ** ����ֵ      �� SUCCESS,FAIL
***************************************************************/
extern int  TSPOrdsend(long ,char *,short );

/**************************************************************
 ** ������      �� TSPCheckHead
 ** ��  ��      :  ����ͷ���
 ** ����ֵ      �� SUCCESS,FAIL
***************************************************************/
extern  int TSPCheckHead(char *);

/**************************************************************
 ** ������      �� FMLDelZERO
 ** ��  ��      :  ����ص�FML����ת��Ϊ�ڲ�FML����
 ** ����ֵ      �� SUCCESS,FAIL
***************************************************************/
extern  int FMLDelZERO();

/**************************************************************
 ** ������      �� FMLAddZERO
 ** ��  ��      :  ���ڲ�FML����ת��Ϊ��ص�FML����
 ** ����ֵ      �� SUCCESS,FAIL
***************************************************************/
extern  int FMLAddZERO();

/**************************************************************
 ** ������      �� StrItoA
 ** ��  ��      :  ��������ת��Ϊ�ַ���
 ** ����ֵ      �� SUCCESS,FAIL
***************************************************************/
extern  char* StrItoA(short );

/**************************************************************
 ** ������      �� StrReverse
 ** ��  ��      :  ���ַ�����ת
 ** ����ֵ      �� SUCCESS,FAIL
***************************************************************/
extern  int StrReverse(char *);

/**************************************************************
 ** ������      �� StrReverse
 ** ��  ��      :  ��������ת��Ϊ�ַ���
 ** ����ֵ      �� SUCCESS,FAIL
***************************************************************/
extern  int StrLtoA(long ,char *);

/**************************************************************
 ** ������      �� StrCmp
 ** ��  ��      :  ���ַ���s1�в���s2
 ** ����ֵ      �� -1,0:�ַ���s1��û��s2
                  �ɹ��򷵻�s2��s1�еĸ���.
***************************************************************/
extern  int StrCmp(char *,char *);

/**************************************************************
 ** ������      �� StrReplace
 ** ��  ��      :  ��s1�е�s2�滻��ָ����s3.,�������ַ���s4
 ** ����ֵ      �� �ɹ��򷵻��滻�ĸ���
                  -1: s1��û��s2
                  -2: strlen(s2) != strlen(s3)
***************************************************************/
extern  int  StrReplace(char *,char *,char *,char *);

/**************************************************************
 ** ������      �� GetProfileString
 ** ��  ��      :  ���ļ� psPfn �е�ָ������Stanza_flag,ȡ����
**                psChar ����ֵ,
**                ���� psRetStr
 ** ����ֵ      �� 0  -- �ɹ�
**               -1 -- �ļ���дʧ��
**               -2 -- û���򶨵Ĺؼ���
**               -3 -- ���봮���Ȳ��Ϸ�
**�޸�ʱ�䣺20170525
**�޸��ˣ�  ly
**�޸����ݣ���������GetProfileString�޸�ΪGetProfileStr
**�޸�ԭ����Sunlink��̬���еĺ������ظ�
***************************************************************/
extern  int GetProfileStr(char* ,char *,char *,char *);

/**************************************************************
 ** ������      �� _Transfer
 ** ��  ��      :  ��δ��ת����ַ�������ת��
 ** ����ֵ      ��
***************************************************************/
extern  int _Transfer(char *,short ,char *);

/**************************************************************
 ** ������      �� _Untransfer
 ** ��  ��      :  ���Ѿ�ת����ַ������з�ת��
 ** ����ֵ      ��
***************************************************************/
extern  int _Untransfer(char *,char *,short *);

/**************************************************************
 ** ������      �� _TransferUNC
 ** ��  ��      :  ɽ����ͨ��δ��ת����ַ�������ת��
 ** ����ֵ      ��
***************************************************************/
extern  int _TransferUNC(char *,short ,char *);

/**************************************************************
 ** ������      �� _UntransferUNC
 ** ��  ��      :  ɽ����ͨ���Ѿ�ת����ַ������з�ת��
 ** ����ֵ      ��
***************************************************************/
extern  int _UntransferUNC(char *,char *,short *);

/**************************************************************
 ** ������      �� _FunUntUNC
 ** ��  ��      :  ɽ����ͨ���Ѿ�ת����ַ������з�ת��
 ** ����ֵ      ��
***************************************************************/
extern  char *_FunUntUNC(char *);

/**************************************************************
 ** ������      �� _CheckSubFun
 ** ��  ��      :  ���alStr������û���Ӻ���
 ** ����ֵ      ��TRUE:�� FALSE:��
***************************************************************/
extern  int _CheckSubFun(char *);

/**************************************************************
 ** ������      �� getsepstr
 ** ��  ��      :  ���ɷָ����������ַ���srcstr[]��ȡ��������Ӵ�
 ** ����ֵ      ���Ӵ�
***************************************************************/
extern  char *getsepstr(char , int , char *);

/**************************************************************
 ** ������      �� _FunSTRIM
 ** ��  ��      :  �ú������ַ���ͷ����β���Ŀո�ȥ��
 ** ����ֵ      :SUCCESS(�ɹ�),FAIL(ʧ��)
***************************************************************/
extern  char* _FunSTRIM(char , char* );

/**************************************************************
 ** ������      �� _FunSRM
 ** ��  ��      :  ɾ���ַ����г��ֵ��Ӵ�
 ** ����ֵ      :�Ѵ�����ַ���
***************************************************************/
extern  char *_FunSRM(char* , char* );

/**************************************************************
 ** ������      �� _FunSFLD
 ** ��  ��      :  ���ַ����е�ָ��λ�ÿ�ʼȡ�������ȵ��Ӵ�
 ** ����ֵ      :�Ѵ�����ַ���
***************************************************************/
extern  char *_FunSFLD( char* ,  short ,  short );

/**************************************************************
 ** ������      �� _FunSGETR
 ** ��  ��      :  ȡ���Ӵ�
 ** ����ֵ      :�Ӵ�
***************************************************************/
extern  char *_FunSGETR( char *,  short );

/**************************************************************
 ** ������      �� _FunSRMZERO
 ** ��  ��      :  ��ȥ�����ַ���ǰ��0
 ** ����ֵ      :ȥ������ַ���
***************************************************************/
extern  char *_FunSRMZERO( char *);

/**************************************************************
 ** ������      �� _FunSLEN
 ** ��  ��      :  �ó�����ַ����ĳ���
 ** ����ֵ      :����ֵ
***************************************************************/
extern  long _FunSLEN( char *);

/**************************************************************
 ** ������      �� _FunSFILL
 ** ��  ��      :  �Ӳ���1�����ȡ����3�����ȵ��ַ���,������ݲ���4(R��L)
                  �ò���2�ַ���ǰ��󲹳䡣
 ** ����ֵ      :ȡ�����ַ���
***************************************************************/
extern  char *_FunSFILL(char* , char , long , char );

/**************************************************************
 ** ������      �� _FunSNULL
 ** ��  ��      :  �ж��ַ����Ƿ�Ϊ��
 ** ����ֵ      :Ϊ�շ���1, ���򷵻�0
***************************************************************/
extern  short _FunSNULL(char *);

/**************************************************************
 ** ������      �� _FunSSPACE
 ** ��  ��      :  ȡָ�����ȵĿո��ַ���
 ** ����ֵ      :
***************************************************************/
extern  char *_FunSSPACE(long );

/**************************************************************
 ** ������      �� _FunSN2T
 ** ��  ��      :  ��δ��ת����ַ�������ת��
 ** ����ֵ      :ת���ַ���
***************************************************************/
extern  char *_FunSN2T(char *);

/**************************************************************
 ** ������      �� _FunST2N
 ** ��  ��      :  ���ַ������з�ת��
 ** ����ֵ      :��ת���ַ���
***************************************************************/
extern  char *_FunST2N(char *);

/**************************************************************
 ** ������      �� _FunSTOTAL
 ** ��  ��      :  ���ַ������з�ת��
 ** ����ֵ      :��ת���ַ���
***************************************************************/
extern  long _FunSTOTAL(char *);

/**************************************************************
 ** ������      �� _FunSD2STR1
 ** ��  ��      :  ��˫���ȸ�����ת�����ַ���
 ** ����ֵ      :ת������ַ���
***************************************************************/
extern  char* _FunSD2STR1(char *);

/**************************************************************
 ** ������      �� _FunSD2STR2
 ** ��  ��      :  ��˫���ȸ�����ת�����ַ���
 ** ����ֵ      :ת������ַ���
***************************************************************/
extern  char* _FunSD2STR2(char *,char *,char *);

/**************************************************************
 ** ������      �� _FunSI2STR
 ** ��  ��      :  ��������,����,������ת����14���ֽڵ��ַ���
 ** ����ֵ      :ת������ַ���
***************************************************************/
extern  char* _FunSI2STR(char *,char );

/**************************************************************
 ** ������      �� _FunSL2HL
 ** ��  ��      :  long �͵ĸߵ��ֽ�ת��
 ** ����ֵ      :ת������ַ���
***************************************************************/
extern  char *_FunSL2HL(char *);

/**************************************************************
 ** ������      �� _FunSL2HS
 ** ��  ��      :  short �͵ĸߵ��ֽ�ת��
 ** ����ֵ      :ת������ַ���
***************************************************************/
extern  char *_FunSL2HS(char *);

/**************************************************************
 ** ������      �� _FunSCHAR
 ** ��  ��      :  ȡ�����ֵ��ַ���ʾ
 ** ����ֵ      :ת������ַ�
***************************************************************/
extern  char* _FunSCHAR(char *);

/**************************************************************
 ** ������      �� _FunSSHORT
 ** ��  ��      :  ���ַ���ת��Ϊ������
 ** ����ֵ      :���������   ��"12"ת��Ϊ0x00,0x0c
***************************************************************/
extern  char* _FunSSHORT(char *);

/**************************************************************
 ** ������      �� _FunSINT
 ** ��  ��      :  ���ַ���ת��Ϊ����
 ** ����ֵ      :���������   ��"12"ת��Ϊ0x00,0x00,0x00,0x0c
***************************************************************/
extern  char* _FunSINT(char *);

/**************************************************************
 ** ������      �� _FunSLONG
 ** ��  ��      :
 ** ����ֵ      :
***************************************************************/
extern  char* _FunSLONG(char *);

/**************************************************************
 ** ������      �� _FunSDOUBLE
 ** ��  ��      :  ���ַ���ת��Ϊ˫���ȸ�����
 ** ����ֵ      :�����˫���ȸ����� һ��˫���ȸ�����ռ�˸��ֽ�
***************************************************************/
extern  char* _FunSDOUBLE(char *);

/**************************************************************
 ** ������      �� seperatestr
 ** ��  ��      :  �ָ��ַ������ַ�������
 ** ����ֵ      :
***************************************************************/
extern  void seperatestr(char strlSep[][iFLDVALUELEN],const char *s );

/**************************************************************
 ** ������      �� addbuf
 ** ��  ��      :  �ָ��ַ������ַ�������
 ** ����ֵ      :
***************************************************************/
extern  void addbuf(char *, char *);

/**************************************************************
 ** ������      �� SetDefault
 ** ��  ��      :  ���ֶ����ó�ȱʡֵ
 ** ����ֵ      :
***************************************************************/
extern  int SetDefault(char *, short, char );

/**************************************************************
 ** ������      �� GetOneField
 ** ��  ��      :  �� Buffer ��ȡ��n���ֶη���Field�С�
                  Buffer�е�ÿ���ֶ���Seperator Ϊ�ָ��ޡ�
                  n �� 1 ��ʼ��
 ** ����ֵ      :  SUCCESS/FAIL
***************************************************************/
extern  int GetOneField(char *,int , char *,char );

/**************************************************************
 ** ������      �� Get_Field
 ** ��  ��      :  �������������ļ� fname ��Ѱ�ҵ� key_n ���ֶ�Ϊ
*			key �ļ�¼,    �Ѹü�¼�ĵ� field_n ���ֶθ���
*			field, �ֶ��� sep Ϊ�ָ���.
 ** ����ֵ      :  �ɹ����� 0, ʧ�ܷ��� -1.
***************************************************************/
extern  short Get_Field(char *,char *,char *,int ,int , char );

/**************************************************************
 ** ������      �� _FunSep
 ** ��  ��      :  ���зָ������ĵ���ֵ��
 ** ����ֵ      :  ��ֵ.
***************************************************************/
extern  char * _FunSep(char *, short );

/**************************************************************
 ** ������      �� _FunSepUNCOM
 ** ��  ��      :  ���зָ������ĵ���ֵ(���ɽ����ͨ���⴦��)��
 ** ����ֵ      :  ��ֵ.
***************************************************************/
extern  char * _FunSepUNCOM(char *, short );

/**************************************************************
 ** ������      �� _FunSCAT
 ** ��  ��      :  ���Ӷ���ַ���
 ** ����ֵ      :  ���Ӻ���ַ���.
***************************************************************/
extern  char* _FunSCAT(char *,char *);

/**************************************************************
 ** ������      �� match
 ** ��  ��      :  ����������ʽ��ƥ��У��
 ** ����ֵ      :  TRUE      :ƥ��
		  FALSE     :��ƥ��.
***************************************************************/
extern  int match(char *, char *);

/**************************************************************
 ** ������      �� _FunHexStr
 ** ��  ��      :
 ** ����ֵ      :
***************************************************************/
extern  char * _FunHexStr(short );

/**************************************************************
 ** ������      �� StrFieldMask(char *name, char *value)
 ** ��  ��      : ���ǺŴ���2/3�ŵ���������
 ** ����ֵ      : 
***************************************************************/
extern char * StrFieldMask(char *name, char *value);

/**************************************************************
 ** ������      �� TSPByteOrder
 ** ��  ��      :  �жϱ����ߵ�λ�ֽڴ洢���Ⱥ�˳��
 ** ����ֵ      :  1����λ�ֽڷ���ǰ��;
 **                0����λ�ֽڷ���ǰ��;
***************************************************************/
extern  int TSPByteOrder();

/**************************************************************
 ** ������      �� TSPStoreOrder
 ** ��  ��      :  �жϱ����ߵ�λ�ֽڴ洢���Ⱥ�˳��
 ** ����ֵ      :  1����λ�ֽڷ���ǰ��;
 **                0����λ�ֽڷ���ǰ��;
***************************************************************/
extern  int TSPStoreOrder();

/**************************************************************
 ** ������      �� Pkg_ntoh
 ** ��  ��      :  ���������ֽ�ת��  ntoh
 ** ����ֵ      :  SUCCESS,FAIL
***************************************************************/
extern  int  Pkg_ntoh(char * );

/**************************************************************
 ** ������      �� Fml_ntoh
 ** ��  ��      :  ǰ̨��������ֽ�ת��
 ** ����ֵ      :  SUCCESS,FAIL
***************************************************************/
extern  int  Fml_ntoh(char  *, short );

/**************************************************************
 ** ������      �� Fml_hton
 ** ��  ��      :  ǰ̨��������ֽ�ת��
 ** ����ֵ      :  SUCCESS,FAIL
***************************************************************/
extern  int  Fml_hton(char  *, short );

/**************************************************************
 ** ������      �� _FunCpu
 ** ��  ��      :  ��ȡ��ǰcpuʹ�����
 ** ����ֵ      :  SUCCESS,FAIL
***************************************************************/
/*
extern  int _FunCpu(char *,char *,char *,char *,short );
*/

/**************************************************************
 ** ������      �� FmlGet
 ** ��  ��      :  ��FML��ų��л�ȡ��ֵ
 ** ����ֵ      :  �ɹ�����FMLOK��ʧ�ܷ���FMLOVERFLOW
***************************************************************/
extern  int FmlGet( char *,char *,char *);

/**************************************************************
 ** ������      �� FmlDel
 ** ��  ��      :  ��FML��ų�ɾ��FML��
 ** ����ֵ      :  �ɹ�����FMLOK��ʧ�ܷ���FMLOVERFLOW
***************************************************************/
extern  int FmlDel( char *,char *);

/**************************************************************
 ** ������      �� _TSPFmlSet
 ** ��  ��      :  ����FML����,��׼��ֵ
 ** ����ֵ      :  ��׼��ֵ
***************************************************************/
extern  int _TSPFmlSet( char *, char* );

/**************************************************************
 ** ������      �� _TSPFmlSetInt
 ** ��  ��      :  ����FML����,��׼��ֵ
 ** ����ֵ      :  ��׼��ֵ
***************************************************************/
extern  int _TSPFmlSetInt( char *, int );

/**************************************************************
 ** ������      �� _TSPFmlSetShort
 ** ��  ��      :  ����FML����,��׼��ֵ
 ** ����ֵ      :  ��׼��ֵ
***************************************************************/
extern  int _TSPFmlSetShort( char *, short );

/**************************************************************
 ** ������      �� _TSPFmlSetFloat
 ** ��  ��      :  ����FML����,��׼��ֵ
 ** ����ֵ      :  ��׼��ֵ
***************************************************************/
extern  int _TSPFmlSetFloat( char *, float );

/**************************************************************
 ** ������      �� _TSPFmlSetDouble
 ** ��  ��      :  ����FML����,��׼��ֵ
 ** ����ֵ      :  ��׼��ֵ
***************************************************************/
extern  int _TSPFmlSetDouble( char *, double );

/**************************************************************
 ** ������      �� FmlSet
 ** ��  ��      :  ����FML��ų�����������ֵ
 ** ����ֵ      :  �ɹ�����FMLOK��ʧ�ܷ���FMLOVERFLOW
***************************************************************/
extern  int FmlSet( char *,char *,char * );

/**************************************************************
 ** ������      �� FmlCat
 ** ��  ��      :  �����µı���
 ** ����ֵ      :  �ɹ�����FMLOK��ʧ�ܷ���FMLOVERFLOW
***************************************************************/
extern  int FmlCat( char *,char *,char *,short );

/**************************************************************
 ** ������      �� TSPFmlunion
 ** ��  ��      :  ��2��FMLԤ���ȡ����,���Ե�2��Ԥ���Ϊ��
 ** ����ֵ      :  SUCCESS,FAIL
***************************************************************/
extern  long TSPFmlunion(char *,char *);

/**************************************************************
 ** ������      �� _TSPGERNAME
 ** ��  ��      :
 ** ����ֵ      :
***************************************************************/
extern  char* _TSPGERNAME(char *,short );

/**************************************************************
 ** ������      �� _FunFmlGet
 ** ��  ��      :   ���ݱ�׼����ȡ�ñ�׼��ֵ
 ** ����ֵ      :  ��׼��ֵ
***************************************************************/
extern  char *_FunFmlGet(char* );
extern  char *_TSPFmlGet(char* );
extern  char *_NtFmlGet(char* );
/**************************************************************
 ** ������      �� _FunNewFmlGet
 ** ��  ��      :   ���ݱ�׼����ȡ�ñ�׼��ֵ
 ** ����ֵ      :  ��׼��ֵ
***************************************************************/
extern  char *_FunNewFmlGet(char* );

/**************************************************************
 ** ������      �� _FunNSP
 ** ��  ��      :   ȡ�ö�����ʽ���ĵ���ֵ
 ** ����ֵ      :  ��ֵ
***************************************************************/
extern  char *_FunNSP(short ,short );
extern  char *_FunNSP1(short ,short );
extern char *_FunNSPEnd(short);

/**************************************************************
 ** ������      �� _FunCNSPSET
 ** ��  ��      :   ȡ�õ�ǰ������ʽ���ĵ���ֵ
 ** ����ֵ      :
***************************************************************/
extern  int _FunCNSPSET(char* ,char *,short  ,short );

/**************************************************************
 ** ������      �� _FunGetMSGHEAD
 ** ��  ��      :   ȡ��ǰ����ͷ��ֵ
 ** ����ֵ      :
***************************************************************/
extern  char* _FunGetMSGHEAD(char *);

/**************************************************************
 ** ������      �� _Fun_MONFML
 ** ��  ��      :
 ** ����ֵ      :
***************************************************************/
extern  char *_Fun_MONFML( char *);

/**************************************************************
 ** ������      �� _FunGetStatus
 ** ��  ��      :   ����ָ����ֵ����״̬
 ** ����ֵ      :  ״ֵ̬
***************************************************************/
extern  char *_FunGetStatus(char *,short ,short );

/**************************************************************
 ** ������      �� _FunGetStatusDt
 ** ��  ��      :    ����ָ����ֵ����״̬(�ϱ���
 ** ����ֵ      :  ״ֵ̬
***************************************************************/
extern  char *_FunGetStatusDt(char *,short ,short );

/**************************************************************
 ** ������      �� TSPFmlCat
 ** ��  ��      :    �����µı���
 ** ����ֵ      :
***************************************************************/
extern  int TSPFmlCat( char *,char *,char *,short );


/**************************************************************
 ** ������      �� TSPQuit
 ** ��  ��      :
 ** ����ֵ      :
***************************************************************/
extern  void    TSPQuit(int );
extern  char *  _FunSDATE(char *);
extern  char *  _FunFmlGetNSP(char * ,short ,short);
extern  char *  _FunFmlGetNSPFromNew(char * ,short ,short);
extern  char *  _FunIsoGet(int);
extern  char *  _FunCodeChange(char *,short,short,short);
extern  unsigned char * _AtoEOCUT(char *, short );

/**************************************************************
 ** ������      �� XmlPackPar
 ** ��  ��      :  �ൺ��˰XML���Ĵ��
 ** ����ֵ      :
***************************************************************/
extern  char *XmlPackPar(char *,char *,char *);

/**************************************************************
 ** ������      �� XmlUnpackPar
 ** ��  ��      :  �ൺ��˰XML���Ľ��
 ** ����ֵ      :
***************************************************************/
extern  char *XmlUnpackPar(char *,char *,char *);

/**************************************************************
 ** ������      �� XmlUnpk
 ** ��  ��      :  SCM��Ŀ��֤ϵͳXML���Ľ��
 ** ��  ��	    :  XML��
 ** ����ֵ      :  XML��ֵ
***************************************************************/
extern  char *XmlUnpk(char *);

/**************************************************************
 ** ������      �� XmlUnpkSeg
 ** ��  ��      :  SCM��Ŀ��֤ϵͳXML���Ľ��
 ** ��  ��	    :  XML��,XML����
 ** ����ֵ      :  XML����ֵ
***************************************************************/
extern  char *XmlUnpkSeg(char *,char *);

/**************************************************************
 ** ������      �� XmlUnpkDtl
 ** ��  ��      :  SCM��Ŀ��֤ϵͳXML���Ľ���ϸ��Ϣ
 ** ��  ��	    :  XML��,XML����
 ** ����ֵ      :  XML��ϸ��
***************************************************************/
extern  char *XmlUnpkDtl(char *,int);

extern	int TSPDbclose();
extern  int _TSPFmlSet(char *,char *);


/**************************************************************
 ** ������      ��GetSep2
 ** ��  ��      ��ȡ�зָ������ĵ���ֵ��
 ** ��  ��      ������
 ** ��������    ��
 ** ����޸����ڣ�
 ** ��������������
 ** ��������    ��
 ** ����ֵ      ����ֵ
***************************************************************/
//extern char * GetSep2(char *Sepsign,int *ilen, char **msg, char* tmpstr);
extern char * GetSep2(char *Sepsign, char **msg);
/**************************************************************
 ** ������      SetSep2
 ** ��  ��      ����ֵ�ָ������ĵ���ֵ��
 ** ��  ��      ������
 ** ��������    ��
 ** ����޸����ڣ�
 ** ��������������
 ** ��������    ��
 ** ����ֵ      ����ֵ
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
 * @brief		�½�Ŀ¼
 * @param[in]	path	Ŀ¼��
 * @retval		0		�ɹ�
 * @retval		-1		ʧ��
 * 
 * �½�Ŀ¼���м�Ŀ¼�����mkdir -p����Ĺ���
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
 * @brief   A-B����ƽṹ
 */
typedef struct _TABLE_SWAP {
	char  swap_name[255 + 1];	/**< ���� */
	short swap_step;			/**< A-B�л����� */
	short swap_flag;			/**< A-B���л���־ 0-����A-B��, 1-����A */
	short swap_stat;			/**< A-B���л�״̬ 0-��������, 1-˫����
								                   2-�л�ʧ��(����) */
	char  name_curr[1 + 1];		/**< ���б�: 'A'-A�� 'B'-B�� */
	char  name_prev[1 + 1];		/**< �󱸱�: 'A'-A�� 'B'-B�� */
} TABLE_SWAP;


#include "globle_tsp30.h"

#endif /* TSP30_H */
