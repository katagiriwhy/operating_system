#ifndef OPERATING_SYSTEM_SEMAPHORE_HPP
#define OPERATING_SYSTEM_SEMAPHORE_HPP

#include <queue>
#include <cstddef>

class Semaphore final {
public:
    explicit Semaphore(int count = 1);

    bool try_acquire(size_t task_id);
    void release();

    std::queue<size_t>& waiting();

private:
    int count_;
    std::queue<size_t> waiting_tasks_;
};

#endif