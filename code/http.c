/*************************************************************************
	> File Name: http.c
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月02日 星期二 20时42分18秒
 ************************************************************************/
//http模块实现
#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>
#define __USE_GNU
#include<string.h>
#include<time.h>
#include"http.h"

//解析请求
int parseRequest(const char* req,HTTP_REQUEST* hreq){
	
	sscanf(req,"%s%s%s",hreq->method,hreq->path,hreq->protocol);//按照空格分隔
	char* con=strcasestr(req,"connection");  //忽略大小写在req中找connection
	if(con){//找到
		sscanf(con,"%*s%s",hreq->connection);
	}
	printf("%d.%ld > [%s][%s][%s][%s]\n",getpid(),syscall(SYS_gettid),
			hreq->method,hreq->path,hreq->protocol,hreq->connection);
	//判断请求方法是否为get
	if(strcasecmp(hreq->method,"get")){
		printf("%d.%ld > 无效的请求方法\n",getpid(),syscall(SYS_gettid));
		return -1;
	}
	//判断协议版本
	if(strcasecmp(hreq->protocol,"http/1.0")&&strcasecmp(hreq->protocol,"http/1.1")){
		printf("%d.%ld > 无效的协议版本\n",getpid(),syscall(SYS_gettid));
		return -1;
	}
	return 0;
}

//构造响应头
int constructHead(const HTTP_RESPOND* hres,char* head){
	char dateTime[32];
	time_t now=time(NULL);
	strftime(dateTime,sizeof(dateTime),"%a %d %b %Y %T",gmtime(&now));
	sprintf(head,"%s %d %s\r\n"
			     "Server: NishuangWebserver 4.0\r\n"
				 "Date: %s\r\n"
				 "Content-Type: %s\r\n"
				 "Content-length: %ld\r\n"
				 "Connection: %s\r\n\r\n",
				 hres->protocol,
				 hres->status,
				 hres->desc,
				 dateTime,
				 hres->type,
				 hres->length,
				 hres->connection);

	return 0;
}


