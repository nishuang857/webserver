/*************************************************************************
	> File Name: signals.c
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月03日 星期三 20时52分13秒
 ************************************************************************/

#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include"signals.h"


//忽略大部分信号
int initSignals(void){
	printf("%d > 忽略大部分信号\n",getpid());
	for(int signum=SIGHUP;signum<=SIGRTMAX;signum++){
		if(signum!=SIGINT&&signum!=SIGTERM){
			signal(signum,SIG_IGN);
		}//保留2号和15号信号
	}
	return 0;
}



