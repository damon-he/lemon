#include <pthread.h>
#include <unistd.h>
#include <iostream>
//#include "mavlink.h"
#include "Thread.hh"
#include "TcpSocket.hh"

using namespace std;
using namespace lemon;

TcpSocket *sendSocket = NULL;


void *ClientThread(void *arg) {
	TcpSocket *socket = new TcpSocket();
	
	SocketAddress localAddress("192.168.3.35", 7885);
	if (socket->bind(localAddress, true) != 0) {
		cout<<"bind error"<<endl;
	}
	SocketAddress serverAddress("192.168.3.230", 7885);
	socket->connect(serverAddress);
	cout<<"connect OK"<<endl;

	char cmd;
	unsigned char buff[256] = {0};
	sendSocket = socket;
	while(1) {
		memset(buff, 0, sizeof(buff));
		int ret = socket->read(buff, sizeof(buff));
		cout<<"recv msg:buff="<<buff<<"len="<<ret<<endl;	
	}

	return (void *)0;
}

void *ServerThread(void *arg) {
	TcpSocket *socket = new TcpSocket();
		
	SocketAddress localAddress("192.168.3.230", 7885);
	if (socket->bind(localAddress, true) != 0) {
		cout<<"bind error"<<endl;
	}
	socket->listen(5);
	
	socket->accept(sendSocket);
	cout<<"accept OK"<<endl;

	char cmd;
	int num = 0;
	unsigned char buff[256] = {0};
	
	while(1) {	
		memset(buff, 0, sizeof(buff));
		int ret = sendSocket->read(buff, sizeof(buff));
		cout<<"recv msg:buff="<<buff<<"len="<<ret<<endl;	
	}

	return (void *)0;

}



void TcpSocketTest(char *role) {
	//Thread *thread = NULL;
	pthread_t threadT;
	pthread_t cmdT;
	if (strcmp(role, "client") == 0) {
		//thread = new Thread(ClientThread, NULL);
		pthread_create(&threadT, NULL, ClientThread, NULL);
	} else if (strcmp(role, "server") == 0) {
		//thread = new Thread(ServerThread, NULL);
		pthread_create(&threadT, NULL, ServerThread, NULL);
	}
	//thread->start();
	//thread->join();
	char cmd = '0';
	int num = 0;
	unsigned char buff[256] = {0};
	
	while (1) {
		cin>>cmd;
		switch(cmd) {
			case 's' :
			case 'S' : {
				memset(buff, 0, sizeof(buff));
				num++;
				snprintf(reinterpret_cast<char *>(buff), sizeof(buff), "hello world num=%d\n", num);
				
				int ret = sendSocket->write(buff, strlen(reinterpret_cast<char *>(buff)) + 1);
				cout<<"send msg:time="<<num<<"len="<<ret<<endl; 
				break;
			}
			default: {
				cout<<"unknow cmd"<<endl;
				break;
			}
		}

	}

}


void *CmdThread(void *arg) {
	char cmd = '0';
	int num = 0;
	unsigned char buff[256] = {0};
	
	while (1) {
		cin>>cmd;
		switch(cmd) {
			case 's' :
			case 'S' : {
				memset(buff, 0, sizeof(buff));
				num++;
				snprintf(reinterpret_cast<char *>(buff), sizeof(buff), "hello world num=%d\n", num);
				
				int ret = sendSocket->write(buff, strlen(reinterpret_cast<char *>(buff)) + 1);
				cout<<"send msg:time="<<num<<"len="<<ret<<endl; 
				break;
			}
			default: {
				cout<<"unknow cmd"<<endl;
				break;
			}
		}

	}
}

int main(int argc, char *argv[]) {
	
	TcpSocketTest(argv[1]);
	
	return 0;
}

