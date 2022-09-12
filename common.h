#pragma once
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/time.h>
#include <ctime>
constexpr key_t shm_key = 0011;
constexpr key_t sem_key = 0012;

struct Data
{
    int64_t us_;
    char data[400];
};

int64_t GetCurrTImeUs() {
    static timeval t;
    ::gettimeofday(&t,nullptr);
    return t.tv_usec;
}
