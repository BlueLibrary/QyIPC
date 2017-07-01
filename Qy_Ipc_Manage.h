#ifndef Qy_Ipc_Manage_H_
#define Qy_Ipc_Manage_H_

#include <vector>
#include <queue>
#include <map>
#include <string>
#include "IQy_Ipc_Base.h"
#include "Qy_IPC_Context.h"
namespace Qy_IPC
{
	
	enum EQyIpcType
	{
		QyIpcClient=0,
		QyIpcServer=1,
	};
	
	
	/***���ֵ***/
	class Qy_IPc_InterCriSec
	{
		public:
			Qy_IPc_InterCriSec(DWORD dwSpinCount = 4096);
			~Qy_IPc_InterCriSec();
			void Lock();
			void Unlock();
			BOOL TryLock();
			DWORD SetSpinCount(DWORD dwSpinCount);
			CRITICAL_SECTION* GetObject();
		private:
			Qy_IPc_InterCriSec(const Qy_IPc_InterCriSec& cs);
			Qy_IPc_InterCriSec operator = (const Qy_IPc_InterCriSec& cs);

		private:
			//�ٽ�ֵ
			CRITICAL_SECTION m_crisec;
	};

	struct SReceiveCacheInfo;
	struct SReceiveData;
	class Qy_Ipc_Manage
	{
	   public:
		      Qy_Ipc_Manage();
			  ~Qy_Ipc_Manage(void);
			 
			  //��ʼ��
			  void                      Init(IQy_HandelReceiveData* pReceiveData,EQyIpcType m_QyIpcType,IQy_IPC_DisConnect *pDisConnect=NULL);
			  
			  //�����ܵ�,����˵���
			  bool                      CreatePipe(const char* StrPipeName,unsigned char ClientMaxCount);
			  //�򿪷������˹ܵ�
			  bool                      OpenServerPipe(const char* StrPipeName);
			  
			  //����
			  void                      Start();
			  void                      Stop();
			  bool                      WritePipe(unsigned char *pBuf,unsigned int Len,HANDLE hPipeInst);
			  bool                      WritePipe(std::string &StrData,HANDLE hPipeInst);
			  BOOL                      DisConnect(HANDLE hPipeInst);
	   private: 
		      static unsigned __stdcall QyIpcManage(LPVOID lpParameter);
			  BOOL                      FreeServer(HANDLE &hPipeInst);
			  void                      FreeClient();
			  //����˶�д
			  void                      RwServer();
			  //�ͻ���д
			  void                      RwClient();
			  //�ְ�����
			  void                      SplicPacket(const HANDLE &hPipeInst,SQy_IPC_MSG_HEADER &header,const char* from,char *pBuf);
			  unsigned int              check_sum(unsigned char * data,unsigned int  DataSize);
			  SQy_IPC_Context           *GetIpcCtx(HANDLE& hPipeInst);
	   protected:
		     
		      void ParseReceiveData(char *buf,int Len,HANDLE hPipeInst);
			  void ReadWritePipe(); 
			  
			  IQy_IPC_DisConnect *m_pDisConnect;
              //�ܵ�ʵ������
			  std::vector<IQy_Ipc_Base*> m_IPC_Vect;
			  //�߳�
			  HANDLE m_ThreadHandles[2];
			  HANDLE m_PipeThreadRWExit;
			  int    m_nIsStart;
			 
			  ///�������ݶ���
			  std::map<std::string,SReceiveCacheInfo*>   m_IPC_ReceiveDataMap;
			  //�������ݶ���
			  std::map<HANDLE,std::queue<SQy_IPC_MSG*>*> m_IPC_SendDataQueueMap;
			  HANDLE         m_ArrayHandle[1024];
			  volatile  int m_ArrayHandleCount;
			  //���պ������ݵĶ���
			  IQy_HandelReceiveData*                     m_pQy_HandelReceiveData;
			  //IPC 
			  EQyIpcType                                 m_QyIpcType;
			  //�ͻ���
			  SQy_IPC_Context                            m_ClientQy_IPC_Context;
			  Qy_IPc_InterCriSec                         m_Lock;
			  bool m_bExit;

	};
}
#endif
