#include "Qy_IPC_PMan.h"
#include <process.h>
#include <Windows.h>
#include <map>
#include "Qy_Ipc_Manage.h"
#include <tlhelp32.h>
typedef struct SProcessInfo
{
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
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
	bool CQy_IPC_PMan::StartProcess(std::string &ExeFile, std::string &CmdLine)
	{
		SProcessInfo *pInfo=(SProcessInfo*)::malloc(sizeof(SProcessInfo));
        memset(pInfo,0,sizeof(SProcessInfo));
		char abcd[256]="";
        strcpy(abcd,CmdLine.c_str());

		LPSTR szCmdline =_strdup(abcd);
		BOOL ret = CreateProcessA(ExeFile.c_str(),szCmdline, NULL, NULL, FALSE, 0, NULL, NULL, &pInfo->si, &pInfo->pi);
		if (!ret) 
		{
			free(pInfo);
			return false;
		}

		G_ProcessInfoMap.insert(std::pair<std::string,SProcessInfo*>(ExeFile,pInfo));
	    return true;
	}

	void CQy_IPC_PMan::WriteData(std::string &ExeFile,char* pBuf,int Len)
	{
		std::string ffff=ExeFile;
		Qy_Ipc_Manage *pInstcen=Qy_Ipc_Manage::GetInstance();
		std::map<std::string,HANDLE>::iterator It=m_ExePipeMap.find(ffff);
		if(It!=m_ExePipeMap.end())
		{
             pInstcen->WritePipe(pBuf,Len,It->second);
		}
		
	}
	void CQy_IPC_PMan::SetExeInfo(std::string &ExeFile,HANDLE Pipe)
	{
		m_ExePipeMap.insert(std::pair<std::string,HANDLE>(ExeFile,Pipe));
	}

	char exchange(char c)
	{

		if(c <= 'Z' && c >= 'A')

			c = tolower(c);

		else if(c >= 'a' && c <= 'z')

			c = toupper(c);

		return c;

	}
	BOOL CQy_IPC_PMan::CloseApp(std::string ExeFile)
	{
		Qy_Ipc_Manage *pInstcen=Qy_Ipc_Manage::GetInstance();
		std::map<std::string,HANDLE>::iterator It=m_ExePipeMap.find(ExeFile);
		if(It!=m_ExePipeMap.end())
		{
			m_ExePipeMap.erase(It);
		}

		std::map<std::string,SProcessInfo*>::iterator Itx=G_ProcessInfoMap.begin();
		for(;Itx!=G_ProcessInfoMap.end();++Itx)
		{
			std::string abcd=Itx->first;
			int i=abcd.find_last_of("\\");
			if(i>-1)
			{
				i++;
				abcd=abcd.substr(i,abcd.length()-i);
				std::transform(abcd.begin(), abcd.end(), abcd.begin(), ::tolower);
				std::transform(ExeFile.begin(), ExeFile.end(), ExeFile.begin(), ::tolower);
				if(abcd==ExeFile)
				{
					//CloseHandle(Itx->second->pi.hThread);
					TerminateProcess(Itx->second->pi.hProcess,0);
					free(Itx->second);
					Itx=G_ProcessInfoMap.erase(Itx);
					break;
				}
			}
		}
		return TRUE;
	}
	CQy_IPC_PMan* CQy_IPC_PMan::pInstance=NULL;
}