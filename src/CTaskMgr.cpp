#include "CTaskMgr.h"

/** 构造函数 **/
CTaskMgr::CTaskMgr()
{
	m_run = false;
	m_serverPort = 0;
	m_parserLineFailed = 0;
}

/** 析构函数 **/
CTaskMgr::~CTaskMgr()
{}

/** 初始化接口 **/
int32_t CTaskMgr::init()
{
	//读取服务端port
	int32_t iValue = -1;
	if (RET::SUC != Config::getCfg(NS_CONFIG::EM_CFGID_CTASK_SERVER_PORT, iValue))
	{
		std::cout<<"CTaskMgr: read server port failed!"<<std::endl;
		return RET::FAIL;
	}
	m_serverPort = (uint16_t)iValue;

	//读取服务端ip
	if (RET::SUC != Config::getCfg(NS_CONFIG::EM_CFGID_CTASK_SERVER_IP, m_serverIp))
	{
		std::cout<<"CTaskMgr: read server ip failed!"<<std::endl;
		return RET::FAIL;
	}

	//初始化socket管理器
	if (RET::SUC != m_socket.connectServer(m_serverPort, m_serverIp))
	{
		std::cout<<"CTaskMgr: connect server failed"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 启动接口 **/
int32_t CTaskMgr::start()
{
	m_run = true;
	m_thread = std::thread(std::bind(&CTaskMgr::process, this));

	return RET::SUC;
}

/** 线程处理接口 **/
void CTaskMgr::process()
{
	DIR *dir = nullptr;
	std::list<std::string> dir_list;
	while (m_run) {
		//打开日志目录
		dir = opendir(NS_CTASKMGR::PATH_OF_CAP_LOG);
		if (nullptr == dir) {
			std::cout<<"open dir failed!"<<std::endl;
			sleep(2);

#ifdef _DEBUG_
			assert(nullptr != dir);
#endif

			continue;
		}

		//读取日志文件名
		struct dirent *pFilename = readdir(dir);
		while (nullptr != pFilename) {
			if (0 != strlen(pFilename->d_name) && '.' != pFilename->d_name[0]
					|| !strcmp(pFilename->d_name + (strlen(pFilename->d_name) - strlen("log")), "log"))
			{
				dir_list.push_back(pFilename->d_name);    
			}
			pFilename = readdir(dir);
		}

		std::string filename;
		while (!dir_list.empty()) {
			filename = dir_list.front();
			std::cout<<filename<<std::endl;

			//处理日志文件
			std::string path = const_cast<char*>(NS_CTASKMGR::PATH_OF_CAP_LOG) + filename;
			processFile(path);

			//删除文件
			remove(path.c_str());
			
			dir_list.pop_front();
		}

		closedir(dir);
		sleep(1);
	}
}

/** 线程join **/
void CTaskMgr::threadJoin()
{
	m_thread.join();
}

/** 处理流量日志 **/
int32_t CTaskMgr::processFile(const std::string &filename)
{
	std::ifstream io(filename);

	std::string str((std::istreambuf_iterator<char>(io)), std::istreambuf_iterator<char>());

	std::string line_str;
	uint32_t uPrevPos = 0;
	int32_t iPos = str.find("\n");
	while (iPos != std::string::npos) {
		line_str = str.substr(uPrevPos, iPos - uPrevPos);

		if (RET::SUC != parserLine(line_str)) {
			m_parserLineFailed++;
		}

		uPrevPos = iPos + 1;
		iPos = str.find("\n", iPos + 1);
	}

	io.close();
	return RET::SUC;
}

/** 解析行字符串 **/
int32_t CTaskMgr::parserLine(std::string line_str)
{
	//解析请求行
	int32_t iPrevPos = line_str.find("\"");
	int32_t iCurPos = line_str.find("\"", iPrevPos + 1);
	if (iPrevPos == std::string::npos || iCurPos == std::string::npos)
	{
		return RET::FAIL;
	}
	std::string url = line_str.substr(iPrevPos + 1, iCurPos - iPrevPos - 1);

	//解析请求头
	iPrevPos = line_str.find("\"", iCurPos + 1);
	iCurPos = line_str.find("\"", iPrevPos + 1);
	if (iPrevPos == std::string::npos || iCurPos == std::string::npos)
	{
		return RET::FAIL;
	}
	std::string header = line_str.substr(iPrevPos + 1, iCurPos - iPrevPos - 1);

	//解析cookie 
	iPrevPos = line_str.find("\"", iCurPos + 1);
	iCurPos = line_str.find("\"", iPrevPos + 1);
	if (iPrevPos == std::string::npos || iCurPos == std::string::npos)
	{
		return RET::FAIL;
	}
	std::string cookie = line_str.substr(iPrevPos + 1, iCurPos - iPrevPos - 1);

	//解析请求体
	iPrevPos = line_str.find("\"", iCurPos + 1);
	iCurPos = line_str.find("\"", iPrevPos + 1);
	if (iPrevPos == std::string::npos || iCurPos == std::string::npos)
	{
		return RET::FAIL;
	}
	std::string body = line_str.substr(iPrevPos + 1, iCurPos - iPrevPos - 1);

	//解析host
	iPrevPos = line_str.find("\"", iCurPos + 1);
	iCurPos = line_str.find("\"", iPrevPos + 1);
	if (iPrevPos == std::string::npos || iCurPos == std::string::npos)
	{
		return RET::FAIL;
	}
	std::string host = line_str.substr(iPrevPos + 1, iCurPos - iPrevPos - 1);

	//发流
	std::string stream = url + "\r\n" + host + "\r\n" + cookie + "\r\n" 
			+ header + "\r\n\n" + body;
	m_socket.sendMsg(stream);

	return RET::SUC;
}
