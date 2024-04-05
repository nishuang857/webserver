/*************************************************************************
	> File Name: server.h
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月04日 星期四 16时03分23秒
 ************************************************************************/
//服务器头文件
#ifndef __SERVER_H_
#define __SERVER_H_
//初始化服务器
int initServer(short port);
//运行服务器
int runServer(const char* home);
//终结化服务器
void deinitServer(void);

#endif
