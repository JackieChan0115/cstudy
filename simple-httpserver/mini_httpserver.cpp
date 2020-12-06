/**
 *	简单的单线程http服务器程序
 *  使用socket实现了一个简单的http服务端,
 *  包含功能：
 *  	访问静态html页面，html文件在html目录下
 *  	访问图片，支持的后缀包含已经jpeg, png,ico，可以自己再加
 *	Author: JackieChan
 *	2020.12.6
 */ 
#include <iostream>
#include <unistd.h>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdarg.h>
#include <signal.h>
using namespace std;
 
// 服务器监听的端口号，使用字符串形式
#define SERVER_PORT  "80"
// 配置的网站根目录
#define HOME_PATH "./html"
 
int is_exist_file(const char* pathname);

// 写日志函数
void log(const char* fmt, ...);
// 处理错误请求响应
void do_error_response(int clientfd, int code);
// 处理请求信息
void do_http_response(int clientfd);
// 向客户端发送数据，path可以是html文件，png,jpeg,ico图片
void sendhtml(int clientfd, const char* path);
// 在客户端socket链接中读取请求信息，按行读取
int get_line(int sockfd, char* buffer, int size);
// 信号处理函数
void signal_handler(int signum);

static int listenfd = 0;
int main(){
	signal(15, signal_handler);
	signal(2, signal_handler);
	// 1. open a socket descriptor
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1){
		log("Failed to Open a Socket Descriptor!"); 
		return -1;
	}
	int opt = 1;
	unsigned int opt_len = sizeof(opt);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, opt_len);
	// 2. bind ip and port on the socket descriptor
    struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(SERVER_PORT));
	if(bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0){
		log("Faild bind the appointed port or address!"); 
		close(listenfd);
		return -1;
	}
	// 3. set listen mode
	if(listen(listenfd, 5) != 0){
	 	log("Failed Listen");
		close(listenfd);
		return -1;
	}
	log("%s%s", "http server has been started at port :" , SERVER_PORT);
	// 4.block and accpet client
	int clientfd;
	int socketlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	// 5.一直循环接听客户端链接。
	while(1){
		log("--------> wait for client <-------------");
		clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, (socklen_t*)&socketlen);
		log("--------------------------------------------------------------------------"); 
		log("%s%s%s%d","Client: ", inet_ntoa(clientaddr.sin_addr), ", port: ", clientaddr.sin_port);
		// read the clientfd
		do_http_response(clientfd);
		close(clientfd);
	}

	close(listenfd);
	return 0;
}
void sendhtml(int clientfd, const char* path){
	log(">>>>>>>>>> response the client");
	
	const char *main_header = "HTTP/1.1 200 OK\r\nServer: JackieChan\r\n";
	
	int type = 0;
	char subfix[10];
	int path_len = strlen(path);
	int subfix_len = 0;
	for(int _i = path_len-1; _i > 0; _i--){
		if(path[_i] == '.'){
			_i ++;
			while(_i < path_len){
				if(path[_i] >= 'A' && path[_i] <= 'Z'){
					subfix[subfix_len++] = path[_i++]-'A'+'a';
				}
				else
					subfix[subfix_len++] = path[_i++];
			}
			break;
		}
	}
	if(subfix_len == 0){ // 访问格式有问题，没有后缀
		log("访问格式有问题，没有后缀");
		return;
	}
	
	char cnttype[64];
	subfix[subfix_len] = '\0';
	if(strcmp(subfix, "jpeg") == 0){
		snprintf(cnttype, sizeof(cnttype), "Content-Type: %s\r\n", "image/jpeg");
	}
	else if(strcmp(subfix, "png") == 0){
		snprintf(cnttype, sizeof(cnttype), "Content-Type: %s\r\n", "image/png");
	}
	else if(strcmp(subfix, "html") == 0){
		snprintf(cnttype, sizeof(cnttype), "Content-Type: %s\r\n", "text/html");
	}
	else if(strcmp(subfix, "ico") == 0){
		snprintf(cnttype, sizeof(cnttype), "Content-Type: %s\r\n", "image/x-icon");
	}

	const char* connection_state = "Cache-Control: no-cache\r\nConnection: close\r\n";


	int len = write(clientfd, main_header, strlen(main_header));
	log("%s%s", "Response Client: ", main_header);
	len = write(clientfd, cnttype, strlen(cnttype));
	log("%s%s", "Response Client: ", cnttype);
	len = write(clientfd, connection_state, strlen(connection_state));
	log("%s%s", "Response Client: ", connection_state);
	FILE* fp = fopen(path, "r");
	if(fp == NULL){
		log("%s%s", path , "read error");
		return;
	}
	int filelength = 0;
	fseek(fp, 0, SEEK_END);
	filelength =(int)ftell(fp);
	char sbuffer[64];
	snprintf(sbuffer, 64, "Content-Length: %d\r\n\r\n", filelength);
	len = write(clientfd, sbuffer, strlen(sbuffer));

	int i = 0;
	char buffer[1030];
	len = 0;
	fseek(fp, 0, SEEK_SET);
	while(!feof(fp)){
		len = fread(buffer,1, 1024, fp);
		write(clientfd, buffer, len);
		i += len;
	}
	log("%s%d%s%d%s", "Response Client: ", i,  "bytes sent, ", filelength, "bytes should be sent" );
	fclose(fp);
}
void do_http_response(int clientfd){
 	char buffer[1024];
	char method[16];
	char url[256]; // 获取http协议第一行的访问链接
	char path[256]; // 将配置的网站目录和url合并，找到要访问的文件路径
	int len = 0;
	len = get_line(clientfd, buffer, sizeof(buffer));
	log("%s%s", "ReadLine: ", buffer);
	if(len == 0){
		return;
	}
	if(len < 5){ // 400 bad request
		do_error_response(clientfd, 400);
		return;
	}
	int i = 0, j = 0;
	while(i < len && !isspace(buffer[i]) && j < sizeof(method)-1){
		method[j] = buffer[i];
		i++;
		j++;
	}
	method[j] = '\0';
	log("%s%s", "request = " , method);
	if(strncasecmp(method,"GET", j) != 0){
		do_error_response(clientfd, 501);
		return;
	}
	while(isspace(buffer[j])) j++;
	i = 0;
	while(j < len && !isspace(buffer[j]) && i < sizeof(url)-1){
		url[i] = buffer[j];
		i++;
		j++;
	}
	url[i] = '\0';
	log("%s%s", "url is " , url);
	
	do{
		len = get_line(clientfd, buffer, sizeof(buffer));
		if(len > 0){
			log("%s%s", "ReadLine:" , buffer);
		}
	}while(len > 0);
	snprintf(path, sizeof(path), "%s%s", HOME_PATH, url);
	if(path[strlen(path)-1] == '/'){
		strcat(path, "index.html"); // 如果没有输入要访问文件，就默认访问/index.html
	}
	log("%s%s", "Response Client: ", path);
    if(is_exist_file(path)){
		sendhtml(clientfd, path);
	}		
	else{
		do_error_response(clientfd, 404);
	}
}

int is_exist_file(const char* pathname){
	struct stat st;
	if(0 != stat(pathname, &st)){
		return 0;
	}
	return 1;
}


int get_line(int sockfd, char* buffer, int size){
	int cnt = 0;
	char ch;

	while(cnt < size-1 && ch != '\n'){
		int len = read(sockfd, &ch, 1);
		if(len == 1){
			if(ch == '\n'){
				buffer[cnt] = '\0';
				break;
			}
			else if(ch == '\r') continue;
			buffer[cnt] = ch;
			cnt++;
		}
		else{
			buffer[cnt] = '\0';
			log("error: connect closed");
			break;
		}
	}
	return cnt;
}

void do_error_response(int clientfd, int code){
	if(code == 400){
		sendhtml(clientfd, "./tpl/400.html");
	}
	else if(code == 404){
		sendhtml(clientfd, "./tpl/404.html");
	}
	else if(code){
		sendhtml(clientfd, "./tpl/501.html");
	}
}
void log(const char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	printf("\n");
	va_end(ap);
}
void signal_handler(int signum){
	log("收到信号：%d");
	close(listenfd);
	exit(0);
}
