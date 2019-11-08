#include "TaskMgr.h"

/** 构造函数 **/
TaskMgr::TaskMgr()
{
	m_run = false;
	m_threadNum = 0;
}

/** 析构函数 **/
TaskMgr::~TaskMgr()
{}

/** 初始化接口 **/
int32_t TaskMgr::init()
{
	//读取线程数量
	int32_t iValue = -1;
	if (RET::SUC != Config::getCfg(NS_CONFIG::EM_CFGID_TASK_THREAD_NUM, iValue))
	{
		std::cout<<"TaskMgr: read thread num failed!"<<std::endl;
		return RET::FAIL;
	}
	m_threadNum = (uint16_t)iValue;

	return RET::SUC;
}

/** 启动接口 **/
int32_t TaskMgr::start()
{
	m_run = true;
	for (uint16_t uIndex = 0; uIndex < m_threadNum; uIndex++)
	{
		NS_TASKMGR::ThreadDetail *pDetail = nullptr;
		try {
			pDetail = new NS_TASKMGR::ThreadDetail();
		} catch(std::bad_alloc) {
			return RET::FAIL;
		}

		pDetail->m_thread = std::thread(std::bind(&TaskMgr::process, this));
		m_threadPool.push_back(pDetail);
	}

	return RET::SUC;
}

/** 线程处理接口 **/
void TaskMgr::process()
{
	while (m_run) {
		std::cout<<"Task"<<std::endl;
		//判断任务队列是否为空
		if (_queue.empty()) {
			continue;
		}

		//取任务
		NS_TASKMGR::Task *pTask = _queue.front();
		m_processTotal++;

		//异常判断
		if (nullptr == pTask) {
			_queue.pop();
			continue;
		}

		//处理新建
		if (pTask->m_bNew) {
			m_newConnectionTotal++;
			if (RET::SUC != newConnection(pTask)) {
				m_newConnectionFailed++;
			}
		//处理并发
		} else {
			m_processConcurrentTotal++;
			if (RET::SUC != processConcurrent(pTask)) {
				m_processConcurrentFailed++;
			}
		}

		//内存释放
		_queue.pop();
		if (nullptr != pTask) {
			delete pTask;
			pTask = nullptr;
		}
	}
}

/** 线程join **/
void TaskMgr::threadJoin()
{
	std::vector<NS_TASKMGR::ThreadDetail*>::iterator iter = m_threadPool.begin();
	while (iter != m_threadPool.end()) {
		(*iter)->m_thread.join();
	}
}

/** 处理新建请求 **/
int32_t TaskMgr::newConnection(NS_TASKMGR::Task *&pTask)
{
	SocketMgr socket;
	if (0 < pTask->m_uReqTotal) {
		uint32_t uTotal = 0;
		uint32_t uFailed = 0;
		while (uTotal < pTask->m_uReqTotal) {
			//建立连接
			if (RET::SUC != socket.connectServer(pTask->m_uServerPort, pTask->m_serverIp))
			{
				uTotal++;
				uFailed++;
				continue;
			}

			//拼接http请求消息，发送至服务端
			std::string httpRequest = pTask->m_method + " " + pTask->m_url + " HTTP/1.1\n"
					+ "Host: " + pTask->m_serverIp + "\n" 
					+ "Cookie: " + pTask->m_cookie + "\n"
					+ "Content-Length: " + std::to_string(pTask->m_body.size()) + "\n\n"
					+ pTask->m_body + "\n";
			if (RET::SUC != socket.sendMsg(httpRequest))
			{
				uTotal++;
				uFailed++;
				continue;
			}

			//关闭连接
			socket.closeConnection(); 
			uTotal++;
		}
	} else {
		uint64_t uBeginTime = Timer::getLocalTime();
	}
	
	return RET::SUC;
}

/** 处理并发请求 **/
int32_t TaskMgr::processConcurrent(NS_TASKMGR::Task *&pTask)
{
	return RET::SUC;
}
