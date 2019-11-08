#ifndef _CTASK_MGR_H_
#define _CTASK_MGR_H_

#include "MemoryDefine.h"
#include "RetCodeDefine.h"
#include "SocketMgr.h"
#include "Config.h"
#include "Timer.h"
#include <queue>
#include <thread>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <list>

namespace NS_CTASKMGR
{
	//默认日志文件目录
	static const char * const PATH_OF_CAP_LOG = "cap/";
}

class CTaskMgr
{
	public:
		/**
		 * @brief 析构函数
		 */
		~CTaskMgr();

		/**
		 * @brief 去除赋值拷贝与拷贝构造函数(c++11)
		 */
		CTaskMgr(CTaskMgr const&) = delete;
		void operator=(CTaskMgr const&) = delete;

		/**
 		 * @brief 获取单例对象
 		 *
 		 * @return 单例对象
 		 */
		static CTaskMgr &getInstance()
		{
			static CTaskMgr _instance;
			return _instance; 
		}

		/**
		 * @brief 重载 内存分配
		 *
		 * @prame size 内存大小
		 *
		 * @return 内存地址
		 */
		static void * operator new(size_t size)
		{
			void *p = (void*)_MEM_NEW_(size);
			return p;
		}

		/**
		 * @brief 重载 内存释放
		 *
		 * @prame p 释放地址
		 */
		static void operator delete(void *p)
		{
			_MEM_DEL_(p);
		}
		
		/**
		 * @brief 初始化接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t init();

		/**
		 * @brief 启动接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t start();

		/**
		 * @brief 线程join
		 */
		void threadJoin();

	private:
		/**
		 * @brief 构造函数
		 */
		CTaskMgr();
		
		/**
		 * @brief 线程处理函数
		 */
		void process();

		/**
		 * @brief 处理日志文件
		 *
		 * @prame filename 文件名
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t processFile(const std::string &filename);

		/**
		 * @brief 处理行文件流
		 *
		 * @prame line_str 文件名
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t parserLine(std::string line_str);

	private:
		/**
		 * @brief 线程状态
		 */
		bool m_run;

		/**
		 * @brief server port
		 */
		uint16_t m_serverPort;

		/**
		 * @brief 解析行失败次数
		 */
		uint32_t m_parserLineFailed;

		/**
		 * @brief server ip
		 */
		std::string m_serverIp;

		/**
		 * @brief Socket管理器
		 */
		SocketMgr m_socket;

		/**
		 * @brief 线程池信息
		 */
		std::thread m_thread;

};

#endif 
