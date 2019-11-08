#include "SocketMgr.h"

/** 构造函数 **/
SocketMgr::SocketMgr()
{
	m_socketFd = -1;
	m_sendFailed = 0;
}

/** 析构函数 **/
SocketMgr::~SocketMgr()
{
}

/** 连接服务器 **/
int32_t SocketMgr::connectServer(uint16_t uPort, std::string serverIp)
{
	//异常判断
	if (serverIp.empty()) {
		return RET::FAIL; 
	}

	//建立socket
	m_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socketFd < 0) {
		return RET::FAIL;
	}

	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(uPort);
	inet_pton(AF_INET, serverIp.c_str(), &server_addr.sin_addr);

	if (0 > connect(m_socketFd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)))
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 发送消息 **/
int32_t SocketMgr::sendMsg(std::string msg)
{
	if (0 > write(m_socketFd, msg.c_str(), msg.size()))
	{
		m_sendFailed++;
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 关闭连接 **/
int32_t SocketMgr::closeConnection()
{
	if (0 > m_socketFd || 0 > close(m_socketFd)) 
	{
		return RET::FAIL;
	}

	return RET::SUC;
}
