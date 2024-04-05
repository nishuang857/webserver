/*************************************************************************
	> File Name: socket.h
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月02日 星期二 21时59分25秒
 ************************************************************************/

//通信模块
#ifndef __SOCKET_H_
#define __SOCKET_H
//初始化套接字
int initSocket(short port);

//接收客户端连接请求
int acceptClient(void);

//接收http请求
char* recvRequest(int conn);

//发送响应头
int sendHead(int conn,const char* head);

//发送响应体
int sendBody(int conn,const char* path);

//终结化套接字
void deinitSocket(void);

#endif

