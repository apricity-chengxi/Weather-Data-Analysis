#include "_public.h"
#include "Define.h"

CLogFile logfile(10);

// 全国气象站点参数结构体。
struct st_stcode
{
    char provname[31]; // 省
    char obtid[11];    // 站号
    char obtname[31];  // 站名
    double lat;        // 纬度
    double lon;        // 经度
    double height;     // 海拔高度
};

// 存放全国气象站点参数的容器。
vector<struct st_stcode> vstcode;

// 把站点参数文件中加载到vstcode容器中。
bool LoadSTCode(const char *inifile);

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

    logfile.Write("crtsurfdata 开始运行\n");

    // 把站点参数文件中加载到vstcode容器中。
    if (LoadSTCode(argv[1]) == false) return -1;

    logfile.Write("crtsurfdata 运行结束\n");

    return 0;
}


// 把站点参数文件中加载到vstcode容器中。
bool LoadSTCode(const char *inifile)
{
    CFile File;

    // 打开站点参数文件。
    if (File.Open(inifile, "r") == false)
    {
        logfile.Write("File.Open(%s) 失败\n", inifile);
        return false;
    }

    char strBuffer[301];

    CCmdStr CmdStr;

    struct st_stcode stcode;

    while (true)
    {
        // 从站点参数文件中读取一行,如果已读取完,跳出循环
        if (File.Fgets(strBuffer, 300, true) == false) break;

#ifdef TEST_MSG
        logfile.Write("---%s---\n", strBuffer);
#endif
        
        // 把读取到的一行拆分
        CmdStr.SplitToCmd(strBuffer, ",", true);

        if (CmdStr.CmdCount() != 6) continue; // 扔掉无效的行

        // 把站点参数的每个数据项保存到站点参数结构体中
        memset(&stcode, 0, sizeof(struct st_stcode));
        CmdStr.GetValue(0, stcode.provname, 30); // 省
        CmdStr.GetValue(1, stcode.obtid, 10);    // 站号
        CmdStr.GetValue(2, stcode.obtname, 30);  // 站名
        CmdStr.GetValue(3, &stcode.lat);         // 纬度
        CmdStr.GetValue(4, &stcode.lon);         // 经度
        CmdStr.GetValue(5, &stcode.height);      // 海拔高度

        // 把站点参数结构体放入站点参数容器
        vstcode.push_back(stcode);
    }

#ifdef TEST_MSG
    for (auto &it : vstcode)
    {
        logfile.Write("provname=%s, obtid=%s, obtname=%s, "
                      "lat=%.2f, lon=%.2f, height=%.2f\n",
                      it.provname, it.obtid, it.obtname, it,
                      it.lon, it.height);
    }
#endif

    return true;
}