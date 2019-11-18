


#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include<iostream>
#include<thread>
#include"EasyTcpClient.hpp"
#pragma comment(lib,"ws2_32.lib")
bool g_bRun = true;



//���߳���������
void cmdThread(EasyTcpClient *clients) {

	while (true) {
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit")) {
			clients->CloseService();
			printf("�˳�\n");
			break;
		}
		else if (0 == strcmp(cmdBuf, "login")) {
			Login login_s;
			strcpy(login_s.userName, "jon");
			strcpy(login_s.password, "jt");
			clients->SendData(&login_s);
		}

		else if (0 == strcmp(cmdBuf, "loginout")) {
			Loginout lo;
			strcpy(lo.userName, "snow");
			clients->SendData(&lo);

		}
		else {
			printf("��֧�ֵ�����\n");

		}

	}
	
	

}


int main()
{
	EasyTcpClient client_s;
	client_s.initSocket();
	client_s.connectService("127.0.0.1", 7000);
	//�����߳�
	std::thread tl(cmdThread, &client_s);
	//���뺯���߳������̷߳���
	tl.detach();
	while (client_s.isRun()) {
		client_s.onRun();
	}
	client_s.CloseService();
	getchar();

	return 0;
}