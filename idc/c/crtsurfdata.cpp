#include "_public.h"
#include "ProjectPath.h"

CLogFile logfile;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {

        // 如果参数非法,给出帮助文档
        printf("Using:./crtsurfdata inifile outpath logfile\n");
        printf("Example:"
               PROJECT_PATH "/idc/bin/crtsurfdata "
               PROJECT_PATH "/idc/ini/stcode.ini "
               "/tmp/surfdata "
               PROJECT_PATH "/idc/log/idc/crtsurfdata.log\n\n");

        printf("inifile 全国气象站点参数文件名。\n");
        printf("outpath 全国气象站点数据文件存放的目录。\n");
        printf("logfile 本程序运行的日志文件名。\n\n");

        return -1;
    }

    // 打开程序的日志文件
    if (logfile.Open(argv[3], "a+", false) == false)
    {
        printf("logfile.Open(%s) failed.\n", argv[3]);
        return -1;
    }

    logfile.Write("crtsurfdata 开始运行。\n");

    // 在这里插入处理业务的代码

    logfile.Write("crtsurfdata 运行结束。\n");

    return 0;
}