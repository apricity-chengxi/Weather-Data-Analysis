# 编译静态库,暂不添加任何源码
add_library(public "")

# 随后用target_sources添加源码
# 这种方式更好,因为可以更细粒度地访问权限
# 例如对cpp文档就是PRIVATE,而hpp则是PUBLIC

target_sources(
    public
PRIVATE
    # 注:${CMAKE_CURRENT_LIST_DIR}代表当前的CMakeLists.txt文档所在的绝对路径
    ${CMAKE_CURRENT_LIST_DIR}/_public.cpp
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/_public.h
)

# 增加头文档目录
target_include_directories(
    public
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)


add_library(PActice "")

target_sources(
    PActice
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/PActice.cpp
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/PActice.h
)