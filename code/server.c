/*************************************************************************
	> File Name: server.c
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月04日 星期四 16时03分23秒
 ************************************************************************/
//服务器源文件文件
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"socket.h"
#include"server.h"
#include"client.h"
#include"signals.h"

//初始化服务器
int initServer(short port){
	//初始化信号
	if(initSignals()==-1){
		return -1;
	}
	//初始化套接字
	if(initSocket(port)==-1){
		return -1;
	}
	return 0;

}
//运行服务器
int runServer(const char* home){
	for(;;){
		//等待并接受客户端的连接请求
		int conn=acceptClient();
		if(conn==-1){
			return -1;
		}
		//创建客户端线程
		pthread_t tid;
		pthread_attr_t attr; //线程属性
		pthread_attr_init(&attr); //初始化线程属性
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); 
		CA* ca=malloc(sizeof(CA));
		ca->conn=conn;
		ca->home=home;
		int error=pthread_create(&tid,&attr,client,ca);
		if(error){
			fprintf(stderr,"pthread_create:%s\n",strerror(error));
			return -1;
		}
	}

	return 0;
}
//终结化服务器
void deinitServer(void){
	//终结化套接字
	deinitSocket();
}


