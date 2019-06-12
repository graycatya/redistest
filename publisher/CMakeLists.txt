
cmake_minimum_required( VERSION 3.0 )

project(publisher)


macro(cross_compile bool0 bool1 bool2)
    
    # 编译生成目录
    IF(${bool0})
        set(PROJECT_PATH ARM)
    else()
        set(PROJECT_PATH PC)
    endif()
    # 生成Debug或Release版本
    IF(${bool1})
        SET(CMAKE_BUILD_TYPE "Debug")
        SET(DEBUG_RELEASE Debug)
    else()
        SET(CMAKE_BUILD_TYPE "Release")
        SET(DEBUG_RELEASE Release)
    endif()
    # 启动gdb调试
    IF(${bool2})
        add_definitions("-Wall -g")
    endif()
    add_definitions("-std=c++11")
    # 生成路径
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/${PROJECT_PATH}/${DEBUG_RELEASE}/Lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CAMKE_BINARY_DIR}/build/${PROJECT_PATH}/${DEBUG_RELEASE}/Lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/${PROJECT_PATH}/${DEBUG_RELEASE}/Bin)


endmacro(cross_compile)

option(DEBUG_RELEASE "Debug or Release" ON)
option(GDB_DEBUG "GDB DEBUG" OFF)
option (_DEBUG ON)


if(_DEBUG)
    add_definitions(-D _DEBUG)
endif(_DEBUG)

cross_compile(CROSS_COMPILE DEBUG_RELEASE GDB_DEBUG)
add_definitions("-Wformat")

link_libraries(hiredis)
link_libraries(event)
link_libraries(pthread)


aux_source_directory(. DIR_SRCS)

add_executable(publisher ${DIR_SRCS})
target_link_libraries(publisher hiredis)
target_link_libraries(publisher event)
target_link_libraries(publisher pthread)
