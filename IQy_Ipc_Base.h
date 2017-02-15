
#ifndef IQy_Ipc_Base_H_
#define IQy_Ipc_Base_H_
#include <string>
namespace Qy_IPC
{
	class IQy_Ipc_Base
	{
	     public:
			  //д����
			   virtual void WriteData(unsigned char* buf,int len)=0;
			   //�Ƿ�ʱ
			   virtual bool GetTimeOut(int CurTime)=0;
			   virtual bool ProcessConnection()=0;
			   virtual bool Disconnect()=0;
	};
	//��������
	class IQy_HandelReceiveData
	{
	     public:
			    virtual void HandelReceiveData(char *buf,int Len,std::string strId)=0;
	};
	//�Ͽ�����
	class IQy_IPC_DisConnect
	{
	    public:
		        //�رպ����HANDLE  hPipeInst
		        virtual void DisConnct(void* hPipeInst)=0;
	};

}
#endif
