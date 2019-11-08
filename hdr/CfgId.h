#ifndef _CFGID_H_
#define _CFGID_H_

#include <iostream> 

#define CFG_MAX_DEPTH 5 

namespace NS_CONFIG
{
	typedef enum CFGID
	{
		EM_CFGID_START = -1,                //枚举保护
		EM_CFGID_SERVER_IP = 0,             //服务端ip
		EM_CFGID_SERVER_PORT,               //服务端port
		EM_CFGID_QUEUE_MAX_SIZE,            //缓冲区最大长度
		EM_CFGID_TASK_THREAD_NUM,           //任务中心启动线程数量
		EM_CFGID_CTASK_SERVER_IP,           //C任务中心服务端ip
		EM_CFGID_CTASK_SERVER_PORT,         //C任务中心服务端port
		EM_CFGID_END                        //枚举保护
	}EM_CFGID;
	
	typedef struct CfgInfo
	{
		NS_CONFIG::EM_CFGID CfgId;                      //配置id
		const char* filename;                           //配置文件名
		const char* cfgpath[CFG_MAX_DEPTH];             //配置路径 
	}CfgInfo;
}

//配置信息数组
static const NS_CONFIG::CfgInfo g_stCfgInfo[] = {
	//Processor
	//服务端ip
	{NS_CONFIG::EM_CFGID_SERVER_IP, "../etc/flood.json", {"processor", "server_ip", (const char *)0}},
	//服务端port
	{NS_CONFIG::EM_CFGID_SERVER_PORT, "../etc/flood.json", {"processor", "server_port", (const char *)0}},
	//缓冲区最大长度
	{NS_CONFIG::EM_CFGID_QUEUE_MAX_SIZE, "../etc/flood.json", {"processor", "queue_max_size", (const char *)0}},
	
	//TaskMgr
	//线程启动数量
	{NS_CONFIG::EM_CFGID_TASK_THREAD_NUM, "../etc/flood.json", {"taskmgr", "thread_num", (const char*)0}},

	//CTaskMgr
	//C任务中心ip
	{NS_CONFIG::EM_CFGID_CTASK_SERVER_IP, "../etc/flood.json", {"ctaskmgr", "server_ip", (const char*)0}},
	//C任务中心port
	{NS_CONFIG::EM_CFGID_CTASK_SERVER_PORT, "../etc/flood.json", {"ctaskmgr", "server_port", (const char*)0}}
};

#endif 
