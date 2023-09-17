cmake_minimum_required(VERSION 3.16)
project(Weather-Data-Analysis)

# 设定cpp标准
set(CMAKE_CXX_STANDARD 11)
# 是否允许使用之前的cpp标准
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 输出 compile_commands.json 文件(用于clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 测试
set(TEST_MSG ON)
set(TEST_1 OFF)
set(TEST_2 OFF)
set(TEST_3 OFF)

# 设置库文档的输出目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib) 

configure_file(
    Define.h.in
    Define.h
)

include_directories(${PROJECT_BINARY_DIR})

add_subdirectory(idc)
add_subdirectory(public)
add_subdirectory(tools)