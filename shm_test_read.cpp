#include "common.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
static std::vector<int64_t> gap;
static double ave = 0.0;
static double max = 0.0;
void ReadFromShm(void* addr) {
    Data d;
    ::memcpy((void*)&d, addr, sizeof(Data));
    int64_t curr = GetCurrTImeUs();

    ave = (gap.size() * ave + curr - d.us_)/(gap.size() + 1);
    gap.push_back(curr - d.us_);
    max = std::max(max, double(curr - d.us_));
}

int main() {
    // 初始化共享内存
    auto shm_id = ::shmget(shm_key, 408 * 2000, IPC_CREAT|0666);
    void* addr= shmat(shm_id, 0, 0);
    if(addr == nullptr) {
        printf("error on shmat,%d",errno);
        return 0;
    }
    // 初始化信号量
    auto sem_shm_id = ::shmget(sem_key, 128, IPC_CREAT|0666|SHM_HUGETLB);
    sem_t* sem = (sem_t *)shmat(sem_shm_id, 0, 0);
    ::sem_init(sem,1,0);
    gap.reserve(2000);
    while (gap.size() < 2000){
        sem_wait(sem);
        ReadFromShm(addr);
        addr = (char*)addr + sizeof(Data);
    }
    printf("平均延时:%f, 最大延时:%f", ave, max);
    return 0;
}