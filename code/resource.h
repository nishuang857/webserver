/*************************************************************************
	> File Name: resource.h
	> Author: nishuang
	> Mail: 1300655890@qq.com 
	> Created Time: 2024年04月03日 星期三 21时22分20秒
 ************************************************************************/

//资源管理头文件
#ifndef __RESOURCE_H_
#define __RESOURCE_H_
//搜索资源
int searchResource(const char* path);

//识别类型
int identifyType(const char* path,char* type);

#endif
