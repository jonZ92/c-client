


#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include<Windows.h>
#include<WinSock2.h>
#include<iostream>
#include<thread>
#pragma comment(lib,"ws2_32.lib")


//设置端口值
const int post_id = 7000;


enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_LOGINOUT,
	CMD_ERROR
};

//定义数据包 
struct dataHead {
	//数据长度
	short dataLength;
	short cmd;
};

struct LoginResult :public dataHead {
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;

};
struct Loginout :public dataHead {
	Loginout()
	{
		dataLength = sizeof(Loginout);
		cmd = CMD_LOGINOUT;
	}
	char userName[32];
};

struct LoginOutResult :public  dataHead {
	LoginOutResult()
	{
		dataLength = sizeof(LoginOutResult);
		cmd = CMD_LOGINOUT_RESULT;
		result = 0;
	}
	int result;

};

//登录数据结构
struct Login :public dataHead {
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char password[32];
};

struct NEWUserLogin :public dataHead
{
	NEWUserLogin() {
		dataLength = sizeof(NEWUserLogin);
		cmd = CMD_NEW_USER_JOIN;
		socket_id = 0;
	}
	int socket_id;
};
//定义结构化数据
struct DataPackage
{

	int age;
	char name[32];
};

bool g_bRun = true;



//多线程输入命令
void cmdThread(SOCKET soc) {

	while (true) {
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit")) {
			g_bRun = false;
			printf("退出\n");
			break;
		}

		else if (0 == strcmp(cmdBuf, "login")) {
			Login login_s;
			strcpy(login_s.userName, "jon");
			strcpy(login_s.password, "jt");
			send(soc, (char*)&login_s, sizeof(Login), 0);
		}

		else if (0 == strcmp(cmdBuf, "loginout")) {

			Loginout lo;
			strcpy(lo.userName, "snow");
			send(soc, (char*)&lo, sizeof(Loginout), 0);

		}
		else {
			printf("不支持的命令\n");

		}

	}
	
	

}


int processs(SOCKET _clientSock) {
	// 5 接收客户端数据请求
	//字节缓冲区
	char szRecv[4096] = {};
	int _cLen = recv(_clientSock, szRecv, sizeof(dataHead), 0);
	dataHead *header = (dataHead*)szRecv;
	if (_cLen <= 0)
	{
		printf("与服务器断开连接。\n");
		return -1;
	}

	switch (header->cmd)
	{
	case CMD_LOGIN_RESULT:
	{
		LoginResult *loginR = (LoginResult*)szRecv;
		recv(_clientSock, szRecv + sizeof(dataHead), header->dataLength - sizeof(dataHead), 0);
		printf("收到命令 数据长度 :%d\n", loginR->dataLength);

	}
	break;
	case CMD_LOGINOUT_RESULT: {
	
		LoginOutResult *loginOut = (LoginOutResult*)szRecv;
		recv(_clientSock, szRecv + sizeof(dataHead), header->dataLength - sizeof(dataHead), 0);
		printf("收到命令 数据长度 :%d\n", loginOut->dataLength);

	}

	 break;
	case CMD_NEW_USER_JOIN: {
		NEWUserLogin *loginjoin = (NEWUserLogin*)szRecv;
		recv(_clientSock, szRecv + sizeof(dataHead), header->dataLength - sizeof(dataHead), 0);
		printf("收到命令 数据长度 :%d\n", loginjoin->dataLength);

	
	}
	break;
	}


}
int main()
{
	//启动windows socket
	WORD word = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(word, &data);

	// 1、建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	// 2、  bind 绑定用与接收客户端链接的网络端口
	if (INVALID_SOCKET == _sock)
	{
		printf("建立socket错误。 \n");
	}
	else {
		printf("建立socket成功。 \n");
	}
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(post_id);
	//_sin.sin_addr.S_un.S_addr = (ULONG)host_id;
	_sin.sin_addr.S_un.S_addr = inet_addr("192.168.2.22");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		printf("连接socket错误。 \n");
	}
	else {
		printf("连接sockett成功。 \n");
	}

	//启动线程
	std::thread tl(cmdThread, _sock);
	tl.detach();
	while (g_bRun)
	{
		fd_set fdReads;
		FD_ZERO(&fdReads);
		FD_SET(_sock, &fdReads);
		timeval t_s = {1,0};
		int ret=select(_sock, &fdReads,0,0,&t_s);
		
		if (ret<0) {
			printf("select 任务结束 \n");
			break;
		}

		if (FD_ISSET(_sock, &fdReads)) {
			FD_CLR(_sock, &fdReads);

			if (-1 == processs(_sock)) {
				printf("select任务结束 \n");
				break;
			}
		}
	

		//printf("空闲时间处理其他业务\n");
	
		//Sleep(1000);
	   

	}

	closesocket(_sock);
	WSACleanup();
	getchar();
	return 0;
}