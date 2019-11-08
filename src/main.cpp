#include "Processor.h"
#include "TaskMgr.h"
#include "CTaskMgr.h"

/** 主程序入口 **/
int32_t main(int32_t argc, char **argv)
{
	/** 处理线程初始化 **/
	if (RET::SUC != Processor::getInstance().init())
	{
		std::cout<<"Flood: Processor init failed!"<<std::endl;
		return RET::FAIL;
	}

	/** 任务中心初始化 **/
	if (RET::SUC != TaskMgr::getInstance().init())
	{
		std::cout<<"Flood: TaskMgr init failed!"<<std::endl;
		return RET::FAIL; 
	}

	/** C任务中心初始化 **/
	if (RET::SUC != CTaskMgr::getInstance().init())
	{
		std::cout<<"Flood: CTaskMgr init failed!"<<std::endl;
		return RET::FAIL; 
	}

	/** 任务中心启动 **/
	if (RET::SUC != TaskMgr::getInstance().start())
	{
		std::cout<<"Flood: TaskMgr start failed!"<<std::endl;
		return RET::FAIL;
	}

	/** C任务中心启动 **/
	if (RET::SUC != CTaskMgr::getInstance().start())
	{
		std::cout<<"Flood: CTaskMgr start failed!"<<std::endl;
		return RET::FAIL;
	}

	/** 处理线程启动 **/
	if (RET::SUC != Processor::getInstance().start())
	{
		std::cout<<"Flood: TaskMgr start failed!"<<std::endl;
		return RET::FAIL;
	}

	TaskMgr::getInstance().threadJoin();
	CTaskMgr::getInstance().threadJoin();
	Processor::getInstance().threadJoin();

	return RET::SUC;
}//end main
