#include "esb.h"
#include "msp.h"

#ifndef lint
static const char rcsid[] =
	"$Id: libmsp.c,v 1.3.0.1 2012/04/12 10:26:47 rbq Exp $";
static const char relid[] =
	"$" "Date: "__FILE__" "__DATE__" "__TIME__" $";
#endif /* not lint */

unsigned short BlockRead(unsigned short ilProcPosi, short iTimeOut, int *iTimeUsed);
unsigned short DeleteProcdsc(pid_t pid,unsigned short *iMbPosi);
unsigned short ReadFileList 
( 
	char *, 
	int , 
	char *, 
	char
);
unsigned short DelMailHead
(
	unsigned int 
);
unsigned short ReadMailHead 
( 
	unsigned int *, 
	Msp_Msg_Head *,
	int *, 
	unsigned int *, 
	char *, 
	char *, 
	char,
	unsigned short
);
short Msp_Del_Msg(unsigned short ilSrcMbPosi);
short MoveDieMail(unsigned int iMailPosi,
		unsigned short iDieMqId,
		unsigned short iDieMqGrpId);

void AddSubProc(unsigned short ilPosi,unsigned short ilFlag)
{
	Msp_Proc_Tab *plProc;	
	short	ilProc;
	unsigned short ilPidPosi;
	
	plProc=(Msp_Proc_Tab *)(pgProcHead+ilPosi*sizeof(Msp_Proc_Tab));

	plProc->iProcBlockFlag = ilFlag;

	return;
}

unsigned short DeleteProcdsc(pid_t pid,unsigned short *iMbPosi)
{	
	Msp_Proc_Tab    *pPre,*pNext,*pTmp;		
	unsigned short  iRet;	
	unsigned short  iPosi;
	
	/*移动指针到进程链中的第一个进程的位置*/
	if (pgPubDsc->iFstUsdProc == pgPubDsc->iMaxAttachNum)
		return MSPFAIL;
	
	ESBP(pgPubDsc->iMailSemId);
	
	pPre = (Msp_Proc_Tab *)(pgProcHead + (pgPubDsc->iFstUsdProc) * sizeof(Msp_Proc_Tab));
	pNext = pPre;
	/*如果要删除的进程不是进程链的第一个进程，循环查找进程链中指定
	  进程的前一个进程，并从链中把指定的进程删除*/
               
	if (pNext->iUserPid != pid)
	{
	   if (pNext->iNextPosi == pgPubDsc->iMaxAttachNum )
	   {
				printf("  pid[%d] is not attach!",pid);
				
				ESBV(pgPubDsc->iMailSemId);
				
				return MSPFAIL;
	   }
	   while (pNext->iNextPosi != pgPubDsc->iMaxAttachNum )
	   {
				pPre = pNext;
				pNext = (Msp_Proc_Tab *)(pgProcHead + 
					(pNext->iNextPosi) * sizeof(Msp_Proc_Tab));
				if(pNext->iUserPid == pid) break;
	   }
	   if ((pNext->iNextPosi == pgPubDsc->iMaxAttachNum)&&
			(pNext->iUserPid != pid))
	   {
	   	   printf("pid[%d] not attach",pid);
				
				ESBV(pgPubDsc->iMailSemId);
				
				return MSPFAIL;
	   }

	   pPre->iNextPosi = pNext->iNextPosi;
	   ESBP(pgPubDsc->iPubSemId);
	   pgPubDsc->iCurAttachNum --;	
	   pNext->iNextPosi = pgPubDsc->iFstEmptyProc;
	   pgPubDsc->iFstEmptyProc = pNext->iSeriNo;
	   ESBV(pgPubDsc->iPubSemId);
	 
	   *iMbPosi = pNext->iMbIdPosi;
	   pNext->iMbIdPosi = -1;
	   pNext->iUserPid = -1;
	   pNext->iProcBlockFlag = 0;
	   /*pNext->lBeginTime = 0;*/
	}
	/*如果要删除的进程是进程链的第一个进程，则删除该进程，
		并将链头指向下一进程*/
	else
	{		
	    ESBP(pgPubDsc->iPubSemId);
	    pgPubDsc->iCurAttachNum --;	
	    pgPubDsc->iFstUsdProc = pNext->iNextPosi;
	    pNext->iNextPosi = pgPubDsc->iFstEmptyProc;
	    pgPubDsc->iFstEmptyProc = pNext->iSeriNo;
	    ESBV(pgPubDsc->iPubSemId);

	   
	   *iMbPosi = pNext->iMbIdPosi;
	    pPre->iMbIdPosi = 0;
	    pPre->iUserPid = 0;
	    pPre->iProcBlockFlag = 0;
	  
	}
	ESBV(pgPubDsc->iMailSemId);
	return(MSPSUCCESS);			
}

unsigned short RcvMsg(int iMsgId,EsbMsg *sMsg, int *iLen,int iTimeOut)
{
	int	iRet=0;
	long	lType;
	int 	iRemainTime;
	unsigned int iSeconds;
	
	lType = sMsg->mtype;
	func = t_catch;
	if ( iTimeOut != -1 )
	{
		iSeconds = iTimeOut;
		if (iSeconds > 1800) {
			printf("超时参数异常, 使用缺省值120秒 iSeconds=%u [%s:%d]",
				iSeconds, __FILE__, __LINE__);
			iSeconds = 120;
		}
		Signal( SIGALRM, func );
		alarm( iSeconds );
	}

	iRet = msgrcv(iMsgId, sMsg, *iLen, lType, 0);
	if ( iTimeOut != -1 )
	{
		iRemainTime = alarm(0);
		if ( iRet < 0)
			return READTIMEOUT;
	}
	if (iRet<0)
	{
	    	printf("ERROR:RecvMsg errno=%s\n", strerror(errno));
		return (ESBFAIL);
	}
	else
	{
		*iLen = iRet;
		return(ESBSUCCESS);
	}
}

