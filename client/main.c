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
	struct sockaddr_in servaddr;
	int servlen;
	char buf[DATA_SIZE];
	int recvlen;
	int tip_add = 1;

	memset(buf, 0, DATA_SIZE);

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
	sock_fd = socket (AF_INET, SOCK_DGRAM, 0);
	if(sock_fd <= 0)
	{
		printf("create scoket fail!\n");
		WSACleanup();
		return 0;
	}
	else
	{
		printf("2.create socket sucess!\n");
	}

	//step3.设置服务端IP和port
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(TIME_PORT);
	servaddr.sin_addr.S_un.S_addr = inet_addr(LOOPBACK_IP);
	servlen = sizeof(servaddr);
	printf("3.set server ip/port over, client init over!\n");
	
	while (1)
	{
		printf("[%2d]input your cmd:", tip_add++);
		scanf("%s",buf);
		if(-1 == sendto(sock_fd, buf, sizeof(buf), 0, (struct sockaddr*)&servaddr, servlen))
		{
			printf("send cmd fail!\n");
			continue;
		}
		else
		{
			printf("send cmd \"%s\"success!\n",buf);
		}

		if(0 == strncmp(buf, "quit", 4))
		{
			printf("send \"quit\" cmd, exit client!\n");
			break;
		}

		recvlen = recvfrom(sock_fd, buf, sizeof(buf), 0, (struct sockaddr*)&servaddr, &servlen);
		if(recvlen <= 0)
		{
			printf("recv data erro!,error code:%d.\n",WSAGetLastError());
			continue;
		}
		else
		{
			buf[recvlen] = '\0';
			printf("time from server: %s\n",buf);
		}
	}

	closesocket(sock_fd);
	WSACleanup();
	getchar();
	return 0;
}
