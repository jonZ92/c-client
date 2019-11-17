


#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include<Windows.h>
#include<WinSock2.h>
#include<iostream>
#include<thread>
#pragma comment(lib,"ws2_32.lib")


//���ö˿�ֵ
const int post_id = 7000;


enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_LOGINOUT,
	CMD_ERROR
};

//�������ݰ� 
struct dataHead {
	//���ݳ���
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

//��¼���ݽṹ
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
//����ṹ������
struct DataPackage
{

	int age;
	char name[32];
};

bool g_bRun = true;



//���߳���������
void cmdThread(SOCKET soc) {

	while (true) {
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit")) {
			g_bRun = false;
			printf("�˳�\n");
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
			printf("��֧�ֵ�����\n");

		}

	}
	
	

}


int processs(SOCKET _clientSock) {
	// 5 ���տͻ�����������
	//�ֽڻ�����
	char szRecv[4096] = {};
	int _cLen = recv(_clientSock, szRecv, sizeof(dataHead), 0);
	dataHead *header = (dataHead*)szRecv;
	if (_cLen <= 0)
	{
		printf("��������Ͽ����ӡ�\n");
		return -1;
	}

	switch (header->cmd)
	{
	case CMD_LOGIN_RESULT:
	{
		LoginResult *loginR = (LoginResult*)szRecv;
		recv(_clientSock, szRecv + sizeof(dataHead), header->dataLength - sizeof(dataHead), 0);
		printf("�յ����� ���ݳ��� :%d\n", loginR->dataLength);

	}
	break;
	case CMD_LOGINOUT_RESULT: {
	
		LoginOutResult *loginOut = (LoginOutResult*)szRecv;
		recv(_clientSock, szRecv + sizeof(dataHead), header->dataLength - sizeof(dataHead), 0);
		printf("�յ����� ���ݳ��� :%d\n", loginOut->dataLength);

	}

	 break;
	case CMD_NEW_USER_JOIN: {
		NEWUserLogin *loginjoin = (NEWUserLogin*)szRecv;
		recv(_clientSock, szRecv + sizeof(dataHead), header->dataLength - sizeof(dataHead), 0);
		printf("�յ����� ���ݳ��� :%d\n", loginjoin->dataLength);

	
	}
	break;
	}


}
int main()
{
	//����windows socket
	WORD word = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(word, &data);

	// 1������һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	// 2��  bind ��������տͻ������ӵ�����˿�
	if (INVALID_SOCKET == _sock)
	{
		printf("����socket���� \n");
	}
	else {
		printf("����socket�ɹ��� \n");
	}
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(post_id);
	//_sin.sin_addr.S_un.S_addr = (ULONG)host_id;
	_sin.sin_addr.S_un.S_addr = inet_addr("192.168.2.22");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		printf("����socket���� \n");
	}
	else {
		printf("����sockett�ɹ��� \n");
	}

	//�����߳�
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
			printf("select ������� \n");
			break;
		}

		if (FD_ISSET(_sock, &fdReads)) {
			FD_CLR(_sock, &fdReads);

			if (-1 == processs(_sock)) {
				printf("select������� \n");
				break;
			}
		}
	

		//printf("����ʱ�䴦������ҵ��\n");
	
		//Sleep(1000);
	   

	}

	closesocket(_sock);
	WSACleanup();
	getchar();
	return 0;
}