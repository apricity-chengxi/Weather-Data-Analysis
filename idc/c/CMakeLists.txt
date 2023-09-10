# 将crtsurfdata.cpp编译成可执行文档
add_executable(crtsurfdata crtsurfdata.cpp)

target_include_directories(
    crtsurfdata
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
    crtsurfdata
PRIVATE
    public
)