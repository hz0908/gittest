#define _GNU_SOURCE /*for CPU_SET*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include "bufferSendrecv.h"
#include "frame.h"
#include "format.h"
#include "datagram.h"
#include "register.h"
#include "dc_delay.h"

#define PTHREAD_PRIORITY 80
#define PTHREAD_CPU 2
#define ONE_SEC 1000000000ULL

static int wake_delay_send = 0;   // 设置苏醒时刻。0代表每个周期初始时刻苏醒
static int period_nsec = 500000; // 设置周期，单位为ns
static int running = 1;

char send_card_name[] = "enp1s0";   // 命令行ifconfig -a 查看
char rev_card_name[] = "enp0s31f6"; // ifconfig enp0s31f6 promisc 将接收端口开启为混杂模式，这样才能收到不符合mac地址的报文

static void normalize(struct timespec *ts)
{
    while (ts->tv_nsec > 999999999)
    {
        ts->tv_sec += 1;
        ts->tv_nsec -= ONE_SEC;
    }

    while (ts->tv_nsec < 0)
    {
        ts->tv_sec -= 1;
        ts->tv_nsec += ONE_SEC;
    }
}

static int set_realtime(pthread_t thread, int priority, int cpu)
{
    cpu_set_t cpuset;
    struct sched_param sp;
    int err, policy;

    if (priority < 0)
    {
        printf("invalid priority\n");
        return 0;
    }

    err = pthread_getschedparam(thread, &policy, &sp);
    if (err)
    {
        printf("pthread_getschedparam error\n");
        return -1;
    }

    sp.sched_priority = priority;

    err = pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);
    if (err)
    {
        printf("pthread_setschedparam error\n");
        return -1;
    }

    if (cpu < 0)
    {
        printf("invalid cpu\n");
        return 0;
    }
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    err = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (err)
    {
        printf("pthread_setaffinity_np error\n");
        return -1;
    }

    return 0;
}

static void stopHandler(int sign)
{
    printf("received ctrl-c\n");
    running = 0;
}

int main()
{
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    // 套接字创建
    send_socketCreate(send_card_name);
    rev_socketCreate(rev_card_name);
    // 帧结构体和参数
    nc_frame_t frm;
    uint8 data[8] = {0};

    struct timespec ts;
    clockid_t clkid = CLOCK_TAI;

    if (set_realtime(pthread_self(), PTHREAD_PRIORITY, PTHREAD_CPU) < 0)
    {
        printf("error in set_realtime.\n");
        return -1;
    }

    clock_gettime(clkid, &ts);
    ts.tv_sec += 2;
    ts.tv_nsec = ONE_SEC - wake_delay_send;
    normalize(&ts);

    while (running)
    {
        clock_nanosleep(clkid, TIMER_ABSTIME, &ts, NULL);

        initframe(&frm);
        dclkdgrmAdd(&frm, DC64COMMAND, data);
        FrameTransmit(&frm);
        FrameReceive(&frm);
        fflush(stdout);

        ts.tv_nsec += period_nsec;
        normalize(&ts);
    }

    // 套接字关闭
    send_rev_socketClose();

    return 0;
}