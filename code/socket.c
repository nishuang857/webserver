/*************************************************************************
	> File Name: socket.h
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月02日 星期二 21时59分25秒
 ************************************************************************/

//通信模块
#include<unistd.h>
#include<fcntl.h>
#include<sys/syscall.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"socket.h"

static int s_sock = -1;//侦听套接字

//初始化套接字
int initSocket(short port){
	printf("%d.%ld > 创建套接字\n",getpid(),syscall(SYS_gettid));
	s_sock=socket(AF_INET,SOCK_STREAM,0);
	if(s_sock==-1){
		perror("socket");
		return -1;
	}	
	printf("%d.%ld > 设置套接字\n",getpid(),syscall(SYS_gettid));
	//端口复用，允许多个套接字绑在同一个端口上
	int on=1;
	if(setsockopt(s_sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))==-1){
		perror("setsockopt");
		return -1;
	}	
	printf("%d.%ld > 组织地址结构\n",getpid(),syscall(SYS_gettid));
	struct sockaddr_in ser;
	ser.sin_family=AF_INET;
	ser.sin_port=htons(port);
	ser.sin_addr.s_addr=INADDR_ANY;
	printf("%d.%ld > 绑定端口号\n",getpid(),syscall(SYS_gettid));
	if(bind(s_sock,(struct sockaddr*)&ser,sizeof(ser))==-1){
		perror("bind");
		return -1;
	}
	printf("%d.%ld > 开启侦听\n",getpid(),syscall(SYS_gettid));
	if(listen(s_sock,1024)==-1){
		perror("listen");
		return -1;
	}
	return 0;
}
//接收客户端连接请求
int acceptClient(void){
	printf("%d.%ld > 等待客户机的连接\n",getpid(),syscall(SYS_gettid));
	struct sockaddr_in cli; //输出客户端地址结构
	socklen_t len=sizeof(cli);
	int conn=accept(s_sock,(struct sockaddr*)&cli,&len);
	if(conn==-1){
		perror("accept");
		return -1;
	}
	printf("%d.%ld > 接收到客户端%s:%hu的连接\n",getpid(),syscall(SYS_gettid),
			       inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
	return conn;
}

//接收http请求
char* recvRequest(int conn){
	char* req=NULL;//存储区首地址
	ssize_t len=0; //总字节数
	for(;;){
		char buf[1024]={};
		ssize_t size=recv(conn,buf,sizeof(buf)-1,0);
		if(size==-1){
			perror("recv");
			free(req);
			return NULL;
		}
		if(size==0){
			printf("%d.%ld > 客户机断开连接\n",getpid(),syscall(SYS_gettid));
			free(req);
			return NULL;
		}
		//扩大存储区
		req=realloc(req,len+size+1);
		//此次接的数据存到扩大的区域
		memcpy(req+len,buf,size+1);
		//总字节数累加
		len+=size;
		//退出循环
		if(strstr(req,"\r\n\r\n")){
			break;
		}
	}
	return req;
}

//发送响应头
int sendHead(int conn,const char* head){
	if(send(conn,head,strlen(head),0)==-1){
		perror("send");
		return -1;
	}
	return 0;
}

//发送响应体
int sendBody(int conn,const char* path){
	int fd=open(path,O_RDONLY);
	if(fd==-1){
		perror("open");
		return -1;
	}
	char buf[1024];
	ssize_t len;
	while((len=read(fd,buf,sizeof(buf)-1))>0){
		if(send(conn,buf,len,0)==-1){
			perror("send");
			return -1;
		}
	}
	if(len==-1){
		perror("read");
		close(fd);
		return -1;
	}
	close(fd);
	return 0;

}

//终结化套接字
void deinitSocket(void){
	close(s_sock);
}


