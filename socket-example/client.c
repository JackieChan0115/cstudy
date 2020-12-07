/**
 * socket server 示例
 * server端流程：
 * 		1.打开socket描述符
 * 		2.绑定ip地址和端口号，这里的端口号必须指定，ip地址可以让bind自己选择
 * 		3.设定监听模式（listen)
 * 		4.通过accept阻塞监听客户端的连接
 * 		5.获取客户端连接的socket描述符进行读写
 *
 * 读写函数: write/send, read/recv
 * 
 * Author: JackieChan
 * 2020.12.07
 *
 */ 
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char** argv){

	if(argc != 3){
		printf("usage : %s <IPv4 address> <port>\n", argv[0]);
		printf("Example : 127.0.0.1 8080");
		return -1;
	}
	int port = 0;
	port = atoi(argv[2]);
 	int sfd = 0;

	// 1.创建一个socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1){
		printf("打开socket链接失败！\n");
		return -1;
	} 
	// 2.向服务器发起请求
	// 获取服务器地址
	struct hostent *h;
	h = gethostbyname(argv[1]);
 	if(h == NULL){
		printf("解析指定主机地址失败!\n");
		return -1;
	}
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port); // 将端口号改成网络字节序，htons
	memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);
	if(connect(sfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("链接服务器失败!\n");
		close(sfd); // 关闭打开的socket描述符
		return -1;
	}
	
	// 3.向sfd里面写入数据和读取数据
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "Hello, server, I am client!");
	int len = write(sfd, buffer, strlen(buffer));
	if(len <= 0){
		printf("向服务端发送数据出现错误！\n");
		close(sfd);
		return -1;
	}
	printf("发送信息成功：%s\n", buffer);
	len = read(sfd, buffer, sizeof(buffer)-1);
	if(len <= 0){
		printf("接收服务端信息失败！\n");
		close(sfd);
		return -1;
	}
	printf("接收到服务端信息：%s\n", buffer);
	return 0;
}
