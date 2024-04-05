/*************************************************************************
	> File Name: http.h
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月02日 星期二 20时49分08秒
 ************************************************************************/

//http模块头文件
#ifndef __HTTP_H_
#define __HTTP_H_
#include<limits.h>
#include<sys/types.h>
//请求中解析出来的关键数据
typedef struct httpRequest{
	char method[32];      //请求方法
	char path[PATH_MAX+1];//路径
	char protocol[32];    //协议版本
	char connection[32];  //连接状态
}HTTP_REQUEST;
//对http请求的解析
int parseRequest(const char* req,HTTP_REQUEST* hreq);

//构造响应头时所需的数据
typedef struct httpRespond{
	char protocol[32];   //协议版本
	int status;			 //状态码
	char desc[64];       //状态描述
	char type[64];       //类型
	off_t length;        //长度
	char connection[32]; //连接状态
}HTTP_RESPOND;
//构造响应头
int constructHead(const HTTP_RESPOND* hres,char * head);


#endif
