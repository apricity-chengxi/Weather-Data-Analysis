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

add_executable(gzipfiles gzipfiles.cpp)

target_include_directories(
    gzipfiles
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
    gzipfiles
PRIVATE
    public
)


add_executable(deletefiles deletefiles.cpp)

target_include_directories(
    deletefiles
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
    deletefiles
PRIVATE
    public
)

add_executable(ftpgetfiles ftpgetfiles.cpp)

target_include_directories(
    ftpgetfiles
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(
    ftpgetfiles
PRIVATE
    public
    ftp
)