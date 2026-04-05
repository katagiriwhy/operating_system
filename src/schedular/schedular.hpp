#ifndef OPERATING_SYSTEM_SCHEDULER_HPP
#define OPERATING_SYSTEM_SCHEDULER_HPP

#include <vector>
#include <queue>
#include <functional>
#include <map>
#include <optional>
#include <cstddef>

#include "task.hpp"
#include "semaphore/semaphore.hpp"

class Scheduler final {
public:
    static constexpr std::size_t kMaxTasks = 32;
    static constexpr std::size_t kMaxResources = 16;

    Scheduler()
    {
        tasks_.reserve(kMaxTasks);
    }

    void addTask(Task task);
    void addSemaphore(int count = 1);
    void addInterrupt(size_t time, std::function<void()> handler);

    void tick();

    [[nodiscard]] std::size_t current_time() const { return current_time_; }
    [[nodiscard]] std::optional<State> task_state(std::size_t id) const;
    [[nodiscard]] std::size_t ready_queue_size() const { return ready_queue_.size(); }
    [[nodiscard]] std::size_t task_count() const { return tasks_.size(); }
    [[nodiscard]] std::size_t semaphore_count() const { return semaphores_.size(); }

private:
    std::vector<Task> tasks_;
    std::vector<Semaphore> semaphores_;

    std::priority_queue<Task*, std::vector<Task*>, Compare> ready_queue_;

    std::map<size_t, std::vector<std::function<void()>>> interrupts_;

    size_t current_time_ = 0;

    Task* getTaskById(size_t id);
};

#endif