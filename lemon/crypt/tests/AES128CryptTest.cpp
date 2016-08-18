
#include <pthread.h>
#include <unistd.h>
#include <iostream>
//#include "mavlink.h"
#include "UdpSocket.hh"
#include "AES128Crypt.hh"

using namespace std;
using namespace lemon;

void *CryptPoint1(void *arg) {
	UdpSocket *socket = new UdpSocket();
	AES128Crypt aes128Crypt;
	unsigned char *data = (unsigned char *)calloc(1, 1024);
	unsigned char *input = (unsigned char *)calloc(1, 1024);
	strcpy(reinterpret_cast<char *>(data), "hello world by damon.he, i like you smart\n");

	//mavlink_message_t msg;
	//mavlink_msg_ait_event_pack(2, 0, &msg, 2, data);
    int len = 0;//mavlink_msg_to_send_buffer(input, &msg);
	
	SocketAddress localAddress("0.0.0.0", 7882);
	socket->bind(localAddress, true);
	
	SocketAddress remoteAddress("192.168.3.230", 7885);
	
	//cout<<"crypt send  start:len="<<len<<" buff="<<input<<endl;

	DataBuff &outDataBuff = aes128Crypt.encryptCBC(input, len);

	sleep(2);
	int sendLen = socket->write(outDataBuff.data, outDataBuff.len, remoteAddress);
	cout<<"crypt send  end:size="<<outDataBuff.len<<endl;	
	return (void *)0;
}

void *CryptPoint2(void *arg) {
	UdpSocket *socket = new UdpSocket();
	unsigned char *input = (unsigned char *)calloc(1024, 1);
	AES128Crypt aes128Crypt;
	
	SocketAddress localAddress("192.168.3.230", 7885);
	if (socket->bind(localAddress, true) != 0) {
		cout<<"bind error"<<endl;
	}
	SocketAddress remoteAddress;
	
	cout<<"crypt read start:bufflen="<<1024<<endl;
	int len = socket->read(input, 1024, &remoteAddress);
	cout<<"crypt read end1:len="<<len<<endl;
	DataBuff &outDataBuff = aes128Crypt.decryptCBC(input, len);

	if (len > 0) {
		// Something received - print out all bytes and parse packet
		//mavlink_message_t msg;
		//mavlink_status_t status;
		printf("Bytes Received: %d\nDatagram: ", (int)len);
		for (int i = 0; i < len; ++i) {
			/*if (mavlink_parse_char(MAVLINK_COMM_0, outDataBuff.data[i], &msg, &status)) {

				printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
				unsigned char tmp[256] = {0};
				//mavlink_msg_ait_event_get_data(&msg, tmp);
				cout<<"udp read end:data="<<tmp<<endl;
			}*/
		}
	}

    return (void *)0;
	//cout<<"crypt read end:data="<<outDataBuff.data<<endl;
}


void AES128CryptTest() {
	pthread_t cryptPoint1_t;
	pthread_t cryptPoint2_t;
	cout<<"aes128 crypt test start"<<endl;
	pthread_create(&cryptPoint1_t, NULL, CryptPoint1, NULL);
	pthread_create(&cryptPoint2_t, NULL, CryptPoint2, NULL);

 	pthread_join(cryptPoint1_t, NULL);
	pthread_join(cryptPoint2_t, NULL);
	cout<<"aes128 crypt test end"<<endl;
}


int main(int argc, char *argv[]) {

	AES128CryptTest();
	
	return 0;
}

