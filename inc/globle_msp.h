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
GLOBAL	Msp_Pub_Tab 	*pgPubDsc;/*公共描述区结构变量指针*/
GLOBAL	unsigned 	short	igBusId;/*本Bus号*/
GLOBAL	unsigned 	char	*pgMspmsg;/*传递报文结构指针*/
GLOBAL	unsigned 	char	*pgMbHead;/*邮箱起始地址*/
GLOBAL	unsigned 	char	*pgMailHead;/*邮件头起始地址*/
GLOBAL	unsigned 	char	*pgProcHead;/*进程起始地址*/
GLOBAL	unsigned 	char	*pgMailBody;/*邮箱体起始地址*/
GLOBAL	unsigned 	char	*pgNodeHead;/*结点起始地址*/
GLOBAL	char	cgShutdownFlag;/*关闭标志*/

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
