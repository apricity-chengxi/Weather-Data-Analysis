cmake_minimum_required(VERSION 3.16)
project(Weather-Data-Analysis)

# 设定cpp标准
set(CMAKE_CXX_STANDARD 11)
# 是否允许使用之前的cpp标准
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 输出 compile_commands.json 文件(用于clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置可执行文件的输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/idc/bin)
# 设置库文档的输出目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib) 

configure_file(
    ProjectPath.h.in
    ProjectPath.h
)

include_directories(${PROJECT_BINARY_DIR})

add_subdirectory(idc)
add_subdirectory(public)