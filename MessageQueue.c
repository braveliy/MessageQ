#ifndef lint
static const char rcsid[] = "$Id: libmspapi30.c,v 1.2.2.2 2011/06/29 18:17:36 mymtom Exp $";
#endif /* not lint */

/**
 * @file	libmspapi30.c
 * @brief	
 */

#include "esb.h"
#include "msp.h"
#include "tsp30.h"
/**********************************************************/
/*Function：TSPMBReqattach()                                */
/*Effect：  按申请临时邮箱                                  */
/*Parameters：                                              */
/*           short *qid      - 邮箱号                     */
/*                                              */
/**********************************************************/
int TSPMBReqattach(
	short 	*qid
)
{
	unsigned short iBusId;
	unsigned short iGroupId;
	int ilRc;

	iBusId	= atoi(getenv("MSP_BUS_ID"));
	iGroupId= atoi(getenv("MSP_GROUP_ID"));
	ilRc 	= Msp_attach((unsigned short *)qid,"NULL",iGroupId,iBusId,iATTACHREQ);
	if(ilRc == MSPSUCCESS)
		return SUCCESS;
	else
		return ilRc;
}
/**********************************************************/
/*Function：TSPMBattach()                                    */
/*Effect：  侦测邮箱                                        */
/*           short qid      - 邮箱号                      */
/*                                              */
/**********************************************************/
int TSPMBattach(short qid)
{
	unsigned short iBusiId;
	unsigned short iGroupId;
	int ilRc;
	iBusiId=atoi(getenv("MSP_BUS_ID"));
	iGroupId=atoi(getenv("MSP_GROUP_ID"));
	ilRc=Msp_attach((unsigned short *)&qid,"NULL", iGroupId, iBusiId, iATTACHNUM);
	if(ilRc==MSPSUCCESS)
		return SUCCESS;
	else
		return ilRc;
}
/**********************************************************/
/*Function：TSPMBdetach()                                    */
/*Effect：  关闭邮箱                                        */
/*                                                        */
/*                                              */
/**********************************************************/
int TSPMBdetach( )
{
	int ilRc;
	ilRc=Msp_Detach();
	if(ilRc==MSPSUCCESS)
		return SUCCESS;
	else
		return ilRc;
}
/**********************************************************/
/*Function：TSPMBread()                                    */
/*Effect：  阻塞向邮箱接收数据                              */
/*Parameters： char *msg_area - 报文                        */
/*           short msg_size - 报文长度                    */
/*           short qid      - 邮箱号                      */
/*           short mpriority- 优先级                      */
/*           short mclass   - 报文类                      */
/*           short mtype    - 报文类型                    */
/*                                              */
/**********************************************************/
int TSPMBread(char *msg_area, unsigned int  *msg_size, short *qid,
short *mpriority, short *mclass, int *mtype)
{
	char flag;
	unsigned short ilClass, ilPriority;
	unsigned short iSrcMbGrpId;
	int ilRc, ilType;
	unsigned int ilMsglen ;
	unsigned short ilQid;
	MSGHEAD *plMsgHead;
	long	Time;
	long		mmSec;



	ilMsglen = iMSGMAXLEN;
	ilQid=*qid;
	ilType = *mtype;
	ilPriority= *mpriority;
	ilClass= *mclass;
	flag=INSHM;
	iSrcMbGrpId=atoi(getenv("MSP_GROUP_ID"));
	ilRc =Msp_Get_Msg(msg_area,&ilMsglen,&ilQid,&iSrcMbGrpId,
		&ilPriority,&ilClass,&ilType,&flag,UNRESERVE,-1);

	if(ilRc==ESBSUCCESS)
	{
		*qid=ilQid;
		*mpriority=ilPriority;
		*mclass=ilClass;
		*mtype=ilType;
		*msg_size=ilMsglen;
		/*modi 20090628*/
		plMsgHead=(MSGHEAD *)msg_area;


		igTrnid	= plMsgHead->lTrnid;
		igSwitch_mq = plMsgHead->switch_mq;/*add  2010-11-4*/
		/* 2010-04-23 mymtom 对非交易报文，日志需要分离 */
		/*
		if(plMsgHead->iLogFlag == SEPFILE)
		*/
		if ( (plMsgHead->iLogFlag  + 0 == SEPFILE)
		  || (plMsgHead->iMsgClass + 0 == iMSGAPLOGIN)
		  || (plMsgHead->iMsgClass + 0 == iMSGAPLOGOUT)
		  || (plMsgHead->iMsgClass + 0 == iMSGAPP)
		  || (plMsgHead->iMsgClass + 0 == iMSGORDER) )
		{
			igLogLevel = plMsgHead->iLogLevel;
			strcpy(sgDebugFile,sgDebugFile_src);
		}else
		{
			igLogLevel = plMsgHead->iLogLevel;
			strcpy(sgDebugFile,plMsgHead->sLogFileName);
			/*
			printf("%s", "");
			printf("begin module debug file :[%s]",sgDebugFile_src);
			*/
		}
		if(plMsgHead->iTraceFlag == GETMODCOST)
		{
			GetTimess(&Time,&mmSec);
			plMsgHead->iInModuleTime = Time;
			plMsgHead->iInModulemmTime = mmSec;
		}
		/*modi  20090628*/
		if(igLogLevel >= 3 )
		{
			printf("Read  :%3d->%3d:pid[%5d] Pri[%4d] Class[%4d] Type[%d] Msglen[%5d]\n",
				ilQid,plMsgHead->iDestMQ,getpid(),ilPriority,ilClass,ilType,ilMsglen);
		}
		return 0;
	}
	else
	{
		return ilRc;
	}
}
/**********************************************************/
/*Function：TSPMBwriteReal()                                */
/*Effect：	  阻塞向邮箱发送数据                              */
/*Parameters： char *msg_area - 报文                        */
/*           short msg_size - 报文长度                    */
/*           short qid      - 邮箱号                      */
/*           short mpriority- 优先级                      */
/*           short mclass   - 报文类                      */
/*           short mtype    - 报文类型                    */
/*	     short Flag     - 是否实时(REALTIME/NOREALTIME)*/
/*                                              */
/**********************************************************/
int TSPMBwriteReal(char 	*msg_area,unsigned int 	msg_size,short 	qid,
	short 	mpriority,short 	mclass,short 	mtype,short iFlag)
{
	unsigned short iGroupId;
	int ilRc;
	iGroupId = atoi(getenv("MSP_GROUP_ID"));
	ilRc = Msp_Put_Msg(msg_area,msg_size,iFlag,qid,iGroupId,mpriority,mclass,mtype,INSHM);
	if(ilRc == MSPSUCCESS)
		return SUCCESS;
	else
		return ilRc;
}
/**********************************************************/
/*Function：TSPMBwrite()                                    */
/*Effect：  阻塞向邮箱发送数据                              */
/*Parameters： char *msg_area - 报文                        */
/*           short msg_size - 报文长度                    */
/*           short qid      - 邮箱号                      */
/*           short mpriority- 优先级                      */
/*           short mclass   - 报文类                      */
/*           short mtype    - 报文类型                    */
/*                                              */
/**********************************************************/
int TSPMBwrite(char *msg_area, unsigned int msg_size, short qid,
		short mpriority, short mclass, int mtype)
{
	unsigned short iGroupId;
	int ilRc;
	long Time,mmSec;
	MSGHEAD *plMsgHead;
  short ilMsgClass;
	
	iGroupId=atoi(getenv("MSP_GROUP_ID"));

	plMsgHead=(MSGHEAD *)msg_area;

	if(igLogLevel >= 3)
		printf("Before Msp_Put_Msg MQ[%d]",qid);
	ilRc =Msp_Put_Msg(msg_area,msg_size,TSPREALTIME,qid,iGroupId,mpriority,mclass,mtype,INSHM);
	if(ilRc==MSPSUCCESS)
	{
		if(igLogLevel >= 3)
		{
		  printf("Write :[%d]->[%d]:pid[%d] Pri[%d] Class[%d] Type[%d] Msglen[%d]\n",plMsgHead->iSrcMQ,qid,getpid(),mpriority,mclass,mtype,msg_size);
		}
		if(plMsgHead->iTraceFlag == GETMODCOST)
		{
			ilMsgClass = plMsgHead->iMsgClass;
			plMsgHead->iMsgClass = iMSGMODCOST;
			GetTimess(&Time,&mmSec);
			plMsgHead->iCostTime = (Time - plMsgHead->iInModuleTime)*1000*1000
													+ (mmSec - plMsgHead->iInModulemmTime);
			ilRc =Msp_Put_Msg(msg_area,iMSGHEADLEN,TSPREALTIME,iMBMONSYS,iGroupId,mpriority,mclass,mtype,INSHM);
			if(ilRc!=MSPSUCCESS)
			{
				printf("SrcModeMQ[%d] Write iMBMONSYS FAIL",plMsgHead->iSrcMQ);
			}
			plMsgHead->iMsgClass = ilMsgClass;
		}
		return SUCCESS;
	}
	else
		return ilRc;
}


