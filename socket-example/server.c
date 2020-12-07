/**
 *	socket client示例
 *	客户端socket创建流程：
 *		1.创建socket描述符
 *		2.设定连接地址（服务端ip地址和端口号）
 *		3.connect服务端
 *		4.通过socket描述符读写
 *
 *	读写函数：write/send, read/recv
 *	
 *	客户端代码编译命令：gcc -o client client.c
 *	./client 127.0.0.1 8080
 *
 *	Authoer: JackieChan
 *	2020.12.07
 */ 
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		printf("Example: %s 8080\n", argv[0]);
		return -1;
	}
	int port = atoi(argv[1]);
	// 1.创建socket描述符
	int listenfd = 0;
	listenfd =socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == 0){
		printf("打开Scoket描述符失败！\n");
		return -1;
	}
	
	// 2.绑定服务端ip和port
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0){
		printf("绑定端口号 %d 失败！\n", port);
		close(listenfd);
		return -1;
	}

	// 3.设置listenfd的监听模式
	if(listen(listenfd, 0) != 0){
		printf("监听端口号 %d 失败!\n", port);
		close(listenfd);
		return -1;
	}
	
	// 4.阻塞等待链接
	struct sockaddr_in clientaddr;

	socklen_t client_len = sizeof(clientaddr);
	memset(&clientaddr, 0, sizeof(clientaddr));
	int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &client_len);
	if(clientfd <= 0){
		printf("服务端关闭\n");
		close(listenfd);
		return -1;
	}
	printf("客户端 %s 已经连接上来了，端口号是：%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	if(read(clientfd, buffer, sizeof(buffer)-1) <= 0){
		printf("从客户端接收信息失败\n");
		close(clientfd);
		close(listenfd);
		return -1;
	}
	printf("从客户端接收到信息: %s\n", buffer);
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "Hello, client, I accept your message!");
	if(write(clientfd, buffer, strlen(buffer)) <= 0){
		printf("向客户端写入信息失败！\n");
		return -1;
	}
	printf("向客户端发送的信息: %s\n", buffer);
	close(clientfd);
	close(listenfd);
	return 0;
}