int WritePackMem( char		*ptrMemStore, int		iPackSemId, char 		*pCommArea, unsigned int	iMsgSize, unsigned long	*lPackPosi)
{
	PackHead	*plPackHead;
	MailPack  	*plPack,*plNewPack,*plUnusdPack;
	unsigned long	lOffset;
	unsigned long	ilNewPosi;
	
	ESBP(iPackSemId);
	plPackHead	=	(PackHead *)ptrMemStore;

	assert(iHEADSIZE == sizeof(PackHead));
	assert(iPACKSIZE == sizeof(MailPack));

	if ((plPackHead->iPackUnused == PACKNULL)||(plPackHead->iUnusedStore < iMsgSize))
	{ 		
		ESBV(iPackSemId);	
		return BUFSMALL;
	}
	lOffset	= plPackHead->iPackUnused;
	while(ESBTRUE)
	{
		plPack = (MailPack *)(ptrMemStore + lOffset);    		
		if ( plPack->iLength >= iMsgSize) 
			break;
		lOffset	= plPack->lNextPosi;
		if ( lOffset == PACKNULL ) 
		{   		
			ESBV(iPackSemId);
			return BUFSMALL;
		}
	}

	if( ( plPack->iLength - iMsgSize ) > 1024)
	{
		/*可以产生一个新的未用块*/
		/*写入新块PACKHEAD*/
		ilNewPosi = lOffset + iPACKSIZE + iMsgSize;
		plNewPack = ( MailPack * )(ptrMemStore + ilNewPosi );
		plNewPack->iLength = plPack->iLength - iMsgSize - iPACKSIZE;
		plNewPack->lPrePosi    = lOffset;
		plNewPack->lNextPosi   = plPack->lNextPosi;
		plNewPack->iPackLength = 0;
		if (plNewPack->lNextPosi != PACKNULL)
		{
			plUnusdPack = (MailPack *)(ptrMemStore + plNewPack->lNextPosi);
			plUnusdPack->lPrePosi = ilNewPosi;
		}
		/*修改用来存放该笔PACK的块头*/
		plPack->lNextPosi =  ilNewPosi;
		plPack->iLength = iMsgSize;
		plPackHead->iUnusedStore -= iPACKSIZE;
	}
	if ( plPack->lPrePosi == PACKNULL )
	{
		plPackHead->iPackUnused = plPack->lNextPosi;
		if (plPack->lNextPosi != PACKNULL)
		{
			plUnusdPack = (MailPack *)(ptrMemStore + plPack->lNextPosi);
			plUnusdPack->lPrePosi = PACKNULL;
		}
	}else
	{
		plNewPack = (MailPack *)(ptrMemStore + plPack->lPrePosi);
		plNewPack->lNextPosi = plPack->lNextPosi;
		if (plPack->lNextPosi != PACKNULL)
		{
			plUnusdPack = (MailPack *)(ptrMemStore + plPack->lNextPosi);
			plUnusdPack->lPrePosi = plPack->lPrePosi;
		}
	}
	memcpy(ptrMemStore + lOffset + iPACKSIZE,pCommArea,iMsgSize);
	plPack->lPrePosi = PACKNULL;
	plPack->lNextPosi = PACKNULL;
	plPack->iPackLength = iMsgSize;
	*lPackPosi = lOffset;
	plPackHead->iUnusedStore -= plPack->iLength;

	ESBV(iPackSemId);

	return ESBSUCCESS;
}


