#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Winsock2.h>
#include <time.h>
#include <errno.h>

#define TIME_PORT 9090
#define DATA_SIZE 256
#define LOOPBACK_IP "127.0.0.1"

int main(int argc, char *argv[])
{
	WSADATA Ws;
	SOCKET sock_fd;
	struct sockaddr_in local;
	struct sockaddr_in from;
	int fromlen;
	char buf[DATA_SIZE];
	int recvlen;
	int tip_add = 1;
	time_t cur_time;

	//step1.winsock服务初始化
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		printf("init winSock fail!\n");
		return 0;
	}
	else
	{
		printf("1.bind sock success!\n");
	}
	
	//step2.创建套接字
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd <= 0)
	{
		printf("create socket fail!\n");
		WSACleanup();
		return 0;
	}
	else
	{
		printf("2.create scoket success!\n");
	}
	
	//step3.设置要绑定的IP和Port
	local.sin_family = AF_INET;
	local.sin_port = htons(TIME_PORT);
	local.sin_addr.S_un.S_addr =  inet_addr(LOOPBACK_IP);
	fromlen = sizeof(from);
	printf("3.set server ip/port over!\n");

	//step4.绑定本机的套接字
	if (0 != bind(sock_fd, (struct sockaddr*)&local, sizeof(local)))
	{
		printf("bind sock erro!\n");
		closesocket(sock_fd);
		WSACleanup();
		return 0;	
	}
	else
	{
		printf("4.bind sock success, server init over!\n");
	}

	printf("waiting request from client...\n");

	while(1)
	{
		recvlen = recvfrom(sock_fd, buf, sizeof(buf), 0, (struct sockaddr*)&from, &fromlen);
		if(recvlen <= 0)
		{
			printf("recv msg erro,erro code:!\n",WSAGetLastError());
			continue;
		}
		
		buf[recvlen] = '\0';
		printf("[%2d]recv msg, client request is \"%s\",", tip_add++, buf);

		if(0 == strncmp(buf, "quit", 4))
		{
			printf("press any key exit!\n");
			break;
		}

		if(0 == strncmp(buf, "time", 4))
		{
			time(&cur_time);
			strcpy(buf, asctime(localtime(&cur_time)));
			if(-1 != sendto(sock_fd, buf, sizeof(buf), 0, (struct sockaddr*)&from, fromlen))
			{
				printf("proc this cmd success!\n");
			}
			else
			{
				printf("proc this cmd fail\n");
			}
		}
	}

	closesocket(sock_fd);
	WSACleanup();
	getchar();
	return 0;
}
