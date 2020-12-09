### struct sockaddr
**struct sockaddr是用来配置ip地址、端口、协议等信息的**
结构代码如下所示:

```c
// 这个结构体的代码可以在下面两个文件中查找到：
// usr/include/bits/types/struct_osockaddr.h中
// usr/include/bits/socket.h的178行
struct sockaddr{
	unsigned short int sa_family;
	unsigned char sa_data[14];
};
```
struct sockaddr结构体宽度为 sizeof(short) + 14 * sizeof(char) = 16 bytes

在socket编程中，常操作的结构体为**struct sockaddr_in**
可以在编辑完**struct sockaddr_in**之后，将**struct sockaddr_in**强制转换为 **struct sockaddr**类型

```c
// 在/usr/include/netinet/in.h文件的237行
struct sockaddr_in{
	unsigned short sin_port; // 2字节
	struct in_addr sin_addr; // 4字节
	unsigned char sin_zero[sizeof(struct sockaddr) - sizeof(short) - sizeof(int)]; // 10字节
};

// 在/usr/include/netinet/in.h文件的30行可以看到以下定义
typedef uint32_t in_addr_t;
struct in_addr{
	in_addr_t s_addr;
};
```
struct sockaddr_in中的 sin_zero是为了与struct sockaddr的宽度一致，刻意定义的占位数组，实际不用。
因为在C中想要将两个类型强制转换，占用内存大小必须一致