/**************************************************************
** Function       : Msp_attach
** Effect      	: 组织发送请求报文，并接受应答。
** Parameters    	: iSrcMbId   :源邮箱号
		  MailBoxName:源邮箱名
		  iSrcGrpId:源邮箱组号		  
		  iAttachType:类型（见C001请求类型）
		  iTimeOut   :超时时间
** return      	: 成功MSPMSPSUCCESS，失败MSPFAIL
****************************************************************/
short Msp_attach
(
  unsigned short *iSrcMbId, 
  char 		*MailBoxName, 
  unsigned short iSrcGrpId,
  unsigned short iBusId,
  unsigned short iAttachType
)
{
	int	ilRet;
	unsigned char *pTmp;
	Msp_Mb_Tab 	*mbdsc,*pmbdsc;
	char	MbBitmap[999];
	unsigned short i,j;
	key_t	ShmKey;
	char	slPath[400];
	

	/*连接公共描述区*/
	if(getenv("MSP_DIR")==NULL)
	{
	   printf("MSP_DIR not set![%s:%d]\n", __FILE__, __LINE__);
	   return MSPFAIL;
	}
	if(getenv("MSP_NODE_ID")==NULL)
	{
	   printf("MSP_NODE_ID not set![%s:%d]\n", __FILE__, __LINE__);
	   return MSPFAIL;
	}
	sprintf(sgMspPath,"%s",getenv("MSP_DIR"));
	igBusId = iBusId;
	sprintf(slPath,"%s/etc/ipckey/maillog/%d",sgMspPath,igBusId);
	ShmKey = ftok(slPath,1);
	if(ShmKey == (key_t)-1)
	{
		printf("ERROR:attach ftok ShmKey error reason:[%s][%s:%d]",
		strerror(errno), __FILE__, __LINE__);
	   return MSPFAIL;
	}
		
	if((pTmp =(unsigned char*)shmGet(ShmKey)) == NULL)
	{
		printf("Core not start![%s:%d]\n", __FILE__, __LINE__);
		return(MSPFAIL);
	}

	pgPubDsc = (Msp_Pub_Tab *)pTmp;
	igSrcMailBoxPosi = MAXMBNUM;
	igUsrPid = getpid();
	if (((void *)(pgMbHead=shmat(pgPubDsc->iMailShmId,0,SHM_RND)))==(void *)-1)
	{
		printf("连共享内存错!");
		return MSPFAIL;
	}
	
	pgMailBody = (unsigned char *)(pgMbHead + pgPubDsc->iToMailBody);
	pgMailHead = (unsigned char *)(pgMbHead + pgPubDsc->iToMailHead);
	pgProcHead = (unsigned char *)(pgMbHead + pgPubDsc->iToProc);
	igBlockSize =pgPubDsc->iBlockSize + sizeof(unsigned int);
	
	switch(iAttachType)
	{
	   case iATTACHNUM:
	   	igSrcGrpId = iSrcGrpId;
	   	igSrcMbId = *iSrcMbId;
		if(FindMbnum(*iSrcMbId,iSrcGrpId,&igSrcMailBoxPosi,NOREALTIME)==MSPFAIL)
		{
		   printf("Msp_Attach_num: FindMbnum Fail! [%s:%d]\n", __FILE__, __LINE__);
		   return ERR_FINDMB;
		}
		mbdsc=(Msp_Mb_Tab *)(pgMbHead + 
				igSrcMailBoxPosi * sizeof(Msp_Mb_Tab)) ;
		

		if ((mbdsc->iProcNum != 0)&&(mbdsc->cMbOpenMode == ENGROSS))
			return ERR_ENGROSS;
		/*查找空链为MAXPROCNUM则没有空间*/			
		if (pgPubDsc->iFstEmptyProc == pgPubDsc->iMaxAttachNum)
			return(NOSPACE);
		if(WriteProcdscSpace(MailBoxName,igUsrPid, igSrcMailBoxPosi,&igProcPosi)==MSPFAIL)
		   return WPROCFAIL;
		   ESBP(pgPubDsc->iMailSemId);
		mbdsc->iProcNum += 1 ;
		   ESBV(pgPubDsc->iMailSemId);

		return MSPSUCCESS;
		
	   case iATTACHNAME:
		if(FindMbname(MailBoxName, &igSrcMailBoxPosi,NOREALTIME)==MSPFAIL)
		{
		  printf("Msp_Attach_nme: FindMbname Fail!\n");
		  return ERR_FINDMB;
		}
		mbdsc=(Msp_Mb_Tab *)(pgMbHead+igSrcMailBoxPosi*
				sizeof(Msp_Mb_Tab));

		if ((mbdsc->iProcNum != 0)&&(mbdsc->cMbOpenMode == ENGROSS))
			return ERR_ENGROSS;
		/*查找空链为MAXPROCNUM则没有空间*/			
		if (pgPubDsc->iFstEmptyProc == pgPubDsc->iMaxAttachNum)
			return(NOSPACE);
		if(WriteProcdscSpace(MailBoxName,igUsrPid,igSrcMailBoxPosi,&igProcPosi)==MSPFAIL)
		   return WPROCFAIL;
		   ESBP(pgPubDsc->iMailSemId);
		mbdsc->iProcNum += 1;
		   ESBV(pgPubDsc->iMailSemId);
		igSrcMbId = mbdsc->iMbId;
		igSrcGrpId = mbdsc->iMbGrpId;
		
		*iSrcMbId = mbdsc->iMbId;

		return MSPSUCCESS;
	   case iATTACHREQ:
		memset(MbBitmap,0,sizeof(MbBitmap));
		/*初始化*/
		for(i = 0;i<pgPubDsc->iMaxMbNum;i++)
		{
		  mbdsc=(Msp_Mb_Tab *)(pgMbHead+i * sizeof(Msp_Mb_Tab));
		  if(mbdsc->cMbDscFlag == 1)
		  	MbBitmap[mbdsc->iMbId-1] = 1;
		}
		/*找到一个MBID*/
		for(i = 0; i < 998;i++)
		{
		   if(MbBitmap[i]==0)
			break;
		}
		if(i == 998 ) 
			return MSPFAIL; /*邮箱号已经分配完毕*/
		else
		{
		   /*找到一个邮箱空间*/
		   for(j = 0;j<pgPubDsc->iMaxMbNum;j++)
		   {
		      mbdsc=(Msp_Mb_Tab *)(pgMbHead + j * sizeof(Msp_Mb_Tab));
		      if(mbdsc->cMbDscFlag == 0) break;
		   }
		   igSrcMailBoxPosi = j;
		   ESBP(pgPubDsc->iMailSemId);
		   sprintf(mbdsc->sMbName,"M0%d",i+1);
		   mbdsc->cMbDscFlag = 1;
		   mbdsc->iMbId = i + 1;
		   mbdsc->iFstMail = pgPubDsc->iMaxMailNum;
		   ESBV(pgPubDsc->iMailSemId);

		   ESBP(pgPubDsc->iPubSemId);
		   pgPubDsc->iCurMbNum += 1;
		   ESBV(pgPubDsc->iPubSemId);

		   if (pgPubDsc->iFstEmptyProc == pgPubDsc->iMaxAttachNum)
					return(NOSPACE);
		   /*igSrcMailBoxPosi = j;--del   20101030*/
		   if(WriteProcdscSpace(MailBoxName,igUsrPid, igSrcMailBoxPosi,&igProcPosi)==MSPFAIL)
		   return WPROCFAIL;

		   *iSrcMbId = mbdsc->iMbId;
		   mbdsc->iProcNum += 1;
		   mbdsc->iMbGrpId = iSrcGrpId;
		   igSrcMbId = *iSrcMbId;
		   igSrcGrpId = iSrcGrpId;
		}
		return MSPSUCCESS;
	   default:
		return PARERR;
	}	
}
/**************************************************************
** Function       : Msp_Put_Msg
** Effect      	: 发送报文
** Parameters    	: 
	sMsg       :报文
	iMsgSize   :报文大小
	cPutType   :请求方式（实时非可靠与可靠非实时）	  
	iDstMbId   :目的邮箱号
	iDstMbGrpId:目的邮箱组号
	iPriority  :邮件优先级
	iClass     :邮件Class
	iType      :邮件类型
	cStoreFlag :邮件内容放置位置 1:放在文件中，文件名为sMsg；0:放在缓存区
** return      	: 成功MSPSUCCESS，失败MSPFAIL
****************************************************************/
short Msp_Put_Msg(
char 		*sMsg, 
unsigned int	iMsgSize,
char            cPutType,	
unsigned short  iDstMbId, 
unsigned short  iDstMbGrpId, 
unsigned short	iPriority,
unsigned short	iClass,
int		iType,
char            cStoreFlag
)
{

	unsigned short 	ilResult;
	Msp_Mb_Tab   	*plMbDsc;
	Msp_Mail_Tab 	*plTmpMailHead,*plPreMailHead,stMailHead;
	unsigned int 	ilMailPosi,ilMailNo;
	char		slOldName[300],slTmpName,stlMsgBuf[2060];
	int 		ilLen, ilTmpLen,ilMsglen,ilType;
	struct	stat	filestat;
	unsigned short ilMbDscPosi,ilQid,ilPriority,ilClass;
	FILE		*fp,*fpTmp;
	long    lDateTime;


	if((pgPubDsc == NULL)||(igSrcMailBoxPosi == MAXMBNUM))
		return(NOTATTACH);
	if(cPutType != REALTIME && cPutType != NOREALTIME 
			&& cPutType != TSPREALTIME )
		return(PARERR);

	/*检查进程是否Attach*/
	if (igSrcMailBoxPosi==MAXMBNUM)
	{
			printf("Msp_Put_Msg: ChkSrcprocAtt Fail--NOTATTACH!\n");
			return NOTATTACH;
	}
		
	if ((iDstMbId/1000)==(atoi(getenv("MSP_NODE_ID"))))
		iDstMbId = iDstMbId % 1000;
	if(iDstMbId > 999)
	{
	  if (FindMbnum(999, 0, &ilMbDscPosi,cPutType)==MSPFAIL)
                return NOMB;
	}
	else
	{
	  if (FindMbnum(iDstMbId,iDstMbGrpId, &ilMbDscPosi,cPutType)==MSPFAIL)
                return NOMB;
	}
	plMbDsc = (Msp_Mb_Tab *)(pgMbHead + ilMbDscPosi * sizeof(Msp_Mb_Tab));

	if ((plMbDsc->cPermFlag == 0 )&&( plMbDsc->iProcNum == 0 ))
		return(NOMB);

	/*检查邮件空间是否够*/
	if ((pgPubDsc->iCurMailNum == pgPubDsc->iMaxMailNum)||
			( plMbDsc->iMailNum >= plMbDsc->iMaxMailNum ))
		return NOSPACE;
	if(cStoreFlag == INFILE)
	{
	   if(stat(sMsg,&filestat)<0)
		return NOTFILE;
	   iMsgSize = filestat.st_size;
	}
	
    ReqFileSer(&ilMailNo);
	
	stMailHead.iSrcMbId = igSrcMbId;
 	stMailHead.iSrcGrpId = igSrcGrpId;
	stMailHead.iDstMbId = iDstMbId;
 	stMailHead.iDstMbGrpId = iDstMbGrpId;
 	stMailHead.iMailNum = ilMailNo;
 	stMailHead.ipriority = iPriority;
 	stMailHead.iclass = iClass;
 	stMailHead.iType = iType;
    stMailHead.iMailSize = iMsgSize;
  
	time(&lDateTime);
	stMailHead.iMailInTime = lDateTime;
	stMailHead.iMailTimeOut = plMbDsc->iMailTimeOut;
	stMailHead.iDieMqId = plMbDsc->iDieMqId;
  

	/*检查存储空间是否够*/

	if(cPutType == NOREALTIME)
	{
	   stMailHead.iMailBeginPosi = pgPubDsc->iMaxMailNum;
	   stMailHead.cStorageFlag = INFILE;
	   sprintf(slOldName,"%s/log/maillog/%d/M%0d",sgMspPath,igBusId,
				ilMailNo);
	   if((fp = fopen(slOldName,"wb+")) == NULL)
	   {
		printf("Open File [%s] Fail",slOldName);
		return MSPFAIL;
	   }
	   if ( fwrite(&stMailHead,sizeof(Msp_Mail_Tab),1,fp) != 1 )
	   {
	    	printf("Write File [%s] Head Fail",slOldName);
	   	return MSPFAIL;
	   }
	   ilTmpLen = iMsgSize;
	   if(cStoreFlag == INSHM)
	   {
             while(ilTmpLen != 0)
             {
		if(ilTmpLen > 8192)
			ilLen = 8192;
		else
			ilLen = ilTmpLen;
                if (fwrite(sMsg,1,ilLen,fp) < 0)
                {
                    printf("Write File [%s] Body fail!\n",slOldName);
                    return MSPFAIL;
                }
		ilTmpLen -= ilLen;
             }
	   }
	   else
	   {
	      if((fpTmp = fopen(sMsg,"rb")) == NULL)
	      {
		printf("Open File [%s] Fail",sMsg);
		return MSPFAIL;
	      }
              while(ilTmpLen != 0)
              {
		if(ilTmpLen > 8192)
			ilLen = 8192;
		else
			ilLen = ilTmpLen;
                if ((ilLen = fread(stlMsgBuf,1,ilLen,fpTmp)) < 0)
                {
                        printf("Read File [%s] fail!\n",sMsg);
                        return MSPFAIL;
                }
                if (fwrite(stlMsgBuf,1,ilLen,fp) < 0)
                {
                    printf("Write File [%s] Body fail!\n",slOldName);
                    return MSPFAIL;
                }
		ilTmpLen -= ilLen;
              }
	      fclose(fpTmp);
	   }
	   fclose(fp);
	}else
	{
	   stMailHead.cStorageFlag = INSHM;
	   if ((ilResult = WritePackMem((char *)pgMailBody,pgPubDsc->iBodySemId,
			sMsg, iMsgSize, 
			&(stMailHead.iMailBeginPosi))) == MSPFAIL)
	   {
		printf("WritePackMem Fail");
		return MSPFAIL;
	   }
	}
	/*将邮件放入邮件头*/
	ESBP(pgPubDsc->iPubSemId);
	ilMailPosi = pgPubDsc->iFstEmptyMH;
	plTmpMailHead = (Msp_Mail_Tab *)(pgMailHead + 
				ilMailPosi * sizeof(Msp_Mail_Tab));
	pgPubDsc->iFstEmptyMH = plTmpMailHead->iNextMail;

	pgPubDsc->iCurMailNum += 1;

	ESBV(pgPubDsc->iPubSemId);
	stMailHead.iMailPosi = ilMailPosi;

	ESBP(pgPubDsc->iMailSemId);
	if(plMbDsc->iFstMail == pgPubDsc->iMaxMailNum)	
	{
		plMbDsc->iFstMail = ilMailPosi;
		stMailHead.iNextMail = pgPubDsc->iMaxMailNum;
	}
	else
	{
		plTmpMailHead =(Msp_Mail_Tab *)(pgMailHead +(plMbDsc->iFstMail) * sizeof(Msp_Mail_Tab));
		/*从邮件链中查找比新邮件优先级低的邮件位置，将新邮件插入该位置*/
		plPreMailHead = plTmpMailHead;
		while ((plTmpMailHead->iNextMail != pgPubDsc->iMaxMailNum) &&
		(plTmpMailHead->ipriority <= iPriority))
		{
			plPreMailHead = plTmpMailHead;
			plTmpMailHead = (Msp_Mail_Tab *)(pgMailHead + \
			(plTmpMailHead->iNextMail) * sizeof(Msp_Mail_Tab));
		}
		if(plPreMailHead == plTmpMailHead)
		{
			if ( plTmpMailHead->ipriority > iPriority )
			{
				plMbDsc->iFstMail = ilMailPosi;
				stMailHead.iNextMail = plTmpMailHead->iMailPosi;
			}else
			{
				plTmpMailHead->iNextMail = ilMailPosi;
				stMailHead.iNextMail = pgPubDsc->iMaxMailNum;
			}
		}else
		{
			if(plTmpMailHead->iNextMail == pgPubDsc->iMaxMailNum)
			{
				plTmpMailHead->iNextMail = ilMailPosi;
				stMailHead.iNextMail = pgPubDsc->iMaxMailNum;
			}
			else
			{
				plPreMailHead->iNextMail = ilMailPosi;
				stMailHead.iNextMail = plTmpMailHead->iMailPosi;
			}
		}
		plTmpMailHead = (Msp_Mail_Tab *)(pgMailHead + \
		ilMailPosi * sizeof(Msp_Mail_Tab));
	}
	memcpy(plTmpMailHead,&stMailHead,sizeof(Msp_Mail_Tab));
	plMbDsc->iMailNum += 1;
	plMbDsc->iMailSize +=iMsgSize;
	plMbDsc->iRcvMailNum += 1;
	if(plMbDsc->iRcvMailNum > 10000000)
		plMbDsc->iRcvMailNum = 0;
	plMbDsc = (Msp_Mb_Tab *)(pgMbHead + igSrcMailBoxPosi * 
			sizeof(Msp_Mb_Tab));
	plMbDsc->iSndMailNum += 1;
	if(plMbDsc->iSndMailNum > 10000000)
		plMbDsc->iSndMailNum = 0;
	ESBV(pgPubDsc->iMailSemId);

	if(pgPubDsc->iInQueFlag != 1)
	{
	  ESBP(pgPubDsc->iPubSemId);
	  pgPubDsc->iInQueFlag = 1;
	  ESBV(pgPubDsc->iPubSemId);
	  esbmsg.mtype = igBusId;
	  SendMsg(pgPubDsc->iInQueId,&esbmsg,sizeof(long));
	}
    return(MSPSUCCESS);
}
/**************************************************************
** Function       : Msp_Get_Gate
** Effect      	: 接收报文
** Parameters    	: sMsg       :报文
		  iUsrLen    :报文大小		  
		  iSrcMbId   :源邮箱号
		  iSrcGrpId:源邮箱组号
		  iSrcBusId  :源邮箱Bus号
		  iSrcNodeId :源邮箱结点号		  	  
		  iPriority  :邮件优先级
		  iClass     :邮件Class
		  iType      :邮件类型
		  cStoreFlag :邮件内容放置位置 
				1:放在文件中，文件名为sMsg；
				0:放在缓存区
		  cReserve   :邮件读后是否保留
		  iTimeOut   :超时时间
** return      	: 成功MSPSUCCESS，失败MSPFAIL
****************************************************************/
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
)
{
	int 		i=0;
	int		iReadsize;
	unsigned short	ilRet;
	FILE 		*pFile, *pFile1;
	unsigned short 	ilResult,*plMbPosi,iDieMq;
	unsigned int	ilFstBkId;
	Msp_Mb_Tab 	*plTmpMb;
	int		ilMailLength;
	char		cStorageMail;
	char 		sFileName[300];
	short		iSeconds;
	int			iTimeUsed;
	
	iSeconds = iTimeOut;
	iTimeUsed = 0;
	if((pgPubDsc == NULL)||(igSrcMailBoxPosi == MAXMBNUM))
		return(NOTATTACH);
	if((*cStoreFlag != INSHM)&&(*cStoreFlag != INFILE))
		return PARERR;
	if(cReserve != UNRESERVE && cReserve != RESERVE)
		return(PARERR);


	plTmpMb = (Msp_Mb_Tab *)(pgMbHead + (igSrcMailBoxPosi) 
					* sizeof(Msp_Mb_Tab));
	stpHead->iPriority = *iPriority;
	stpHead->iClass	   = *iClass;
	stpHead->iType	   = *iType;
	iDieMq = plTmpMb->iDieMqId;
	for(;;)
	{
		/*根据相应的位置获得相应邮箱的头邮件位置*/
		cStorageMail = INSHM;

		ESBP(pgPubDsc->iMailSemId);

		if(ReadMailHead( &(plTmpMb->iFstMail), stpHead, &ilMailLength,
		&ilFstBkId, sFileName, &cStorageMail, cReserve, iDieMq)==NOMAIL)
		{
			ESBV(pgPubDsc->iMailSemId);
			if(BlockRead(igProcPosi,iSeconds, &iTimeUsed) == READTIMEOUT)
			{
				return READTIMEOUT;
			}
			if (iTimeOut != -1)
			{
				iSeconds = iSeconds - iTimeUsed;
				if (iSeconds <= 0)
					return READTIMEOUT;
			}
		}
		else
		{
			if (plTmpMb->iMailNum == 0) 
			{
				ESBV(pgPubDsc->iMailSemId);

			return MSPFAIL;
			}
			break;
		}
	}
	*iPriority = stpHead->iPriority;
	*iClass = stpHead->iClass;
	*iType = stpHead->iType;
	*iSrcMbId = stpHead->iSrcMbId;
	*iSrcGrpId = stpHead->iSrcGrpId;
	
       /*根据相应邮箱的头位置查找相应优先级、组、类型的邮件头，并修改相应链表*/
	plTmpMb->iMailNum -= 1;
	plTmpMb->iMailSize -= ilMailLength;
	ESBV(pgPubDsc->iMailSemId);
	/*如果找到邮件读出邮件并放入内存或者文件*/
	if(ilMailLength >= *iUsrLen )
	{
		*cStoreFlag = INFILE;
		sprintf(sMsg,"%d",ilFstBkId);
	}
	*iUsrLen = ilMailLength;
	switch(cStorageMail)
	{
	   case INSHM:
		return (ReadPackMem((char *)pgMailBody,pgPubDsc->iBodySemId,
				sMsg,cReserve,iUsrLen, ilFstBkId));
	   	break;
	   case INFILE:
	        if(ReadFileList(sMsg,*iUsrLen,sFileName,*cStoreFlag)==MSPFAIL)
	        	return MSPFAIL;
	        if (cReserve == UNRESERVE)
		    unlink(sFileName);
	   	break;
	}
	return MSPSUCCESS;
}

