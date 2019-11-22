


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
	//虚析构函数
	virtual ~EasyTcpClient() 
	{
		CloseService();
	}
	//初始化socket
	void initSocket() 
	{
#ifdef _WIN32
		//启动windows socket
		WORD word = MAKEWORD(2, 2);
		WSADATA data;
		WSAStartup(word, &data);
#endif
		
		if (INVALID_SOCKET!= _socket) {
			printf("socket关闭连接。 \n");
			CloseService();
		}
		// 1、建立一个socket
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		// 2、  bind 绑定用与接收客户端链接的网络端口
		if (INVALID_SOCKET == _socket)
		{
			printf("建立socket错误。 \n");
		}
		else {
			printf("建立socket成功。 \n");
		}
	
	}
	//连接服务器
	int connectService(const char* ip,unsigned short port)
	{
		if (INVALID_SOCKET == _socket) {
			printf("socket初始化。 \n");
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
			printf("连接socket错误。 \n");
		}
		else {
			printf("连接sockett成功。 \n");
		}
		return ret;
	}
	//关闭服务器
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
	//接收数据
	//发送数据
	//处理数据
	bool onRun() {	
		if (isRun()) {
			fd_set fdReads;
			FD_ZERO(&fdReads);
			FD_SET(_socket, &fdReads);
			timeval t_s = { 1,0 };
			int ret = select(_socket, &fdReads, 0, 0, &t_s);

			if (ret < 0) {
				printf("select 任务结束 \n");
				return false;
			}
			if (FD_ISSET(_socket, &fdReads)) {
				FD_CLR(_socket, &fdReads);

				if (-1 == RecvData(_socket)) {
					printf("select任务结束 \n");
					return false;
				}
			}
			return true;
		}
		return false;
	}

	//判断是否运行中

	bool isRun() {
		return _socket!= INVALID_SOCKET;
	}
	int RecvData(SOCKET _clientSock) {
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
			printf("收到命令 数据长度 :%d\n", loginR->dataLength);
		}
		break;
		case CMD_LOGINOUT_RESULT: {
			LoginOutResult *loginOut = (LoginOutResult*)header;
			printf("收到命令 数据长度 :%d\n", loginOut->dataLength);
		}
								  break;
		case CMD_NEW_USER_JOIN: {
			NEWUserLogin *loginjoin = (NEWUserLogin*)header;
			printf("收到命令 数据长度 :%d\n", loginjoin->dataLength);
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