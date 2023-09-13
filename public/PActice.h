#pragma once
#include "_public.h"

namespace Deamon
{
    namespace PActice
    {
        // 进程心跳信息的结构体
        struct st_procinfo
        {
            int pid;        // 进程id
            char pname[51]; // 进程名称,可以为空
            int timeout;    // 超时时间,单位:秒
            time_t atime;   // 最后一次心跳的时间,用整数表示
        };

        constexpr int   MAXNUMP = 1000;   // 最大的进程数量
        constexpr key_t SHMKEYP = 0x5095; // 共享内存的key
        constexpr key_t SEMKEYP = 0x5095; // 信号量的key

        // 查看共享内存:  ipcs -m
        // 删除共享内存:  ipcrm -m shmid
        // 查看信号量:    ipcs -s
        // 删除信号量:    ipcrm sem semid

        // 进程心跳操作类
        class CPActive
        {
        private:
            CSEM m_sem;         // 用于给共享内存加锁的信号量id
            int m_shmid;        // 共享内存的id
            int m_pos;          // 当前进程在共享内存进程组中的位置
            st_procinfo *m_shm; // 指向共享内存的地址空间

        public:
            CPActive(); // 初始化成员变量

            // 把当前进程的心跳信息加入共享内存进程组中
            bool AddPInfo(const int timeout, const char *pname = 0, CLogFile *logfile = 0);

            // 更新共享内存进程组中当前进程的心跳时间
            bool UptATime();

            ~CPActive(); // 从共享内存中删除当前进程的心跳记录
        };
    }
}