short Msp_Get_MqInfo(
unsigned short  iSrcMbId, 
unsigned short  iSrcGrpId,
unsigned int   *MailNum,	/*邮箱当前邮件数*/
unsigned int   *RcvMailNum,	/*邮箱接收邮件数*/
unsigned int   *SndMailNum	/*邮箱发送邮件数*/
)
{
	int 	i;
	Msp_Mb_Tab 	*pTmp;
	
	if((pgPubDsc == NULL)||(igSrcMailBoxPosi == MAXMBNUM))
		return(NOTATTACH);
	
	for(i=0;i<pgPubDsc->iMaxMbNum;i++)
	{
		pTmp =(Msp_Mb_Tab *)(pgMbHead + i*sizeof(Msp_Mb_Tab));
		if ((pTmp->iMbId == iSrcMbId)&&(pTmp->iMbGrpId == iSrcGrpId))
		{
			*MailNum = pTmp->iMailNum;
			*RcvMailNum = pTmp->iRcvMailNum;
			*SndMailNum = pTmp->iSndMailNum;
			return MSPSUCCESS;		
		}
	}
	return MSPFAIL;
}

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
)
{
	Msp_Msg_Head	stMsgHead;

	return(Msp_Get_Gate(sMsg,iUsrLen,iSrcMbId,iSrcGrpId,iPriority,iClass,
		iType,cStoreFlag,cReserve,iTimeOut,&stMsgHead));
}
/**************************************************************
** Function       : Msp_Detach
** Effect      	: 断开与源邮箱的连接
** Parameters    	: iTimeOut   :超时时间
** return      	: 成功MSPSUCCESS，失败MSPFAIL
****************************************************************/
short Msp_Detach()
{
	Msp_Mb_Tab *pMbBuf;
	pid_t ilPid;
	unsigned short ilSrcMbPosi;

	printf("%s %s", "函数开始", __func__);
	if((igSrcMbId == MAXMBNUM)||(pgPubDsc == NULL)||(pgMbHead == NULL))
	{
		printf("Msp_Detach: NOTATTACH\n");
		return NOTATTACH;
	}
	
	ilPid = getpid();
	ilSrcMbPosi = 1000;
	if ( DeleteProcdsc( ilPid,&ilSrcMbPosi ) == MSPFAIL)
	{
		
		shmdt((char *)pgPubDsc);
		shmdt((char *)pgMbHead);
		
		pgPubDsc = 0;
		pgMbHead = 0;

		return DELPROCFAIL;
	}

	pMbBuf = (Msp_Mb_Tab *)(pgMbHead + ilSrcMbPosi 
		* sizeof(Msp_Mb_Tab));
	if( pMbBuf->iProcNum == 0 )
		return(NOTATTACH);
	
	if(( pMbBuf->iProcNum == 0 )&&(pMbBuf->cPermFlag == TMPMB))
	{
		Msp_Del_Msg(ilSrcMbPosi);
		ESBP(pgPubDsc->iMailSemId);
	   	pMbBuf->cMbDscFlag = 0;
		ESBV(pgPubDsc->iMailSemId);

		ESBP(pgPubDsc->iPubSemId);
		pgPubDsc->iCurMbNum -= 1;
		ESBV(pgPubDsc->iPubSemId);
	}
	
	ESBP(pgPubDsc->iMailSemId);
	pMbBuf->iProcNum -= 1;
	ESBV(pgPubDsc->iMailSemId);

	shmdt((char *)pgPubDsc);
	shmdt((char *)pgMbHead);
	igSrcMailBoxPosi = MAXMBNUM;



	pgPubDsc = 0;
	pgMbHead = 0;

	return MSPSUCCESS;

}

