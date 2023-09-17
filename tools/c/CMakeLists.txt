# 将procctl.cpp编译成可执行文档
add_executable(procctl procctl.cpp)

target_include_directories(
    procctl
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
    procctl
PRIVATE
    public
)

add_executable(checkproc checkproc.cpp)

target_include_directories(
    checkproc
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
    checkproc
PRIVATE
    public
)