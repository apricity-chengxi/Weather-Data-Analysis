#include "PActice.h"

namespace Deamon
{
    namespace PActice
    {
        CPActive::CPActive()
        {
            m_shmid = 0;
            m_pos = -1;
            m_shm = 0;
        }

        // 把当前进程的心跳信息加入共享内存进程组中
        bool CPActive::AddPInfo(const int timeout, const char *pname, CLogFile *logfile)
        {
            if (m_pos != -1) return true;

            if (m_sem.init(SEMKEYP) == false) // 初始化信号量
            {
                if (logfile != 0)
                    logfile->Write("创建/获取信号量(%x)失败\n", SEMKEYP);
                else
                    printf("创建/获取信号量(%x)失败\n", SEMKEYP);

                return false;
            }

            // 创建/获取共享内存,键值为SHMKEYP,大小为MAXNUMP个st_procinfo结构体的大小
            if ((m_shmid = shmget((key_t)SHMKEYP, MAXNUMP * sizeof(struct st_procinfo), 0666 | IPC_CREAT)) == -1)
            {
                if (logfile != 0)
                    logfile->Write("创建/获取共享内存(%x)失败\n", SHMKEYP);
                else
                    printf("创建/获取共享内存(%x)失败\n", SHMKEYP);

                return false;
            }

            // 将共享内存连接到当前进程的地址空间
            m_shm = (struct st_procinfo *)shmat(m_shmid, 0, 0);

            struct st_procinfo stprocinfo; // 当前进程心跳信息的结构体
            memset(&stprocinfo, 0, sizeof(stprocinfo));

            stprocinfo.pid = getpid();                                      // 当前进程号
            stprocinfo.timeout = timeout;                                   // 超时时间
            stprocinfo.atime = time(0);                                     // 当前时间
            STRNCPY(stprocinfo.pname, sizeof(stprocinfo.pname), pname, 50); // 进程名

            // 进程id是循环使用的,如果曾经有一个进程异常退出,没有清理自己的心跳信息,
            // 它的进程信息将残留在共享内存中,不巧的是,当前进程重用了上述进程的id,
            // 这样就会在共享内存中存在两个进程id相同的记录,守护进程检查到残留进程的
            // 心跳时,会向进程id发送退出信号,这个信号将误杀当前进程

            // 如果共享内存中存在当前进程编号,一定是其它进程残留的数据,当前进程就重用该位置
            for (int ii = 0; ii < MAXNUMP; ii++)
            {
                if ((m_shm + ii)->pid == stprocinfo.pid || (m_shm + ii)->pid == 0)
                {
                    m_pos = ii;
                    break;
                }
            }

            if (m_pos == -1)
            {
                if (logfile != 0)
                    logfile->Write("共享内存空间已用完\n");
                else
                    printf("共享内存空间已用完\n");

                m_sem.V(); // 解锁

                return false;
            }

            // 把当前进程的心跳信息存入共享内存的进程组中
            memcpy(m_shm + m_pos, &stprocinfo, sizeof(struct st_procinfo));

            m_sem.V(); // 解锁

            return true;
        }

        // 更新共享内存进程组中当前进程的心跳时间
        bool CPActive::UptATime()
        {
            if (m_pos == -1) return false;

            (m_shm + m_pos)->atime = time(0);

            return true;
        }

        CPActive::~CPActive()
        {
            // 把当前进程从共享内存的进程组中移去
            if (m_pos != -1) memset(m_shm + m_pos, 0, sizeof(struct st_procinfo));

            // 把共享内存从当前进程中分离
            if (m_shm != 0) shmdt(m_shm);
        }
    }
}