short Msp_Del_Msg(unsigned short ilSrcMbPosi)
{	
	Msp_Mail_Tab *plCur,*plNext;
	unsigned int ilNextPosi,ilCurPosi,ilPrePosi;
	int i = 0;
	char slFileName[300];
	Msp_Mb_Tab *pMbBuf;
	
	pMbBuf = (Msp_Mb_Tab *)(pgMbHead + ilSrcMbPosi 
		* sizeof(Msp_Mb_Tab));
    /*如果邮件链第一个邮件位置为pgPubDsc->iMaxMailNum时，标志内存块中没有邮件*/
	if (pMbBuf->iFstMail == pgPubDsc->iMaxMailNum)
		return(NOMAIL);	
	/*移动指针到邮件链中的第一个邮件的位置*/
	ESBP(pgPubDsc->iMailSemId);

	ilPrePosi = pMbBuf->iFstMail;

	plCur =(Msp_Mail_Tab *)(pgMailHead + ilPrePosi * sizeof(Msp_Mail_Tab));
	/*查找符合条件的邮件位置*/	
	while (1)
	{
		 ilNextPosi = plCur->iNextMail;
		 if(plCur->cStorageFlag == INSHM)
		 		DelPackMem((char *)pgMailBody,pgPubDsc->iBodySemId,plCur->iMailBeginPosi);
		 else
		 	{
		 		sprintf(slFileName,"%s/log/maillog/%d/M%0d",sgMspPath,
						igBusId,plCur->iMailNum);
				unlink(slFileName);
		 	}
		 DelMailHead(plCur->iMailPosi);
		 if(ilNextPosi == pgPubDsc->iMaxMailNum) 
			break;
		 plNext = (Msp_Mail_Tab *)(pgMailHead + ilNextPosi * sizeof(Msp_Mail_Tab));
	 }/*endwhile*/
	 ESBV(pgPubDsc->iMailSemId);

	  return MSPSUCCESS;
}
/**************************************************************
** Function       : FindMail
** Effect      	: 查找邮件描述区
** Parameters    	: fstmail    : 要查找的邮件链头邮件描述区号；
		   srcmbid    : 原邮箱的id号；
		   priority   : 邮件的优先级；
		   class      : 用户定义；
		   type       : 用户定义；
		   mailposi   : 邮件描述区号；		  	  
** return      	: 成功 MSPSUCCESS，失败 MSPFAIL
****************************************************************/
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
)
{	
	Msp_Mail_Tab *plCur,*plPre;
	unsigned int ilPrePosi,ilCurPosi;
	long    lDateTime;
	Msp_Mb_Tab   	*plMqCur;


    /*如果邮件链第一个邮件位置为pgPubDsc->iMaxMailNum时，标志内存块中没有邮件*/
	if (*ilFstMail == pgPubDsc->iMaxMailNum)
		return(NOMAIL);	

	/*判断是否还有后续死信队列*/
	if(iDieMq != 0)
 	{
		/*移动指针到邮件链中的第一个邮件的位置*/
		plMqCur = (Msp_Mb_Tab *)(pgMbHead + (igSrcMailBoxPosi) 
		* sizeof(Msp_Mb_Tab));
		ilCurPosi = *ilFstMail;
		ilPrePosi = ilCurPosi;
		/*查找符合条件的邮件位置*/	
		time(&lDateTime);	
		while (ilCurPosi != pgPubDsc->iMaxMailNum)
		{
			plCur =(Msp_Mail_Tab *)(pgMailHead + ilCurPosi * sizeof(Msp_Mail_Tab));
			
			if((lDateTime - plCur->iMailInTime) > plCur->iMailTimeOut)
			{
				if(ilCurPosi == ilPrePosi)
				{
					*ilFstMail = plCur->iNextMail;
				}else
				{
					plPre =(Msp_Mail_Tab *)(pgMailHead + ilPrePosi * sizeof(Msp_Mail_Tab));
					plPre->iNextMail = plCur->iNextMail;
				}
				if(MoveDieMail(plCur->iMailPosi,plCur->iDieMqId,plCur->iDstMbGrpId) != MSPSUCCESS)
				{
					printf("迁移超时报文到死信队列失败！");
				}

				plMqCur->iMailNum -= 1;
				plMqCur->iMailSize -= plCur->iMailSize;
				plMqCur->iSndMailNum += 1;
				if(plMqCur->iSndMailNum > 10000000)
				plMqCur->iSndMailNum = 0;	
					
				ilCurPosi = *ilFstMail;
				ilPrePosi = ilCurPosi;
				continue;
			}
			ilPrePosi = ilCurPosi;
			ilCurPosi = plCur->iNextMail;
		}/*endwhile*/
		if (*ilFstMail == pgPubDsc->iMaxMailNum)
			return(NOMAIL); 

		/*移动指针到邮件链中的第一个邮件的位置*/
	}
	/*判断是否还有后续死信队列*/
	ilPrePosi = *ilFstMail;

	plCur =(Msp_Mail_Tab *)(pgMailHead + ilPrePosi * sizeof(Msp_Mail_Tab));
	/*查找符合条件的邮件位置*/	

	if(((stpHead->iClass == 0) &&(stpHead->iType == 0)) ||
		(plCur->iNextMail == pgPubDsc->iMaxMailNum))
	{
		if(plCur->iNextMail == pgPubDsc->iMaxMailNum)
		{
		  if (((plCur->iclass != 0) && (stpHead->iClass != 0) && 
			(plCur->iclass != stpHead->iClass)) || 
		    	((stpHead->iType != 0) && 
				(plCur->iType != stpHead->iType)))
		    	return NOMAIL;		   
		}
		stpHead->iPriority = plCur->ipriority;
		stpHead->iClass = plCur->iclass;
		stpHead->iType = plCur->iType;
		stpHead->iSrcMbId = plCur->iSrcMbId;
		stpHead->iSrcGrpId = plCur->iSrcGrpId;
		stpHead->iDstMbId = plCur->iDstMbId;
		stpHead->iDstMbGrpId = plCur->iDstMbGrpId;
		*ilFstBkId = plCur->iMailBeginPosi;
		*iUsrLen = plCur->iMailSize;
		*cStorageFlag = plCur->cStorageFlag;
		sprintf(slFileName,"%s/log/maillog/%d/M%0d",sgMspPath,
			igBusId,plCur->iMailNum);
		if (cReserve == UNRESERVE)
		{
		    *ilFstMail = plCur->iNextMail;		    
		    DelMailHead(plCur->iMailPosi);
		}
		return(MSPSUCCESS);
	}else
	{
	   plPre = plCur;
	   while (1)
	   {
		if (((plCur->iclass == 0) || (stpHead->iClass == 0) || 
			(plCur->iclass == stpHead->iClass)) && 
		    ((stpHead->iType == 0) || 
			(plCur->iType == stpHead->iType)))
		{
		    stpHead->iPriority = plCur->ipriority;
		    stpHead->iClass = plCur->iclass;
		    stpHead->iType = plCur->iType;
		    stpHead->iSrcMbId = plCur->iSrcMbId;
		    stpHead->iSrcGrpId = plCur->iSrcGrpId;
		    stpHead->iDstMbId = plCur->iDstMbId;
		    stpHead->iDstMbGrpId = plCur->iDstMbGrpId;
		    *ilFstBkId = plCur->iMailBeginPosi;
		    *iUsrLen = plCur->iMailSize;
		    *cStorageFlag = plCur->cStorageFlag;
		    sprintf(slFileName,"%s/log/maillog/%d/M%0d",sgMspPath,
				    igBusId,plCur->iMailNum);
		    if (cReserve == UNRESERVE)
		    {
		      if( plPre == plCur)
		         *ilFstMail = plCur->iNextMail;
		      else
		      	plPre->iNextMail = plCur->iNextMail;
		      DelMailHead(plCur->iMailPosi);
		    }
 		    return(MSPSUCCESS);
		 }
		 if(plCur->iNextMail == pgPubDsc->iMaxMailNum) 
			break;
	   	 plPre = plCur;
		 plCur = (Msp_Mail_Tab *)(pgMailHead + 
			        (plCur->iNextMail) * sizeof(Msp_Mail_Tab));
	    }/*endwhile*/
	    return NOMAIL;
	 }/*endif*/
}

