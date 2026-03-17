#ifndef OPERATING_SYSTEM_SCHEDULER_HPP
#define OPERATING_SYSTEM_SCHEDULER_HPP

#include <vector>
#include <queue>

#include "task.hpp"
#include "semaphore/semaphore.hpp"

class Scheduler final {
public:
    void addTask(Task task);
    void addSemaphore(int count = 1);

    void tick();

private:
    std::vector<Task> tasks_;
    std::vector<Semaphore> semaphores_;

    std::priority_queue<Task*, std::vector<Task*>, Compare> ready_queue_;

    Task* getTaskById(size_t id);
};

#endif