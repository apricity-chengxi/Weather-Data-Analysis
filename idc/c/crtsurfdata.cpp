#include "_public.h"
#include "Define.h"

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

// 全国气象站点分钟观测数据结构
struct st_surfdata
{
    char obtid[11];     // 站点代码
    char ddatetime[21]; // 数据时间:格式yyyymmddhh24miss
    int t;              // 气温:单位,0.1摄氏度
    int p;              // 气压:0.1百帕。
    int u;              // 相对湿度,0-100之间的值
    int wd;             // 风向,0-360之间的值
    int wf;             // 风速:单位0.1m/s
    int r;              // 降雨量:0.1mm
    int vis;            // 能见度:0.1米
};

// 存放全国气象站点分钟观测数据的容器
vector<struct st_surfdata> vsurfdata;

char strddatetime[21]; // 观测数据的时间

// 模拟生成全国气象站点分钟观测数据,存放在vsurfdata容器中
void CrtSurfData();

CFile File; // 文件操作对象

// 把容器vsurfdata中的全国气象站点分钟观测数据写入文件
bool CrtSurfFile(const char *outpath, const char *datafmt);

CLogFile logfile; // 日志类

void EXIT(int sig); // 程序退出和信号2、15的处理函数

int main(int argc, char *argv[])
{
    if (argc != 5 && argc != 6)
    {
        // 如果参数非法,给出帮助文档
        // clang-format off
        printf("Using:./crtsurfdata inifile outpath logfile datafmt [datetime]\n");
        printf("Example:"\
               PROJECT_PATH "/idc/bin/crtsurfdata "\
               PROJECT_PATH "/idc/ini/stcode.ini "\
               "/tmp/idc/surfdata "\
               PROJECT_PATH "/log/idc/crtsurfdata.log "\
               "xml,json,csv "\
               "20231018123456\n\n");

        printf("inifile 全国气象站点参数文件名\n");
        printf("outpath 全国气象站点数据文件存放的目录\n");
        printf("logfile 本程序运行的日志文件名\n");
        printf("datafmt 生成数据文件的格式,支持xml、json和csv三种格式,中间用逗号分隔\n");
        printf("datetime 这是一个可选参数,表示生成指定时间的数据和文件\n\n\n");
        // clang-format on
        return -1;
    }


    // 关闭全部的信号和输入输出
    // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程
    // 但请不要用 "kill -9 +进程号" 强行终止
    CloseIOAndSignal(true);
    signal(SIGINT, EXIT);
    signal(SIGTERM, EXIT);

    // 打开程序的日志文件
    if (logfile.Open(argv[3], "a+", false) == false)
    {
        printf("logfile.Open(%s) failed.\n", argv[3]);
        return -1;
    }

    logfile.Write("crtsurfdata 开始运行\n");

    // 把站点参数文件中加载到vstcode容器中。
    if (LoadSTCode(argv[1]) == false) return -1;


    // 获取当前时间，当作观测时间
    memset(strddatetime, 0, sizeof(strddatetime));
    if (argc == 5)
        LocalTime(strddatetime, "yyyymmddhh24miss");
    else
        STRCPY(strddatetime, sizeof(strddatetime), argv[5]);

    // 模拟生成全国气象站点分钟观测数据,存放在vsurfdata容器中
    CrtSurfData();

    // 把容器vsurfdata中的全国气象站点分钟观测数据写入文件
    if (strstr(argv[4], "xml") != 0) CrtSurfFile(argv[2], "xml");
    if (strstr(argv[4], "json") != 0) CrtSurfFile(argv[2], "json");
    if (strstr(argv[4], "csv") != 0) CrtSurfFile(argv[2], "csv");

    logfile.Write("crtsurfdata 运行结束\n");

    return 0;
}


// 把站点参数文件中加载到vstcode容器中。
bool LoadSTCode(const char *inifile)
{
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

#if defined(TEST_MSG) && defined(TEST_1)
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

#if defined(TEST_MSG) && defined(TEST_2)
    for (auto &it : vstcode)
    {
        logfile.Write("provname=%s, obtid=%s, obtname=%s, "\
                      "lat=%.2f, lon=%.2f, height=%.2f\n",\
                      it.provname, it.obtid, it.obtname, it,\
                      it.lon, it.height);
    }
#endif

    return true;
}