/**********************************************************/
/*Function：TSPMBreadnw()                                    */
/*Effect：  阻塞向邮箱接收数据                              */
/*Parameters： char *msg_area - 报文                        */
/*           short msg_size - 报文长度                    */
/*           short qid      - 邮箱号                      */
/*           short mpriority- 优先级                      */
/*           short mclass   - 报文类                      */
/*           short mtype    - 报文类型                    */
/*           short wtime    - 超时时间                    */
/*                                              */
/**********************************************************/
int TSPMBreadnw(char *msg_area, unsigned int *msg_size, short *qid,
short *mpriority, short *mclass, int *mtype, short wtime)
{
	char flag;
	unsigned short ilClass, ilPriority;
	unsigned short iSrcMbGrpId;
	int ilRc, ilType;
	unsigned int ilMsglen ;
	long Time;
	unsigned short ilQid;
	MSGHEAD *plMsgHead;

	ilMsglen = iMSGMAXLEN;
	ilQid=*qid;
	ilType = *mtype;
	ilPriority= *mpriority;
	ilClass= *mclass;
	flag=INSHM;
	iSrcMbGrpId=atoi(getenv("MSP_GROUP_ID"));
	ilRc =Msp_Get_Msg(msg_area,&ilMsglen,&ilQid,&iSrcMbGrpId,
		&ilPriority,&ilClass,&ilType,&flag,UNRESERVE,wtime);

	if(ilRc==MSPSUCCESS)
	{
		*qid=ilQid;
		*mpriority=ilPriority;
		*mclass=ilClass;
		*mtype=ilType;
		*msg_size=ilMsglen;
	/*modi  20090628*/
		plMsgHead=(MSGHEAD *)msg_area;
		igSwitch_mq = plMsgHead->switch_mq;/*add  2010-11-4*/
		igTrnid = plMsgHead->lTrnid;
		if(plMsgHead->iLogFlag == SEPFILE)
		{
			igLogLevel = plMsgHead->iLogLevel;
			strcpy(sgDebugFile,sgDebugFile_src);
		}else
		{
			igLogLevel = plMsgHead->iLogLevel;
			strcpy(sgDebugFile,plMsgHead->sLogFileName);
			/*
			printf("%s", "");
			printf("begin module debug file :[%s]",sgDebugFile_src);
			*/
		}
			if(plMsgHead->iTraceFlag == GETMODCOST)
			{
						time(&Time);
						plMsgHead->iInModuleTime = Time;
			}
		/*modi  20090628*/
		if(igLogLevel >= 3)
		{
			printf("Readnw:%3d->%3d:pid[%5d] Pri[%4d] Class[%4d] Type[%d] Msglen[%5d]\n",plMsgHead->iDestMQ,*qid,getpid(),*mpriority,*mclass,*mtype,*msg_size,wtime);
		}
		return SUCCESS;
	}
	else
		return ilRc;
}
