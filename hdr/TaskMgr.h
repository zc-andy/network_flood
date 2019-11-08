#ifndef _TASKMGR_H_
#define _TASKMGR_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "SocketMgr.h"
#include "Timer.h"
#include "Config.h"
#include <queue>
#include <thread>
#include <vector>

namespace NS_TASKMGR
{
	class Task
	{
		public:
			/**
			 * @brief 构造函数
			 */
			Task()
			{
				m_bNew = false;
				m_uClientPort = 0;
				m_uServerPort = 0;
				m_uTimeTotal = 0;
				m_uReqTotal = 0;
			}

			/**
			 * @brief 析构函数
			 */
			~Task()
			{
				m_bNew = false;
				m_uClientPort = 0;
				m_uServerPort = 0;
				m_uTimeTotal = 0;
				m_uReqTotal = 0;
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

		public:
			bool m_bNew;                      //新建标记
			uint16_t m_uClientPort;           //客户端port
			uint16_t m_uServerPort;           //服务端port
			uint32_t m_uTimeTotal;            //洪水时间
			uint32_t m_uReqTotal;             //请求总次数
			std::string m_clientIp;           //客户端ip
			std::string m_serverIp;           //服务端ip
			std::string m_method;             //请求方法
			std::string m_url;                //请求url
			std::string m_cookie;             //cookie体
			std::string m_body;               //请求body
	};

	/** 线程信息类 **/
	class ThreadDetail
	{
		public:
			/**
			 * @brief 构造函数
			 */
			ThreadDetail()
			{}

			/**
			 * @brief 析构函数
			 */
			~ThreadDetail()
			{}

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

		public:
			std::thread m_thread;               //线程对象
	};
}

class TaskMgr
{
	public:
		/**
		 * @brief 析构函数
		 */
		~TaskMgr();

		/**
		 * @brief 去除赋值拷贝与拷贝构造函数(c++11)
		 */
		TaskMgr(TaskMgr const&) = delete;
		void operator=(TaskMgr const&) = delete;

		/**
 		 * @brief 获取单例对象
 		 *
 		 * @return 单例对象
 		 */
		static TaskMgr &getInstance()
		{
			static TaskMgr _instance;
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
		TaskMgr();
		
		/**
		 * @brief 线程处理函数
		 */
		void process();

		/**
		 * @brief 处理新建请求
		 *
		 * @prame pTask 任务结构体
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t newConnection(NS_TASKMGR::Task *&pTask);

		/**
		 * @brief 处理并发请求
		 *
		 * @prame pTask 任务结构体
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败  
		 */
		int32_t processConcurrent(NS_TASKMGR::Task *&pTask);

	private:
		/**
		 * @brief 线程状态
		 */
		bool m_run;

		/**
		 * @brief 线程启动个数
		 */
		uint16_t m_threadNum;

		/**
		 * @brief 接收任务总次数
		 */
		uint32_t m_processTotal;

		/**
		 * @brief 处理新建总次数
		 */
		uint32_t m_newConnectionTotal;

		/**
		 * @brief 处理新建失败次数
		 */
		uint32_t m_newConnectionFailed;

		/**
		 * @brief 处理并发总次数
		 */
		uint32_t m_processConcurrentTotal;

		/**
		 * @brief 处理并发失败次数
		 */
		uint32_t m_processConcurrentFailed;

		/**
		 * @brief 任务队列
		 */
		std::queue<NS_TASKMGR::Task *> _queue;

		/**
		 * @brief 线程池信息
		 */
		std::vector<NS_TASKMGR::ThreadDetail*> m_threadPool;

};

#endif 