short MoveDieMail(unsigned int iMailPosi,
		unsigned short iDieMqId,
		unsigned short iDieMqGrpId)
{

	Msp_Mb_Tab   	*plMbDsc;
	Msp_Mail_Tab 	*plCurMailHead,*plPreMailHead,*plTmpMailHead;
	signed long		lDateTime;
	unsigned short	ilMbDscPosi;

	if((pgPubDsc == NULL)||(igSrcMailBoxPosi == MAXMBNUM))
		return(NOTATTACH);

	if(iDieMqId == 0)
  	{
  		printf("未配置死信队列，报文丢弃");
  		return MSPSUCCESS;
  	}

	/*检查进程是否Attach*/

	if (igSrcMailBoxPosi==MAXMBNUM)
	{
		printf("Msp_Put_Msg: ChkSrcprocAtt Fail--NOTATTACH!\n");
		return NOTATTACH;
	}
					
	if ((iDieMqId/1000)==(atoi(getenv("MSP_NODE_ID"))))
		iDieMqId = iDieMqId % 1000;
	if(iDieMqId > 999)
	{
	  if (FindMbnum(999, 0, &ilMbDscPosi,NOREALTIME)==MSPFAIL)
                return NOMB;
	}
	else
	{
	  if (FindMbnum(iDieMqId,iDieMqGrpId, &ilMbDscPosi,NOREALTIME)==MSPFAIL)
                return NOMB;
	}
	plMbDsc = (Msp_Mb_Tab *)(pgMbHead + ilMbDscPosi * sizeof(Msp_Mb_Tab));


	if ((plMbDsc->cPermFlag == 0 )&&( plMbDsc->iProcNum == 0 ))
		return(NOMB);


	/*检查邮件空间是否够*/
	if ((pgPubDsc->iCurMailNum == pgPubDsc->iMaxMailNum)||
			( plMbDsc->iMailNum >= plMbDsc->iMaxMailNum ))
	{
		printf("ERROR:MoveDieMail DieMqId Nospace");
		return NOSPACE;
	}

	 plTmpMailHead =(Msp_Mail_Tab *)(pgMailHead + iMailPosi * sizeof(Msp_Mail_Tab));

	time(&lDateTime);
	plTmpMailHead->iMailInTime = lDateTime;
	plTmpMailHead->iMailTimeOut = plMbDsc->iMailTimeOut;
	plTmpMailHead->iDieMqId = plMbDsc->iDieMqId;

		
	if(plMbDsc->iFstMail == pgPubDsc->iMaxMailNum)	
	{
		plMbDsc->iFstMail = iMailPosi;
		plTmpMailHead->iNextMail = pgPubDsc->iMaxMailNum;
	}
	else
	{
	     plCurMailHead =(Msp_Mail_Tab *)(pgMailHead +(plMbDsc->iFstMail) * sizeof(Msp_Mail_Tab));
             /*从邮件链中查找比新邮件优先级低的邮件位置，将新邮件插入该位置*/
	     plPreMailHead = plCurMailHead;
		while ((plCurMailHead->iNextMail != pgPubDsc->iMaxMailNum) &&
                       (plCurMailHead->ipriority <= plTmpMailHead->ipriority))
		{
			plPreMailHead = plCurMailHead;
			plCurMailHead = (Msp_Mail_Tab *)(pgMailHead + \
                 	(plCurMailHead->iNextMail) * sizeof(Msp_Mail_Tab));
		}
	    if(plPreMailHead == plCurMailHead)
	    {
			 if ( plCurMailHead->ipriority > plTmpMailHead->ipriority )
			 {
				plMbDsc->iFstMail = iMailPosi;
				plTmpMailHead->iNextMail = plCurMailHead->iMailPosi;
			 }else
			 {
				plCurMailHead->iNextMail = iMailPosi;
				plTmpMailHead->iNextMail = pgPubDsc->iMaxMailNum;
			 }
	    }else
	    {
	        if(plCurMailHead->iNextMail == pgPubDsc->iMaxMailNum)
	        {
				plCurMailHead->iNextMail = iMailPosi;
				plTmpMailHead->iNextMail = pgPubDsc->iMaxMailNum;
	        }
	        else
	        {
				plPreMailHead->iNextMail = iMailPosi;
				plTmpMailHead->iNextMail = plCurMailHead->iMailPosi;
	        }
	    }
	}
	plMbDsc->iMailNum += 1;
	plMbDsc->iMailSize +=plTmpMailHead->iMailSize;
	plMbDsc->iRcvMailNum += 1;
	if(plMbDsc->iRcvMailNum > 10000000)
		plMbDsc->iRcvMailNum = 0;
	plMbDsc = (Msp_Mb_Tab *)(pgMbHead + igSrcMailBoxPosi * 
			sizeof(Msp_Mb_Tab));
	plMbDsc->iSndMailNum += 1;
	if(plMbDsc->iSndMailNum > 10000000)
		plMbDsc->iSndMailNum = 0;

	if(pgPubDsc->iInQueFlag != 1)
	{
	  ESBP(pgPubDsc->iPubSemId);
	  pgPubDsc->iInQueFlag = 1;
	  ESBV(pgPubDsc->iPubSemId);
	  esbmsg.mtype = igBusId;
	  SendMsg(pgPubDsc->iInQueId,&esbmsg,sizeof(long));
	}
  return(MSPSUCCESS);
}

