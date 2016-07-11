#pragma once
#ifndef NoNeedWindowHeade
#include <Windows.h>
#endif
#include <vector>
#include <queue>
#include <map>
#include <string>
#include "IQy_Ipc_Base.h"
#include "Qy_IPC_Context.h"
#ifndef Qy_Ipc_Manage_H_
#define Qy_Ipc_Manage_H_
namespace Qy_IPC
{
	
	enum EQyIpcType
	{
		Client=0,
		Server=1,
	};
	typedef struct SReceiveData
	{
		int DataLen;
		char *Buf;
		int PktId;
	};
	typedef struct SReceiveCacheInfo
	{
		GUID Guid;
		int TotalLen;
		int CurLen;
		//�ܵ����
		HANDLE hPipeInst;
		std::vector<SReceiveData *>* pDataList;
	};
	class IQy_IPC_DisConnect
	{
	    public:
		//�رպ����
		virtual void DisConnct(HANDLE hPipeInst)=0;
	};
	/***���ֵ***/
	class Qy_IPc_InterCriSec
	{
		public:
			Qy_IPc_InterCriSec(DWORD dwSpinCount = 4096)
			{
				::InitializeCriticalSectionAndSpinCount(&m_crisec, dwSpinCount);
			}
			~Qy_IPc_InterCriSec()
			{
				::DeleteCriticalSection(&m_crisec);
			}

			void Lock()                            {::EnterCriticalSection(&m_crisec);}
			void Unlock()                          {::LeaveCriticalSection(&m_crisec);}
			BOOL TryLock()                         {return ::TryEnterCriticalSection(&m_crisec);}
			DWORD SetSpinCount(DWORD dwSpinCount) {return ::SetCriticalSectionSpinCount(&m_crisec, dwSpinCount);}

			CRITICAL_SECTION* GetObject()          {return &m_crisec;}

		private:
			Qy_IPc_InterCriSec(const Qy_IPc_InterCriSec& cs);
			Qy_IPc_InterCriSec operator = (const Qy_IPc_InterCriSec& cs);

		private:
			//�ٽ�ֵ
			CRITICAL_SECTION m_crisec;
	};
	class Qy_Ipc_Manage
	{
	   public:
			  ~Qy_Ipc_Manage(void);
			  static Qy_Ipc_Manage* GetInstance();
			  static void FreeInstance();
			  static unsigned __stdcall QyIpcManage(LPVOID lpParameter);
			  int Init(IQy_HandelReceiveData* pReceiveData,EQyIpcType m_QyIpcType,IQy_IPC_DisConnect *pDisConnect=NULL);
			  void Start();
			  bool CreatePipe(std::string PipeName,unsigned int PipeInstanceCount);
			  bool OpenServerPipe(std::string PipeName);
			  bool WritePipe(char *pBuf,int Len,HANDLE hPipeInst);
			  BOOL DisConnect(HANDLE hPipeInst);
	   private:
		      static unsigned __stdcall QyIpcHeartRate(LPVOID lpParameter);
			  Qy_Ipc_Manage();
			  static Qy_Ipc_Manage* m_Instance;
	   protected:
		      IQy_IPC_DisConnect *m_pDisConnect;
		      void ParseReceiveData(char *buf,int Len,HANDLE hPipeInst);
		      void WriteIpcHeartRate();
			  void ReadWritePipe();
              //�ܵ�ʵ������
			  std::vector<IQy_Ipc_Base*> m_IPC_Vect;
			  //�߳�
			  HANDLE m_ThreadHandles[10];
			  int m_nIsStart;
			  /****�����¼�*******/
			  HANDLE m_HIpcHeartRateEvent;
			  std::map<std::string,SReceiveCacheInfo*> m_IPC_ReceiveDataMap;
			  //�������ݶ���
			  std::map<HANDLE,std::queue<SQy_IPC_MSG*>*> m_IPC_SendDataQueueMap;
			  HANDLE m_ArrayHandle[1024];
			  int m_ArrayHandleCount;
			  //���պ������ݵĶ���
			  IQy_HandelReceiveData* m_pQy_HandelReceiveData;
			  //IPC 
			  EQyIpcType m_QyIpcType;
			  //�ͻ���
			  SQy_IPC_Context m_ClientQy_IPC_Context;
			  Qy_IPc_InterCriSec m_Lock;
			  bool m_bExit;

	};
}
#endif
