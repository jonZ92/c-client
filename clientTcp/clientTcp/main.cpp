


#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include<Windows.h>
#include<WinSock2.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")



enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT_RESULT,
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


//����ṹ������
struct DataPackage
{

	int age;
	char name[32];
};
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
	_sin.sin_port = htons(7000);
	_sin.sin_addr.S_un.S_addr = inet_addr("192.168.2.22");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		printf("����socket���� \n");
	}
	else {
		printf("����sockett�ɹ��� \n");
	}


	while (true)
	{
		// 3��������������
		char cmdBuf[128] = {};
		scanf("%s", cmdBuf);
		// 4����������
		if (0 == strcmp(cmdBuf, "exit"))
		{
			printf("�յ��˳����� exit ��������� \n");
			break;
			//continue;
		}
		else if (0 == strcmp(cmdBuf, "login")) {

			Login login;
			strcpy(login.userName, "jonk");
			strcpy(login.password, "honey");
			//���������������

			send(_sock, (char*)&login, sizeof(login), 0);
			//���շ���������
			LoginResult rs = {};
			recv(_sock, (char*)&rs, sizeof(rs), 0);
			printf("LoginResult %d \n", rs.result);


		}
		else if (0 == strcmp(cmdBuf, "loginout")) {
			Loginout out;
			strcpy(out.userName, "jonk");
			send(_sock, (char*)&out, sizeof(out), 0);
			LoginOutResult rl = {};
			recv(_sock, (char*)&rl, sizeof(rl), 0);
			printf("LoginOutResult %d \n", rl.result);

		}
		else {
			printf("��֧�ֵ��������������! \n");
		}


	}

	closesocket(_sock);
	WSACleanup();
	getchar();
	return 0;
}