/**************************************************************
** Function      	: DelMailHead
** Effect      	: 将一个不用的邮件头连到空邮件头链中
** Parameters    	: iPosi        : 不用的邮件头位置；		  	  
** return      	: 成功 MSPSUCCESS，失败 MSPFAIL
****************************************************************/
unsigned short DelMailHead(unsigned int iPosi)
{
	Msp_Mail_Tab *pTmp;
	/*计算进程空间开始地址*/

	ESBP(pgPubDsc->iPubSemId);
	pTmp = (Msp_Mail_Tab *)(pgMailHead + iPosi * sizeof(Msp_Mail_Tab));
	pTmp->iNextMail = pgPubDsc->iFstEmptyMH;
	pgPubDsc->iFstEmptyMH = iPosi;
	pgPubDsc->iCurMailNum -= 1;		    	
	ESBV(pgPubDsc->iPubSemId);
	return(MSPSUCCESS);
}
unsigned short ReadMemList
( 
	char 		*msg,
	int 		msgsize, 
	unsigned int 	iFstbkid,
	char		cStorageFlag
)
{
	unsigned int 	ilNextbkid,ilCurbkid,*ilIntTmp;  
	char 		*plTmp;
	unsigned int 	ilMsgSize;
	FILE		*fp;
	
	
	ilNextbkid = iFstbkid;
	if( cStorageFlag == INFILE)
	{
	    if ((fp = fopen(msg, "w+")) == NULL)
	    {
			return(MSPFAIL);
	    }
	}else
	{
		plTmp = msg;
	}
	while((ilNextbkid != pgPubDsc->iMaxStoreNum ) && (msgsize > 0))
	{
		ilCurbkid = ilNextbkid;
		ilIntTmp = (unsigned int *)(pgMailBody + ilCurbkid * 
					igBlockSize);
		ilNextbkid = *ilIntTmp;
		if( msgsize > pgPubDsc->iBlockSize)
			ilMsgSize = pgPubDsc->iBlockSize;	
		else
			ilMsgSize = msgsize;

		if(cStorageFlag == INFILE)
		{
		    fwrite((void *)(pgMailBody + ilCurbkid * igBlockSize 
			  	+ sizeof(unsigned int)),ilMsgSize,1,fp);
		}
		else
		{
		    memcpy(plTmp,(unsigned  char *)(pgMailBody + ilCurbkid * 
			igBlockSize + sizeof(unsigned int)),ilMsgSize );
			plTmp+=ilMsgSize;
		}		

		msgsize = msgsize - ilMsgSize;
	}
	if(cStorageFlag == INFILE)
		fclose(fp);
	return MSPSUCCESS;
}
unsigned short ReadFileList
( 
	char 	*msg,
	int 	msgsize, 
	char	*sFileName,
	char	cStorageFlag
)
{
	unsigned char	slTmpBuf[2100];  
	char 		*plTmp;
	unsigned int 	ilMsgSize;
	FILE		*fpDst,*fpSrc;
	
	
	if( cStorageFlag == INFILE)
	{
	    if ((fpDst = fopen(msg, "wb+")) == NULL)
	    {
			return(MSPFAIL);
	    }
	}else
	{
		plTmp = msg;
	}
	if ((fpSrc = fopen(sFileName, "rb")) == NULL)
	{
		return(MSPFAIL);
	}
	fseek(fpSrc,sizeof(Msp_Mail_Tab),SEEK_SET);
	while((!feof(fpSrc)) && (msgsize > 0))
	{
		if( msgsize > pgPubDsc->iBlockSize)
			ilMsgSize = 2048;	
		else
			ilMsgSize = msgsize;
		if((ilMsgSize = fread(slTmpBuf,1,ilMsgSize,fpSrc)) <= 0)
		{
		    if(cStorageFlag == INFILE)
		    {
		    	fclose(fpDst);
		    }
		    fclose(fpSrc);
		    return MSPFAIL;
		}		
		
		if(cStorageFlag == INFILE)
		{
			fwrite((void *)slTmpBuf,ilMsgSize,1,fpDst);
		}
		else
		{
			memcpy(plTmp,slTmpBuf,ilMsgSize );
			plTmp+=ilMsgSize;
		}		

		msgsize = msgsize - ilMsgSize;
	}
	fclose(fpSrc);
	if (cStorageFlag == INFILE)
		fclose(fpDst);
	return MSPSUCCESS;
}

