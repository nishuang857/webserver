/*************************************************************************
	> File Name: main.c
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月04日 星期四 16时22分11秒
 ************************************************************************/
//主模块
#include<stdio.h>
#include<stdlib.h>
#include"server.h"

int main(int argc,char* argv[]){
	//初始化服务器
	if(initServer(argc<2 ? 80:atoi(argv[1]))==-1){
		return -1;
	}
	//运行服务器
	if(runServer(argc<3 ? "../home":argv[2])==-1){
		return -1;
	}
	//终结化服务器
	deinitServer();
	return 0;
}

