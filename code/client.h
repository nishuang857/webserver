/*************************************************************************
	> File Name: client.h
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月04日 星期四 11时04分28秒
 ************************************************************************/

//线程模块头文件
#ifndef __CLIENT_H_
#define __CLIENT_H_
//线程所需的参数
typedef struct clientArgs{
	const char* home; //资源在本地的存储路径
	int conn;         //通信套接字
}CA;

//线程过程函数负责和客户端通信
void* client(void* arg);

#endif

