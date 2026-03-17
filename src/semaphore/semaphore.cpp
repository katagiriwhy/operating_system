#include "semaphore.hpp"

Semaphore::Semaphore(int count) : count_(count) {}

bool Semaphore::try_acquire(size_t task_id) {
    if (count_ > 0) {
        --count_;
        return true;
    }

    waiting_tasks_.push(task_id);
    return false;
}

void Semaphore::release() {
    ++count_;
}

std::queue<size_t>& Semaphore::waiting() {
    return waiting_tasks_;
}