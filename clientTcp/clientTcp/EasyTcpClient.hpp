


#ifndef  EasyTcpClient_hpp_
#define EasyTcpClient_hpp_
#ifdef _WIN32	
			#define WIN32_LEAN_AND_MEAN
			#define _WINSOCK_DEPRECATED_NO_WARNINGS 
			#pragma comment(lib,"ws2_32.lib")
			#include<Windows.h>
			#include<WinSock2.h>
#else
			#include<unistd.h>
			#include<arpa/inet.h>
			#include<string.h>
			#define SOCKET int
			#define INVALID_SOCKET (SOCKET)(~0)
			#define SOCKET_ERROR                  (-1) 
#endif
#include<iostream>
#include"MessageHeader.hpp"


class EasyTcpClient
{
private:
	SOCKET _socket;
public:
	EasyTcpClient() 
	{
		_socket = INVALID_SOCKET;
	}
	//����������
	virtual ~EasyTcpClient() 
	{
		CloseService();
	}
	//��ʼ��socket
	void initSocket() 
	{
#ifdef _WIN32
		//����windows socket
		WORD word = MAKEWORD(2, 2);
		WSADATA data;
		WSAStartup(word, &data);
#endif
		
		if (INVALID_SOCKET!= _socket) {
			printf("socket�ر����ӡ� \n");
			CloseService();
		}
		// 1������һ��socket
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		// 2��  bind ��������տͻ������ӵ�����˿�
		if (INVALID_SOCKET == _socket)
		{
			printf("����socket���� \n");
		}
		else {
			printf("����socket�ɹ��� \n");
		}
	
	}
	//���ӷ�����
	int connectService(const char* ip,unsigned short port)
	{
		if (INVALID_SOCKET == _socket) {
			printf("socket��ʼ���� \n");
			initSocket();
		}
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
		_sin.sin_addr.s_addr = inet_addr(ip);
#endif
		int ret = connect(_socket,(sockaddr*)&_sin, sizeof(sockaddr_in));
		if (SOCKET_ERROR == ret)
		{
			printf("����socket���� \n");
		}
		else {
			printf("����sockett�ɹ��� \n");
		}
		return ret;
	}
	//�رշ�����
	void CloseService() 
	{
		if (_socket!= INVALID_SOCKET) {
#ifdef _WIN32
			closesocket(_socket);
			WSACleanup();
#else
				close(_socket);
#endif
		}
	}
	//��������
	//��������
	//��������
	bool onRun() {	
		if (isRun()) {
			fd_set fdReads;
			FD_ZERO(&fdReads);
			FD_SET(_socket, &fdReads);
			timeval t_s = { 1,0 };
			int ret = select(_socket, &fdReads, 0, 0, &t_s);

			if (ret < 0) {
				printf("select ������� \n");
				return false;
			}
			if (FD_ISSET(_socket, &fdReads)) {
				FD_CLR(_socket, &fdReads);

				if (-1 == RecvData(_socket)) {
					printf("select������� \n");
					return false;
				}
			}
			return true;
		}
		return false;
	}

	//�ж��Ƿ�������

	bool isRun() {
		return _socket!= INVALID_SOCKET;
	}
	int RecvData(SOCKET _clientSock) {
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
		recv(_clientSock, szRecv + sizeof(dataHead), header->dataLength - sizeof(dataHead), 0);
		OnNetMsg(header);
		return 0;
	}

	void OnNetMsg( dataHead *header ) {
		switch (header->cmd)
		{
		case CMD_LOGIN_RESULT:
		{
			LoginResult *loginR = (LoginResult*)header;
			printf("�յ����� ���ݳ��� :%d\n", loginR->dataLength);
		}
		break;
		case CMD_LOGINOUT_RESULT: {
			LoginOutResult *loginOut = (LoginOutResult*)header;
			printf("�յ����� ���ݳ��� :%d\n", loginOut->dataLength);
		}
								  break;
		case CMD_NEW_USER_JOIN: {
			NEWUserLogin *loginjoin = (NEWUserLogin*)header;
			printf("�յ����� ���ݳ��� :%d\n", loginjoin->dataLength);
		}
								break;
		}
	}
	int SendData(dataHead *header) {

		if (isRun()&& header) {
			send(_socket, (char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
	}
};
#endif