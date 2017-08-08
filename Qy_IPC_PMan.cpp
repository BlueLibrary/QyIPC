#include "Qy_IPC_PMan.h"
#include <process.h>
#include <Windows.h>
#include <map>
#include "Qy_Ipc_Manage.h"
#include <tlhelp32.h>
typedef struct SProcessInfo
{
	PROCESS_INFORMATION pi;
	STARTUPINFOW si;
};

bool KillProcessFromName(std::wstring strProcessName)  
{  
	 transform(strProcessName.begin(), strProcessName.end(), strProcessName.begin(),  toupper);   
	//�������̿���(TH32CS_SNAPPROCESS��ʾ�������н��̵Ŀ���)  
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  

	//PROCESSENTRY32���̿��յĽṹ��  
	PROCESSENTRY32 pe;  

	//ʵ������ʹ��Process32First��ȡ��һ�����յĽ���ǰ�����ĳ�ʼ������  
	pe.dwSize = sizeof(PROCESSENTRY32);  


	//�����IFЧ��ͬ:  
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   ��Ч�ľ��  
	if(!Process32First(hSnapShot,&pe))  
	{  
		return false;  
	}  

	//���ַ���ת��ΪСд  
	//strProcessName.MakeLower();  

	//��������Ч  ��һֱ��ȡ��һ�����ѭ����ȥ  
	while (Process32Next(hSnapShot,&pe))  
	{  

		//pe.szExeFile��ȡ��ǰ���̵Ŀ�ִ���ļ�����  
		std::wstring scTmp = pe.szExeFile;

		//����ִ���ļ���������Ӣ����ĸ�޸�ΪСд  
		 transform(scTmp.begin(), scTmp.end(), scTmp.begin(),  toupper);   

		//�Ƚϵ�ǰ���̵Ŀ�ִ���ļ����ƺʹ��ݽ������ļ������Ƿ���ͬ  
		//��ͬ�Ļ�Compare����0  
		if(scTmp==strProcessName)  
		{  

			//�ӿ��ս����л�ȡ�ý��̵�PID(������������е�PID)  
			DWORD dwProcessID = pe.th32ProcessID;  
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);  
			::TerminateProcess(hProcess,0);  
			CloseHandle(hProcess);  
			return true;  
		}  
		
	}  
	return false;  
}  

namespace Qy_IPC
{
	std::map<std::string,SProcessInfo*>  G_ProcessInfoMap;
	CQy_IPC_PMan::CQy_IPC_PMan()
	{

	}
	CQy_IPC_PMan::~CQy_IPC_PMan()
	{

	}
    bool CQy_IPC_PMan::KillProcessFromName(std::wstring strProcessName)
	{
		return ::KillProcessFromName(strProcessName);
	}
	CQy_IPC_PMan* CQy_IPC_PMan::GetInstance()
	{
		if(pInstance==NULL)
		{
			pInstance = new CQy_IPC_PMan();
		}
		return pInstance;
	}
	void CQy_IPC_PMan::FreeInstance()
	{
		delete pInstance;
		pInstance=NULL;
	}
	bool CQy_IPC_PMan::StartApp(std::wstring ExeFile, std::wstring CmdLine)
	{
		SProcessInfo Info,*pInfo=&Info;
        memset(pInfo,0,sizeof(SProcessInfo));
		
		BOOL ret = CreateProcessW(ExeFile.c_str(),(LPWSTR)CmdLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &pInfo->si, &pInfo->pi);
		if (!ret) 
		{
			
			return false;
		}
	    return true;
	}
	char exchange(char c)
	{

		if(c <= 'Z' && c >= 'A')

			c = tolower(c);

		else if(c >= 'a' && c <= 'z')

			c = toupper(c);

		return c;

	}
	
	CQy_IPC_PMan* CQy_IPC_PMan::pInstance=NULL;
}