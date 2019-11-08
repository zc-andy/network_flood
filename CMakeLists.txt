# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12.0)

if(CMAKE_BUILD_RPATH)
    message("CMAKE_BUILD_RPATH: ${CMAKE_BUILD_RPATH}")
endif()

# 项目信息
project (flood)
set (FLOOD_VERSION 1)
set (FLOOD_VERSION 0)
set (FLOOD_VERSION 0)

# 设置编译选项

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES DEBUG)) 
	add_definitions(-D_DEBUG_VERSION_)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g -rdynamic") 
endif()	

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES RELEASE))
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
endif()

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES GTEST))
	add_definitions(-D_GTEST_)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g -fprofile-arcs -ftest-coverage") 
endif()

# 目录下的所有源文件
aux_source_directory(src FLOOD_SRC_FILES)

if (CMAKE_BUILD_TYPE MATCHES GTEST)
	aux_source_directory(gtest FLOOD_TEST_FILES)
endif()	

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/hdr)

# 库文件目录
include_directories(${CMAKE_BUILD_RPATH}/include)
link_directories(${CMAKE_BUILD_RPATH}/lib)

# 指定生成目标
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
	add_executable(flood ${FLOOD_SRC_FILES}) 
	target_link_libraries(flood PRIVATE yajl)
	target_link_libraries(flood PRIVATE pthread)
endif()

if (CMAKE_BUILD_TYPE MATCHES RELEASE)
	add_executable(flood ${FLOOD_SRC_FILES})
	target_link_libraries(flood PRIVATE yajl)
endif()

if (CMAKE_BUILD_TYPE MATCHES GTEST)
	#添加编译选项
	add_executable(flood_gtest ${FLOOD_SRC_FILES} 
					${FLOOD_TEST_FILES})
	target_link_libraries(flood_gtest PRIVATE gtest)
	target_link_libraries(flood_gtest PRIVATE yajl)
	target_link_libraries(flood_gtest PRIVATE gcov)

	#执行程序
	add_custom_target(test
	COMMAND ./flood_gtest
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
	COMMENT "run test suit.")

	add_custom_target(gen_coverage
	COMMAND lcov -d CMakeFiles/flood_gtest.dir/src -t flood_gtest -o 'flood_gtest.cov.info' -b . -c
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
	COMMENT "genarate coverage info")

	add_custom_target(gen_coverage_html
	COMMAND genhtml -o cov_result flood_gtest.cov.info
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
	COMMENT "genarate coverage info to ./cov_result")
	
	add_dependencies(gen_coverage test)
	add_dependencies(gen_coverage flood_gtest)
	add_dependencies(gen_coverage_html gen_coverage)
endif()
