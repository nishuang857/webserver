/*************************************************************************
	> File Name: client.c
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月04日 星期四 11时04分28秒
 ************************************************************************/

//线程模块源文件
#include<unistd.h>
#include<stdio.h>
#include<sys/syscall.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include"http.h"
#include"socket.h"
#include"resource.h"
#include"client.h"

//线程过程函数负责和客户端通信
void* client(void* arg){
	CA* ca=(CA*)arg;
	printf("%d.%ld > 客户机线程处理开始\n",getpid(),syscall(SYS_gettid));
	for(;;){
		printf("%d.%ld > 接受请求\n",getpid(),syscall(SYS_gettid));
		char *req=recvRequest(ca->conn);
		if(req==NULL){
			break;
		}
		printf("%d.%ld > 请求电文:\n%s\n",getpid(),syscall(SYS_gettid),req);
		printf("%d.%ld > 解析请求：\n",getpid(),syscall(SYS_gettid));
		HTTP_REQUEST hreq={};//用来存储解析得到的数据
		if(parseRequest(req,&hreq)==-1){
			free(req);//释放存储区
			break;
		}
		free(req);//解析成功，后续使用hreq即可，存储区释放
		
		//处理路径
		char root[PATH_MAX+1];//资源路径
		char path[PATH_MAX+1];//真实路径
		
		strcpy(root,ca->home);// /home/ns/webserver/home/home
		//处理路径最后的/
		if(root[strlen(root)-1]=='/'){
			root[strlen(root)-1]=='\0';
		}
		//真实路径
		strcpy(path,root);  
		strcat(path,hreq.path);//拼接
		if(strcmp(hreq.path,"/")==0){
			strcat(path,"index.html");
		}

		//构造响应所需的数据
		HTTP_RESPOND hres={"HTTP/1.1",200,"OK","text/html"};
		//搜索资源
		if(searchResource(path)==-1){//没找到
			hres.status=404;
			strcpy(hres.desc,"NOT FOUND");
			strcpy(path,root);
			strcat(path,"/404.html");
		}else if(identifyType(path,hres.type)==-1){	
			hres.status=404;
			strcpy(hres.desc,"NOT FOUND");
			strcpy(path,root);
			strcat(path,"/404.html");
		}

		//长度
		struct stat st;//输出文件元数据
		if(stat(path,&st)==-1){
			break;
		}
		hres.length=st.st_size;

		//连接状态,请求连接状态是啥，响应连接状态就是啥
		if(strlen(hreq.connection)){
			strcpy(hres.connection,hreq.connection);
		}else if(strcasecmp(hreq.protocol,"http/1.0")==0){
			strcpy(hres.connection,"close");
		}else{
			strcpy(hres.connection,"kepp-alive");
		}
		
		//构造响应
		printf("%d.%ld > 构造响应\n",getpid(),syscall(SYS_gettid));
		char head[1024];
		if(constructHead(&hres,head)==-1){
			break;
		}
		printf("%d.%ld > 发送响应头\n",getpid(),syscall(SYS_gettid));
		if(sendHead(ca->conn,head)==-1){
			break;
		}
		printf("%d.%ld > 发送响应体\n",getpid(),syscall(SYS_gettid));
		if(sendBody(ca->conn,path)==-1){
			break;
		}
		
		//如果连接状态是close,结束循环
		if(strcasecmp(hres.connection,"close")==0){
			break;
		}
	}
	close(ca->conn);
	free(ca);
	printf("%d.%ld > 客户机线程结束\n",getpid(),syscall(SYS_gettid));
	return NULL;
}


