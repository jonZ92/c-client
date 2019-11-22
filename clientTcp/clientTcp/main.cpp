



#include<iostream>
#include<thread>
#include"EasyTcpClient.hpp"
//全局变量初始化
bool g_bRun = true;

//多线程输入命令
void cmdThread(EasyTcpClient *clients) {

	while (true) {
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit")) {
			clients->CloseService();
			printf("退出\n");
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
			printf("不支持的命令\n");
		}

	}

}


int main()
{
	EasyTcpClient client_s;
	//client_s.initSocket();
	client_s.connectService("127.0.0.1", 7000);

	//EasyTcpClient client_s2;
	//client_s2.connectService("127.0.0.1", 7000);
	//启动线程
	
	std::thread t1(cmdThread, &client_s);
	//输入函数线程与主线程分离
	t1.detach();
	//std::thread t2(cmdThread, &client_s2);
	
	//	t2.detach();
	while (client_s.isRun()) {
		client_s.onRun();
	}
	client_s.CloseService();
	printf("已退出/n");
	getchar();
	return 0;
}