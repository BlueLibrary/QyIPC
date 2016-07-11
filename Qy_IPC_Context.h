#ifndef NoNeedWindowHeade
#include <Windows.h>
#endif
#include <vector>
#ifndef Qy_IPC_Context_H_
#define Qy_IPC_Context_H_
namespace Qy_IPC
{
	#define PipeBufferSize 1024*100
	#define CONNECTING_STATE 0   
	#define READING_STATE 1   
	#define WRITING_STATE 2  
	#define WRITOK_STATE 3  
	typedef struct SQy_IPC_Context
	{  
		//�ܵ��ص�
		OVERLAPPED oOverlap;  
		//д��
		OVERLAPPED oWriteOverlap;  
		//�ܵ����
		HANDLE hPipeInst;  
		//�����ݵ��¼�
		HANDLE hDataEvent;
		//ʵ�ʶ�����ֽ���
		DWORD cbRead;   
		//Ҫд����ֽ���
		DWORD cbToWrite;
		//״̬
		DWORD dwState;   //0,׼�����ӣ�1������
		int UpdataTime;//����ʱ��
		char ReceiveBuf[PipeBufferSize];
		char SendBuf[PipeBufferSize];
	}; 
#pragma  pack(1)
	typedef struct SQy_IPC_MSG_HEADER
	{
	   int	     MsgType;
	   GUID	     PktGuid;
       int       PktId;
       int DataLen;
	   int TotalDataLen;
	};
#pragma pack()
	typedef struct SQy_IPC_MSG
	{
		char *pBuf;
		int	 Len;
		//�ܵ����
		HANDLE hPipeInst;
	};
    
}
#endif