unsigned short	BlockRead(unsigned short ilProcPosi,short iTimeOut, int *iTimeUsed)
{
	unsigned short ilRet = MSPSUCCESS;
	int 		iLen;
	int         iSeconds;
	time_t		tick0, tick;

	iSeconds = iTimeOut;
	AddSubProc(ilProcPosi,1);
	/*接收消息*/
	esbmsg.mtype = igUsrPid;
	iLen = sizeof(long);
	tick0 = time(NULL);
	if (((ilRet = RcvMsg(pgPubDsc->iOutQueId, &esbmsg, &iLen,iSeconds))) == ESBFAIL)
	{
	   AddSubProc(ilProcPosi,0);
	   printf("BlockRead Read Eror[%d] reason[%s]",errno,strerror(errno));
	   tick = time(NULL);
	   *iTimeUsed = tick - tick0;
	   return READQUEERR;
	}
	AddSubProc(ilProcPosi,0);
    tick = time(NULL);
    *iTimeUsed = tick - tick0;
	return ilRet;
}

/** 
 * @fn short Msp_Detach_Proc(pid_t ilPid)
 * @brief 断开进程与源邮箱的连接
 * 
 * @param ilPid
 * 
 * @retval MSPSUCCESS 成功 
 * @retval MSPFAIL 失败
 */
short Msp_Detach_Proc(pid_t ilPid)
{
	Msp_Mb_Tab *pMbBuf;
	unsigned short ilSrcMbPosi;

	if((igSrcMbId == MAXMBNUM)||(pgPubDsc == NULL)||(pgMbHead == NULL))
	{
		printf("Msp_Detach_Proc: NOTATTACH\n");
		return NOTATTACH;
	}

	if ( DeleteProcdsc( ilPid,&ilSrcMbPosi ) == MSPFAIL)
	{
		return DELPROCFAIL;
	}

	pMbBuf = (Msp_Mb_Tab *)(pgMbHead + ilSrcMbPosi 
		* sizeof(Msp_Mb_Tab));
	if( pMbBuf->iProcNum == 0 )
		return(NOTATTACH);
	
	if(( pMbBuf->iProcNum == 1 )&&(pMbBuf->cPermFlag == TMPMB))
	{
		Msp_Del_Msg(ilSrcMbPosi);
		ESBP(pgPubDsc->iMailSemId);
	   	pMbBuf->cMbDscFlag = 0;
		ESBV(pgPubDsc->iMailSemId);

		ESBP(pgPubDsc->iPubSemId);
		pgPubDsc->iCurMbNum -= 1;
		ESBV(pgPubDsc->iPubSemId);
	}
	
	ESBP(pgPubDsc->iMailSemId);
	pMbBuf->iProcNum -= 1;
	ESBV(pgPubDsc->iMailSemId);

	return MSPSUCCESS;

}