// 模拟生成全国气象站点分钟观测数据,存放在vsurfdata容器中
void CrtSurfData()
{
    // 播随机数种子
    srand(time(0));

    struct st_surfdata stsurfdata;

    // 遍历气象站点参数的vstcode容器
    for (int ii = 0; ii < vstcode.size(); ii++)
    {
        memset(&stsurfdata, 0, sizeof(struct st_surfdata));

        // 用随机数填充分钟观测数据的结构体
        strncpy(stsurfdata.obtid, vstcode[ii].obtid, 10); // 站点代码
        strncpy(stsurfdata.ddatetime, strddatetime, 14);  // 数据时间:格式yyyymmddhh24miss
        stsurfdata.t = rand() % 351;                      // 气温:单位,0.1摄氏度
        stsurfdata.p = rand() % 265 + 10000;              // 气压:0.1百帕
        stsurfdata.u = rand() % 100 + 1;                  // 相对湿度,0-100之间的值
        stsurfdata.wd = rand() % 360;                     // 风向,0-360之间的值
        stsurfdata.wf = rand() % 150;                     // 风速:单位0.1m/s
        stsurfdata.r = rand() % 16;                       // 降雨量:0.1mm
        stsurfdata.vis = rand() % 5001 + 100000;          // 能见度:0.1米

        // 把观测数据的结构体放入vsurfdata容器
        vsurfdata.push_back(stsurfdata);
    }
}

// 把容器vsurfdata中的全国气象站点分钟观测数据写入文件。
bool CrtSurfFile(const char *outpath, const char *datafmt)
{
    // 拼接生成数据的文件名,例如:/tmp/idc/surfdata/SURF_ZH_20210629092200_2254.csv
    char strFileName[301];
    sprintf(strFileName, "%s/SURF_ZH_%s_%d.%s", outpath, strddatetime, getpid(), datafmt);

    // 打开文件
    if (File.OpenForRename(strFileName, "w") == false)
    {
        logfile.Write("File.OpenForRename(%s) failed.\n", strFileName);
        return false;
    }

    // 写入第一行标题
    if (strcmp(datafmt, "csv") == 0) File.Fprintf("站点代码,数据时间,气温,气压,相对湿度,风向,风速,降雨量,能见度\n");

    // 遍历存放观测数据的vsurfdata容器
    if (strcmp(datafmt, "xml") == 0)
    {
        File.Fprintf("<data>\n");
        for (auto &it : vsurfdata)
        {
            // 写入一条记录
            File.Fprintf("<obtid>%s</obtid><ddatetime>%s</ddatetime><t>%.1f</t><p>%.1f</p>"\
                        "<u>%d</u><wd>%d</wd><wf>%.1f</wf><r>%.1f</r><vis>%.1f</vis><endl/>\n",\
                        it.obtid, it.ddatetime, it.t / 10.0, it.p / 10.0,\
                        it.u, it.wd, it.wf / 10.0, it.r / 10.0, it.vis / 10.0);
        }
        File.Fprintf("</data>\n");
    }
    else if (strcmp(datafmt, "json") == 0)
    {
        File.Fprintf("{\"data\":[\n");
        bool is_first = true;
        for (auto &it : vsurfdata)
        {
            if (is_first) is_first = false;
            else File.Fprintf(",\n");
            File.Fprintf("{\"obtid\":\"%s\",\"ddatetime\":\"%s\",\"t\":\"%.1f\",\"p\":\"%.1f\","
                         "\"u\":\"%d\",\"wd\":\"%d\",\"wf\":\"%.1f\",\"r\":\"%.1f\",\"vis\":\"%.1f\"}",
                         it.obtid, it.ddatetime, it.t / 10.0, it.p / 10.0,
                         it.u, it.wd, it.wf / 10.0, it.r / 10.0, it.vis / 10.0);
        }
        File.Fprintf("\n]}\n");
    }
    else if (strcmp(datafmt, "csv") == 0) for (auto &it : vsurfdata)
    {
        File.Fprintf("%s,%s,%.1f,%.1f,%d,%d,%.1f,%.1f,%.1f\n",\
                     it.obtid, it.ddatetime, it.t / 10.0, it.p / 10.0,\
                     it.u, it.wd, it.wf / 10.0, it.r / 10.0, it.vis / 10.0);
    }

    // 关闭文件
    File.CloseAndRename();

    // 修改文件的时间属性
    UTime(strFileName, strddatetime);

    logfile.Write("生成数据文件%s成功,数据时间%s,记录数%d\n", strFileName, strddatetime, vsurfdata.size());

    return true;
}

// 程序退出和信号2、15的处理函数
void EXIT(int sig)
{
    logfile.Write("程序退出, sig=%d\n\n", sig);

    exit(0);
}
