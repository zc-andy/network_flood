#ifndef _SOCKETMGR_H_
#define _SOCKETMGR_H_

#include "MemoryDefine.h"
#include "RetCodeDefine.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

class SocketMgr
{
	public:
		/**
		 * @brief 构造函数
		 */
		SocketMgr();

		/**
		 * @brief 析构函数
		 */
		~SocketMgr();

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
		 * @brief 新建连接
		 *
		 * @prame uPort 服务端port; serverIp 服务端ip
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t connectServer(uint16_t uPort, std::string serverIp);

		/**
		 * @brief 发送消息
		 *
		 * @prame msg 消息体
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t sendMsg(std::string msg);

		/**
		 * @brief 关闭连接
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t closeConnection();

	private:
		/**
		 * @brief 套接字
		 */
		int32_t m_socketFd;

		/**
		 * @brief 发送失败次数统计
		 */
		uint32_t m_sendFailed;
};

#endif
