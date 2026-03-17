#ifndef OPERATING_SYSTEM_SCHEDULER_HPP
#define OPERATING_SYSTEM_SCHEDULER_HPP

#include <vector>
#include <queue>
#include <functional>
#include <map>

#include "task.hpp"
#include "semaphore/semaphore.hpp"

class Scheduler final {
public:
    void addTask(Task task);
    void addSemaphore(int count = 1);
    void addInterrupt(size_t time, std::function<void()> handler);

    void tick();

private:
    std::vector<Task> tasks_;
    std::vector<Semaphore> semaphores_;

    std::priority_queue<Task*, std::vector<Task*>, Compare> ready_queue_;

    std::map<size_t, std::vector<std::function<void()>>> interrupts_;

    size_t current_time_ = 0;

    Task* getTaskById(size_t id);
};

#endif