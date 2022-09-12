#include "common.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
void WriteToShm(void* addr) {
    Data d;
    d.us_ = GetCurrTImeUs();
    ::memcpy(addr, (void*)&d, sizeof(Data));
}

int main() {
    // 初始化共享内存
    auto shm_id = ::shmget(shm_key, 408 * 2000, IPC_CREAT|0666|SHM_HUGETLB);
    void* addr= shmat(shm_id, 0, 0);
    if(addr == nullptr) {
        printf("error on shmat,%d",errno);
        return 0;
    }
    // 初始化信号量
    auto sem_shm_id = ::shmget(sem_key, 128, IPC_CREAT|0666);
    sem_t* sem = (sem_t *)shmat(sem_shm_id, 0, 0);
    ::sem_init(sem,1,0);
    std::string cmd;
    std::cout << "请输入命令"<<std::endl;
    std::cin >> cmd;
    if(!cmd.empty()) {
        for(int i = 0;i < 200; ++i){
            usleep(50000);
            for(int j = 0; j<10; ++j){
                WriteToShm(addr);
                addr = (char*)addr + sizeof(Data);
                sem_post(sem);
            }
        }
    }
    return 0;
}