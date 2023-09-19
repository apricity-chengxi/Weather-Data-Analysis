#include "Define.h"

#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

// 启动数据中心后台服务程序
int main(int argc, char *argv[])
{
    // 检查服务程序是否超时,配置在/etc/rc.local中由root用户执行
    string command1;
    command1 = command1
               + PROJECT_PATH + "/tools/bin/procctl "
               + "30 "
               + PROJECT_PATH + "/tools/bin/checkproc";
    cout << "command1: 检查服务程序是否超时" << endl;
    if (system(command1.c_str()) == 0)
        cout << "command1 ok" << endl;
    else
        cout << "command1 failed" << endl;
    
    // 压缩数据中心后台服务程序的备份日志
    string command2;
    command2 = command2
               + PROJECT_PATH + "/tools/bin/procctl "
               + "300 "
               + PROJECT_PATH + "/tools/bin/gzipfiles "
               + PROJECT_PATH + "/tools/idc "
               + "/log/idc "
               + "\"*.log.20*\" "
               + "0.02";
    cout << "command2: 压缩数据中心后台服务程序的备份日志" << endl;
    if (system(command2.c_str()) == 0)
        cout << "command2 ok" << endl;
    else
        cout << "command2 failed" << endl;

    // 生成用于测试的全国气象站点观测的分钟数据
    string command3;
    command3 = command3
               + PROJECT_PATH + "/tools/bin/procctl "
               + "60 "
               + PROJECT_PATH + "/idc/bin/crtsurfdata "
               + PROJECT_PATH + "/idc/ini/stcode.ini "
               + "/tmp/idc/surfdata "
               + PROJECT_PATH + "/log/idc/crtsurfdata.log "
               + "xml,json,csv";
    cout << "command3: 生成用于测试的全国气象站点观测的分钟数据" << endl;
    if (system(command3.c_str()) == 0)
        cout << "command3 ok" << endl;
    else
        cout << "command3 failed" << endl;

    // 清理原始的全国气象站点观测的分钟数据目录/tmp/idc/surfdata中的历史数据文件
    string command4;
    command4 = command4
               + PROJECT_PATH + "/tools/bin/procctl "
               + "300 "
               + PROJECT_PATH + "/tools/bin/deletefiles "
               + "/tmp/idc/surfdata "
               + "* "
               + "0.04";
    cout << "command4: 清理原始的全国气象站点观测的分钟数据目录/tmp/idc/surfdata中的历史数据文件" << endl;
    if (system(command4.c_str()) == 0)
        cout << "command4 ok" << endl;
    else
        cout << "command4 failed" << endl;

    return 0;
}