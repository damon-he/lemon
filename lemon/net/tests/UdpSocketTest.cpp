#include <pthread.h>
#include <unistd.h>
#include <iostream>
//#include "mavlink.h"
#include "UdpSocket.hh"

using namespace std;
using namespace lemon;

void *UdpPoint1(void *arg) {
	UdpSocket *socket = new UdpSocket();
	unsigned char buff[1024] = {0};
	unsigned char tmp[14] = {0};
	SocketAddress localAddress("0.0.0.0", 7882);
	socket->bind(localAddress, true);

	strcpy(reinterpret_cast<char *>(tmp), "hello world\n");
	
	//mavlink_message_t msg;
	//mavlink_msg_ait_event_pack(2, 0, &msg, 2, tmp);
        int len = 0;//mavlink_msg_to_send_buffer(buff, &msg);
	
	SocketAddress remoteAddress("192.168.3.230", 7885);

	
	
	cout<<"udp send hello world start:len="<<len<<endl;
	sleep(2);
	len = socket->write(buff, len,
			remoteAddress);
	cout<<"udp send hello world end:len="<<len<<endl;	
	return (void *)0;
}

void *UdpPoint2(void *arg) {
	UdpSocket *socket = new UdpSocket(true);
	unsigned char buff[1024] = {0};
	unsigned char tmp[1024] = {0};
	SocketAddress localAddress("192.168.3.230", 7885);
	if (socket->bind(localAddress, true) != 0) {
		cout<<"bind error"<<endl;
	}
	SocketAddress remoteAddress;
	
	cout<<"udp read start"<<endl;
	int recsize = socket->read(buff, sizeof(buff), &remoteAddress);
	cout<<"udp read end,len="<<recsize<<endl;
	if (recsize > 0) {
		// Something received - print out all bytes and parse packet
		//mavlink_message_t msg;
		//mavlink_status_t status;
		printf("Bytes Received: %d\nDatagram: ", (int)recsize);
		for (int i = 0; i < recsize; ++i) {
			/*if (mavlink_parse_char(MAVLINK_COMM_0, buff[i], &msg, &status)) {
				printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
				//mavlink_msg_ait_event_get_data(&msg, tmp);
				cout<<"udp read end:data="<<tmp<<endl;
			}*/
		}
	}
    return (void *)0;
}


void UdpSocketTest() {
	pthread_t udpPoint1_t;
	pthread_t udpPoint2_t;
	cout<<"udp test start"<<endl;
	pthread_create(&udpPoint1_t, NULL, UdpPoint1, NULL);
	pthread_create(&udpPoint2_t, NULL, UdpPoint2, NULL);

 	pthread_join(udpPoint1_t, NULL);
	pthread_join(udpPoint2_t, NULL);
	cout<<"udp test end"<<endl;
}


int main(int argc, char *argv[]) {

	UdpSocketTest();
	
	return 0;
}

