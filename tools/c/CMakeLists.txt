# 将procctl.cpp编译成可执行文档
add_executable(procctl procctl.cpp)

target_include_directories(
    procctl
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
