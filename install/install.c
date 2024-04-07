// install.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#pragma comment(lib, "Shlwapi.lib")
#pragma warning(disable:6031)
#include <stdio.h>
#include <Windows.h>
#include <Shlwapi.h>


// 创建/修改注册表值与数据 
int CreateReg(HKEY RootKey, const char * SubKey, const char * Valuename, const char * Data)
{
	int res = 0;
	HKEY hKey;//句柄
	//创建值 
	long CReg = RegCreateKeyExA(RootKey, SubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (CReg == ERROR_SUCCESS)
	{
		//设置值的数据 
		//printf("设置值与数据为（值:数据）:\n%s:%s\n", Valuename, Data);
		long SRet = RegSetValueExA(hKey, Valuename, 0, REG_SZ, (BYTE *)Data, strlen(Data));
		if (SRet == ERROR_SUCCESS)
		{
			res = 1;
		}
	}
	RegCloseKey(hKey);
	return res;
}

//查询子健的值的数据 
int QueryReg(HKEY RootKey, const char * SubKey, const char * Valuename, const char * buf, DWORD size) {
	HKEY hKey;//句柄
	long ORet = RegOpenKeyExA(RootKey, SubKey, 0, KEY_ALL_ACCESS, &hKey);
	if (ORet != ERROR_SUCCESS) {
		//printf("无效的路径:%s-%s,err:%d\n", SubKey, Valuename, ORet);
		return 0;
	}
	else {
		long QRet = RegQueryValueExA(hKey, Valuename, 0, NULL, (BYTE *)buf, &size);
		//printf("查询结果（值:数据）:\n%s:%s\n", Valuename, buf);
		RegCloseKey(hKey);
		return 1;
	}
}





int main()
{
	//判断文件是否齐全
	char * SWName = "\\fnm.exe";
	char * cmdName = "\\fnm_init.cmd";
	char currentPath[MAX_PATH];//用于存放获取的路径信息。
	char currentSWNamePath[MAX_PATH];//用于存放获取的路径信息。
	char currentcmdNamePath[MAX_PATH];//用于存放获取的路径信息。
	DWORD currentPathLen = GetModuleFileNameA(NULL, currentPath, MAX_PATH);
	if (currentPathLen <= 0) {
		printf("获取当前路径失败!");
		getchar();
		return;
	}
	DWORD i = 0;
	for (i = currentPathLen; i > 0; i--) {
		if (currentPath[i] == '\\') {
			currentPath[i] = '\0';
			break;
		}
	}
	if (i == 0) {
		printf("路径错误:%s!", currentPath);
		getchar();
		return;
	}
	strcpy_s(currentSWNamePath, MAX_PATH, currentPath);
	strcpy_s(currentcmdNamePath, MAX_PATH, currentPath);
	int len = strlen(SWName) + strlen(currentSWNamePath) + 1;
	strcat_s(currentSWNamePath, len, SWName);

	len = strlen(cmdName) + strlen(currentcmdNamePath) + 1;
	strcat_s(currentcmdNamePath, len, cmdName);
	if (FALSE == PathFileExistsA(currentcmdNamePath) || FALSE == PathFileExistsA(currentSWNamePath)) {
		printf("未在当前文件夹下找到文件:%s或%s!", currentcmdNamePath, currentSWNamePath);
		getchar();
		return;
	}
	char * targetPath = "C:\\Program Files (x86)\\fnm";
	char * SWtargetPath = "C:\\Program Files (x86)\\fnm\\fnm.exe";
	char * cmdtargetPath = "C:\\Program Files (x86)\\fnm\\fnm_init.cmd";

	if (FALSE == CreateDirectoryA(targetPath, NULL)) {
		printf("创建文件夹:%s失败!", targetPath);
		getchar();
		return;
	}

	if (0 == CopyFileExA(currentSWNamePath, SWtargetPath, NULL, NULL, NULL, COPY_FILE_OPEN_SOURCE_FOR_WRITE))
	{
		printf("复制文件到:%s失败!", SWtargetPath);
		getchar();
		return;
	}

	if (0 == CopyFileExA(currentcmdNamePath, cmdtargetPath, NULL, NULL, NULL, COPY_FILE_OPEN_SOURCE_FOR_WRITE))
	{
		printf("复制文件到:%s失败!", cmdtargetPath);
		getchar();
		return;
	}



	char buf[10000];
	char * target = "C:\\Program Files (x86)\\fnm;";
	char * path = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
	char * key = "path";
	if (0 == QueryReg(HKEY_LOCAL_MACHINE, path, key, buf, 10000)) {
		printf("获取当前环境变量失败!");
		getchar();
		return;
	}
	if (strstr(buf, target) == NULL) {
		len = strlen(target) + strlen(buf) + 1;
		strcat_s(buf, len, target);
		if (0 == CreateReg(HKEY_LOCAL_MACHINE, path, key, buf)) {
			printf("设置环境变量失败!");
			getchar();
			return;
		}
	}


	char * autorunPath = "SOFTWARE\\Microsoft\\Command Processor";
	char * autorunValue = "\"C:\\Program Files (x86)\\fnm\\fnm_init.cmd\"";
	if (0 == CreateReg(HKEY_LOCAL_MACHINE, autorunPath, "AutoRun", autorunValue)) {
		printf("设置环境变量失败!");
		getchar();
		return;
	}
	printf("安装成功!\n按任意键退出\n");
	getchar();
}
