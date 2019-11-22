#ifndef MessageHeader_hpp_
#define MessageHeader_hpp_

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
