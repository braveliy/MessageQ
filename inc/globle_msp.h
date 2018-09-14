#ifndef GLOBLE_MSP_H
#define GLOBLE_MSP_H

#ifndef GLOBAL
#define GLOBAL extern
#endif

#include "esb.h"


GLOBAL	short	 igLogLevel_src;
GLOBAL	short    igLogLevel;
GLOBAL	char 	 sgDebugFile[256];
GLOBAL	char     sgDebugFile_src[256];
GLOBAL	EsbMsg   	esbmsg;
GLOBAL	Msp_Pub_Tab 	*pgPubDsc;/*�����������ṹ����ָ��*/
GLOBAL	unsigned 	short	igBusId;/*��Bus��*/
GLOBAL	unsigned 	char	*pgMspmsg;/*���ݱ��Ľṹָ��*/
GLOBAL	unsigned 	char	*pgMbHead;/*������ʼ��ַ*/
GLOBAL	unsigned 	char	*pgMailHead;/*�ʼ�ͷ��ʼ��ַ*/
GLOBAL	unsigned 	char	*pgProcHead;/*������ʼ��ַ*/
GLOBAL	unsigned 	char	*pgMailBody;/*��������ʼ��ַ*/
GLOBAL	unsigned 	char	*pgNodeHead;/*�����ʼ��ַ*/
GLOBAL	char	cgShutdownFlag;/*�رձ�־*/

GLOBAL	SHARECFG	shacfg;
GLOBAL	struct MAILBOXCFG	mailboxcfg;

GLOBAL	unsigned int	igBlockSize;
GLOBAL	unsigned short	igNodeId;
GLOBAL	unsigned short	igSrcMbId,igSrcGrpId,igSrcMailBoxPosi,igProcPosi;
GLOBAL	pid_t	igUsrPid;
GLOBAL	unsigned int	igSendMsgLen;
GLOBAL	char     sgMspPath[256];

GLOBAL	long	igTrnid;

#endif /* GLOBLE_MSP_H */
