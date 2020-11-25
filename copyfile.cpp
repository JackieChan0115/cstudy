/**
 *	本文件是文件复制程序Demo
 *	操作方式: ./copyfile sourcefilename dstfilename
 *	只允许复制文件
 *	Author: JackieChan
 *	2020.11.25
 */ 
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){

	if(argc != 3){
		printf("这是一个文件拷贝程序\n");
		printf("使用格式为 ./copyfile sourcefilename destfilename\n");
		printf("例如：./copyfile ./tmp1/file1.txt ./tmp2/file2.txt\n");
		printf("sourcefilename  源文件名称\n");
		printf("destfilename  目标文件名称\n\n");
		return -1;
	}
	
	// 定义文件操作符指针
	FILE *srcfp = NULL, *dstfp = NULL;
	
	// 以只读方式打开文件
	if((srcfp = fopen(argv[1],"rb")) == 0){
		printf("fopen(%s) failed!", argv[1]);
		fclose(srcfp);
		return -1;
	}
	// 以只写方式打开文件
	if((dstfp = fopen(argv[2],"wb")) == 0){
		printf("fopen(%s) failed!", argv[2]);
		fclose(dstfp);
		return -1;
	}
	
	int nread = 0;
	char buffer[500];
	while(1){
		memset(buffer,0,sizeof(buffer));
		if((nread = fread(buffer,1,500,srcfp)) == 0) break;
		fwrite(buffer,1,nread,dstfp);
	}

	//关闭两个文件操作符
	fclose(srcfp);
	fclose(dstfp);
	printf("文件复制成功\n");
	return 0;
}

